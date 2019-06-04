---
title: BreizhCTF 2k19
---

Pour les challenges avec des server, j'ai utilisé ma librairie [SacriCat](https://github.com/Sacriyana/SacriCat) qui sera mise à jour sous peu.

# Cococomparizon - Programmation - 75pts
## Le problème

Un challenge de programmation basique basé sur le bruteforce et sur le temps.

```python
def cococompare_da_flag(flagplz):
    if(len(flagplz) == 0):
        return False
    if(len(flagplz) > len(flag)):
        return False
    if flagplz.lower() == "exit":
        exit(1)
    for i in range(len(flagplz)):
        if flagplz[i] != flag[i]:
            return False
        sleep(0.25)
    return True
```

En somme le temps va augmenter à chaque bon caractère du flag.
On va d'abord tester à la main quelque combinaison pour éviter de bruteforce tous les caractères :
- bzhctf{
- breizhctf{
- BZHCTF{
- BREIZHCTF{

Le premier est le bon. On a le début du flag.

## La solution

On va tester les caractères imprimables jusqu'à en avoir un bon puis on recommence. On peut directement tester le caractère `{` histoire de gagner un peu de temps.

```python
#!/usr/bin/python3
#coding: utf-8
from sacricat.client import Server, logging

from time import time,sleep
import string

def try_str(client,str):
    client.sendLine(str)
    temps = time()
    r = client.recv()
    if '>>> ' not in r:
        r = client.recv()
    temps = time() - temps
    return temps

client = Server('ctf.bzh',9002, prompt=">>> ",timeout=5, logLevel=logging.SENT)
print(client.recv())
print(client.recvUntil('\r\nWhatz da flag >>> '))
waiting_time = 0.25
flag = 'bzhctf{'

for i in range(len(flag),50):

    flag_temp = flag + '}'
    print('try %s' % flag_temp)
    temps = try_str(client,flag_temp)
    print(((waiting_time)*(i+1)-0.1))
    print(temps)
    print(((waiting_time)*(i+1)+0.1))
    if temps < ((waiting_time)*(i+1)+0.1) and temps > ((waiting_time)*(i+1)-0.1):
        flag += c
        print("flag: %s" % flag)
        break
    sleep(0.01)

    for c in string.printable:
        flag_temp = flag + c
        print('try %s' % flag_temp)
        temps = try_str(client,flag_temp)
        print(((waiting_time)*(i+1)-0.1))
        print(temps)
        print(((waiting_time)*(i+1)+0.1))
        if temps < ((waiting_time)*(i+1)+0.1) and temps > ((waiting_time)*(i+1)-0.1):
            flag += c
            break
        sleep(0.01)
```

Et au bout de quelque minute, le flag tombe : `bzhctf{tim1ng_l0l}`

# Do you Google - Misc - 100pts

Ici, nous devons chercher sur Google les anciens flags du breizhCTF pour déchiffrer un fichier contenant le mot de passe.

```
Instructions:
I've encrypted the file flag.txt and I lost the password. Your goal 
will be to decrypt this file.

mitsurugi@dojo:~/chall/breizh/MISC$ openssl enc -aes256 -base64 -in flag.txt -out flag.txt.enc
enter aes-256-cbc encryption password:
Verifying - enter aes-256-cbc encryption password:
mitsurugi@dojo:~/chall/breizh/MISC$ file flag.txt.enc 
flag.txt.enc: ASCII text
mitsurugi@dojo:~/chall/breizh/MISC$

Warning: this is not a crypto challenge, this is a "google" challenge.

I was reading some writeups of previous BreizhCTF, and I copy one of the 
flags. I've then encrypt the file flag.txt.
Your goal will be to search through all writeups of previous BREIZHCTF and
try all flags until you find the good one.
```

J'ai pu récolter 26 flags différents :

```
BREIZHCTF{TDDE!!!Bon_OK_J_avoue_La_Crypto_Et_SaxX_C_EST_L_OPPOSE!!!TDDE}
BZHCTF{0bFu5c4710n_15_n07_53Curi7y_w3lc0m3_t0_JS_w0rld}
BZHCTF{1_w45_br0k3_bu7_n0_m0r3}
BZHCTF{3v3rY_d4y_1_m_Shuff71n6}
BZHCTF{3v3rY_d4y_1_n_Shuff71n6}
BZHCTF{Ok_You_G0t_m3_It_was_T0o_eASy_4_YoU}
BZHCTF{R3fl3x10n_15_50_c00l_1f_Y0u_4r3_n0oBzz!#GAME}
BZHCTF{Such_4_Pitty_S33m5_d47_b17c0in_c0ll4ps3d_Bu7_BreizhC0in_M19H7_B3_4n_4lt3n4t1v3!}
BZHCTF{This_key_exchange_Sux}
BZHCTF{bl4ck_duck_d0nt_4sk_m3_why_I_was_n0t_r3allt_inspir3d}
BZHCTF{d4t_vulnzzz_0mfg}
BZHCTF{flag_pyjail_1}
BZHCTF{flag_pyjail_2}
BZHCTF{flag_pyjail_3}
BZHCTF{w3_4r3_r34lly_gl4d_70_533_y0u_w3lc0me}
BZHCTF{wh3n_1_s4y_C0w_Y0u_s4y_C0w}
BZHCTF{y0u_kn0w_by_n0w_4_l177l3_b17_m0r3_4b0u7_d1ff3r3n7_py7h0n_exceptions_4nd_m4yb3_m0r3}
bzhctf{Bru73f0rC1n9_hm4c_15n7_4_pUss35}
bzhctf{XoRXoRXoR}
bzhctf{YOLOCRYPTO2017}
bzhctf{cust0m_b4sic_4u70_h07_k3ys}
bzhctf{cust0m_e4svc_4u70_h07_k3ys}
bzhctf{h3llf35t_rul3z}
bzhctf{h4cking_to_b3_pr3s1dent_is_tr4g1ck}
bzhctf{nsa_b4ckd00ring_you_since_1952}
bzhctf{s1gn_my_455}
```

A l'aide du flag `bzhctf{cust0m_b4sic_4u70_h07_k3ys}`, j'ai pu déchiffrer le fichier :

```
You're really good at searching google \o/
Maybe you're lucky, too.

Flag is:
BZHCTF{Sometimes_it_is_faster_to_do_a_google_search_instead_of_bruteforcing}

Now some random blahblahblah to increase the size of the file.
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
blahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblahblah
```

# Guess my age - Programmation - 200pts

Un autre challenge de programmation très simple basé sur la dichotomie. Il faut trouver un nombre compris entre 0 et 10 puissance nombre d'étape et bien sûr, en un minimum de coup car sinon on est 

```
[2019-04-12 23:51:55][+] ctf.bzh:9004 - Connected
New turn ! [0,10]
[0,5]
[0,2]
[0,1]
New turn ! [0,100]
[0,50]
[25,50]
[25,37]
[31,37]
New turn ! [0,1000]
```

A chaque tour, la borne supérieure augmente de 10. On va faire ça en Python. La seule "petite" chose est qu'il faut utiliser la division `//` et non la division `/` car sinon vous allez avoir des problèmes d'approximation à partir d'une étape élevée. Et bien sûr, pas besoins spécifier un long puisque le type long a disparu en Python3.

```python
#!/usr/bin/python3
#coding: utf-8
from sacricat.client import Server, logging
import re

low_re = re.compile('Low: ([0-9])+')
high_re = re.compile('High: ([0-9])+')

client = Server('ctf.bzh',9004, prompt="\r\nInput a value:\r\n",timeout=5, logLevel=logging.BASIC)
r = client.recvUntil()

while 1:
    low = int(low_re.search(r)[0].split(': ')[1])
    high = int(high_re.search(r)[0].split(': ')[1])

    v = high//2
    client.sendLine(str(v))
    r = client.recvUntil()
    
    print("New turn ! [%s,%s]"% (low,high))

    while 'Correct!\r\n' not in r:
        if 'Too many guesses!' in r:
            exit(0)
        if 'Lower' in r :
            high = v
        if 'Higher' in r :
            low = v

        print("[%s,%s]"% (low,high))

        v = (low+high)//2
        client.sendLine(str(v))

        r = client.recvUntil()
    if 'BREIZHCTF' in r:
        print(r)
        exit(0)
```

Et après quelques secondes, le flag apparait :

```
[3117373241250201284661545573821,3117373241250201284661545573836]
Correct!
BREIZHCTF{l1f3_Is_T00_bioutifoul_mate_so_dont_forget_to_breath_and_LIVE}
```

# Hallowed by thy name - Crypto - 300pts

[Tipi'hack](https://tipi-hack.github.io/) a déjà publier un [write-up](https://tipi-hack.github.io/2019/04/14/breizhctf-19-hallowed-be-thy-name.html) très complet.

Je vais vous proposer une autre solution où la team [Hackatsuki](https://ctftime.org/team/65393) m'a donné un p’tit coup de pouce. (Une petite erreur de comparaison sur mon premier essai avec la methode de tipihack)

Ce challenge est basé sur la gestion des seeds dans les random. Et bien sûr la génération de nombre (variable `r`) est identique pour n'importe quel client car pour rappel, les générations avec une même seed donneront toujours la même suite de nombre.

```python
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
init_seed = random.randint(0,65535)
class client(Thread):
	...
    def get_keystream(self, r, length):
    	r2 = random.Random()
    	seed = r.randint(0, 65535)
    	r2.seed(seed)
        mask = ''
        for i in range(length):
            mask += chr(r2.randint(0, 255))
        return mask

    def run(self):
        r = random.Random()
        r.seed(init_seed)

        self.sock.send(b'Welcome to the Cipherizator !\n1 : Enter plain, we give you the cipher\n2 : Need a flag ?\n3 : Exit')    
        while 1:
            self.sock.send(b'\n>>> ')
            response = self.sock.recv(2).decode().strip()
            if response == "1":
                self.sock.send(b'\nEnter plain : ')
                plain = self.sock.recv(1024).decode().strip()
                mask = self.get_keystream(r, len(plain))
                self.sock.send(b'Your secret : %s' % self.xor(mask, plain))
            elif response == "2":
                mask = self.get_keystream(r, len(FLAG))
                self.sock.send(b'Your secret : %s' % self.xor(mask, FLAG))
	...
if __name__ == "__main__":
	...
    while 1:
        clientsocket, address = serversocket.accept()
        print("new client : %s" % clientsocket)
        client(clientsocket, address)
```

Donc si on arrive à avoir le masque utilisé pour le flag alors il sera possible de le déchiffrer. En sachant que la génération est identique pour n'importe quel client, il suffit dés lors de réaliser 2 connexions d'une commande chacune :
- 1 : récupérer la base64 du flag pour déterminer la taille du masque
- 2 : envoyer une chaine de caractère de la longueur du flag composée uniquement que de `\x00`, car A = A xor 0, pour ainsi obtenir lez masque utilisé.

```python
#!/usr/bin/python3
#coding: utf-8

from sacricat.client import Server, logging
import base64
import random

def get_keystream(r, length):
	r2 = random.Random()
	seed = r.randint(0, 65535)
	r2.seed(seed)
	mask = ''
	for i in range(length):
		mask += chr(r2.randint(0, 255))
	return mask

def xor(a, b):
	cipher = ''
	for i in range(len(a)):
		cipher += chr((a[i]) ^ (b[i]))
	return cipher

def send_plain(plain):
	client = Server('ctf.bzh',11000, prompt=">>> ",timeout=5, logLevel=logging.SENT)
	client.sendLine('1')
	client.recvUntil('Enter plain : ')
	client.sendLine(plain)
	r = client.recvUntil()
	client.close()
	# print(r)
	secret = r.split(':')[1].strip()
	secret = secret.split('>>>')[0].strip()
	print(secret)
	return base64.b64decode(secret)


def send_flag():
	client = Server('ctf.bzh',11000, prompt=">>> ",timeout=5, logLevel=logging.SENT)
	client.sendLine('2')
	client.recvUntil('')
	r = client.recvUntil()
	client.close()
	secret = r.split(':')[1].strip()
	secret = secret.split('>>>')[0].strip()
	print(secret)
	return base64.b64decode(secret)

cipher_flag = send_flag()
print(len(cipher_flag))
cipher_test = send_plain('\x00'*len(cipher_flag))
print(xor(cipher_flag,cipher_test))


```

Et voila le flag `bzhctf{The_sands_of_time_for_me_are_running_low}` : 

```
./solve.py
wokxVwGu9MpawtfYUNRyuPjbM/XdO7H1vBU1ckUwuPlbMl6oV1WD4/WUcx+d+I4V
48
oPNZNHXIj54yp4irMboWy6e0VaqpUtyQ43NaABpd3aY6QDv3JSDtjZz6FEDxl/lo
bzhctf{The_sands_of_time_for_me_are_running_low}
```

# Tendu comme des strings - Programmation - 100pts

Allez, un petit challenge d'OCR. On utilise [Tesseract](https://github.com/tesseract-ocr/tesseract) avec un wrapper python. J'ai essayé le wrapper `pytesseract` mais il n'y a pas fonctionné et comme je n'avais pas envie de configurer l'image (le dpi notamment), j'ai testé `pyocr` qui a marché du premier coup.

NB : ce code est un "chouia" améliorable

```
#!/usr/bin/python3
#coding: utf-8

from io import BytesIO
from PIL import Image
import requests
import re
import base64
import pyocr
import pyocr.builders

tools = pyocr.get_available_tools()
tool = tools[0] # Tesseract, maybe other if you had install another OCR.
langs = tool.get_available_languages()
lang = langs[0] # Language : EN

png_re = re.compile('base64,(.*?)"')

s = requests.Session()
r = s.get("http://ctf.bzh:9003/")
pngs = png_re.findall(r.text)
png = Image.open(BytesIO(base64.b64decode(pngs[0])))
txt = tool.image_to_string(png,lang=lang,builder=pyocr.builders.TextBuilder())
sol = txt
png = Image.open(BytesIO(base64.b64decode(pngs[1])))
txt = tool.image_to_string(png,lang=lang,builder=pyocr.builders.TextBuilder())
sol += txt

r = s.post("http://ctf.bzh:9003/",data={'string':sol})
pngs = png_re.findall(r.text)
png = Image.open(BytesIO(base64.b64decode(pngs[0])))
txt = tool.image_to_string(png,lang=lang,builder=pyocr.builders.TextBuilder())
sol = txt
png = Image.open(BytesIO(base64.b64decode(pngs[1])))
txt = tool.image_to_string(png,lang=lang,builder=pyocr.builders.TextBuilder())
sol += txt

r = s.post("http://ctf.bzh:9003/",data={'string':sol})
pngs = png_re.findall(r.text)
png = Image.open(BytesIO(base64.b64decode(pngs[0])))
txt = tool.image_to_string(png,lang=lang,builder=pyocr.builders.TextBuilder())
sol = txt
png = Image.open(BytesIO(base64.b64decode(pngs[1])))
txt = tool.image_to_string(png,lang=lang,builder=pyocr.builders.TextBuilder())
sol += txt
r = s.post("http://ctf.bzh:9003/",data={'string':sol})
print(r.text)


```

Et voila le flag: `bzhctf{you_know_how_to_read_simple_images!}`

```
./solve.py
<!DOCTYPE html>
<html>
        <head>
                <meta charset="UTF-8">
                <title>Can you defeat this strings?</title>
        </head>
        <body>
                <center>
                      
<br>
<h2>Well done! Here is the flag : <h1>bzhctf{you_know_how_to_read_simple_images!}</h1></h2>
<br>
```

# Tout les chemins mènent au flag - Misc/Programmation - 150pts

Nous voilà avec un autre challenge de programmation classique où il faut battre 500 fois le server à un jeu du morpion. Malheureusement, il y avait une configuration où le serveur avait des ratés. Donc j'ai relancé mon script pour avoir le flag : `bzhctf{tous_les_chemins_menent_au_flag_certains_sont_plus_courts_que_dautres_ou_pas}`

Pour la résolution du morpion, je laisse ça à un [projet Github](https://github.com/marcyves/petits-jeux-Python/blob/master/morpion/morpionOO.py), autant gagner du temps.

```
#!/usr/bin/python3
#coding: utf-8
from sacricat.client import Server, logging
import numpy as np
import re
import sys

class morpion:
    def __init__(self):
        self.table_jeu   = ["", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
        self.pion_joueur = "K"
        self.pion_ordi   = "O"

    def affiche_jeu(self):
        print ("  " + self.table_jeu[1] + " | " + self.table_jeu[2] + " | " + self.table_jeu[3] + "")
        print (" -----------")
        print ("  " + self.table_jeu[4] + " | " + self.table_jeu[5] + " | " + self.table_jeu[6] + "")
        print (" -----------")
        print ("  " + self.table_jeu[7] + " | " + self.table_jeu[8] + " | " + self.table_jeu[9] + "")

    def cherche_position_gagnante(self, pion):
        if (self.table_jeu[1] == pion and self.table_jeu[2] == pion and self.table_jeu[3] == "3" ):
            return 3
        if (self.table_jeu[1] == pion and self.table_jeu[2] == "2"  and self.table_jeu[3] == pion ):
            return 2
        if (self.table_jeu[1] == "1"  and self.table_jeu[2] == pion and self.table_jeu[3] == pion ):
            return 1
        if (self.table_jeu[4] == "4"  and self.table_jeu[5] == pion and self.table_jeu[6] == pion ):
            return 4
        if (self.table_jeu[4] == pion and self.table_jeu[5] == "5"  and self.table_jeu[6] == pion ):
            return 5
        if (self.table_jeu[4] == pion and self.table_jeu[5] == pion and self.table_jeu[6] == "6" ):
            return 6
        if (self.table_jeu[7] == "7" and self.table_jeu[8] == pion and self.table_jeu[9] == pion ):
            return 7
        if (self.table_jeu[7] == pion and self.table_jeu[8] == "8" and self.table_jeu[9] == pion ):
            return 8
        if (self.table_jeu[7] == pion and self.table_jeu[8] == pion and self.table_jeu[9] == "9" ):
            return 9
        if (self.table_jeu[1] == "1" and self.table_jeu[4] == pion and self.table_jeu[7] == pion ):
            return 1
        if (self.table_jeu[1] == pion and self.table_jeu[4] == "4" and self.table_jeu[7] == pion ):
            return 4
        if (self.table_jeu[1] == pion and self.table_jeu[4] == pion and self.table_jeu[7] == "7" ):
            return 7
        if (self.table_jeu[2] == "2" and self.table_jeu[5] == pion and self.table_jeu[8] == pion ):
            return 2
        if (self.table_jeu[2] == pion and self.table_jeu[5] == "5" and self.table_jeu[8] == pion ):
            return 5
        if (self.table_jeu[2] == pion and self.table_jeu[5] == pion and self.table_jeu[8] == "8" ):
            return 8
        if (self.table_jeu[3] == "3" and self.table_jeu[6] == pion and self.table_jeu[9] == pion ):
            return 3
        if (self.table_jeu[3] == pion and self.table_jeu[6] == "6" and self.table_jeu[9] == pion ):
            return 6
        if (self.table_jeu[3] == pion and self.table_jeu[6] == pion and self.table_jeu[9] == "9" ):
            return 9
        if (self.table_jeu[1] == "1" and self.table_jeu[5] == pion and self.table_jeu[9] == pion ):
            return 1
        if (self.table_jeu[1] == pion and self.table_jeu[5] == "5" and self.table_jeu[9] == pion ):
            return 5
        if (self.table_jeu[1] == pion and self.table_jeu[5] == pion and self.table_jeu[9] == "9" ):
            return 9
        if (self.table_jeu[7] == "7" and self.table_jeu[5] == pion and self.table_jeu[3] == pion ):
            return 7
        if (self.table_jeu[7] == pion and self.table_jeu[5] == "5" and self.table_jeu[3] == pion ):
            return 5
        if (self.table_jeu[7] == pion and self.table_jeu[5] == pion and self.table_jeu[3] == "3" ):
            return 3
        return 0

    def jeu_ordi(self):
        # L'ordi cherche d'abord une position gagnante pour lui
        coup_ordi = self.cherche_position_gagnante(self.pion_ordi)
        if coup_ordi == 0:
            # ensuite il vérifie que le joueur n'est pas en position de gagner
            coup_ordi = self.cherche_position_gagnante(self.pion_joueur)
            if coup_ordi == 0:
                # reste à voir si le centre est encore libre
                if self.table_jeu[5] == "5":
                    coup_ordi = 5
                else :
                    # Sinon cherche le premier endroit libre
                    for i in range(10):
                        if (self.table_jeu[i]== str(i)):
                            coup_ordi = i
                            break
        return coup_ordi


    def parse(self,resp):
        '''
             |  K  |   
        ===================
             |  S  |   
        ===================
             |     |
         '''
        print(resp)
        resp = resp.split('Turn number')[0]
        lines = resp.split('===================')
        table = ["", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
        print(lines)
        i = 1
        for line in lines:
            for case in line.split('|'):
                case = case.strip()
                print('case %s: %s'%(i,case))
                if case == 'K':
                    table[i] = self.pion_joueur
                if case == 'S':
                    table[i] = self.pion_ordi
                i += 1
        self.table_jeu = table
        self.affiche_jeu()

client = Server('ctf.bzh',42000, prompt="\r\nWhere would you like to place S (1-9)? ",timeout=5, logLevel=logging.SENT)
m = morpion()

while 1:
    r = client.recvUntil()
    r = r.split("The board look like this:")[2]
    if "THEMACHINE BEATS YOU" in r:
        exit(1)
    m.parse(r)
    coup = m.jeu_ordi()
    print(coup)
    client.sendLine(str(coup))
```
