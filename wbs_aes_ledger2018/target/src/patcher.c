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
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


unsigned char dum[16] = {0x12,0x34,0x56,0x78,0x9a,0xBC,0xDE,0xF0,0x21,0x43,0x65,0x98,0xBA,0xDC,0xFE,0x34};

unsigned char targetcrc[16] = {0x9b,0x04,0x57,0x44,0xe5,0x1f,0xf0,0xf9,0xe8,0x4a,0x54,0xda,0x68,0xe0,0x52,0xb8};

int main(int argc, char** argv) {

   cout << argv[1] << endl;
   std::ifstream input( argv[1], std::ios::binary );
   std::vector<unsigned char> buffer((
        std::istreambuf_iterator<char>(input)), 
        (std::istreambuf_iterator<char>()));

   unsigned char crc[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

   for(int i = 0; i< (int) buffer.size(); i++){
      crc[i%16] ^= buffer[i];
   }
   
   int idx = -1;
   // look for dummy pattern
   for(int i = 0; i < (int)buffer.size() - 16; i++){
      //printf("%x",buffer[i]);
      bool b = true;
      for(int j = 0; j < 16; j++){
         b &= (buffer[i+j] == dum[j]);
      }
      if(b) idx = i;
   }
   // patch it:
   for(int i = 0; i<16;  i++)
      buffer[idx + i] ^= targetcrc[i] ^ crc[i];

    ofstream myFile ("ctf-patched", ios::out | ios::binary);
    myFile.write ((char *)buffer.data(), buffer.size());
    myFile.close();

   return 0;
}
