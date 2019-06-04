
undefined4 PyInit_pytector(void)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  
                    /* 0x1490  1  PyInit_pytector */
  iVar1 = PyEval_GetFrame();
  iVar4 = 6;
  do {
    if (iVar1 == 0) goto LAB_100014ed;
    iVar1 = *(int *)(iVar1 + 0xc);
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  if ((iVar1 != 0) && (iVar1 = *(int *)(iVar1 + 0x18), iVar1 != 0)) {
    uVar2 = FUN_10001000(iVar1);
    if ((char)uVar2 != 0) {
      uVar2 = FUN_10001140(iVar1);
      if ((char)uVar2 != 0) {
        uVar2 = FUN_10001280();
        if ((char)uVar2 != 0) {
          uVar3 = PyModule_Create2(&DAT_10004018,0x3f5);
          return uVar3;
        }
      }
    }
  }
LAB_100014ed:
  PyErr_SetString(*(undefined4 *)PyExc_RuntimeError_exref,"The Pytector cannot protect the code");
  return 0;
}

