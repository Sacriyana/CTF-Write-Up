uint __fastcall FUN_10001140(undefined4 param_1)

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
  
  local_c4[0] = 0xc;
  local_c4[1] = 0x24;
  local_c4[2] = 0x17;
  local_c4[3] = 0x11;
  local_b4 = 0x1b;
  uStack176 = 0x22;
  uStack172 = 0x12;
  uStack168 = 0x19;
  local_a4 = 0x21;
  uStack160 = 0x2a;
  uStack156 = 0x16;
  uStack152 = 0x15;
  local_94 = 0x2d;
  uStack144 = 0x14;
  uStack140 = 0x23;
  uStack136 = 0xd;
  local_84 = 0x1e;
  uStack128 = 0x26;
  uStack124 = 0x1f;
  uStack120 = 0x1c;
  local_74 = 0x1a;
  uStack112 = 10;
  uStack108 = 0x2c;
  uStack104 = 0x1d;
  local_64 = 9;
  uStack96 = 0xb;
  uStack92 = 2;
  uStack88 = 4;
  local_14 = 0x10;
  local_54 = 0xe;
  uStack80 = 1;
  uStack76 = 0x25;
  uStack72 = 0xf;
  local_10 = 0x2b;
  local_44 = 0x29;
  uStack64 = 0x13;
  uStack60 = 0x27;
  uStack56 = 0x18;
  local_c = 0x28;
  local_34 = 6;
  uStack48 = 7;
  uStack44 = 0x2e;
  uStack40 = 0x20;
  local_24 = 5;
  uStack32 = 8;
  uStack28 = 0;
  uStack24 = 3;
  local_8 = param_1;
  uVar1 = PyDict_GetItemString(param_1,&DAT_100030e8);
  if (((*(uint *)(*(int *)(uVar1 + 4) + 0x54) & 0x2000000) != 0) && (*(int *)(uVar1 + 8) == 0x2f)) {
    uVar2 = PyList_New(0x2f);
    uVar1 = uVar2;
    if (uVar2 != 0) {
      iVar4 = 0;
      do {
        uVar3 = PyLong_FromLong(local_c4[iVar4]);
        uVar1 = PyList_SetItem(uVar2,iVar4,uVar3);
        if (uVar1 == 0xffffffff) goto LAB_10001270;
        iVar4 = iVar4 + 1;
      } while (iVar4 < 0x2f);
      uVar1 = PyDict_SetItemString(local_8,&DAT_100030ec,uVar2);
      if (uVar1 != 0xffffffff) {
        return CONCAT31((int3)(uVar1 >> 8),1);
      }
    }
  }
LAB_10001270:
  return uVar1 & 0xffffff00;
}