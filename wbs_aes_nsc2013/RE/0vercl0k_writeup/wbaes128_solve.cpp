/*
    wbaes128_solve.cpp - NoSuchCon 2013 Crackme AES Whitebox brute-forcer.
    Copyright (C) 2013 Axel "0vercl0k" Souchet - http://www.twitter.com/0vercl0k

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Kudos to @elvanderb for that really awesome-tough-motherfuckin-challenge.
*/
#include <windows.h>
#include <stdio.h>
#include "wbaes128.h"

void bf(unsigned char output_wanted[16])
{
    unsigned char memory[876] = {0}, out[148] = {0}, output[16] = {0}, key[16] = {0};
    unsigned char out137_wanted, out140_wanted, out131_wanted, out144_wanted,
        out136_wanted, out146_wanted, out139_wanted, out147_wanted,
        out128_wanted, out135_wanted, out132_wanted, out138_wanted,
        out145_wanted, out143_wanted, out142_wanted, out141_wanted;

    out137_wanted = output_wanted[0xE];
    out140_wanted = output_wanted[0xB];
    out131_wanted = output_wanted[0x4];
    out144_wanted = output_wanted[0x1];
    out136_wanted = output_wanted[0xA];
    out146_wanted = output_wanted[0x7];
    out139_wanted = output_wanted[0x0];
    out147_wanted = output_wanted[0xD];
    out128_wanted = output_wanted[0x3];
    out135_wanted = output_wanted[0xC];
    out132_wanted = output_wanted[0x6];
    out138_wanted = output_wanted[0x9];
    out145_wanted = output_wanted[0x8];
    out143_wanted = output_wanted[0x2];
    out142_wanted = output_wanted[0xF];
    out141_wanted = output_wanted[0x5];

    unsigned char memory826_wanted, memory804_wanted, memory808_wanted, memory768_wanted;
    for(unsigned int i = 0; i < 0x100; ++i)
    {
        memory[826] = i;
        for(unsigned int j = 0; j < 0x100; ++j)
        {
            memory[804] = j;
            for(unsigned int k = 0; k < 0x100; ++k)
            {
                memory[808] = k;
                for(unsigned int l = 0; l < 0x100; ++l)
                {
                    memory[768] = l;

                    //ROUND9-1
                    // Ligne jaune state[0], state[5]
                    memory[827] = T_005CAA61[memory[826]];
                    memory[832] = T_005BDCDA[memory[826]];
                    memory[838] = T_005754C6[memory[826]];
                    memory[843] = TH_0055D0A8[memory[826]];
                    //
                    memory[824] = T_0057C6B8[memory[804]];
                    out[120] = TH_004AEBEB[memory[804]];
                    memory[821] = TH_0051E704[memory[804]];
                    memory[857] = TH_004133D1[memory[804]];
                    // state[8], state[13]
                    memory[809] = T_005E5E79[memory[808]];
                    memory[813] = T_0044BC1D[memory[808]];
                    out[122] = T_0052FA24[memory[808]];
                    out[125] = T_005BC670[memory[808]];
                    //
                    memory[787] = T_004086F7[memory[768]];
                    out[117] = TH_004BD0A5[memory[768]];
                    memory[774] = TH_005E970B[memory[768]];
                    memory[797] = T_00463932[memory[768]];
                    // Ligne orange 1
                    out[133] = T16_0050C7B8[memory[824]][memory[827]];
                    memory[861] = T16_0050C7B8[memory[857]][memory[832]];
                    memory[848] = T16_0050C7B8[memory[821]][memory[838]];
                    memory[859] = T16_0050C7B8[out[120]][memory[843]];
                    //
                    memory[811] = T16_00415EFE[memory[809]][memory[787]];
                    memory[820] = T16_00415EFE[memory[813]][memory[774]];
                    out[123] = T16_00415EFE[out[122]][out[117]];
                    memory[836] = T16_00415EFE[out[125]][memory[797]];
                    // Ligne orange 2
                    memory[860] = T16_004EF23E[out[133]][memory[811]];
                    memory[867] = T16_004EF23E[memory[861]][memory[836]];
                    memory[849] = T16_004EF23E[memory[848]][memory[820]];
                    memory[871] = T16_004EF23E[memory[859]][out[123]];
                    // Ligne jaune
                    out[137] = T_0044116A[memory[860]];
                    out[140] = T_00482BB5[memory[867]];
                    out[131] = T_00493D6C[memory[849]];
                    out[144] = T_00534D4A[memory[871]];

                    if(out[137] == out137_wanted && out[140] == out140_wanted && out[131] == out131_wanted && out[144] == out144_wanted)
                    {
                        memory826_wanted = memory[826];
                        memory804_wanted = memory[804];
                        memory808_wanted = memory[808];
                        memory768_wanted = memory[768];
                        printf("R9-1: DONE.\n");
                        goto next1;
                    }
                }
            }
        }
    }

next1:
    unsigned char memory743_wanted, memory785_wanted, out129_wanted, out113_wanted;

    for(unsigned int i = 0; i < 0x100; ++i)
    {
        memory[743] = i;
        for(unsigned int j = 0; j < 0x100; ++j)
        {
            memory[785] = j;
            for(unsigned int k = 0; k < 0x100; ++k)
            {
                out[129] = k;
                for(unsigned int l = 0; l < 0x100; ++l)
                {
                    out[113] = l;
                    //ROUND9-2
                    // Ligne jaune state[1], state[6]
                    memory[754] = T_0052F77D[memory[743]];
                    out[114] = TH_004451B7[memory[743]];
                    out[115] = T_00487994[memory[743]];
                    memory[766] = T_005EF150[memory[743]];
                    //
                    memory[817] = T_00467517[memory[785]];
                    memory[786] = T_004E93C7[memory[785]];
                    memory[791] = TH_00535295[memory[785]];
                    memory[823] = T_0054AB7F[memory[785]];
                    // state[12], state[11]
                    memory[852] = TH_00495A03[out[129]];
                    memory[850] = T_004C4E3B[out[129]];
                    memory[851] = T_004BC105[out[129]];
                    memory[866] = T_00577802[out[129]];
                    //
                    memory[765] = T_00408D55[out[113]];
                    memory[761] = T_0058DA35[out[113]];
                    memory[764] = T_00533B5B[out[113]];
                    memory[798] = T_005BAE3C[out[113]];
                    // Ligne orange 1
                    out[130] = T16_0050C7B8[memory[817]][memory[754]];
                    memory[794] = T16_0050C7B8[memory[791]][out[114]];
                    memory[834] = T16_0050C7B8[memory[823]][out[115]];
                    memory[819] = T16_0050C7B8[memory[786]][memory[766]];
                    //
                    out[134] = T16_00415EFE[memory[852]][memory[765]];
                    memory[864] = T16_00415EFE[memory[850]][memory[761]];
                    memory[858] = T16_00415EFE[memory[851]][memory[764]];
                    memory[873] = T16_00415EFE[memory[866]][memory[798]];
                    // Ligne orange 2
                    memory[855] = T16_004EF23E[out[130]][out[134]];
                    memory[875] = T16_004EF23E[memory[794]][memory[873]];
                    memory[865] = T16_004EF23E[memory[834]][memory[858]];
                    memory[870] = T16_004EF23E[memory[819]][memory[864]];
                    // Ligne jaune
                    out[136] = TH_004B0C3E[memory[855]];
                    out[146] = T_005BA7D5[memory[875]];
                    out[139] = TH_0049CA9E[memory[865]];
                    out[147] = TH_0048085F[memory[870]];

                    if(out[136] == out136_wanted && out[146] == out146_wanted && out[139] == out139_wanted && out[147] == out147_wanted)
                    {
                        memory743_wanted = memory[743];
                        memory785_wanted = memory[785];
                        out129_wanted = out[129];
                        out113_wanted = out[113];
                        printf("R9-2: DONE.\n");
                        goto next2;
                    }
                }
            }
        }
    }

next2:
    unsigned char memory801_wanted, memory757_wanted, memory781_wanted, out116_wanted;
    for(unsigned int i = 0; i < 0x100; ++i)
    {
        memory[801] = i;
        for(unsigned int j = 0; j < 0x100; ++j)
        {
            memory[757] = j;
            for(unsigned int k = 0; k < 0x100; ++k)
            {
                memory[781] = k;
                for(unsigned int l = 0; l < 0x100; ++l)
                {
                    out[116] = l;
                    //ROUND9-3
                    // Ligne jaune state[2], state[7],
                    memory[814] = TH_00609991[memory[801]];
                    memory[822] = T_005D674C[memory[801]];
                    memory[829] = TH_005C34C7[memory[801]];
                    out[126] = TH_004E3712[memory[801]];
                    //
                    memory[795] = T_005E0595[memory[757]];
                    memory[762] = TH_005DAF94[memory[757]];
                    memory[772] = T_004FFABF[memory[757]];
                    memory[800] = T_0060BD0F[memory[757]];
                    // state[15], state[10]
                    out[121] = T_00602CED[memory[781]];
                    memory[784] = TH_00584F6F[memory[781]];
                    memory[816] = T_00432D08[memory[781]];
                    memory[818] = TH_0060E32A[memory[781]];
                    //
                    memory[780] = TH_005A3F2F[out[116]];
                    out[118] = T_004367BF[out[116]];
                    memory[793] = TH_00490D7F[out[116]];
                    memory[810] = TH_004583DA[out[116]];
                    // Ligne orange 1
                    memory[815] = T16_0050C7B8[memory[814]][memory[795]];
                    memory[833] = T16_0050C7B8[memory[822]][memory[800]];
                    memory[830] = T16_0050C7B8[memory[829]][memory[762]];
                    memory[854] = T16_0050C7B8[out[126]][memory[772]];
                    //
                    memory[825] = T16_00415EFE[out[121]][memory[780]];
                    memory[790] = T16_00415EFE[memory[784]][out[118]];
                    memory[828] = T16_00415EFE[memory[816]][memory[810]];
                    out[124] = T16_00415EFE[memory[818]][memory[793]];
                    // Ligne orange 2
                    out[127] = T16_004EF23E[memory[815]][memory[825]];
                    memory[847] = T16_004EF23E[memory[833]][memory[790]];
                    memory[844] = T16_004EF23E[memory[830]][memory[828]];
                    memory[856] = T16_004EF23E[memory[854]][out[124]];
                    // Ligne jaune
                    out[128] = T_004A5D12[out[127]];
                    out[135] = T_005D1023[memory[847]];
                    out[132] = T_00558431[memory[844]];
                    out[138] = T_005996B4[memory[856]];

                    if(out[128] == out128_wanted && out[135] == out135_wanted && out[132] == out132_wanted && out[138] == out138_wanted)
                    {
                        memory801_wanted = memory[801];
                        memory757_wanted = memory[757];
                        memory781_wanted = memory[781];
                        out116_wanted = out[116];
                        printf("R9-3: DONE.\n");
                        goto next3;
                    }
                }
            }
        }
    }

next3:
    unsigned char memory769_wanted, memory744_wanted, memory837_wanted, memory773_wanted;
    for(unsigned int i = 0; i < 0x100; ++i)
    {
        memory[769] = i;
        for(unsigned int j = 0; j < 0x100; ++j)
        {
            memory[744] = j;
            for(unsigned int k = 0; k < 0x100; ++k)
            {
                memory[837] = k;
                for(unsigned int l = 0; l < 0x100; ++l)
                {
                    memory[773] = l;
                    //ROUND9-4
                    // Ligne jaune state[3], state[4]
                    memory[775] = T_0058D445[memory[769]];
                    memory[782] = T_004C900C[memory[769]];
                    memory[789] = T_00493003[memory[769]];
                    memory[802] = T_00524C3B[memory[769]];
                    //
                    memory[746] = T_0042CC6B[memory[744]];
                    memory[747] = T_005A6860[memory[744]];
                    memory[748] = T_005FD18F[memory[744]];
                    memory[755] = TH_00537B8B[memory[744]];
                    // state[9], state[14]
                    memory[839] = T_005C588B[memory[837]];
                    memory[842] = T_0049AB28[memory[837]];
                    memory[846] = T_00481426[memory[837]];
                    memory[862] = T_0048D491[memory[837]];
                    //
                    memory[776] = T_004915B9[memory[773]];
                    out[119] = T_005CE505[memory[773]];
                    memory[783] = TH_004AF055[memory[773]];
                    memory[803] = T_004A51B1[memory[773]];
                    // Ligne orange 1
                    memory[812] = T16_0050C7B8[memory[775]][memory[748]];
                    memory[788] = T16_0050C7B8[memory[782]][memory[746]];
                    memory[835] = T16_0050C7B8[memory[789]][memory[755]];
                    memory[807] = T16_0050C7B8[memory[802]][memory[747]];
                    //
                    memory[840] = T16_00415EFE[memory[839]][memory[783]];
                    memory[863] = T16_00415EFE[memory[842]][memory[776]];
                    memory[853] = T16_00415EFE[memory[846]][out[119]];
                    memory[868] = T16_00415EFE[memory[862]][memory[803]];
                    // Ligne orange 2
                    memory[874] = T16_004EF23E[memory[812]][memory[853]];
                    memory[872] = T16_004EF23E[memory[788]][memory[868]];
                    memory[869] = T16_004EF23E[memory[835]][memory[863]];
                    memory[845] = T16_004EF23E[memory[807]][memory[840]];
                    // Ligne jaune
                    out[145] = TH_004866A5[memory[874]];
                    out[143] = T_004977CB[memory[872]];
                    out[142] = T_0049635D[memory[869]];
                    out[141] = T_005C2F74[memory[845]];

                    if(out[145] == out145_wanted && out[143] == out143_wanted && out[142] == out142_wanted && out[141] == out141_wanted)
                    {
                        memory769_wanted = memory[769];
                        memory744_wanted = memory[744];
                        memory837_wanted = memory[837];
                        memory773_wanted = memory[773];
                        printf("R9-4: DONE.\n");
                        goto next4;
                    }
                }
            }
        }
    }

next4:
    //R8
    unsigned char memory706_wanted,memory711_wanted,memory699_wanted,memory637_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[706] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[711] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[699] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[637] = l;
                    memory[645] = T_0058CC1C[memory[637]];
                    memory[651] = T_0053FC6A[memory[637]];
                    memory[671] = T_0058AF6B[memory[637]];
                    memory[698] = TH_004CF9B8[memory[637]];
                    memory[701] = T_00597959[memory[699]];
                    memory[702] = T_004897CB[memory[699]];
                    memory[705] = T_005BEFB4[memory[699]];
                    memory[709] = TH_0056FED7[memory[706]];
                    memory[714] = T_004644B5[memory[711]];
                    memory[715] = T_004AC845[memory[711]];
                    memory[716] = T16_0050C7B8[memory[709]][memory[671]];
                    memory[717] = T_00401C80[memory[706]];
                    memory[718] = TH_005449AD[memory[706]];
                    memory[719] = T16_0050C7B8[memory[718]][memory[651]];
                    memory[723] = T_0042F91F[memory[711]];
                    out[108] = T_00583E25[memory[699]];
                    memory[731] = TH_0055FF52[memory[711]];
                    memory[734] = T16_00415EFE[memory[723]][memory[705]];
                    memory[739] = T16_00415EFE[memory[731]][memory[702]];
                    memory[740] = T_00506B9D[memory[706]];
                    out[110] = T16_00415EFE[memory[715]][out[108]];
                    memory[743] = T16_004EF23E[memory[719]][memory[739]];
                    memory[753] = T16_00415EFE[memory[714]][memory[701]];
                    memory[773] = T16_004EF23E[memory[716]][memory[753]];
                    memory[778] = T16_0050C7B8[memory[717]][memory[698]];
                    memory[779] = T16_0050C7B8[memory[740]][memory[645]];
                    memory[781] = T16_004EF23E[memory[779]][memory[734]];
                    memory[804] = T16_004EF23E[memory[778]][out[110]];

                    if(memory[773] == memory773_wanted && memory[804] == memory804_wanted && memory[781] == memory781_wanted && memory[743] == memory743_wanted){
                        memory706_wanted = memory[706];
                        memory711_wanted = memory[711];
                        memory699_wanted = memory[699];
                        memory637_wanted = memory[637];
                        printf("R8-1: DONE.\n");
                        goto next5;
                    }
                }
            }
        }
    }

next5:
    unsigned char memory724_wanted,memory673_wanted,memory667_wanted,memory663_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[724] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[673] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[667] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[663] = l;
                    memory[665] = T_004D2FCB[memory[663]];
                    memory[669] = T_0056EFE7[memory[663]];
                    memory[670] = T_0040608C[memory[667]];
                    memory[676] = TH_005EAAB6[memory[663]];
                    memory[684] = T_004504B1[memory[663]];
                    memory[685] = T_0055DA07[memory[673]];
                    memory[688] = TH_00576681[memory[667]];
                    memory[690] = T_005A19A2[memory[667]];
                    memory[692] = T_0040B6EB[memory[673]];
                    memory[700] = T16_00415EFE[memory[669]][memory[670]];
                    memory[704] = T16_00415EFE[memory[676]][memory[688]];
                    memory[707] = TH_005376FF[memory[667]];
                    out[107] = TH_005CE105[memory[673]];
                    memory[726] = TH_005388B7[memory[673]];
                    memory[727] = T_0048E003[memory[724]];
                    memory[736] = T_0043B5F1[memory[724]];
                    memory[737] = T16_0050C7B8[memory[685]][memory[727]];
                    out[109] = T16_00415EFE[memory[665]][memory[690]];
                    memory[752] = T16_0050C7B8[memory[692]][memory[736]];
                    memory[756] = TH_00575775[memory[724]];
                    memory[757] = T16_004EF23E[memory[737]][memory[704]];
                    memory[758] = TH_004A67C8[memory[724]];
                    memory[763] = T16_00415EFE[memory[684]][memory[707]];
                    memory[767] = T16_0050C7B8[out[107]][memory[758]];
                    memory[768] = T16_004EF23E[memory[752]][out[109]];
                    memory[785] = T16_004EF23E[memory[767]][memory[763]];
                    memory[792] = T16_0050C7B8[memory[726]][memory[756]];
                    memory[837] = T16_004EF23E[memory[792]][memory[700]];

                    if(memory[785] == memory785_wanted && memory[757] == memory757_wanted && memory[837] == memory837_wanted && memory[768] == memory768_wanted){
                        memory724_wanted = memory[724];
                        memory673_wanted = memory[673];
                        memory667_wanted = memory[667];
                        memory663_wanted = memory[663];
                        printf("R8-2: DONE.\n");
                        goto next6;
                    }
                }
            }
        }
    }
next6:
    unsigned char memory730_wanted,out99_wanted,memory675_wanted,memory635_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[730] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            out[99] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[675] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[635] = l;
                    memory[636] = T_004BC3CE[memory[635]];
                    memory[646] = TH_0044A971[memory[635]];
                    memory[654] = T_004CFFC5[memory[635]];
                    memory[660] = T_0046F3BA[memory[635]];
                    memory[680] = T_0057DD21[out[99]];
                    memory[682] = T_0042F6E4[memory[675]];
                    memory[686] = TH_00534279[memory[675]];
                    memory[689] = T_004B5286[out[99]];
                    memory[691] = TH_004C876B[memory[675]];
                    memory[693] = T_0049EFF5[out[99]];
                    memory[696] = TH_004DDF61[memory[675]];
                    memory[703] = T16_00415EFE[memory[693]][memory[636]];
                    out[104] = T16_00415EFE[memory[680]][memory[654]];
                    memory[720] = T_00562CCC[out[99]];
                    memory[722] = T16_00415EFE[memory[720]][memory[646]];
                    memory[732] = T16_00415EFE[memory[689]][memory[660]];
                    memory[735] = T_00451A46[memory[730]];
                    memory[741] = T16_0050C7B8[memory[682]][memory[735]];
                    memory[744] = T16_004EF23E[memory[741]][memory[722]];
                    memory[749] = T_00493F83[memory[730]];
                    memory[759] = TH_005EED05[memory[730]];
                    out[112] = T16_0050C7B8[memory[686]][memory[749]];
                    out[113] = T16_004EF23E[out[112]][memory[732]];
                    memory[770] = TH_00404A9A[memory[730]];
                    memory[777] = T16_0050C7B8[memory[696]][memory[759]];
                    memory[796] = T16_0050C7B8[memory[691]][memory[770]];
                    memory[801] = T16_004EF23E[memory[796]][out[104]];
                    memory[808] = T16_004EF23E[memory[777]][memory[703]];

                    if(out[113] == out113_wanted && memory[744] == memory744_wanted && memory[801] == memory801_wanted && memory[808] == memory808_wanted){
                        memory730_wanted = memory[730];
                        out99_wanted = out[99];
                        memory675_wanted = memory[675];
                        memory635_wanted = memory[635];
                        printf("R8-3: DONE.\n");
                        goto next7;
                    }
                }
            }
        }
    }
next7:
    unsigned char memory725_wanted,memory742_wanted,out101_wanted,memory674_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[725] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[742] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                out[101] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[674] = l;
                    out[100] = TH_0056ADA8[memory[674]];
                    memory[681] = T_00607547[memory[674]];
                    out[102] = T_005DE68F[out[101]];
                    out[103] = T_004087F7[memory[674]];
                    memory[710] = TH_0042B2EF[memory[674]];
                    memory[712] = T16_0050C7B8[memory[710]][out[102]];
                    memory[713] = T_0059C8DF[out[101]];
                    memory[721] = T16_0050C7B8[out[100]][memory[713]];
                    out[106] = TH_005FA43C[out[101]];
                    memory[728] = T_0055FDA6[memory[725]];
                    memory[729] = T_004C6B9A[memory[725]];
                    memory[733] = T16_0050C7B8[memory[681]][out[106]];
                    memory[738] = TH_005D5273[memory[725]];
                    memory[745] = TH_005D371C[memory[742]];
                    memory[750] = T_004E78F1[memory[742]];
                    memory[751] = T16_00415EFE[memory[745]][memory[738]];
                    out[111] = T_0041512B[memory[742]];
                    memory[760] = T16_00415EFE[memory[750]][memory[729]];
                    out[116] = T16_004EF23E[memory[721]][memory[760]];
                    memory[769] = T16_004EF23E[memory[712]][memory[751]];
                    memory[771] = T_00606C68[out[101]];
                    memory[799] = T_0047129C[memory[742]];
                    memory[805] = T16_00415EFE[memory[799]][memory[728]];
                    memory[806] = T16_0050C7B8[out[103]][memory[771]];
                    memory[826] = T16_004EF23E[memory[733]][memory[805]];
                    memory[831] = TH_0058BECA[memory[725]];
                    memory[841] = T16_00415EFE[out[111]][memory[831]];
                    out[129] = T16_004EF23E[memory[806]][memory[841]];

                    if(memory[769] == memory769_wanted && out[129] == out129_wanted && out[116] == out116_wanted && memory[826] == memory826_wanted){
                        memory725_wanted = memory[725];
                        memory742_wanted = memory[742];
                        out101_wanted = out[101];
                        memory674_wanted = memory[674];
                        printf("R8-4: DONE.\n");
                        goto next8;
                    }
                }
            }
        }
    }
next8:
    unsigned char out94_wanted,memory611_wanted,memory559_wanted,memory548_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[94] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[611] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[559] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[548] = l;
                    out[83] = T_005DEAEF[memory[548]];
                    out[84] = TH_0060AD23[memory[548]];
                    out[85] = TH_00575BFD[memory[548]];
                    memory[564] = T_0045C770[memory[559]];
                    memory[568] = TH_004A4B3E[memory[559]];
                    memory[569] = T_0044453C[memory[548]];
                    memory[589] = TH_00432ECE[memory[559]];
                    memory[613] = T_0048B93E[memory[611]];
                    memory[614] = TH_005C86CF[memory[611]];
                    memory[617] = T_0058F67A[memory[559]];
                    memory[624] = T16_00415EFE[memory[614]][out[84]];
                    out[90] = T16_00415EFE[memory[613]][memory[569]];
                    out[93] = TH_005D7C8F[memory[611]];
                    memory[641] = TH_00532754[memory[611]];
                    out[95] = T16_00415EFE[memory[641]][out[85]];
                    memory[661] = TH_0056746B[out[94]];
                    out[96] = T16_0050C7B8[memory[568]][memory[661]];
                    memory[666] = T_005DD256[out[94]];
                    memory[673] = T16_004EF23E[out[96]][out[95]];
                    memory[679] = T_004ACD42[out[94]];
                    memory[687] = T16_0050C7B8[memory[589]][memory[679]];
                    memory[694] = TH_005B4BDF[out[94]];
                    memory[695] = T16_0050C7B8[memory[617]][memory[694]];
                    memory[697] = T16_00415EFE[out[93]][out[83]];
                    memory[699] = T16_004EF23E[memory[695]][memory[697]];
                    memory[708] = T16_0050C7B8[memory[564]][memory[666]];
                    memory[730] = T16_004EF23E[memory[687]][out[90]];
                    memory[742] = T16_004EF23E[memory[708]][memory[624]];

                    if(memory[730] == memory730_wanted && memory[673] == memory673_wanted && memory[742] == memory742_wanted && memory[699] == memory699_wanted){
                        out94_wanted = out[94];
                        memory611_wanted = memory[611];
                        memory559_wanted = memory[559];
                        memory548_wanted = memory[548];
                        printf("R7-1: DONE.\n");
                        goto next9;
                    }
                }
            }
        }
    }

next9:
    unsigned char memory644_wanted,memory583_wanted,memory574_wanted,memory552_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[644] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[583] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[574] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[552] = l;
                    memory[554] = T_004EDB4B[memory[552]];
                    memory[560] = T_0047EE61[memory[552]];
                    memory[561] = T_00403870[memory[552]];
                    memory[562] = TH_00509804[memory[552]];
                    memory[577] = T_004A4174[memory[574]];
                    memory[579] = T16_0050C7B8[memory[577]][memory[561]];
                    memory[586] = TH_004072D4[memory[583]];
                    memory[587] = T_00544F66[memory[574]];
                    memory[591] = T16_0050C7B8[memory[587]][memory[560]];
                    memory[594] = T_0048DAA1[memory[583]];
                    memory[598] = T_00540038[memory[583]];
                    memory[600] = T_0051E414[memory[574]];
                    memory[606] = T16_0050C7B8[memory[600]][memory[554]];
                    out[89] = T_005FF9F3[memory[574]];
                    memory[618] = T_005CC7EF[memory[583]];
                    memory[628] = T16_0050C7B8[out[89]][memory[562]];
                    memory[648] = T_0047E845[memory[644]];
                    memory[649] = T_0054A7E2[memory[644]];
                    memory[650] = T_00474CC1[memory[644]];
                    memory[652] = T_004AE413[memory[644]];
                    memory[658] = T16_00415EFE[memory[652]][memory[586]];
                    memory[662] = T16_00415EFE[memory[650]][memory[594]];
                    out[97] = T16_00415EFE[memory[649]][memory[598]];
                    memory[667] = T16_004EF23E[memory[606]][out[97]];
                    memory[675] = T16_004EF23E[memory[628]][memory[662]];
                    memory[678] = T16_00415EFE[memory[648]][memory[618]];
                    out[101] = T16_004EF23E[memory[579]][memory[658]];
                    memory[711] = T16_004EF23E[memory[591]][memory[678]];

                    if(memory[675] == memory675_wanted && memory[711] == memory711_wanted && out[101] == out101_wanted && memory[667] == memory667_wanted){
                        memory644_wanted = memory[644];
                        memory583_wanted = memory[583];
                        memory574_wanted = memory[574];
                        memory552_wanted = memory[552];
                        printf("R7-2: DONE.\n");
                        goto next10;
                    }
                }
            }
        }
    }

next10:
    unsigned char out91_wanted,memory608_wanted,memory570_wanted,memory524_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[91] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[608] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[570] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[524] = l;
                    memory[525] = T_00447776[memory[524]];
                    memory[540] = T_004ACA7D[memory[524]];
                    memory[544] = T_0046F733[memory[524]];
                    memory[571] = T_005C333B[memory[570]];
                    memory[573] = T_004575BB[memory[524]];
                    memory[581] = TH_00428681[memory[570]];
                    memory[582] = T_00591DD0[memory[570]];
                    memory[588] = T_00406B98[memory[570]];
                    memory[610] = T_0053E57B[memory[608]];
                    memory[612] = T16_0050C7B8[memory[610]][memory[525]];
                    memory[615] = T_0057B697[memory[608]];
                    memory[619] = T_00571090[memory[608]];
                    memory[620] = T16_0050C7B8[memory[619]][memory[573]];
                    memory[621] = T16_0050C7B8[memory[615]][memory[544]];
                    memory[626] = T_0054FA90[memory[608]];
                    memory[627] = T_005F80BC[out[91]];
                    memory[629] = T_00550DB7[out[91]];
                    memory[630] = TH_00412FD1[out[91]];
                    memory[631] = T16_00415EFE[memory[571]][memory[629]];
                    memory[632] = T16_00415EFE[memory[582]][memory[630]];
                    memory[633] = T16_0050C7B8[memory[626]][memory[540]];
                    memory[635] = T16_004EF23E[memory[612]][memory[631]];
                    memory[637] = T16_004EF23E[memory[620]][memory[632]];
                    memory[642] = T_005EE8F9[out[91]];
                    memory[656] = T16_00415EFE[memory[588]][memory[642]];
                    memory[663] = T16_004EF23E[memory[633]][memory[656]];
                    memory[672] = T16_00415EFE[memory[581]][memory[627]];
                    memory[674] = T16_004EF23E[memory[621]][memory[672]];

                    if(memory[637] == memory637_wanted && memory[663] == memory663_wanted && memory[635] == memory635_wanted && memory[674] == memory674_wanted){
                        out91_wanted = out[91];
                        memory608_wanted = memory[608];
                        memory570_wanted = memory[570];
                        memory524_wanted = memory[524];
                        printf("R7-3: DONE.\n");
                        goto next11;
                    }
                }
            }
        }
    }

next11:
    unsigned char memory655_wanted,memory634_wanted,memory593_wanted,memory551_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[655] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[634] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[593] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[551] = l;
                    memory[553] = T_0052BCAF[memory[551]];
                    memory[555] = T_004BB307[memory[551]];
                    memory[558] = TH_00451B46[memory[551]];
                    memory[572] = TH_0045BAA9[memory[551]];
                    memory[599] = T_0055300B[memory[593]];
                    memory[602] = T_004D8DDE[memory[593]];
                    memory[603] = T_005736DC[memory[593]];
                    memory[605] = T_004CAD58[memory[593]];
                    memory[607] = T16_00415EFE[memory[572]][memory[602]];
                    memory[625] = T16_00415EFE[memory[553]][memory[605]];
                    out[92] = T16_00415EFE[memory[558]][memory[603]];
                    memory[638] = T_00408530[memory[634]];
                    memory[639] = TH_00536692[memory[634]];
                    memory[643] = TH_00462A6A[memory[634]];
                    memory[647] = T_0056066B[memory[634]];
                    memory[653] = T16_00415EFE[memory[555]][memory[599]];
                    memory[657] = T_0042E929[memory[655]];
                    memory[659] = T16_0050C7B8[memory[657]][memory[647]];
                    memory[664] = TH_0049DCAE[memory[655]];
                    out[98] = T16_0050C7B8[memory[664]][memory[639]];
                    memory[668] = T_00563339[memory[655]];
                    out[99] = T16_004EF23E[out[98]][memory[607]];
                    memory[677] = T16_0050C7B8[memory[668]][memory[643]];
                    memory[683] = T_005520E4[memory[655]];
                    memory[706] = T16_004EF23E[memory[659]][memory[625]];
                    out[105] = T16_0050C7B8[memory[683]][memory[638]];
                    memory[724] = T16_004EF23E[out[105]][memory[653]];
                    memory[725] = T16_004EF23E[memory[677]][out[92]];

                    if(out[99] == out99_wanted && memory[706] == memory706_wanted && memory[725] == memory725_wanted && memory[724] == memory724_wanted){
                        memory655_wanted = memory[655];
                        memory634_wanted = memory[634];
                        memory593_wanted = memory[593];
                        memory551_wanted = memory[551];
                        printf("R7-4: DONE.\n");
                        goto next12;
                    }
                }
            }
        }
    }
next12:
    unsigned char out86_wanted,out82_wanted,memory526_wanted,memory438_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[86] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            out[82] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[526] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[438] = l;
                    memory[443] = T_0057869A[memory[438]];
                    memory[444] = TH_005D05A8[memory[438]];
                    memory[445] = T_00615EFA[memory[438]];
                    memory[465] = T_005E3418[memory[438]];
                    memory[535] = TH_005512C2[memory[526]];
                    memory[536] = T_0058048E[memory[526]];
                    memory[546] = TH_0051D10B[memory[526]];
                    memory[550] = TH_004E1D3C[out[82]];
                    memory[567] = TH_0061171D[out[82]];
                    memory[575] = T_004CE14D[out[86]];
                    out[87] = T16_00415EFE[memory[575]][memory[465]];
                    memory[578] = T_0049AF5C[out[82]];
                    memory[580] = T_004D348E[out[86]];
                    memory[584] = T_005C25A0[memory[526]];
                    memory[585] = T16_0050C7B8[memory[550]][memory[546]];
                    memory[592] = T16_0050C7B8[memory[567]][memory[584]];
                    memory[596] = T_0045E969[out[86]];
                    memory[597] = T_005B988F[out[82]];
                    memory[604] = T16_00415EFE[memory[596]][memory[445]];
                    memory[609] = T16_0050C7B8[memory[578]][memory[535]];
                    memory[616] = T_00456329[out[86]];
                    memory[622] = T16_00415EFE[memory[580]][memory[443]];
                    memory[623] = T16_00415EFE[memory[616]][memory[444]];
                    out[91] = T16_004EF23E[memory[609]][memory[622]];
                    memory[640] = T16_0050C7B8[memory[597]][memory[536]];
                    memory[644] = T16_004EF23E[memory[640]][out[87]];
                    out[94] = T16_004EF23E[memory[585]][memory[604]];
                    memory[655] = T16_004EF23E[memory[592]][memory[623]];

                    if(out[94] == out94_wanted && memory[644] == memory644_wanted && out[91] == out91_wanted && memory[655] == memory655_wanted){
                        out86_wanted = out[86];
                        out82_wanted = out[82];
                        memory526_wanted = memory[526];
                        memory438_wanted = memory[438];
                        printf("R6-1: DONE.\n");
                        goto next13;
                    }
                }
            }
        }
    }
next13:
    unsigned char out73_wanted,memory489_wanted,memory472_wanted,out68_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[73] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[489] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[472] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    out[68] = l;
                    out[69] = TH_004E2F62[out[68]];
                    memory[467] = TH_00526850[out[68]];
                    memory[468] = T_005266E5[out[68]];
                    memory[474] = T_00562A1B[memory[472]];
                    memory[475] = TH_00466CAF[memory[472]];
                    memory[476] = T_0048F7D0[out[68]];
                    memory[482] = TH_0059234C[memory[472]];
                    memory[497] = TH_004D06C9[memory[472]];
                    memory[502] = T_004022E4[out[73]];
                    memory[503] = TH_0057E0F3[memory[489]];
                    memory[507] = T_004D32C1[out[73]];
                    memory[510] = T_00506D45[memory[489]];
                    out[75] = T16_0050C7B8[memory[507]][memory[482]];
                    memory[513] = T16_0050C7B8[memory[502]][memory[474]];
                    memory[516] = T_005BE44A[memory[489]];
                    memory[520] = T_005DDD0F[memory[489]];
                    memory[521] = T16_00415EFE[memory[510]][memory[467]];
                    memory[522] = T16_00415EFE[memory[516]][memory[468]];
                    memory[532] = T_0040FAF9[out[73]];
                    memory[539] = T16_0050C7B8[memory[532]][memory[475]];
                    memory[541] = T_005C8BD0[out[73]];
                    memory[545] = T16_0050C7B8[memory[541]][memory[497]];
                    memory[552] = T16_004EF23E[memory[539]][memory[522]];
                    memory[556] = T16_00415EFE[memory[503]][out[69]];
                    memory[559] = T16_004EF23E[memory[545]][memory[521]];
                    memory[565] = T16_00415EFE[memory[520]][memory[476]];
                    memory[570] = T16_004EF23E[memory[513]][memory[556]];
                    memory[593] = T16_004EF23E[out[75]][memory[565]];

                    if(memory[552] == memory552_wanted && memory[559] == memory559_wanted && memory[593] == memory593_wanted && memory[570] == memory570_wanted){
                        out73_wanted = out[73];
                        memory489_wanted = memory[489];
                        memory472_wanted = memory[472];
                        out68_wanted = out[68];
                        printf("R6-1: DONE.\n");
                        goto next14;
                    }
                }
            }
        }
    }
next14:
    unsigned char memory494_wanted,out63_wanted,memory492_wanted,memory478_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[494] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            out[63] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[492] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[478] = l;
                    memory[436] = T_00411850[out[63]];
                    memory[463] = T_00586378[out[63]];
                    memory[473] = T_0049763E[out[63]];
                    memory[486] = T_0042E454[memory[478]];
                    memory[496] = T_004B73DA[memory[492]];
                    memory[498] = T_004B8D39[memory[494]];
                    memory[499] = T_005A6E0C[memory[492]];
                    memory[501] = TH_0050082B[memory[478]];
                    memory[504] = T_004D1070[memory[494]];
                    memory[505] = T_00468A34[memory[478]];
                    memory[506] = T_0045A3D0[memory[478]];
                    memory[509] = T_00585D47[memory[492]];
                    memory[511] = T_004B985B[memory[492]];
                    memory[512] = T16_0050C7B8[memory[506]][memory[463]];
                    memory[514] = T_0057B1F7[memory[494]];
                    memory[515] = T_0040898F[out[63]];
                    memory[518] = T16_00415EFE[memory[498]][memory[511]];
                    memory[519] = T_004E524B[memory[494]];
                    out[76] = T16_0050C7B8[memory[486]][memory[436]];
                    memory[524] = T16_004EF23E[memory[512]][memory[518]];
                    memory[528] = T16_0050C7B8[memory[505]][memory[515]];
                    memory[533] = T16_00415EFE[memory[519]][memory[499]];
                    memory[537] = T16_00415EFE[memory[514]][memory[509]];
                    memory[543] = T16_0050C7B8[memory[501]][memory[473]];
                    memory[547] = T16_00415EFE[memory[504]][memory[496]];
                    memory[548] = T16_004EF23E[memory[528]][memory[547]];
                    memory[551] = T16_004EF23E[out[76]][memory[533]];
                    memory[574] = T16_004EF23E[memory[543]][memory[537]];

                    if(memory[574] == memory574_wanted && memory[551] == memory551_wanted && memory[548] == memory548_wanted && memory[524] == memory524_wanted){
                        memory494_wanted = memory[494];
                        out63_wanted = out[63];
                        memory492_wanted = memory[492];
                        memory478_wanted = memory[478];
                        printf("R6-3: DONE.\n");
                        goto next15;
                    }
                }
            }
        }
    }
next15:
    unsigned char out78_wanted,memory527_wanted,memory481_wanted,memory458_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[78] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[527] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[481] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[458] = l;
                    memory[459] = TH_0057074D[memory[458]];
                    memory[466] = TH_0050B219[memory[458]];
                    out[72] = T_005AC690[memory[458]];
                    memory[485] = TH_004E5B97[memory[481]];
                    memory[488] = T_005AE24F[memory[481]];
                    memory[493] = T16_00415EFE[memory[459]][memory[485]];
                    memory[495] = TH_004B19F5[memory[481]];
                    memory[523] = T16_00415EFE[memory[466]][memory[495]];
                    out[79] = T_0044C23E[out[78]];
                    memory[529] = T_00461EEF[memory[527]];
                    memory[530] = TH_0058A368[memory[458]];
                    out[80] = T_004967DE[memory[481]];
                    out[81] = TH_004AA14A[memory[527]];
                    memory[534] = TH_0048ADD9[out[78]];
                    memory[538] = T_0043E179[memory[527]];
                    memory[542] = T16_00415EFE[memory[530]][memory[488]];
                    memory[557] = T16_0050C7B8[out[79]][memory[538]];
                    memory[563] = T16_00415EFE[out[72]][out[80]];
                    memory[566] = T16_0050C7B8[memory[534]][out[81]];
                    memory[576] = T_0045A1DD[memory[527]];
                    out[88] = T_004B4D42[out[78]];
                    memory[583] = T16_004EF23E[memory[557]][memory[493]];
                    memory[590] = T_005D5C65[out[78]];
                    memory[595] = T16_0050C7B8[memory[590]][memory[529]];
                    memory[601] = T16_0050C7B8[out[88]][memory[576]];
                    memory[608] = T16_004EF23E[memory[566]][memory[523]];
                    memory[611] = T16_004EF23E[memory[595]][memory[563]];
                    memory[634] = T16_004EF23E[memory[601]][memory[542]];

                    if(memory[608] == memory608_wanted && memory[634] == memory634_wanted && memory[583] == memory583_wanted && memory[611] == memory611_wanted){
                        out78_wanted = out[78];
                        memory527_wanted = memory[527];
                        memory481_wanted = memory[481];
                        memory458_wanted = memory[458];
                        printf("R6-4: DONE.\n");
                        goto next16;
                    }
                }
            }
        }
    }
next16:

    unsigned char out65_wanted,out61_wanted,memory375_wanted,memory405_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[65] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            out[61] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[375] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[405] = l;
                    out[51] = T_0043465C[memory[375]];
                    out[58] = T_0048E52A[memory[375]];
                    out[59] = T_0055DFF0[memory[375]];
                    memory[407] = T_004B0474[memory[405]];
                    memory[409] = T_00455281[memory[405]];
                    memory[410] = T_004CDF34[memory[405]];
                    memory[412] = T_004E1386[memory[405]];
                    memory[432] = TH_004140DE[out[61]];
                    memory[435] = TH_0046FE7A[out[61]];
                    memory[439] = TH_0047B8D1[out[61]];
                    out[64] = T_0043A044[memory[375]];
                    memory[446] = T16_00415EFE[out[51]][memory[432]];
                    memory[447] = T_0047C9A1[out[61]];
                    memory[453] = T16_00415EFE[out[58]][memory[439]];
                    memory[455] = T_004BDEF0[out[65]];
                    out[66] = T16_0050C7B8[memory[455]][memory[409]];
                    memory[460] = T_004E7093[out[65]];
                    memory[470] = T16_00415EFE[out[64]][memory[435]];
                    memory[484] = T16_00415EFE[out[59]][memory[447]];
                    memory[487] = T_004457FF[out[65]];
                    memory[489] = T16_004EF23E[out[66]][memory[484]];
                    memory[491] = T16_0050C7B8[memory[487]][memory[412]];
                    memory[492] = T16_004EF23E[memory[491]][memory[453]];
                    memory[500] = T16_0050C7B8[memory[460]][memory[410]];
                    memory[517] = TH_0042BB10[out[65]];
                    out[77] = T16_0050C7B8[memory[517]][memory[407]];
                    out[78] = T16_004EF23E[out[77]][memory[470]];
                    memory[526] = T16_004EF23E[memory[500]][memory[446]];

                    if(out[78] == out78_wanted && memory[489] == memory489_wanted && memory[526] == memory526_wanted && memory[492] == memory492_wanted){
                        out65_wanted = out[65];
                        out61_wanted = out[61];
                        memory375_wanted = memory[375];
                        memory405_wanted = memory[405];
                        printf("R5-1: DONE.\n");
                        goto next17;
                    }
                }
            }
        }
    }
next17:
    unsigned char out56_wanted,memory400_wanted,memory402_wanted,memory374_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[56] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[400] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[402] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[374] = l;
                    memory[376] = T_00590BF8[memory[374]];
                    memory[388] = T_004AC243[memory[374]];
                    memory[395] = T_00595042[memory[374]];
                    memory[406] = T_00567088[memory[400]];
                    memory[411] = T_004AACCC[memory[402]];
                    memory[413] = T_005D03FD[memory[400]];
                    memory[415] = T16_0050C7B8[memory[413]][memory[376]];
                    memory[416] = T_0052A590[memory[400]];
                    memory[417] = TH_0045CE4E[out[56]];
                    memory[418] = T_00471AE2[memory[374]];
                    memory[419] = T_0050A4E5[memory[402]];
                    memory[423] = TH_00431F0C[out[56]];
                    memory[424] = T_0046CFB3[memory[402]];
                    memory[425] = T16_0050C7B8[memory[406]][memory[388]];
                    memory[426] = TH_004059EB[out[56]];
                    memory[427] = T16_00415EFE[memory[411]][memory[426]];
                    memory[428] = T16_00415EFE[memory[419]][memory[423]];
                    memory[429] = T16_00415EFE[memory[424]][memory[417]];
                    out[63] = T16_004EF23E[memory[425]][memory[428]];
                    memory[438] = T16_004EF23E[memory[415]][memory[427]];
                    memory[441] = TH_00597442[memory[402]];
                    memory[451] = T_005D4BF9[memory[400]];
                    memory[457] = T16_0050C7B8[memory[416]][memory[418]];
                    memory[458] = T16_004EF23E[memory[457]][memory[429]];
                    memory[462] = T_0050AF89[out[56]];
                    memory[477] = T16_00415EFE[memory[441]][memory[462]];
                    memory[483] = T16_0050C7B8[memory[451]][memory[395]];
                    out[73] = T16_004EF23E[memory[483]][memory[477]];

                    if(memory[438] == memory438_wanted && out[63] == out63_wanted && out[73] == out73_wanted && memory[458] == memory458_wanted){
                        out56_wanted = out[56];
                        memory400_wanted = memory[400];
                        memory402_wanted = memory[402];
                        memory374_wanted = memory[374];
                        printf("R5-2: DONE.\n");
                        goto next18;
                    }
                }
            }
        }
    }

next18:
    unsigned char memory430_wanted,memory391_wanted,memory385_wanted,memory365_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[430] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[391] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[385] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[365] = l;
                    memory[370] = TH_0049E8FD[memory[365]];
                    memory[377] = TH_005E929C[memory[365]];
                    memory[382] = T_00566C35[memory[365]];
                    memory[390] = T_00494CC6[memory[385]];
                    memory[392] = TH_00445D16[memory[391]];
                    memory[393] = TH_005729D8[memory[391]];
                    memory[394] = T16_0050C7B8[memory[377]][memory[390]];
                    memory[396] = TH_005B25D2[memory[391]];
                    memory[399] = T_0053BEA6[memory[365]];
                    out[60] = T_005E2680[memory[385]];
                    memory[420] = TH_0043627D[memory[385]];
                    memory[421] = T_00590054[memory[385]];
                    memory[431] = T_0044961E[memory[430]];
                    memory[433] = TH_0052502C[memory[430]];
                    memory[434] = T16_00415EFE[memory[396]][memory[433]];
                    memory[440] = T16_00415EFE[memory[393]][memory[431]];
                    memory[450] = T_005755C6[memory[391]];
                    memory[454] = T16_0050C7B8[memory[382]][out[60]];
                    out[68] = T16_004EF23E[memory[454]][memory[434]];
                    out[70] = T_0061373C[memory[430]];
                    memory[471] = T16_0050C7B8[memory[399]][memory[420]];
                    memory[478] = T16_004EF23E[memory[471]][memory[440]];
                    memory[479] = T16_0050C7B8[memory[370]][memory[421]];
                    out[74] = TH_004AFA74[memory[430]];
                    memory[508] = T16_00415EFE[memory[450]][out[70]];
                    memory[527] = T16_004EF23E[memory[394]][memory[508]];
                    memory[549] = T16_00415EFE[memory[392]][out[74]];
                    out[86] = T16_004EF23E[memory[479]][memory[549]];

                    if(out[86] == out86_wanted && memory[478] == memory478_wanted && memory[527] == memory527_wanted && out[68] == out68_wanted){
                        memory430_wanted = memory[430];
                        memory391_wanted = memory[391];
                        memory385_wanted = memory[385];
                        memory365_wanted = memory[365];
                        printf("R5-3: DONE.\n");
                        goto next19;
                    }
                }
            }
        }
    }
next19:
    unsigned char memory449_wanted,out50_wanted,memory401_wanted,memory342_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[449] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            out[50] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[401] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[342] = l;
                    memory[343] = T_00503DEB[memory[342]];
                    memory[346] = T_005524BB[memory[342]];
                    memory[349] = TH_00546068[memory[342]];
                    memory[357] = T_005089E2[memory[342]];
                    memory[373] = T_005F8715[out[50]];
                    out[52] = T_004B6728[out[50]];
                    out[54] = T16_00415EFE[memory[373]][memory[349]];
                    memory[387] = T_004C2FA0[out[50]];
                    memory[389] = T16_00415EFE[memory[387]][memory[357]];
                    memory[403] = T_00531E4C[memory[401]];
                    memory[404] = T_00456878[memory[401]];
                    memory[408] = T16_00415EFE[out[52]][memory[343]];
                    memory[414] = T_005F9DCB[memory[401]];
                    memory[442] = T_0058A86E[memory[401]];
                    memory[452] = T_00454971[memory[449]];
                    memory[456] = T_0048EEA9[memory[449]];
                    out[67] = T16_0050C7B8[memory[404]][memory[456]];
                    memory[461] = T_005E883D[memory[449]];
                    memory[464] = T16_0050C7B8[memory[403]][memory[452]];
                    memory[469] = T16_0050C7B8[memory[442]][memory[461]];
                    memory[472] = T16_004EF23E[memory[464]][out[54]];
                    memory[480] = T_005BEA12[out[50]];
                    memory[481] = T16_004EF23E[out[67]][memory[408]];
                    out[71] = T16_00415EFE[memory[480]][memory[346]];
                    memory[490] = T_005FD54E[memory[449]];
                    memory[494] = T16_004EF23E[memory[469]][memory[389]];
                    memory[531] = T16_0050C7B8[memory[414]][memory[490]];
                    out[82] = T16_004EF23E[memory[531]][out[71]];

                    if(memory[481] == memory481_wanted && memory[494] == memory494_wanted && out[82] == out82_wanted && memory[472] == memory472_wanted){
                        memory449_wanted = memory[449];
                        out50_wanted = out[50];
                        memory401_wanted = memory[401];
                        memory342_wanted = memory[342];
                        printf("R5-4: DONE.\n");
                        goto next20;
                    }
                }
            }
        }
    }
next20:
    unsigned char memory362_wanted,out36_wanted,memory289_wanted,memory297_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[362] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            out[36] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[289] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[297] = l;
                    out[33] = T_004E0EE9[memory[289]];
                    memory[291] = TH_0058E478[memory[289]];
                    out[35] = T_0047DC4C[memory[289]];
                    memory[306] = T_00435CA2[memory[297]];
                    memory[307] = TH_00440BA0[memory[297]];
                    memory[312] = TH_00429661[memory[297]];
                    out[39] = T_00612B48[out[36]];
                    memory[319] = T16_00415EFE[memory[306]][out[35]];
                    memory[321] = T_004A9510[out[36]];
                    memory[324] = TH_005B9E28[memory[297]];
                    out[42] = TH_00476B82[memory[289]];
                    memory[327] = T_005845E6[out[36]];
                    memory[328] = T_005BACEF[out[36]];
                    memory[330] = T16_00415EFE[memory[312]][out[42]];
                    memory[337] = T16_00415EFE[memory[324]][memory[291]];
                    memory[348] = T16_00415EFE[memory[307]][out[33]];
                    out[49] = T_0042CA59[memory[362]];
                    memory[371] = T_00441B5F[memory[362]];
                    memory[372] = T16_0050C7B8[memory[371]][memory[321]];
                    memory[374] = T16_004EF23E[memory[372]][memory[337]];
                    memory[378] = T_005C1377[memory[362]];
                    memory[379] = T_0050B6CB[memory[362]];
                    memory[386] = T16_0050C7B8[memory[378]][memory[327]];
                    memory[391] = T16_004EF23E[memory[386]][memory[330]];
                    memory[398] = T16_0050C7B8[memory[379]][out[39]];
                    out[57] = T16_0050C7B8[out[49]][memory[328]];
                    memory[401] = T16_004EF23E[memory[398]][memory[348]];
                    out[61] = T16_004EF23E[out[57]][memory[319]];

                    if(memory[401] == memory401_wanted && out[61] == out61_wanted && memory[374] == memory374_wanted && memory[391] == memory391_wanted){
                        memory362_wanted = memory[362];
                        out36_wanted = out[36];
                        memory289_wanted = memory[289];
                        memory297_wanted = memory[297];
                        printf("R4-1: DONE.\n");
                        goto next21;
                    }
                }
            }
        }
    }

next21:
    unsigned char memory341_wanted,memory314_wanted,out34_wanted,memory311_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[341] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[314] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                out[34] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[311] = l;
                    memory[313] = TH_0045221F[memory[311]];
                    out[38] = T_0052750E[memory[311]];
                    memory[316] = T_005D88E5[out[34]];
                    memory[317] = T_00574BF5[memory[314]];
                    memory[322] = T_004AB89F[out[34]];
                    memory[323] = T_00507B3A[memory[314]];
                    memory[325] = T_00505359[memory[311]];
                    memory[331] = TH_0053159F[memory[311]];
                    memory[332] = TH_0052151A[out[34]];
                    memory[333] = T16_00415EFE[memory[331]][memory[316]];
                    memory[334] = TH_0060B819[out[34]];
                    memory[336] = T16_00415EFE[memory[313]][memory[332]];
                    memory[340] = T16_00415EFE[memory[325]][memory[334]];
                    memory[344] = T_005BC40E[memory[314]];
                    out[45] = T_0050C539[memory[314]];
                    memory[350] = T_00467E37[memory[341]];
                    memory[353] = T16_0050C7B8[memory[350]][out[45]];
                    memory[356] = T_0058FC77[memory[341]];
                    memory[358] = T16_00415EFE[out[38]][memory[322]];
                    out[48] = TH_00579905[memory[341]];
                    memory[360] = T16_0050C7B8[out[48]][memory[323]];
                    memory[364] = T16_0050C7B8[memory[356]][memory[317]];
                    memory[365] = T16_004EF23E[memory[353]][memory[340]];
                    memory[368] = TH_00443980[memory[341]];
                    out[50] = T16_004EF23E[memory[360]][memory[333]];
                    memory[384] = T16_0050C7B8[memory[368]][memory[344]];
                    out[56] = T16_004EF23E[memory[364]][memory[358]];
                    memory[405] = T16_004EF23E[memory[384]][memory[336]];

                    if(out[56] == out56_wanted && out[50] == out50_wanted && memory[405] == memory405_wanted && memory[365] == memory365_wanted){
                        memory341_wanted = memory[341];
                        memory314_wanted = memory[314];
                        out34_wanted = out[34];
                        memory311_wanted = memory[311];
                        printf("R4-2: DONE.\n");
                        goto next22;
                    }
                }
            }
        }
    }
next22:
    unsigned char out40_wanted,memory280_wanted,memory284_wanted,memory249_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        out[40] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[280] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[284] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[249] = l;
                    memory[250] = TH_0043344D[memory[249]];
                    memory[255] = TH_004592D9[memory[249]];
                    memory[258] = T_004AA81E[memory[249]];
                    memory[263] = TH_00564E89[memory[249]];
                    memory[282] = TH_00453254[memory[280]];
                    memory[286] = T_00441529[memory[284]];
                    memory[287] = TH_004B25ED[memory[284]];
                    memory[290] = T_004C74F4[memory[284]];
                    memory[299] = TH_004A37BF[memory[284]];
                    memory[302] = T_005BD923[memory[280]];
                    memory[303] = T16_00415EFE[memory[282]][memory[290]];
                    out[37] = T_00404090[memory[280]];
                    memory[315] = T16_00415EFE[out[37]][memory[287]];
                    memory[318] = T16_00415EFE[memory[302]][memory[286]];
                    out[41] = TH_005519E3[out[40]];
                    memory[329] = T16_0050C7B8[memory[255]][out[41]];
                    memory[335] = TH_0060919E[out[40]];
                    memory[342] = T16_004EF23E[memory[329]][memory[318]];
                    memory[351] = T_0059DFCA[out[40]];
                    out[47] = T_004122F3[memory[280]];
                    memory[366] = T16_00415EFE[out[47]][memory[299]];
                    memory[367] = T16_0050C7B8[memory[263]][memory[335]];
                    memory[369] = T16_0050C7B8[memory[250]][memory[351]];
                    memory[375] = T16_004EF23E[memory[367]][memory[366]];
                    out[53] = T_00412DB0[out[40]];
                    memory[385] = T16_004EF23E[memory[369]][memory[315]];
                    memory[397] = T16_0050C7B8[memory[258]][out[53]];
                    memory[400] = T16_004EF23E[memory[397]][memory[303]];

                    if(memory[342] == memory342_wanted && memory[400] == memory400_wanted && memory[375] == memory375_wanted && memory[385] == memory385_wanted){
                        out40_wanted = out[40];
                        memory280_wanted = memory[280];
                        memory284_wanted = memory[284];
                        memory249_wanted = memory[249];
                        printf("R4-3: DONE.\n");
                        goto next23;
                    }
                }
            }
        }
    }
next23:
    unsigned char memory354_wanted,memory326_wanted,memory283_wanted,memory288_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[354] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[326] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[283] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[288] = l;
                    memory[293] = TH_0040C767[memory[283]];
                    memory[295] = T_00541346[memory[288]];
                    memory[296] = T_00606A9F[memory[288]];
                    memory[300] = T_005A9BBC[memory[283]];
                    memory[304] = T_0044218C[memory[288]];
                    memory[308] = T_00466985[memory[288]];
                    memory[309] = TH_00497EBD[memory[283]];
                    memory[320] = TH_00469A81[memory[283]];
                    out[43] = T_005B0C5F[memory[326]];
                    out[44] = TH_004933A7[memory[326]];
                    memory[345] = T16_0050C7B8[out[43]][memory[309]];
                    memory[347] = T_004C21E9[memory[326]];
                    memory[352] = T16_0050C7B8[out[44]][memory[300]];
                    memory[359] = T_005618D3[memory[354]];
                    memory[361] = T_005C0F53[memory[326]];
                    memory[363] = T16_0050C7B8[memory[361]][memory[320]];
                    memory[380] = T16_0050C7B8[memory[347]][memory[293]];
                    memory[381] = T16_00415EFE[memory[295]][memory[359]];
                    memory[383] = T_005F19FE[memory[354]];
                    out[55] = T_004BF574[memory[354]];
                    memory[402] = T16_004EF23E[memory[345]][memory[381]];
                    memory[422] = T_0043D247[memory[354]];
                    out[62] = T16_00415EFE[memory[308]][memory[422]];
                    memory[430] = T16_004EF23E[memory[363]][out[62]];
                    memory[437] = T16_00415EFE[memory[296]][memory[383]];
                    memory[448] = T16_00415EFE[memory[304]][out[55]];
                    memory[449] = T16_004EF23E[memory[380]][memory[437]];
                    out[65] = T16_004EF23E[memory[352]][memory[448]];

                    if(memory[430] == memory430_wanted && memory[402] == memory402_wanted && memory[449] == memory449_wanted && out[65] == out65_wanted){
                        memory354_wanted = memory[354];
                        memory326_wanted = memory[326];
                        memory283_wanted = memory[283];
                        memory288_wanted = memory[288];
                        printf("R4-4: DONE.\n");
                        goto next24;
                    }
                }
            }
        }
    }
next24:
    unsigned char memory239_wanted,memory203_wanted,memory204_wanted,memory189_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[239] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[203] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[204] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[189] = l;
                    memory[198] = TH_0048749F[memory[189]];
                    memory[200] = T_005E028B[memory[189]];
                    memory[207] = T_00530278[memory[203]];
                    memory[209] = TH_0048CD52[memory[204]];
                    out[25] = T_004773C7[memory[204]];
                    memory[210] = T16_00415EFE[memory[207]][memory[198]];
                    memory[213] = T_00543833[memory[204]];
                    memory[221] = T_004396AD[memory[203]];
                    memory[225] = T_00549735[memory[189]];
                    memory[226] = TH_004BD9FB[memory[189]];
                    memory[227] = T_00585FFA[memory[203]];
                    memory[228] = T_005F6EBF[memory[204]];
                    out[29] = T16_00415EFE[memory[221]][memory[200]];
                    memory[232] = T16_00415EFE[memory[227]][memory[225]];
                    memory[234] = T_0044EC23[memory[203]];
                    memory[240] = T16_00415EFE[memory[234]][memory[226]];
                    memory[247] = T_004DB9FF[memory[239]];
                    memory[248] = T16_0050C7B8[memory[247]][memory[209]];
                    memory[249] = T16_004EF23E[memory[248]][out[29]];
                    memory[254] = TH_00429AEC[memory[239]];
                    out[31] = T16_0050C7B8[memory[254]][memory[228]];
                    memory[262] = T_005F8F99[memory[239]];
                    memory[276] = T_00503165[memory[239]];
                    memory[281] = T16_0050C7B8[memory[276]][memory[213]];
                    memory[288] = T16_004EF23E[memory[281]][memory[210]];
                    memory[289] = T16_004EF23E[out[31]][memory[232]];
                    memory[310] = T16_0050C7B8[memory[262]][out[25]];
                    memory[314] = T16_004EF23E[memory[310]][memory[240]];

                    if(memory[288] == memory288_wanted && memory[314] == memory314_wanted && memory[249] == memory249_wanted && memory[289] == memory289_wanted){
                        memory239_wanted = memory[239];
                        memory203_wanted = memory[203];
                        memory204_wanted = memory[204];
                        memory189_wanted = memory[189];
                        printf("R3-1: DONE.\n");
                        goto next25;
                    }
                }
            }
        }
    }
next25:
    unsigned char memory275_wanted,memory238_wanted,out21_wanted,memory159_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[275] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[238] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                out[21] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[159] = l;
                    memory[162] = T_0040D1B5[memory[159]];
                    memory[163] = T_0051D9B2[memory[159]];
                    memory[165] = TH_006107F5[memory[159]];
                    memory[187] = T_00541C75[out[21]];
                    out[23] = T_004CAFED[memory[159]];
                    memory[195] = T_005AEB8E[out[21]];
                    memory[211] = T_0045AA97[out[21]];
                    memory[212] = T_0052B582[out[21]];
                    memory[241] = T_00535749[memory[238]];
                    memory[243] = T_004B5649[memory[238]];
                    memory[244] = T16_00415EFE[memory[165]][memory[243]];
                    memory[245] = T16_00415EFE[memory[162]][memory[241]];
                    memory[257] = T_0044A435[memory[238]];
                    memory[266] = T_005A721A[memory[238]];
                    memory[271] = T16_00415EFE[memory[163]][memory[266]];
                    memory[274] = T16_00415EFE[out[23]][memory[257]];
                    out[32] = T_005410C6[memory[275]];
                    memory[278] = TH_005BC8E9[memory[275]];
                    memory[285] = T_0040B54E[memory[275]];
                    memory[298] = T16_0050C7B8[memory[278]][memory[195]];
                    memory[301] = T16_0050C7B8[memory[285]][memory[212]];
                    memory[311] = T16_004EF23E[memory[301]][memory[271]];
                    out[40] = T16_004EF23E[memory[298]][memory[245]];
                    memory[339] = T16_0050C7B8[out[32]][memory[187]];
                    out[46] = T_0040BBDF[memory[275]];
                    memory[354] = T16_004EF23E[memory[339]][memory[274]];
                    memory[355] = T16_0050C7B8[out[46]][memory[211]];
                    memory[362] = T16_004EF23E[memory[355]][memory[244]];

                    if(memory[362] == memory362_wanted && out[40] == out40_wanted && memory[311] == memory311_wanted && memory[354] == memory354_wanted){
                        memory275_wanted = memory[275];
                        memory238_wanted = memory[238];
                        out21_wanted = out[21];
                        memory159_wanted = memory[159];
                        printf("R3-2: DONE.\n");
                        goto next26;
                    }
                }
            }
        }
    }
next26:
    unsigned char memory237_wanted,memory229_wanted,memory179_wanted,memory190_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[237] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[229] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[179] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[190] = l;
                    memory[181] = TH_0055E47C[memory[179]];
                    memory[191] = T_005E11D2[memory[190]];
                    memory[192] = T_00591CAB[memory[190]];
                    out[24] = T_00555B85[memory[190]];
                    memory[205] = TH_005DD41F[memory[190]];
                    memory[206] = T_00583200[memory[179]];
                    memory[208] = T_006115AF[memory[179]];
                    memory[215] = T_00566596[memory[179]];
                    memory[230] = T_00499114[memory[229]];
                    memory[231] = T_005E3BF8[memory[229]];
                    memory[235] = TH_0043764D[memory[229]];
                    out[30] = T_005F0DA7[memory[229]];
                    memory[242] = T_0059EC14[memory[237]];
                    memory[253] = T16_00415EFE[memory[215]][memory[242]];
                    memory[259] = T_00462734[memory[237]];
                    memory[260] = T16_0050C7B8[memory[205]][memory[235]];
                    memory[264] = T16_0050C7B8[memory[192]][memory[231]];
                    memory[265] = T_004CBC82[memory[237]];
                    memory[267] = T16_0050C7B8[memory[191]][out[30]];
                    memory[269] = T16_0050C7B8[out[24]][memory[230]];
                    memory[270] = T16_00415EFE[memory[181]][memory[259]];
                    memory[277] = T16_00415EFE[memory[208]][memory[265]];
                    memory[283] = T16_004EF23E[memory[267]][memory[253]];
                    memory[284] = T16_004EF23E[memory[260]][memory[277]];
                    memory[294] = T_004469A3[memory[237]];
                    memory[297] = T16_004EF23E[memory[264]][memory[270]];
                    memory[338] = T16_00415EFE[memory[206]][memory[294]];
                    memory[341] = T16_004EF23E[memory[269]][memory[338]];

                    if(memory[297] == memory297_wanted && memory[284] == memory284_wanted && memory[341] == memory341_wanted && memory[283] == memory283_wanted){
                        memory237_wanted = memory[237];
                        memory229_wanted = memory[229];
                        memory179_wanted = memory[179];
                        memory190_wanted = memory[190];
                        printf("R3-3: DONE.\n");
                        goto next27;
                    }
                }
            }
        }
    }
next27:
    unsigned char memory236_wanted,memory172_wanted,memory186_wanted,memory153_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[236] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[172] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[186] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[153] = l;
                    memory[155] = TH_005A54C3[memory[153]];
                    memory[164] = TH_00604EFC[memory[153]];
                    memory[167] = T_0057F791[memory[153]];
                    memory[169] = T_005010F6[memory[153]];
                    memory[176] = T_00470A1D[memory[172]];
                    out[22] = T16_0050C7B8[memory[176]][memory[169]];
                    memory[180] = T_0047E3EE[memory[172]];
                    memory[188] = T_004BE72D[memory[186]];
                    memory[194] = TH_005D83F4[memory[186]];
                    memory[196] = T16_0050C7B8[memory[180]][memory[164]];
                    memory[197] = T_0057E97B[memory[186]];
                    out[26] = TH_0047ABFC[memory[186]];
                    out[28] = TH_00559A77[memory[172]];
                    memory[246] = TH_005C0149[memory[236]];
                    memory[251] = T16_0050C7B8[out[28]][memory[155]];
                    memory[252] = T_006055B8[memory[236]];
                    memory[256] = T16_00415EFE[memory[246]][memory[197]];
                    memory[261] = T_004EEAB1[memory[172]];
                    memory[268] = T_00546C5D[memory[236]];
                    memory[272] = T16_00415EFE[memory[252]][out[26]];
                    memory[273] = T_005F2816[memory[236]];
                    memory[279] = T16_0050C7B8[memory[261]][memory[167]];
                    memory[280] = T16_004EF23E[memory[251]][memory[256]];
                    memory[292] = T16_00415EFE[memory[268]][memory[188]];
                    out[34] = T16_004EF23E[memory[196]][memory[272]];
                    memory[305] = T16_00415EFE[memory[273]][memory[194]];
                    out[36] = T16_004EF23E[memory[279]][memory[305]];
                    memory[326] = T16_004EF23E[out[22]][memory[292]];

                    if(out[34] == out34_wanted && memory[280] == memory280_wanted && memory[326] == memory326_wanted && out[36] == out36_wanted){
                        memory236_wanted = memory[236];
                        memory172_wanted = memory[172];
                        memory186_wanted = memory[186];
                        memory153_wanted = memory[153];
                        printf("R3-4: DONE.\n");
                        goto next28;
                    }
                }
            }
        }
    }
next28:
    unsigned char memory144_wanted,memory119_wanted,memory87_wanted,memory60_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[144] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[119] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[87] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[60] = l;
                    memory[61] = T_005CD110[memory[60]];
                    memory[62] = TH_005DF8BB[memory[60]];
                    memory[70] = T_004A33EA[memory[60]];
                    memory[73] = TH_00527045[memory[60]];
                    memory[93] = T_00492ADF[memory[87]];
                    memory[111] = T_005EB74F[memory[87]];
                    memory[120] = T_005D945F[memory[87]];
                    memory[135] = TH_004E7DAB[memory[87]];
                    memory[137] = T_005E0141[memory[119]];
                    memory[140] = TH_00525ABF[memory[119]];
                    memory[142] = T_005EA1E4[memory[119]];
                    memory[146] = T16_00415EFE[memory[142]][memory[73]];
                    memory[147] = T16_00415EFE[memory[137]][memory[62]];
                    memory[149] = TH_004BD577[memory[119]];
                    memory[152] = T_0053DB26[memory[144]];
                    memory[157] = T_0049D1EE[memory[144]];
                    memory[160] = T16_0050C7B8[memory[152]][memory[135]];
                    memory[161] = T16_00415EFE[memory[149]][memory[70]];
                    memory[166] = T16_0050C7B8[memory[157]][memory[111]];
                    memory[170] = T16_00415EFE[memory[140]][memory[61]];
                    memory[172] = T16_004EF23E[memory[166]][memory[161]];
                    out[21] = T16_004EF23E[memory[160]][memory[146]];
                    memory[183] = TH_004DD93D[memory[144]];
                    memory[193] = TH_0057A64B[memory[144]];
                    memory[202] = T16_0050C7B8[memory[183]][memory[93]];
                    memory[203] = T16_004EF23E[memory[202]][memory[147]];
                    memory[219] = T16_0050C7B8[memory[193]][memory[120]];
                    memory[237] = T16_004EF23E[memory[219]][memory[170]];

                    if(memory[172] == memory172_wanted && memory[237] == memory237_wanted && out[21] == out21_wanted && memory[203] == memory203_wanted){
                        memory144_wanted = memory[144];
                        memory119_wanted = memory[119];
                        memory87_wanted = memory[87];
                        memory60_wanted = memory[60];
                        printf("R2-1: DONE.\n");
                        goto next29;
                    }
                }
            }
        }
    }
next29:
    unsigned char memory132_wanted,out11_wanted,memory89_wanted,memory86_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[132] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            out[11] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                memory[89] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[86] = l;
                    memory[100] = T_00446CB5[memory[89]];
                    memory[102] = T_005DF1B0[memory[89]];
                    memory[103] = T_005EE523[out[11]];
                    out[12] = T_0047333E[memory[86]];
                    memory[105] = T_00555127[memory[89]];
                    memory[106] = TH_005DA709[memory[86]];
                    memory[108] = TH_0042CED6[out[11]];
                    out[14] = T_00526CAF[memory[86]];
                    memory[114] = T_004139D1[memory[86]];
                    memory[116] = TH_00470304[memory[89]];
                    memory[121] = T16_0050C7B8[memory[108]][out[12]];
                    memory[131] = T_0040D60B[out[11]];
                    memory[133] = T16_0050C7B8[memory[131]][memory[114]];
                    memory[134] = T_005730DD[memory[132]];
                    memory[138] = TH_004D925D[memory[132]];
                    memory[141] = T_005C53AA[out[11]];
                    out[16] = T_0042ABCA[memory[132]];
                    out[18] = T16_00415EFE[out[16]][memory[116]];
                    memory[151] = T16_00415EFE[memory[138]][memory[102]];
                    memory[153] = T16_004EF23E[memory[121]][out[18]];
                    memory[154] = T16_0050C7B8[memory[141]][out[14]];
                    memory[158] = T_005224A6[memory[132]];
                    out[19] = T16_00415EFE[memory[134]][memory[100]];
                    memory[159] = T16_004EF23E[memory[154]][memory[151]];
                    out[20] = T16_0050C7B8[memory[103]][memory[106]];
                    memory[184] = T16_00415EFE[memory[158]][memory[105]];
                    memory[189] = T16_004EF23E[memory[133]][out[19]];
                    memory[190] = T16_004EF23E[out[20]][memory[184]];

                    if(memory[190] == memory190_wanted && memory[153] == memory153_wanted && memory[189] == memory189_wanted && memory[159] == memory159_wanted){
                        memory132_wanted = memory[132];
                        out11_wanted = out[11];
                        memory89_wanted = memory[89];
                        memory86_wanted = memory[86];
                        printf("R2-2: DONE.\n");
                        goto next30;
                    }
                }
            }
        }
    }
next30:
    unsigned char memory214_wanted,memory118_wanted,out8_wanted,memory56_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[214] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[118] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                out[8] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[56] = l;
                    memory[63] = T_0044B82D[memory[56]];
                    memory[65] = T_0047DAAC[memory[56]];
                    memory[72] = T_00541EC9[memory[56]];
                    memory[77] = TH_004C65D4[memory[56]];
                    memory[84] = T_0040D386[out[8]];
                    memory[109] = T_0045C489[out[8]];
                    memory[113] = T_004AB779[out[8]];
                    memory[122] = TH_004D8446[memory[118]];
                    memory[123] = T_005A76A3[memory[118]];
                    memory[124] = T16_0050C7B8[memory[123]][memory[65]];
                    memory[126] = TH_004663A4[memory[118]];
                    memory[136] = T16_0050C7B8[memory[126]][memory[63]];
                    memory[139] = T_004A7DF3[out[8]];
                    memory[177] = T16_0050C7B8[memory[122]][memory[77]];
                    memory[178] = T_00437DF4[memory[118]];
                    memory[185] = T16_0050C7B8[memory[178]][memory[72]];
                    memory[216] = T_004D6FF1[memory[214]];
                    memory[217] = T16_00415EFE[memory[216]][memory[84]];
                    memory[218] = TH_00608432[memory[214]];
                    memory[220] = T_0054C671[memory[214]];
                    memory[222] = T_0058B151[memory[214]];
                    memory[223] = T16_00415EFE[memory[220]][memory[113]];
                    memory[224] = T16_00415EFE[memory[218]][memory[109]];
                    memory[229] = T16_004EF23E[memory[185]][memory[223]];
                    memory[233] = T16_00415EFE[memory[222]][memory[139]];
                    memory[236] = T16_004EF23E[memory[136]][memory[233]];
                    memory[238] = T16_004EF23E[memory[124]][memory[217]];
                    memory[239] = T16_004EF23E[memory[177]][memory[224]];

                    if(memory[229] == memory229_wanted && memory[238] == memory238_wanted && memory[239] == memory239_wanted && memory[236] == memory236_wanted){
                        memory214_wanted = memory[214];
                        memory118_wanted = memory[118];
                        out8_wanted = out[8];
                        memory56_wanted = memory[56];
                        printf("R2-3: DONE.\n");
                        goto next31;
                    }
                }
            }
        }
    }
next31:
    unsigned char memory101_wanted,memory148_wanted,out10_wanted,memory85_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        memory[101] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            memory[148] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                out[10] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    memory[85] = l;
                    out[9] = TH_005C085A[memory[85]];
                    memory[95] = T_00554D97[memory[85]];
                    memory[104] = TH_005E4E34[out[10]];
                    memory[107] = T_0057EFC4[memory[85]];
                    memory[110] = T_0049A73D[memory[85]];
                    out[13] = T_0056E697[memory[101]];
                    memory[115] = T_005AFC32[memory[101]];
                    memory[117] = T_005288C4[memory[101]];
                    memory[125] = T_0052231A[out[10]];
                    memory[127] = T_0042628A[out[10]];
                    memory[129] = T16_0050C7B8[memory[107]][memory[127]];
                    memory[130] = T16_0050C7B8[memory[95]][memory[104]];
                    out[15] = T_0042FFBB[out[10]];
                    out[17] = T16_0050C7B8[memory[110]][memory[125]];
                    memory[143] = T16_0050C7B8[out[9]][out[15]];
                    memory[150] = T_0040F199[memory[148]];
                    memory[156] = T16_00415EFE[memory[115]][memory[150]];
                    memory[168] = T_005FDEF0[memory[148]];
                    memory[171] = T_00461B15[memory[148]];
                    memory[173] = T_0049542C[memory[148]];
                    memory[179] = T16_004EF23E[memory[130]][memory[156]];
                    memory[182] = T16_00415EFE[memory[117]][memory[171]];
                    memory[186] = T16_004EF23E[out[17]][memory[182]];
                    memory[199] = T16_00415EFE[out[13]][memory[168]];
                    memory[201] = TH_0041135D[memory[101]];
                    memory[204] = T16_004EF23E[memory[129]][memory[199]];
                    out[27] = T16_00415EFE[memory[201]][memory[173]];
                    memory[275] = T16_004EF23E[memory[143]][out[27]];

                    if(memory[179] == memory179_wanted && memory[186] == memory186_wanted && memory[275] == memory275_wanted && memory[204] == memory204_wanted){
                        memory101_wanted = memory[101];
                        memory148_wanted = memory[148];
                        out10_wanted = out[10];
                        memory85_wanted = memory[85];
                        printf("R2-4: DONE.\n");
                        goto next32;
                    }
                }
            }
        }
    }
next32:
    unsigned char key0xa_wanted,key0x5_wanted,key0xf_wanted,key0x0_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        key[0xa] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            key[0x5] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                key[0xf] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    key[0x0] = l;
                    memory[2] = T_00430909[key[0x0]];
                    memory[9] = T_004E19C5[key[0xa]];
                    memory[10] = T_0055BF00[key[0xf]];
                    memory[18] = TH_005D3C2C[key[0xf]];
                    memory[20] = T16_00415EFE[memory[9]][memory[10]];
                    memory[21] = T_0043584A[key[0xa]];
                    memory[24] = TH_004DB25D[key[0xf]];
                    memory[26] = TH_004D3B58[key[0xa]];
                    memory[34] = T16_00415EFE[memory[26]][memory[18]];
                    out[3] = TH_004E567C[key[0x0]];
                    memory[38] = T_00438C5E[key[0x0]];
                    memory[45] = T_0045FB03[key[0x0]];
                    memory[53] = TH_005AE5CB[key[0xf]];
                    memory[57] = T16_00415EFE[memory[21]][memory[24]];
                    memory[81] = T_004DF02B[key[0x5]];
                    memory[82] = T16_0050C7B8[memory[2]][memory[81]];
                    memory[86] = T16_004EF23E[memory[82]][memory[20]];
                    memory[90] = T_0045102A[key[0x5]];
                    memory[91] = TH_004EC336[key[0x5]];
                    memory[92] = T16_0050C7B8[memory[45]][memory[91]];
                    memory[94] = T16_0050C7B8[out[3]][memory[90]];
                    memory[96] = T_005FCF50[key[0x5]];
                    memory[112] = T16_0050C7B8[memory[38]][memory[96]];
                    memory[118] = T16_004EF23E[memory[112]][memory[34]];
                    memory[119] = T16_004EF23E[memory[94]][memory[57]];
                    memory[128] = T_004C0931[key[0xa]];
                    memory[145] = T16_00415EFE[memory[128]][memory[53]];
                    memory[148] = T16_004EF23E[memory[92]][memory[145]];

                    if(memory[148] == memory148_wanted && memory[119] == memory119_wanted && memory[118] == memory118_wanted && memory[86] == memory86_wanted){
                        key0xa_wanted = key[0xa];
                        key0x5_wanted = key[0x5];
                        key0xf_wanted = key[0xf];
                        key0x0_wanted = key[0x0];
                        printf("R1-1: DONE.\n");
                        goto next33;
                    }
                }
            }
        }
    }

next33:
    unsigned char key0xb_wanted,key0x6_wanted,key0x1_wanted,key0xc_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        key[0xb] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            key[0x6] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                key[0x1] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    key[0xc] = l;
                    memory[1] = TH_00605BC8[key[0xc]];
                    memory[6] = T_005B71A8[key[0xb]];
                    memory[12] = T_005C1BF2[key[0xc]];
                    memory[13] = T_004B765D[key[0x1]];
                    memory[19] = T_0061487A[key[0x6]];
                    memory[25] = T16_0050C7B8[memory[13]][memory[19]];
                    out[0] = T_0057247B[key[0x1]];
                    memory[28] = T_0042759C[key[0xc]];
                    memory[30] = T16_00415EFE[memory[6]][memory[12]];
                    out[1] = T_004EB656[key[0x6]];
                    memory[31] = T16_0050C7B8[out[0]][out[1]];
                    memory[32] = TH_006151EB[key[0xc]];
                    memory[36] = TH_005FF3D0[key[0x1]];
                    memory[43] = TH_005B3F86[key[0xb]];
                    memory[44] = TH_0044709C[key[0xb]];
                    memory[49] = T16_00415EFE[memory[44]][memory[28]];
                    memory[52] = TH_0047906A[key[0x6]];
                    memory[55] = T16_0050C7B8[memory[36]][memory[52]];
                    memory[56] = T16_004EF23E[memory[55]][memory[30]];
                    memory[60] = T16_004EF23E[memory[31]][memory[49]];
                    memory[66] = T16_00415EFE[memory[43]][memory[32]];
                    memory[71] = T_005EDB6A[key[0x1]];
                    memory[79] = T_0042733A[key[0x6]];
                    memory[83] = T16_0050C7B8[memory[71]][memory[79]];
                    memory[85] = T16_004EF23E[memory[83]][memory[66]];
                    memory[97] = TH_0054DC4E[key[0xb]];
                    memory[98] = T16_00415EFE[memory[97]][memory[1]];
                    memory[132] = T16_004EF23E[memory[25]][memory[98]];

                    if(memory[132] == memory132_wanted && memory[60] == memory60_wanted && memory[56] == memory56_wanted && memory[85] == memory85_wanted){
                        key0xb_wanted = key[0xb];
                        key0x6_wanted = key[0x6];
                        key0x1_wanted = key[0x1];
                        key0xc_wanted = key[0xc];
                        printf("R1-2: DONE.\n");
                        goto next34;
                    }
                }
            }
        }
    }
next34:
    unsigned char key0x2_wanted,key0xd_wanted,key0x8_wanted,key0x7_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        key[0x2] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            key[0xd] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                key[0x8] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    key[0x7] = l;
                    memory[5] = TH_0043E740[key[0xd]];
                    memory[8] = T_0054AFF9[key[0x8]];
                    memory[15] = T_004E8F8C[key[0x7]];
                    memory[17] = T_005899A3[key[0x8]];
                    memory[22] = T_0051ECCD[key[0x7]];
                    memory[27] = T_005B9172[key[0x7]];
                    memory[35] = TH_004DF6BB[key[0x8]];
                    memory[41] = T_004EB1FE[key[0x7]];
                    memory[47] = T16_00415EFE[memory[8]][memory[5]];
                    memory[48] = T_0051CFC7[key[0x2]];
                    memory[50] = T16_0050C7B8[memory[48]][memory[41]];
                    memory[51] = T_00582673[key[0x2]];
                    out[5] = T16_0050C7B8[memory[51]][memory[15]];
                    memory[59] = T_005D01E5[key[0xd]];
                    memory[67] = T_0040F763[key[0xd]];
                    memory[68] = T16_00415EFE[memory[17]][memory[59]];
                    memory[69] = T_006023BC[key[0x8]];
                    memory[74] = TH_00533D7D[key[0x2]];
                    memory[75] = T16_00415EFE[memory[35]][memory[67]];
                    out[6] = T16_0050C7B8[memory[74]][memory[27]];
                    out[7] = TH_00502CD2[key[0xd]];
                    memory[89] = T16_004EF23E[out[5]][memory[47]];
                    out[10] = T16_004EF23E[memory[50]][memory[75]];
                    memory[99] = T16_00415EFE[memory[69]][out[7]];
                    memory[144] = T16_004EF23E[out[6]][memory[99]];
                    memory[174] = T_004C5A48[key[0x2]];
                    memory[175] = T16_0050C7B8[memory[174]][memory[22]];
                    memory[214] = T16_004EF23E[memory[175]][memory[68]];

                    if(memory[89] == memory89_wanted && memory[144] == memory144_wanted && out[10] == out10_wanted && memory[214] == memory214_wanted){
                        key0x2_wanted = key[0x2];
                        key0xd_wanted = key[0xd];
                        key0x8_wanted = key[0x8];
                        key0x7_wanted = key[0x7];
                        printf("R1-3: DONE.\n");
                        goto next35;
                    }
                }
            }
        }
    }
next35:
    unsigned char key0xe_wanted,key0x4_wanted,key0x3_wanted,key0x9_wanted;
    for(unsigned int i = 0; i < 0x100; ++i){
        key[0xe] = i;
        for(unsigned int j = 0; j < 0x100; ++j){
            key[0x4] = j;
            for(unsigned int k = 0; k < 0x100; ++k){
                key[0x3] = k;
                for(unsigned int l = 0; l < 0x100; ++l){
                    key[0x9] = l;
                    memory[0] = TH_0054F184[key[0xe]];
                    memory[3] = T_00550AA4[key[0xe]];
                    memory[4] = TH_0055ACDF[key[0x3]];
                    memory[7] = TH_0042D6CD[key[0x9]];
                    memory[11] = T_005CA23E[key[0xe]];
                    memory[14] = T_005B8E5C[key[0x4]];
                    memory[16] = T16_00415EFE[memory[7]][memory[0]];
                    memory[23] = T_00543307[key[0x9]];
                    memory[29] = TH_005ED212[key[0x9]];
                    memory[33] = T16_00415EFE[memory[23]][memory[11]];
                    memory[37] = T_004B6115[key[0x3]];
                    out[2] = T16_0050C7B8[memory[4]][memory[14]];
                    memory[39] = T_00554AAB[key[0x4]];
                    out[4] = T_004A3C76[key[0x3]];
                    memory[40] = T16_0050C7B8[memory[37]][memory[39]];
                    memory[42] = TH_00543B97[key[0x9]];
                    memory[46] = TH_00553461[key[0x4]];
                    memory[54] = T_00538E75[key[0x3]];
                    memory[58] = TH_00479E07[key[0x4]];
                    memory[64] = T16_00415EFE[memory[42]][memory[3]];
                    memory[76] = TH_004D2B65[key[0xe]];
                    memory[78] = T16_0050C7B8[memory[54]][memory[46]];
                    memory[80] = T16_0050C7B8[out[4]][memory[58]];
                    out[8] = T16_004EF23E[memory[78]][memory[33]];
                    memory[87] = T16_004EF23E[memory[80]][memory[16]];
                    memory[88] = T16_00415EFE[memory[29]][memory[76]];
                    out[11] = T16_004EF23E[memory[40]][memory[64]];
                    memory[101] = T16_004EF23E[out[2]][memory[88]];

                    if(memory[87] == memory87_wanted && out[8] == out8_wanted && out[11] == out11_wanted && memory[101] == memory101_wanted){
                        key0xe_wanted = key[0xe];
                        key0x4_wanted = key[0x4];
                        key0x3_wanted = key[0x3];
                        key0x9_wanted = key[0x9];
                        printf("R1-4: DONE.\n");
                        goto next36;
                    }
                }
            }
        }
    }

next36:
    printf("Serial: %.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", key0x0_wanted,key0x1_wanted,key0x2_wanted,key0x3_wanted,key0x4_wanted,key0x5_wanted,key0x6_wanted,key0x7_wanted,key0x8_wanted,key0x9_wanted,key0xa_wanted,key0xb_wanted,key0xc_wanted,key0xd_wanted,key0xe_wanted,key0xf_wanted);
}

int main()
{
    //0vercl0k / ee83f3833e201bdecbef269dbd4a25f0
    // unsigned char output_wanted[16] = {0xb9, 0x95, 0xd8, 0x2b, 0x73, 0x1b, 0x19, 0x30, 0xa3, 0xc6, 0x26, 0x2d, 0xce, 0x7c, 0x40, 0x62};
    // AES Whitebox Unboxing: No Such Problem - 0vercl0k / 
    unsigned char output_wanted[16] = {0xee, 0x87, 0x0c, 0xd1, 0x40, 0x8b, 0x7a, 0x09, 0xf8, 0x3d, 0xba, 0x4b, 0x1e, 0x8c, 0x81, 0xb5};

    bf(output_wanted);
    return 0;
}