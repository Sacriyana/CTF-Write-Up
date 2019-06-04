# The Pytector - Reverse

Énoncé : `Retrouver le numéro de série pour valider le challenge`

## Une fausse piste pour un vrai problème

### Récupération du script

Une fois l'archive extraite, nous tombons sur un ensemble de fichiers qui fait fortement penser à du Python packagé dans un exécutable Windows.  
Si on ne sait pas comment prendre le début de l'exercice Google et là pour vous aider. Une petite recherche du style "décompiler EXE python" suffira pour vous mettre sur la voie des outils à utiliser.  

Commencer nous allons nous servir de 2 outils pour récupérer une version intelligible du script embarqué dans l'exécutable :
- [python-exe-unpacker](https://github.com/countercept/python-exe-unpacker)
- [uncompyle6](https://github.com/rocky/python-uncompyle6)

Le premier va nous servir à "dezipper" l'archive :
```
$ python3 ../../python-exe-unpacker/python_exe_unpack.py -i check_serial.exe
[*] On Python 3.7
[*] Processing check_serial.exe
[*] Pyinstaller version: 2.1+
[*] This exe is packed using pyinstaller
[*] Unpacking the binary now
[*] Python version: 37
[*] Length of package: 1177687 bytes
[*] Found 7 files in CArchive
[*] Beginning extraction...please standby
[*] Found 133 files in PYZ archive
[*] Successfully extracted pyinstaller exe.
```

On trouve dans les fichiers archivés le fichier fake_check qui va nous intéresser par la suite :

```
check_serial/dist/unpacked/check_serial.exe  ll
total 1.2M
drwxr-xr-x 3 sacriyana sacriyana 4.0K May 22 07:50 ./
drwxr-xr-x 3 sacriyana sacriyana 4.0K May 22 07:50 ../
-rw-r--r-- 1 sacriyana sacriyana 1.2M May 22 07:50 PYZ-00.pyz
drwxr-xr-x 2 sacriyana sacriyana 4.0K May 22 07:50 PYZ-00.pyz_extracted/
-rw-r--r-- 1 sacriyana sacriyana 1.5K May 22 07:50 fake_check
-rw-r--r-- 1 sacriyana sacriyana 4.1K May 22 07:50 pyiboot01_bootstrap
-rw-r--r-- 1 sacriyana sacriyana 1.8K May 22 07:50 pyimod01_os_path
-rw-r--r-- 1 sacriyana sacriyana 9.2K May 22 07:50 pyimod02_archive
-rw-r--r-- 1 sacriyana sacriyana  19K May 22 07:50 pyimod03_importers
-rw-r--r-- 1 sacriyana sacriyana  357 May 22 07:50 struct
```

Le deuxième outil, uncompyle6, va nous servir à dé-bytecoder ce fameux fake_check.  
Pour fonctionner, uncompyle6 prend en entrée un fichier de type pyc (c'est du python précompilé).  
Changeons donc notre fake_check en fake_check.pyc pour voir ce qu'il se passe.

Le logiciel nous retourne des erreurs. En se renseignant un peu et en allant voir d'autres write-ups, on se rend compte qu'il manque un préfixe à notre fichier.  
Ce header est propre à chaque version de python3. Nous savons que c'est du python 3.7.3 qui a été utilisé. On va donc essayer de trouver un fichier pyc possédant ce header et que uncompyle6 peut déchiffrer.  
Et comme tout bon projet qui se respecte, uncompyle6 possède des fichiers de quasi toutes les versions de python, dont la nôtre. Téléchargeons le fichier [01_and_not_else.pyc](https://github.com/rocky/python-uncompyle6/blob/master/test/bytecode_3.7_run/01_and_not_else.pyc) pour examiner comment il est construit.

- le fichier 01_and_not_else.pyc :
```
$ xxd 01_and_not_else.pyc 
00000000: 420d 0d0a 0000 0000 8845 ce5c 9301 0000  B........E.\....
00000010: e300 0000 0000 0000 0000 0000 0004 0000  ................
00000020: 0040 0000 0073 3200 0000 6400 6401 8400  .@...s2...d.d...
00000030: 5a00 7824 6402 4400 5d1c 5c03 5a01 5a02  Z.x$d.D.].\.Z.Z.
00000040: 5a03 6500 6502 6503 8302 6501 6b02 730e  Z.e.e.e...e.k.s.
00000050: 7404 8201 710e 5700 6403 5300 2904 6302  t...q.W.d.S.).c.
```

- notre fichier :
```
xxd fake_check.pyc 
00000000: e300 0000 0000 0000 0000 0000 002f 0000  ............./..
00000010: 0040 0000 0073 f600 0000 6400 6401 6402  .@...s....d.d.d.
00000020: 6403 6404 6405 6406 6407 6408 6409 640a  d.d.d.d.d.d.d.d.
00000030: 640b 640c 640d 640d 640e 640f 6410 640d  d.d.d.d.d.d.d.d.
```

Il manque une ligne entière à notre fichier : `420d 0d0a 0000 0000 8845 ce5c 9301 0000`.
Ajoutons-là pour récupérer notre script :
```python
# uncompyle6 fake_check.pyc 
# uncompyle6 version 3.3.2
# Python bytecode 3.7 (3394)
# Decompiled from: Python 3.7.3 (default, May 16 2019, 12:35:00) 
# [GCC 8.3.0]
# Embedded file name: fake_check.py
# Size of source mod 2**32: 403 bytes
passwd = [
 432, 360, 264, 184, 56, 344, 340, 268, 280, 172, 64, 300, 320, 356, 356, 296, 328, 504, 356, 276, -4, 348, 52, 304, 256, 264, 184, 48, 280, 344, 312, 168, 296, -4, 128, 256, 264, 184, 40, 296, 340, 52, 280, 80, 504, 300, 296]
p = [43, 8, 26, 11, 3, 1, 46, 10, 28, 23, 13, 33, 0, 45, 34, 37, 16, 5, 36, 32, 9, 4, 20, 19, 25, 22, 17, 7, 27, 14, 42, 41, 18, 2, 6, 40, 30, 29, 15, 21, 44, 24, 39, 12, 35, 38, 31]

def check(serial):
    global p
    global passwd
    serialsize = len(serial)
    key = b'Complex is better than complicated'
    result = []
    for i in range(serialsize):
        result.append(((serial[p[i]] ^ key[(i % len(key))]) << 2) - 4)

    return result == passwd


def successful(serial):
    print('Good job!\nECSC{{{}}}'.format(serial.decode('utf-8')))


def defeated():
    print('Not really')


def main():
    import pytector
    serial = input('Serial number: ').encode('utf-8')
    if check(serial):
        successful(serial)
    else:
        defeated()


if __name__ == '__main__':
    main()
# okay decompiling fake_check.pyc
```
### Récupération de la fausse clé

Il est simple d'écrire la fonction inverse de check :

```python
def reverse_serial_1():
    passwd = [432, 360, 264, 184, 56, 344, 340, 268, 280, 172, 64, 300, 320, 356, 356, 296, 328, 504, 356, 276, -4, 348, 52, 304, 256, 264, 184, 48, 280, 344, 312, 168, 296, -4, 128, 256, 264, 184, 40, 296, 340, 52, 280, 80, 504, 300, 296]
    p = [43, 8, 26, 11, 3, 1, 46, 10, 28, 23, 13, 33, 0, 45, 34, 37, 16, 5, 36, 32, 9, 4, 20, 19, 25, 22, 17, 7, 27, 14, 42, 41, 18, 2, 6, 40, 30, 29, 15, 21, 44, 24, 39, 12, 35, 38, 31]
    serial = [' '] * 47
    for i in range(0,47):
        for c in string.printable:
            result = ((ord(c) ^ key[i % len(key)]) << 2) - 4
            if result == passwd[i]:
                serial[p[i]] = c
                break
    return ''.join(serial)
# 42dc6_ba4ad_f14g!_....._....._....._....._.....
```

Bien sûr, ce n'est pas le bon flag !

Alors pourquoi ce ne serait pas le bon ? Eh bien tout simplement, car les paramètres et même l'algorithme vont changer.

## Pytector, une librairie sympathique

En reprenant tranquillement le script fake_check.py, il est possible de noter l'import de la librairie pytector : `import pytector`
À partir de ce moment, il va falloir mettre les mains dans le cambouis. C'est parti !

Le fichier est un pyd, une librairie en somme. Quelques recherches sur le net nous apprennent que c'est du c compilé et que, donc, ça va être plus compliqué. Par chance, la NSA a pensé à nous avec Ghidra. Les plus fervents anciens vous conseilleront surement [IDA](https://github.com/NationalSecurityAgency/ghidra).

Nous arrivons sur cette fonction qui possède un nom fort sympathique : `PyInit_pytector`

```c
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
```

Elle tellement sympathique que c'est elle qui est appelée à chaque import du module pytector si celui-ci n'existe pas déjà. Mais alors, on import un module sur lequel on ne demande aucune classes, méthodes, mais qui pourtant fait des actions ? Creusons.

J'ai oublié de le préciser, mais bien évidemment, il est possible de s'appuyer sur winDBG en parallèle de la lecture pour vérifier nos constations.

Il faut examiner les fonctions suivantes : 
- `FUN_10001000`
- `FUN_10001140`
- `FUN_10001280`

Attaquons-nous à la fonction `FUN_10001000(iVar1);` qui est la première appelée. Celle-ci renvoie un entier pour savoir si tout c'est bien déroulé.

Nous remarquons qu'un tableau de 84 valeurs est chargé sur la pile au début. Ensuite le code semble aller chercher un pointeur vers une structure représentant l'objet "passwd". Et comme on est dans un CTF, il y peut de chance pour que ce soit une coïncidence. En regardant bien la fonction, on voit les appels à :
- PyDict_GetItemString
- PyLong_FromLong
- PyList_SetItem
- PyDict_SetItemString

qui sont toutes des fonctions C pour manipuler du Python. Il est simple de supposer que la méthode permet de modifier le tableau passwd du script fake_check.py.

Nous faisons la même analyse pour la fonction FUN_10001140 qui va modifier le tableau p.

/!\ Petit warning, méfiez-vous des valeurs retournées par ghidra pour les tableaux, car elles ne sont pas tout à fait dans le bon ordre à cause des registres XMM utilisées pour les chargés.
Le plus simple est encore de mettre un breakpoint au début de la fonction et d'aller voir en mémoire avec GDB ce qu'il s'y passe. /!\

Vous trouverez le code source associé à ces fonctions dans le dossier fichiers_c.

Pour la dernière fonction FUN_10001280, c'est bien plus malin ce qu'il s'y passe. Au début, je pensais naïvement que c'était la clé (ce qui semblait logique par rapport aux fonctions précédentes) qui était changée, mais non pas du tout.

Prenons le code bloc à bloc :
- Création d'une liste avec les éléments chargés avec les registres MMX : 
```c
    do {
      uVar2 = PyLong_FromLong(local_158[iVar5]);
      pcVar3 = (char *)PyList_SetItem(pcVar1,iVar5,uVar2);
      if (pcVar3 == (char *)0xffffffff) goto LAB_10001478;
      iVar5 = iVar5 + 1;
    } while (iVar5 < 0x54);

    pcVar1 = (char *)PyBytes_FromObject(pcVar1);
    pcVar3 = pcVar1;
```
- récupération du point vers un dictionnaire qui est représenté par "check". Cela fait fortement penser à la fonction check du script initial.
```c
if (pcVar1 != (char *)0x0) {
	pcVar3 = (char *)PyDict_GetItemString(local_8,"check");

	if (pcVar3 != (char *)0x0) {
```
- ensuite, 2 valeurs numériques vont être modifiées si les pointeurs pointent vers les bonnes choses. On met un 4 et un 42. La première idée vient que le 2 et le 4 ont été supprimé par ces valeurs.
De plus, l'index de l'item augmente, nous savons donc que le 2 devient un 4 et le 4, 42.
```c
if (*(int *)(iVar5 + 8) == 4) {
	local_8 = pcVar3;
	uVar2 = PyLong_FromLong(4);
	pcVar3 = (char *)PyTuple_SetItem(iVar5,2,uVar2);

	if (pcVar3 != (char *)0xffffffff) {
		uVar2 = PyLong_FromLong(0x2a);
		pcVar3 = (char *)PyTuple_SetItem(iVar5,3,uVar2);
```

- et enfin (c'est bientôt le bout), un comportement assez étrange où nous allons écraser les valeurs d'un tableau avec celles provenant du tableau qui a été construit au début de la fonction.
```c
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
```

Il faut analyser les données brutes enregistrées pour comprendre ce qu'il se passe ici. Rien de tel que windgb pour nous fournir ceci. Mais on peut aussi le faire avec les valeurs dans ghidra :
```c
local_158[0] = 0x74;
local_158[1] = 0;
local_158[2] = 0x7c;
local_158[3] = 0;
local_148 = 0x83;
uStack324 = 1;
uStack320 = 0x7d;
...
```
Si nous ouvrons le fichier fake_check (sans l'extention py !), nous remarquons une chaine d'octets assez proches de celle qui est en mémoire. De plus le tableau fait pile-poil la bonne taille :
```
0074 007c 0083 017d 0164 017d 0267
007d 0378 3a74 017c 0183 0144 005d 2e7d
047c 03a0 027c 0074 037c 0419 0019 007c
027c 0474 007c 0283 0116 0019 0064 023e
0041 0064 0317 00a1 0101 0071 1a57 007c
0374 046b 0253 0029 044e 7322
```

Nous arrivons à la fin de notre reverse. Il peut y avoir plusieurs méthodes, je pense. Personnellement j'ai dump le tableau que j'ai ensuite inséré à la place de l'ancien tableau dans le fichier fake_check.pyc et j'ai régénéré le fichier py.

Et voilà, notre nouvelle fonction check avec toutes ces modifications : 
```py
def check(serial):
    global p
    global passwd
    serialsize = len(serial)
    key = b'Complex is better than complicated'
    result = []
    for i in range(serialsize):
        result.append((serial[p[i]] ^ key[(i % len(key))] << 4) + 42)

    return result == passwd
```

Nous mettons ensuite p et passwd à jour pour obtenir le flag en modifiant un peu notre algorithme :

```
def reverse_serial_2():
    p = ["0x0c","0x24","0x17","0x11","0x1b","0x22","0x12","0x19","0x21","0x2a","0x16","0x15","0x2d","0x14","0x23","0x0d","0x1e","0x26","0x1f","0x1c","0x1a","0x0a","0x2c","0x1d","0x09","0x0b","0x02","0x04","0x0e","0x01","0x25","0x0f","0x29","0x13","0x27","0x18","0x06","0x07","0x2e","0x20","0x05","0x08","0x00","0x03","0x10","0x2b","0x28"]
    p = parse_array(p)
    passwd = ["0x47b","0x6f2","0x6b9","0x789","0x71d","0x690","0x810","0x28e","0x71e","0x732","0x28b","0x642","0x65b","0x7a0","0x749","0x65d","0x76b","0x260","0x79f","0x6db","0x64a","0x6af","0x263","0x699","0x6ee","0x6b9","0x78b","0x71f","0x6cb","0x62e","0x653","0x7a2","0x639","0x69d","0x47d","0x6ec","0x70a","0x78c","0x6cb","0x690","0x809","0x25c","0x720","0x72a","0x260","0x63e","0x65c"]
    passwd = parse_array(passwd)
    serial = [' '] * 47
    for i in range(0,47):
        for c in string.printable:
            result = (ord(c) ^ key[(i % len(key))] << 4) + 42
            if result == passwd[i]:
                serial[p[i]] = c
                break
    return ''.join(serial)
```

Et le flag tombe tout seul : `ECSC{f4a05_0b24e_ac186_f368a_2d031_a56d6_896cb_849aa}`
