# 3635_incidents

Ce challenge était composé de 3 étapes distinctes. L'objectif principal était de récupérer un document chiffré par un ransomware en possédant une image de la RAM.

- Énoncé 1 :
```
Une victime de plus tombée sous le coup d’un rançongiciel. Le paiement de la rançon n’est pas envisagé vu le montant demandé. Vous êtes appelé pour essayer de restaurer les fichiers chiffrés.

Une suite d’éléments est nécessaire pour avancer dans l’investigation et constituer le rapport d’incident.

Pour commencer, quel est le nom du fichier exécutable de ce rançongiciel, son identifiant de processus et quel est devenu le nom du fichier flag.docx une fois chiffré ? Donnez le SHA1 de ce nom avec son extension.

Note : l’image disque fait environ 440 Mo compressés et environ 1.4 Go décompressée. Réponse attendue au format ECSC{nom_du_rançongiciel.exe:PiD:sha1}.

$ sha256sum mem.dmp ce117720fa4126f57814b3a779a7eb4ba21570e3f5dfd44a6706771783a46f1b mem.dmp
```

- Énoncé 2 : `Retrouvez la clé de chiffrement de ce rançongiciel!`

- Énoncé 3 : `Déchiffrez le fichier "flag.docx" ci-joint!`

# Partie 1 - la prise en main de volatily

Étant donné que nous allons manipuler une image mémoire, le logiciel tout indiqué est [volatily](https://www.volatilityfoundation.org/). 

Tout d'abord, nous devons trouver le bon profilé de notre image mémoire. Nous pouvons déjà avoir une idée de celui-ci grâce au module imageinfo :
```
volatility -f mem.dmp imageinfo
Volatility Foundation Volatility Framework 2.6
INFO    : volatility.debug    : Determining profile based on KDBG search...
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
WARNING : volatility.debug    : Alignment of WindowsCrashDumpSpace64 is too small, plugins will be extremely slow
          Suggested Profile(s) : Win10x64_17134, Win10x64_10240_17770, Win10x64_10586, Win10x64_14393, Win10x64, Win2016x64_14393, Win10x64_16299, Win10x64_15063 (Instantiated with Win10x64_15063)
                     AS Layer1 : SkipDuplicatesAMD64PagedMemory (Kernel AS)
                     AS Layer2 : WindowsCrashDumpSpace64 (Unnamed AS)
                     AS Layer3 : FileAddressSpace (/home/sacriyana/prive_data/security/chall/ecsc/forensic/3615_incidents_1/mem.dmp)
                      PAE type : No PAE
                           DTB : 0x1ab000L
                          KDBG : 0xf801f433ba60L
          Number of Processors : 2
     Image Type (Service Pack) : 0
                KPCR for CPU 0 : 0xfffff801f4394000L
                KPCR for CPU 1 : 0xffffd0012eb07000L
             KUSER_SHARED_DATA : 0xfffff78000000000L
           Image date and time : 2019-05-08 20:04:11 UTC+0000
     Image local date and time : 2019-05-08 22:04:11 +0200
```

Nous avons donc une image mémoire prise sur un Windows 10 (c'est une VM, mais ce n'est pas important). Nous testons les profils, jusqu'à tomber sur le bon : Win10x64_10586, car nous pouvons décoder la liste
des processus.

```
volatility -f mem.dmp --profile Win10x64_10586 pstree
Volatility Foundation Volatility Framework 2.6
Name                                                  Pid   PPid   Thds   Hnds Time
-------------------------------------------------- ------ ------ ------ ------ ----
 0xffffe0000f65a040:System                              4      0    136      0 2019-05-08 19:57:03 UTC+0000
. 0xffffe00010e4b040:smss.exe                         256      4      3      0 2019-05-08 19:57:03 UTC+0000
 0xffffe00011344080:winlogon.exe                      544    464      5      0 2019-05-08 19:57:05 UTC+0000
. 0xffffe00012034080:userinit.exe                    3120    544      0 ------ 2019-05-08 19:57:14 UTC+0000
.. 0xffffe000116e3080:explorer.exe                   3184   3120     86      0 2019-05-08 19:57:14 UTC+0000
... 0xffffe00012774080:OneDrive.exe                  3080   3184     17      0 2019-05-08 19:57:29 UTC+0000
... 0xffffe00012854840:MSASCui.exe                   5840   3184      6      0 2019-05-08 20:01:01 UTC+0000
... 0xffffe000125a7840:firefox.exe                   4040   3184     59      0 2019-05-08 19:59:06 UTC+0000
...
```

### Le fichier

Commençons simple le nom du fichier à trouver. Nous allons;utiliser le module filescan permettant de connaitre les chemins de fichier présent (ou partiellement) en mémoire.
On récupère tous les fichiers .chiffré et on regarde chaque base64. Et ding, nous avons un gagnant : `ZmxhZy5kb2N4 => flag.docx`


```
volatility -f mem.dmp --profile Win10x64_10586 filescan
...
0x0000e000123988d0     16      0 R--r-d \Device\HarddiskVolume4\ZmxhZy5kb2N4.chiffré
...
```

Et son SHA1 : `ZmxhZy5kb2N4.chiffré => c9a12b109a58361ff1381fceccdcdcade3ec595a`

### Le processus

En listant la liste des processus d'ouverts avec le module pslist, nous trouvons un exécutable étrange : assistance.exe
Nous allons l'examiner de plus prés à l'aide du module psinfo de MonnappaKa.

```
volatility --plugins ~/volatility_2.6_lin64_standalone/plugins/MonnappaKa -f mem.dmp --profile Win10x64_10586 psinfo -p 5208
Volatility Foundation Volatility Framework 2.6
Process Information:
	Process: assistance.exe PID: 5208
	Parent Process: explorer.exe PPID: 3184
	Creation Time: 2019-05-08 20:00:16 UTC+0000
	Process Base Name(PEB): assistance.exe
	Command Line(PEB): "C:\Users\TNKLSAI3TGT7O9\Downloads\assistance.exe" 

VAD and PEB Comparison:
	Base Address(VAD): 0x400000
	Process Path(VAD): \Users\TNKLSAI3TGT7O9\Downloads\assistance.exe
	Vad Protection: PAGE_EXECUTE_WRITECOPY
	Vad Tag: Vad 

	Base Address(PEB): 0x400000
	Process Path(PEB): C:\Users\TNKLSAI3TGT7O9\Downloads\assistance.exe
	Memory Protection: PAGE_EXECUTE_WRITECOPY
	Memory Tag: Vad 

Similar Processes:
C:\Users\TNKLSAI3TGT7O9\Downloads\assistance.exe
	assistance.exe(5208) Parent:explorer.exe(3184) Start:2019-05-08 20:00:16 UTC+0000

Suspicious Memory Regions:
---------------------------------------------------
```

Et voilà nous avons notre exécutable puisqu'il vient du dossier téléchargement de l'utilisateur. Pour en avoir le coeur net, on le dump et nous le scannons sur virus total, il a un taux de détection à 30/66.

```
volatility --plugins ~/volatility_2.6_lin64_standalone/plugins/MonnappaKa -f mem.dmp --profile Win10x64_10586 procdump -p 5208 -D dump
Volatility Foundation Volatility Framework 2.6
Process(V)         ImageBase          Name                 Result
------------------ ------------------ -------------------- ------
0xffffe000106bb840 0x0000000000400000 assistance.exe       OK: executable.5208.exe
```

Voilà le flag : ECSC{assistance.exe:5208:c9a12b109a58361ff1381fceccdcdcade3ec595a}

# Partie 2 - Sherlock Holmes

## La méthode basique

Tout d'abord, récupérons la partie mémoire associée au processus : `8544  volatility --plugins ~/Downloads/volatility_2.6_lin64_standalone/plugins/MonnappaKa -f mem.dmp --profile Win10x64_10586 memmap -p 5208`

Une fois récupéré, il est possible de faire tourner un petit string en mettant la sortie dans un fichier histoire de pouvoir grep dessus facilement.

On peut chercher des mots faciles comme bitcoin, key, ransomware. On trouve assez le message suivant :

```
S ONT ETE CHIFFRES EN AES-256-CTR
	VOTRE IDENTIFIANT EST
	cd18c00bb476764220d05121867d62de
	ENVOYER 1 
 PAR TELEPATHIE A
	PUIS PRENDRE CONTACT AU 3615-Incident
	POUR FAIRE PART DE VOTRE TRISTESSE
	LE YOGA PEUT AIDER EN CAS DE CRISE :)
	</pre>
```

On possède donc l'ID de la victime. Ne nous manque que la clé de chiffrement. Cherchons l'id en mémoire.

```
strings 5208.dmp | grep cd18c00bb476764220d05121867d62de -B 5 -C 5
[/LL
95511870061fb3a2899aa6b2dc9838aa
"C:\Users\TNKLSAI3TGT7O9\Downloads\assistance.exe" 
C:\Users\TNKLSAI3TGT7O9\Downloads\assistance.exe
S-1-5-21-2377780471-3200203716-3353778491-1000
{"id": "cd18c00bb476764220d05121867d62de", "enckey": "
cd18c00bb476764220d05121867d62de64e0821c53c7d161099be2188b6cac24cd18c00bb476764220d05121867d62de64e0821c53c7d161099be2188b6cac2495511870061fb3a2899aa6b2dc9838aa422d81e7e1c2aa46aa51405c13fed15b95511870061fb3a2899aa6b2dc9838aa422d81e7e1c2aa46aa51405c13fed15b
Encrypting C:\Users\Administrateur\Contacts\desktop.ini...
C:\Users\TNKLSA~1\AppData\Local\Temp\desktop.ini
C:\Users\TNKLSA~1\AppData\Local\Temp\desktop.ini
Encrypting C:\Users\Administrateur\Documents\desktop.ini...
C:\Users\TNKLSA~1\AppData\Local\Temp\desktop.ini
--
Accept-Encoding: gzip
payload=%8D%9C%E1r%FD%D7%9F%05%2A%9F%D6%E9A%3B%22%DC%F9%0D%ED%12%B3%04-I%A9yb4hYT%3D.%C6%A4Y%E7K%BF%E66%E1%856%C9%7B%5B%87%E2%B7%84%D4%A16%CB%12N%ED%F2%F8L%DA%122k%D0%BB%AF%AC%00%CB%15%99%EE%C5%E3%5DU%26%11%B3%FD%F26+%2A%C1q%E3%9Cq%9A%01%DA%F7%9D%D1%F3%89c%FFMfg%B1%F2%D3n%926%0FG2%7Dl%E9h%F4%E8%09%D4%C0%85lAf%A5E%0EaG.7%BF%3Cu11%AC%3Ew%7D%87%B9DG%19%7D%C0%99~u%12%FFF%FD%A6O%00%A1h%E4%85%9DV%7D%E6.%AA%3A%40%8F%CD2%99%D7%07%A7%B9%81n%D2kx%8E%15%9D%1B%9Eln%2CN%81%26t%BD%1E%83x%2FP%96%85%21%F6+%19x%3FK%92%F9%A8%EF%D0%AC%BA%8Fi%85%CC0%CBCi%BER%3Ew%04%C7%D3%BC%D5%EE%82%5D%E0~%B7%25%7F%C6%A9%D5%91J%60%3BI%D2%09%A6%A3%3A%82%E8%D48%93%A8n%FB%5CB%0E%16%1CGX%B1%2B%3F%0E1%B0%9Fj%B8%93%03%DC%1CX%AE%FB%A9e%B4%AD%0A%A2%B3%93%D1%BC%CA%11%C0%3BVq%C3%8E0%B3%8D%F6%FF%D5%CAT%DAB4%7B%9C%A4%CF%5B%93.%89%98Y%8C%5E%8B%CF%0E%B3%0Dn%E4%B1%D7%A0%B7%81%BE%9A%FB.%E9U%3E%0F%1A%F2%E3%03%CC%B4H%2A%BE%9E%A2%82%94%A3%94%92%975%D5%F2j%B6%96_%B1%F2%D5%8E%CC%16wJ%08%E0%FC%F0%7F%AC%E6%3E%F8%60%97%D1%D3%AE%BE%E8%D0q%FF6tU%99x%9D%8E1XL%FE%BB%A8%EA%E8h%05%F4%E4%A1%9F%FDfHg%04%B0%CFx+%8FM%9B%1E%DFO%D0%A4%FF%02%19%96%7F%90%24%26%EE.Q3%82%D6%0CW%D8%12%FD%D0%BF%82%FC%92%FBv%94%AB%E5%A5%F1%A6%AFk%A8%DBEM%E3%C0%7D%B3%1A%1Cv%CAa%B5%C3%3E%1F%21s%B8%BF%94%DAP%60%3B%11%03nGS%F3t%C7%0A%A9%C1%8Ai%CA%9C
Press enter to quit
S ONT ETE CHIFFRES EN AES-256-CTR
	VOTRE IDENTIFIANT EST
	cd18c00bb476764220d05121867d62de
	ENVOYER 1 
 PAR TELEPATHIE A
	PUIS PRENDRE CONTACT AU 3615-Incident
	POUR FAIRE PART DE VOTRE TRISTESSE
	LE YOGA PEUT AIDER EN CAS DE CRISE :)
--
v.,L6
.jUO
	<pre>
	VOS FICHIERS ONT ETE CHIFFRES EN AES-256-CTR
	VOTRE IDENTIFIANT EST
	cd18c00bb476764220d05121867d62de
	ENVOYER 1 
 PAR TELEPATHIE A
	PUIS PRENDRE CONTACT AU 3615-Incident
	POUR FAIRE PART DE VOTRE TRISTESSE
	LE YOGA PEUT AIDER EN CAS DE CRISE :)
	</pre>
	<pre>
	VOS FICHIERS ONT ETE CHIFFRES EN AES-256-CTR
	VOTRE IDENTIFIANT EST
	cd18c00bb476764220d05121867d62de
	ENVOYER 1 
 PAR TELEPATHIE A
	PUIS PRENDRE CONTACT AU 3615-Incident
	POUR FAIRE PART DE VOTRE TRISTESSE
	LE YOGA PEUT AIDER EN CAS DE CRISE :)
--
FILE0
FILE0
	<pre>
	VOS FICHIERS ONT ETE CHIFFRES EN AES-256-CTR
	VOTRE IDENTIFIANT EST
	cd18c00bb476764220d05121867d62de
	ENVOYER 1 
 PAR TELEPATHIE A
	PUIS PRENDRE CONTACT AU 3615-Incident
	POUR FAIRE PART DE VOTRE TRISTESSE
	LE YOGA PEUT AIDER EN CAS DE CRISE :)
```

Nous trouvons dès lors des candidats potentiels :
- 64e0821c53c7d161099be2188b6cac24
- 95511870061fb3a2899aa6b2dc9838aa
- 422d81e7e1c2aa46aa51405c13fed15b

On peut les tester un par un. Et le 95511870061fb3a2899aa6b2dc9838aa ressort victorieux de la sélection.

Mais est-ce qu'il n'y aurait pas une méthode sûre pour trouver à coup sûr la clé ?

## La méthode avancée avec un peu de reverse

Tout d'abord, nous avons besoin d'information supplémentaire sur le ransomware pour comprendre les structures de données utilisées.
Par chance, nous avons cette information en mémoire:

```
$ strings 5208.dmp | grep ransom                                    
%github.com/mauri870/ransomware/client
'github.com/mauri870/ransomware/cryptofs
path	github.com/mauri870/ransomware/build/ransomware
mod	github.com/mauri870/ransomware	(devel)	
github.com/mauri870/ransomware/cryptofs.(*File).Encrypt
github.com/mauri870/ransomware/cryptofs.(*File).ReplaceBy
github.com/mauri870/ransomware/utils.GenerateRandomANString
github.com/mauri870/ransomware/utils.StringInSlice
github.com/mauri870/ransomware/utils.SliceContainsSubstring
github.com/mauri870/ransomware/utils.GetDrives
github.com/mauri870/ransomware/utils.GetCurrentUser
github.com/mauri870/ransomware/utils.RenameFile
github.com/mauri870/ransomware/utils.init
github.com/mauri870/ransomware/cmd.glob..func1
github.com/mauri870/ransomware/cmd.init.ializers
github.com/mauri870/ransomware/cmd.init
github.com/mauri870/ransomware/client.New
go.(*struct { Files []*github.com/mauri870/ransomware/cryptofs.File; sync.Mutex }).Lock
go.(*struct { Files []*github.com/mauri870/ransomware/cryptofs.File; sync.Mutex }).Unlock
...
```

C'est donc un [ransomware développé en go](https://github.com/mauri870/ransomware).
Il utilise une structure appelée map en go pour stocker l'identifiant de la victime, l'id, et la clé de chiffrement, 'enckey'.

[master/cmd/ransomware/ransomware.go](https://github.com/mauri870/ransomware/blob/master/cmd/ransomware/ransomware.go):109 :
```
func encryptFiles() {
	keys := make(map[string]string)
```

Parfait, il ne nous reste plus qu'à "reverse" la structure map de go. Malheuseuement, il va falloir le faire à la main. Je n'ai pas trouvé de ressources en lignes l'expliquant clairement.

Voici notre petit programme test :
```go
package main

import "fmt"

func main(){
	testmap := make(map[string]string)
	testmap["id"] = "cd18c00bb476764220d05121867d62de"
	testmap["enckey"] = "64e0821c53c7d161099be2188b6cac24"

	fmt.Println(testmap["id"])
	fmt.Println(testmap["enckey"])
}
```

Voici l'état des mémoires :
- Avec 1 élément:
```
0xc00007ee48:	0x01	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee50:	0x00	0x00	0x00	0x00	0x7d	0xbc	0x34	0x50
0xc00007ee58:	0x78	0xee	0x07	0x00	0xc0	0x00	0x00	0x00
0xc00007ee60:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee68:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee70:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee78:	0xf5	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee80:	0xd0	0xf8	0x4c	0x00	0x00	0x00	0x00	0x00
0xc00007ee88:	0x02	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee90:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee98:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007eea0:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007eea8:	0x00	0x00	0x00	0x00


00000000: 0100 0000 0000 0000 0000 0000 7dbc 3450  ............}.4P
00000010: 78ee 0700 c000 0000 0000 0000 0000 0000  x...............
00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000030: f500 0000 0000 0000 d0f8 4c00 0000 0000  ..........L.....
00000040: 0200 0000 0000 0000 0000 0000 0000 0000  ................
00000050: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000060: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000070: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000080: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000090: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000b0: 0000 0000 0000 0000 ed40 4d00 0000 0000  .........@M.....
000000c0: 2000 0000 0000 0000 0000 0000 0000 0000   ...............
000000d0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000e0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000f0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000100: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000110: 0000 0000 0000 0000 0000 0000 0000 0000  ................
```


- Avec 2 éléments
```
0xc00007ee48:	0x02	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee50:	0x00	0x00	0x00	0x00	0x7d	0xbc	0x34	0x50
0xc00007ee58:	0x78	0xee	0x07	0x00	0xc0	0x00	0x00	0x00
0xc00007ee60:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee68:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee70:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee78:	0xf5	0x04	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee80:	0xd0	0xf8	0x4c	0x00	0x00	0x00	0x00	0x00
0xc00007ee88:	0x02	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007ee90:	0x6a	0xfc	0x4c	0x00	0x00	0x00	0x00	0x00
0xc00007ee98:	0x06	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007eea0:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0xc00007eea8:	0x00	0x00	0x00	0x00

00000000: 0200 0000 0000 0000 0000 0000 7dbc 3450  ............}.4P
00000010: 78ee 0700 c000 0000 0000 0000 0000 0000  x...............
00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000030: f504 0000 0000 0000 d0f8 4c00 0000 0000  ..........L.....
00000040: 0200 0000 0000 0000 6afc 4c00 0000 0000  ........j.L.....
00000050: 0600 0000 0000 0000 0000 0000 0000 0000  ................
00000060: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000070: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000080: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000090: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000b0: 0000 0000 0000 0000 ed40 4d00 0000 0000  .........@M.....
000000c0: 2000 0000 0000 0000 8d40 4d00 0000 0000   ........@M.....
000000d0: 2000 0000 0000 0000 0000 0000 0000 0000   ...............
000000e0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000f0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000100: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000110: 0000 0000 0000 0000 0000 0000 0000 0000  ................
```

Cette mémoire réprésente la table de hach pour l'objet testmap. Nous avons tout d'abord la taille de la table (ici 1 puis 2), ensuite 2 pointeurs pour les id avec leur taille et après 2 pointeurs vers les jeux de donnés avec leur taille.
Le premier pointeur `7dbc 3450` ne va pas nous intéresser, mais le deuxième `78ee 0700`, oui, car il va déterminer quel est l'emplacement mémoire du début des pointeurs pointant vers la valeur pour une clé donnée. On doit donc aller chercher à la position 0x0007ee78 de la mémoire pour avoir les pointeurs. Et par une heureuse coïncidence, nous retrouvons nos pointeurs :
- 0x004cf8d0 pour la clé 'id'
- 0x004cfc6a pour la clé 'enckey'
- 0x004d40ed pour la valeur de testmap['id']
- 0x004d408d pour la valeur de testmap['enckey']

On peut remarquer la longueur des clés est codée juste avant le pointeur : 0200 et 0060 ce qui donne id de longueur 2 et enckey de longueur 6, respectivement.

Voilà ce que l'on possède en somme : testmap = map[string]string = {[0x4cf8d0 "id"] = 0x4d40ed "cd18c00bb476764220d05121867d62de", 
  [0x4cfc6a "enckey"] = 0x4d408d "64e0821c53c7d161099be2188b6cac24"}

Essaye de reconstruire cette structure, mais avec notre problème. Nous allons prendre enckey comme point de départ (plus simple à chercher que id dans 250Mo de données).
On le trouve à l'adresse 001d4a36

```
001d4a20: 6f73 6564 636f 6f6b 6965 646f 6d61 696e  osedcookiedomain
001d4a30: 6566 656e 6365 656e 636b 6579 6578 7065  efenceenckeyexpe
001d4a40: 6374 676f 7068 6572 6865 6164 6572 6970  ctgopherheaderip
```

Sauf que nous sommes dans le dmp et pas dans les adresses virtuelles du processus. Pas de panique volatility a pensé à tout, car il peut générer les adresses virtuelles des process en fonction des adresses physiques. Ainsi :

```
volatility --plugins ~/Downloads/volatility_2.6_lin64_standalone/plugins/MonnappaKa -f ../3615_incidents_1/mem.dmp --profile Win10x64_10586 memmap -p 5208 | grep "0x1d4000"
Volatility Foundation Volatility Framework 2.6
0x0000000000661000 0x000000003f41f000             0x1000           0x1d4000
```

Dorénavant, nous savons que l'adresse virtuel de "enckey" est 0x00661a36 cad  361a6600 en mémoire.

Recherchons ce pointeur. Nous tombons cette portions de la mémoire (il y plusieurs candidats celui-ci est le premier à ressembler à ce qe l'on avait en mémoire avec notre programme test) : (à l'adresse 0036b400)
```
0036b320: 0e12 6600 0300 0000 8a14 6600 0400 0000  ..f.......f.....
0036b330: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b340: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b350: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b360: be4c dd00 0000 0000 b615 6600 0400 0000  .L........f.....
0036b370: d517 6600 0500 0000 9c1a 6600 0600 0000  ..f.......f.....
0036b380: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b390: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b3a0: 0000 0000 0000 0000 c110 6600 0200 0000  ..........f.....
0036b3b0: 0e12 6600 0300 0000 8a14 6600 0400 0000  ..f.......f.....
0036b3c0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b3d0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b3e0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b3f0: be94 0000 0000 0000 4911 6600 0200 0000  ........I.f.....
0036b400: 361a 6600 0600 0000 0000 0000 0000 0000  6.f.............
0036b410: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b420: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b430: 0000 0000 0000 0000 c043 8312 2000 0000  .........C.. ...
0036b440: 4044 8312 2000 0000 0000 0000 0000 0000  @D.. ...........
0036b450: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b460: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b470: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b480: 6500 0000 0000 0000 d517 6600 0500 0000  e.........f.....
0036b490: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b4a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b4b0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
0036b4c0: 0000 0000 0000 0000 60d6 6b00 80e0 8a12  ........`.k.....
```

On y trouve ceci : `49 11 66 00 02 00 00 00 36 1A 66 00 06 00`

Et donc si nous allons à l'adresse virtuelle 0x00661149 (avec la correspondance de memmap n'oublions pas) nous trouverons la chaine de caractères `id`: `001d4140: 4667 6f68 3269 2969 4969 6469 7069 766a  Fgoh2i)iIidipivj`  (où id est entre iI et ip).

Bingo ! Nous venons de trouver notre map `key` dans la mémoire. Nous pouvons donc en déduire de manière sûre et certaine l'emplacement de `enckey`:

Nous trouvons juste après les 2 adresses qui représentent les valeurs du map:
- id (ligne 0036b430) => `C0 43 83 12` => `0x128343c0`
- enckey (ligne 0036b440) => `40 44 83 12` => `0x12834440`

Sans oublier l'offset lié au memdump : 

`0x0000000012834000 0x000000003282b000             0x1000           0x357000`

Il faut donc chercher les adresses suivantes :
- id => 003573c0
- enckey => 00357440

Ainsi donc, il y avait bien la bonne clé dans nos 4 concurrents de la méthode 1.

```
003573c0: 6364 3138 6330 3062 6234 3736 3736 3432  cd18c00bb4767642
003573d0: 3230 6430 3531 3231 3836 3764 3632 6465  20d05121867d62de
003573e0: 3634 6530 3832 3163 3533 6337 6431 3631  64e0821c53c7d161
003573f0: 3039 3962 6532 3138 3862 3663 6163 3234  099be2188b6cac24
00357400: 3935 3531 3138 3730 3036 3166 6233 6132  95511870061fb3a2
00357410: 3839 3961 6136 6232 6463 3938 3338 6161  899aa6b2dc9838aa
00357420: 3432 3264 3831 6537 6531 6332 6161 3436  422d81e7e1c2aa46
00357430: 6161 3531 3430 3563 3133 6665 6431 3562  aa51405c13fed15b
00357440: 3935 3531 3138 3730 3036 3166 6233 6132  95511870061fb3a2
00357450: 3839 3961 6136 6232 6463 3938 3338 6161  899aa6b2dc9838aa
```

Finalement :
- id : `cd18c00bb476764220d05121867d62de` 
- enckey : `95511870061fb3a2899aa6b2dc9838aa`

Le flag pour terminer : `ECSC{95511870061fb3a2899aa6b2dc9838aa}`

# Partie 3 - le calme après la tempête

Il faut retourner le fichier flag.docx en clair. Chance pour nous, son chiffré nous est donné par l'énoncé.  
Nous  allons nous servir du code du ransomware pour avoir notre petit déchiffreur et voilà.

```go
// Package cryptofs provides a basic abstraction for file encryption/decryption
package main

import (
	"crypto/aes"
	"crypto/cipher"
	"io"
	"os"
)

// Type File wrap an os.FileInfo
type File struct {
	os.FileInfo
	Extension string // The file extension without dot
	Path      string // The absolute path of the file
}

// Decrypt the file content with AES-CTR with the given key
// sending then to dst
func (file *File) Decrypt(key string, dst io.Writer) error {
	// Open the encrypted file
	inFile, err := os.Open(file.Path)
	if err != nil {
		return err
	}
	defer inFile.Close()

	// Create a 128 bits cipher.Block for AES-256
	block, err := aes.NewCipher([]byte(key))
	if err != nil {
		return err
	}

	// Retrieve the iv from the encrypted file
	iv := make([]byte, aes.BlockSize)
	inFile.Read(iv)

	// Get a stream for encrypt/decrypt in counter mode (best performance I guess)
	stream := cipher.NewCTR(block, iv)

	// Open a stream to decrypt and write to dst
	reader := &cipher.StreamReader{S: stream, R: inFile}

	// Copy the input file to the dst, decrypting as we go.
	if _, err = io.Copy(dst, reader); err != nil {
		return err
	}

	return nil
}

func main(){
	key := "95511870061fb3a2899aa6b2dc9838aa"
	file := new(File)
	file.Path = "data"
	outFile, err := os.OpenFile("flag.docx", os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0600)
	if err != nil {
		err = nil
	}
	defer outFile.Close()
	file.Decrypt(key, outFile)
}
```

Le flag final : ECSC{M4ud1t3_C4mp4gn3_2_r4NC0nG1c13L}
