
uint __fastcall FUN_10001000(undefined4 param_1)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 local_c4 [4];
  undefined4 local_b4;
  undefined4 uStack176;
  undefined4 uStack172;
  undefined4 uStack168;
  undefined4 local_a4;
  undefined4 uStack160;
  undefined4 uStack156;
  undefined4 uStack152;
  undefined4 local_94;
  undefined4 uStack144;
  undefined4 uStack140;
  undefined4 uStack136;
  undefined4 local_84;
  undefined4 uStack128;
  undefined4 uStack124;
  undefined4 uStack120;
  undefined4 local_74;
  undefined4 uStack112;
  undefined4 uStack108;
  undefined4 uStack104;
  undefined4 local_64;
  undefined4 uStack96;
  undefined4 uStack92;
  undefined4 uStack88;
  undefined4 local_54;
  undefined4 uStack80;
  undefined4 uStack76;
  undefined4 uStack72;
  undefined4 local_44;
  undefined4 uStack64;
  undefined4 uStack60;
  undefined4 uStack56;
  undefined4 local_34;
  undefined4 uStack48;
  undefined4 uStack44;
  undefined4 uStack40;
  undefined4 local_24;
  undefined4 uStack32;
  undefined4 uStack28;
  undefined4 uStack24;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_c4[0] = 0x47b;
  local_c4[1] = 0x6f2;
  local_c4[2] = 0x6b9;
  local_c4[3] = 0x789;
  local_b4 = 0x71d;
  uStack176 = 0x690;
  uStack172 = 0x810;
  uStack168 = 0x28e;
  local_a4 = 0x71e;
  uStack160 = 0x732;
  uStack156 = 0x28b;
  uStack152 = 0x642;
  local_94 = 0x65b;
  uStack144 = 0x7a0;
  uStack140 = 0x749;
  uStack136 = 0x65d;
  local_84 = 0x76b;
  uStack128 = 0x260;
  uStack124 = 0x79f;
  uStack120 = 0x6db;
  local_74 = 0x64a;
  uStack112 = 0x6af;
  uStack108 = 0x263;
  uStack104 = 0x699;
  local_64 = 0x6ee;
  uStack96 = 0x6b9;
  uStack92 = 0x78b;
  uStack88 = 0x71f;
  local_14 = 0x260;
  local_54 = 0x6cb;
  uStack80 = 0x62e;
  uStack76 = 0x653;
  uStack72 = 0x7a2;
  local_10 = 0x63e;
  local_44 = 0x639;
  uStack64 = 0x69d;
  uStack60 = 0x47d;
  uStack56 = 0x6ec;
  local_c = 0x65c;
  local_34 = 0x70a;
  uStack48 = 0x78c;
  uStack44 = 0x6cb;
  uStack40 = 0x690;
  local_24 = 0x809;
  uStack32 = 0x25c;
  uStack28 = 0x720;
  uStack24 = 0x72a;
  local_8 = param_1;
  uVar1 = PyDict_GetItemString(param_1,"passwd");
  if (((*(uint *)(*(int *)(uVar1 + 4) + 0x54) & 0x2000000) != 0) && (*(int *)(uVar1 + 8) == 0x2f)) {
    uVar2 = PyList_New(0x2f);
    uVar1 = uVar2;
    if (uVar2 != 0) {
      iVar4 = 0;
      do {
        uVar3 = PyLong_FromLong(local_c4[iVar4]);
        uVar1 = PyList_SetItem(uVar2,iVar4,uVar3);
        if (uVar1 == 0xffffffff) goto LAB_10001130;
        iVar4 = iVar4 + 1;
      } while (iVar4 < 0x2f);
      uVar1 = PyDict_SetItemString(local_8,"passwd",uVar2);
      if (uVar1 != 0xffffffff) {
        return CONCAT31((int3)(uVar1 >> 8),1);
      }
    }
  }
LAB_10001130:
  return uVar1 & 0xffffff00;
}

