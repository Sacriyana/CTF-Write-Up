
uint FUN_10001280(void)

{
  char *pcVar1;
  undefined4 uVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  undefined4 local_158 [4];
  undefined4 local_148;
  undefined4 uStack324;
  undefined4 uStack320;
  undefined4 uStack316;
  undefined4 local_138;
  undefined4 uStack308;
  undefined4 uStack304;
  undefined4 uStack300;
  undefined4 local_128;
  undefined4 uStack292;
  undefined4 uStack288;
  undefined4 uStack284;
  undefined4 local_118;
  undefined4 uStack276;
  undefined4 uStack272;
  undefined4 uStack268;
  undefined4 local_108;
  undefined4 uStack260;
  undefined4 uStack256;
  undefined4 uStack252;
  undefined4 local_f8;
  undefined4 uStack244;
  undefined4 uStack240;
  undefined4 uStack236;
  undefined4 local_e8;
  undefined4 uStack228;
  undefined4 uStack224;
  undefined4 uStack220;
  undefined4 local_d8;
  undefined4 uStack212;
  undefined4 uStack208;
  undefined4 uStack204;
  undefined4 local_c8;
  undefined4 uStack196;
  undefined4 uStack192;
  undefined4 uStack188;
  undefined4 local_b8;
  undefined4 uStack180;
  undefined4 uStack176;
  undefined4 uStack172;
  undefined4 local_a8;
  undefined4 uStack164;
  undefined4 uStack160;
  undefined4 uStack156;
  undefined4 local_98;
  undefined4 uStack148;
  undefined4 uStack144;
  undefined4 uStack140;
  undefined4 local_88;
  undefined4 uStack132;
  undefined4 uStack128;
  undefined4 uStack124;
  undefined4 local_78;
  undefined4 uStack116;
  undefined4 uStack112;
  undefined4 uStack108;
  undefined4 local_68;
  undefined4 uStack100;
  undefined4 uStack96;
  undefined4 uStack92;
  undefined4 local_58;
  undefined4 uStack84;
  undefined4 uStack80;
  undefined4 uStack76;
  undefined4 local_48;
  undefined4 uStack68;
  undefined4 uStack64;
  undefined4 uStack60;
  undefined4 local_38;
  undefined4 uStack52;
  undefined4 uStack48;
  undefined4 uStack44;
  undefined4 local_28;
  undefined4 uStack36;
  undefined4 uStack32;
  undefined4 uStack28;
  undefined4 local_18;
  undefined4 uStack20;
  undefined4 uStack16;
  undefined4 uStack12;
  char *local_8;
  
  local_158[0] = 0x74;
  local_158[1] = 0;
  local_158[2] = 0x7c;
  local_158[3] = 0;
  local_148 = 0x83;
  uStack324 = 1;
  uStack320 = 0x7d;
  uStack316 = 1;
  local_138 = 100;
  uStack308 = 1;
  uStack304 = 0x7d;
  uStack300 = 2;
  local_128 = 0x67;
  uStack292 = 0;
  uStack288 = 0x7d;
  uStack284 = 3;
  local_118 = 0x78;
  uStack276 = 0x3a;
  uStack272 = 0x74;
  uStack268 = 1;
  local_108 = 0x7c;
  uStack260 = 1;
  uStack256 = 0x83;
  uStack252 = 1;
  local_f8 = 0x44;
  uStack244 = 0;
  uStack240 = 0x5d;
  uStack236 = 0x2e;
  local_e8 = 0x7d;
  uStack228 = 4;
  uStack224 = 0x7c;
  uStack220 = 3;
  local_d8 = 0xa0;
  uStack212 = 2;
  uStack208 = 0x7c;
  uStack204 = 0;
  local_c8 = 0x74;
  uStack196 = 3;
  uStack192 = 0x7c;
  uStack188 = 4;
  local_b8 = 0x19;
  uStack180 = 0;
  uStack176 = 0x19;
  uStack172 = 0;
  local_a8 = 0x7c;
  uStack164 = 2;
  uStack160 = 0x7c;
  uStack156 = 4;
  local_98 = 0x74;
  uStack148 = 0;
  uStack144 = 0x7c;
  uStack140 = 2;
  local_88 = 0x83;
  uStack132 = 1;
  uStack128 = 0x16;
  uStack124 = 0;
  local_78 = 0x19;
  uStack116 = 0;
  uStack112 = 100;
  uStack108 = 2;
  local_68 = 0x3e;
  uStack100 = 0;
  uStack96 = 0x41;
  uStack92 = 0;
  local_58 = 100;
  uStack84 = 3;
  uStack80 = 0x17;
  uStack76 = 0;
  local_48 = 0xa1;
  uStack68 = 1;
  uStack64 = 1;
  uStack60 = 0;
  local_38 = 0x71;
  uStack52 = 0x1a;
  uStack48 = 0x57;
  uStack44 = 0;
  local_28 = 0x7c;
  uStack36 = 3;
  uStack32 = 0x74;
  uStack28 = 4;
  local_18 = 0x6b;
  uStack20 = 2;
  uStack16 = 0x53;
  uStack12 = 0;
  pcVar1 = (char *)PyList_New(0x54);
  pcVar3 = pcVar1;
  if (pcVar1 != (char *)0x0) {
    iVar5 = 0;
    
    do {
      uVar2 = PyLong_FromLong(local_158[iVar5]);
      pcVar3 = (char *)PyList_SetItem(pcVar1,iVar5,uVar2);
      if (pcVar3 == (char *)0xffffffff) goto LAB_10001478;
      iVar5 = iVar5 + 1;
    } while (iVar5 < 0x54);

    pcVar1 = (char *)PyBytes_FromObject(pcVar1);
    pcVar3 = pcVar1;
    
    if (pcVar1 != (char *)0x0) {
      pcVar3 = (char *)PyDict_GetItemString(local_8,"check");

      if (pcVar3 != (char *)0x0) {
    
        pcVar3 = *(char **)(pcVar3 + 8);
        iVar5 = *(int *)(pcVar3 + 0x24);
    
        if (*(int *)(iVar5 + 8) == 4) {
          local_8 = pcVar3;
          uVar2 = PyLong_FromLong(4);
          pcVar3 = (char *)PyTuple_SetItem(iVar5,2,uVar2);
    
          if (pcVar3 != (char *)0xffffffff) {
            uVar2 = PyLong_FromLong(0x2a);
            pcVar3 = (char *)PyTuple_SetItem(iVar5,3,uVar2);
    
            if (pcVar3 != (char *)0xffffffff) {
              iVar5 = *(int *)(local_8 + 0x20);
              iVar4 = *(int *)(iVar5 + 8);
    
              if (iVar4 == *(int *)(pcVar1 + 8)) {
    
                if (0 < iVar4) {
                  pcVar3 = (char *)(iVar5 + 0x10);
                  do {
                    if (*pcVar3 != (pcVar1 + -iVar5)[(int)pcVar3]) {
                      *pcVar3 = (pcVar1 + -iVar5)[(int)pcVar3];
                    }
                    pcVar3 = pcVar3 + 1;
                    iVar4 = iVar4 + -1;
                  } while (iVar4 != 0);
                }
                return CONCAT31((int3)((uint)pcVar3 >> 8),1);
              }
            }
          }
        }
      }
    }
  }
LAB_10001478:
  return (uint)pcVar3 & 0xffffff00;
}