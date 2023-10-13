/*******************************************************************************
*   Ledger CTF 2018 - Security Team
*   (c) 2018 - Ledger
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/



#include <stdio.h>
#include "sboxes.c"
#include "common.h"
#include "components.c"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

using namespace std;



// One more step. 
// Simple state machine. retrieve the current operation for the cipher number r.
// perform the next step / and increment the state for this encryption
inline void oneStep(int r){
   int op = scheduling[r];
   switch(op){
      case 0: // masking
         addroundkey(states[r], mask);
      break;
      case 1: case 5: case 9: case 13: case 17: case 21: case 25: case 29: case 33: case 37: // subbytes
         subAndAdd(states[r], mask, (int) (op/4) );
      break;
      case 2: case 6: case 10: case 14: case 18: case 22: case 26: case 30: case 34: case 38: // SR
         shiftrows(states[r]);
      break;
      case 3: case 7: case 11: case 15: case 19: case 23: case 27: case 31: case 35: // MX
         mixcolumns(states[r]);
      break;
      case 4: case 8: case 12: case 16: case 20: case 24: case 28: case 32: case 36: // masking MX
         addroundkey(states[r],mask3);
      break;
      case 39: // unmasking
         addroundkey(states[r],shiftedmask);
      break;
   }
   scheduling[r]++;
}


// random scheduling
inline void schedule(){
   bool finishJob = false;

   // Job is finished when all the 15 encryption have been performed
   while(!finishJob){
      // Generate a random deciding which of the 15 encryptions performs one step ahead
      int r = rand() % NCIPHER;
      // if the encryption is not finished yet
      while(scheduling[r] == ENDCIPHER) r = rand() % NCIPHER;

      // Check if debug is on going
      struct timespec now, tmstart;
      clock_gettime(CLOCK_REALTIME, &tmstart);
      // one step ahead for encryption number r
      oneStep(r);
      clock_gettime(CLOCK_REALTIME, &now);
      double msec = (double)((now.tv_sec+now.tv_nsec*1e-9) - (double)(tmstart.tv_sec+tmstart.tv_nsec*1e-9));
      msec *= 1000;

      // If more than 500 ms -> a fault is infected onto the whole cipher using the masks
      for(int i = 0; i < 4; i++){
         for(int j = 0; j < 4; j++){
            mask3[i][j] ^= (unsigned char) (msec / 500) * (rand() % 255);
            mask[i][j] ^= (unsigned char) (msec / 500) * (rand() % 255);
         }
      }
      
      // Job is finished when all the 15 encryption have been performed
      finishJob = true;
      for(int i = 0; i< NCIPHER; i++)
         finishJob &= (scheduling[i] == ENDCIPHER);
   }
}

int perms[NCIPHER][16] = {
   {13,14,6,8,11,12,0,9,1,10,15,7,4,2,5,3},
   {13,6,9,7,15,5,8,2,10,1,12,14,4,3,11,0},
   {7,9,3,4,13,6,1,10,11,15,0,2,12,8,5,14},
   {15,12,2,8,10,4,9,7,14,11,0,1,13,5,3,6},
   {0,2,14,13,6,5,4,12,9,10,11,8,3,7,15,1},
   {4,14,0,8,9,6,3,2,5,15,10,12,1,13,11,7},
   {8,13,3,14,12,6,5,7,11,2,9,1,0,15,10,4},
   {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, // This one contains the correct input
   {13,14,6,8,11,12,0,9,1,10,15,7,4,2,5,3},
   {13,6,9,7,15,5,8,2,10,1,12,14,4,3,11,0},
   {7,9,3,4,13,6,1,10,11,15,0,2,12,8,5,14},
   {15,12,2,8,10,4,9,7,14,11,0,1,13,5,3,6},
   {0,2,14,13,6,5,4,12,9,10,11,8,3,7,15,1},
   {4,14,0,8,9,6,3,2,5,15,10,12,1,13,11,7},
   {8,13,3,14,12,6,5,7,11,2,9,1,0,15,10,4},
};

// Key is : f0331ce0266adace86a8a13bfa146740
// sol is  0ab7982c0ec65fc9c2412d527470d768
int main(int argc, char** argv) {
   // init Rand
   srand(time(NULL));

   // Usage
   if(argc != 2){
      printf("usage : %s <serial>\n where serial is 16 bytes provided in hex\n ex: %s 00112233445566778899AABBCCDDEEFF",argv[0],argv[0]); 
      return 1;
   }
   if ( strlen(argv[1]) != 32){
      printf("usage : %s <serial>\n where serial is 16 bytes provided in hex\n ex: %s 00112233445566778899AABBCCDDEEFF",argv[0],argv[0]); 
      return 1;
   }

   // Parsing argv[1]
   // Plaintexts to cipher
   // 15 pT are stored.
   // They are permutations of the input
   // 7 permutations are performed twice
   // 1 identity permutation
   // The xor of the 15 gives the result
   u8 m[NCIPHER][16];
   for(int i = 0; i < 16; i++){
      char hx[2];
      int curHx;
      hx[0] = argv[1][2*i];
      hx[1] = argv[1][2*i+1];
      curHx = strtol(hx,NULL,16);
      for(int j=0; j< NCIPHER; j++){
         m[j][perms[j][i]] = (unsigned char) curHx;
      }
   }

   // init scheduling
   for(int i = 0; i< NCIPHER; i++)
      scheduling[i] = 0;

   // Init masking / Same mask for all the 15 inputs
   for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++){
         u8 r = rand() % 256;
         mask[i][j] = r;
         mask3[i][j] = r;
         shiftedmask[i][j] = r;
      }
   }
   shiftrows(shiftedmask);
   shiftrows(mask3);
   mixcolumns(mask3);
   for(int i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++)
         mask3[i][j] ^= mask[i][j];


   // compute a crc-like of the launched executable
   std::ifstream input( argv[0], std::ios::binary );
   std::vector<unsigned char> buffer((
        std::istreambuf_iterator<char>(input)), 
        (std::istreambuf_iterator<char>()));
   for(int i = 0; i< (int) buffer.size(); i++){
      crc[i%16] ^= buffer[i];
   }

   
   // result buffer
   u8 res[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
   // target 
   u8 exp128[16] = {0x13,0x37,0x69,0x42,0x13,0x37,0x69,0x42,0x13,0x37,0x69,0x42,0x13,0x37,0x69,0x42};

   // Copy the input in states buffer with correct align
   for(int ll=0; ll< NCIPHER; ll++)
      for(int i=0; i<4; ++i)
         for(int j=0; j<4; ++j)
            states[ll][j][i] = m[ll][4*i+j];
   // Trig the computation with random scheduling
   schedule();

   // Pull out the 15 results and xor them to retrieve the result of the AES
   for(int ll=0; ll< NCIPHER; ll++){
      for(int i=0; i<4; ++i){
         for(int j=0; j<4; ++j){
            res[j*4+i] ^= states[ll][i][j];
         }
      }
   }
   
   // Check result and compare
   bool b = true;
   for(int i = 0; i< 16; i++){
      b &= (res[i] == exp128[i]);
   }
   if(b){
      printf("**** Login Successful ****\n");
      return 0;
   }
   else{
      printf("**** Login Failed ****\n");
      return 1;
   }

   return 0;
}
