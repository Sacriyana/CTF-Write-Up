# Factorial - MISC

Enoncés :
```
Tout est là : https://fr.wikipedia.org/wiki/Factorielle

nc challenges.ecsc-teamfrance.fr 3000
```

Ce challenge a pour but de manipuler des factorielles au début petites, mais qui vont s'agrandir au fil du temps.
L'exercice principal étant de trouver les n derniers nombres non nuls d'une factorielle avec n <= 5.

Pour les premières étapes, je vous laisserais lire le script solve.py car ce n'est pas la que ce situe le challenge.

Le réel challenge vient à partir de 26 où l'on a un nombre à 8, puis à 12 et enfin à 50 à calculer.

Sur les problèmes de programmation et les maths, je n'aime pas recoder la roue. Je me suis donc mis à la recherche d'un script le faisant pour moi.  
Je suis rapidement tombé sur [l'exercice 160 du projet euler](https://projecteuler.net/problem=160) qui demandait de trouver les 5 derniers chiffres non nuls d'une factorielle.
Et bien évidemment, si j'en possède 5 j'en possède donc 2,3 ou 4. J'ai tout d'abord testé avec le [script Python de Nayuki](https://github.com/nayuki/Project-Euler-solutions/blob/master/python/p160.py). Malheureusement celui-ci n'était pas suffisamment performant pour les dernières étapes. J'ai donc continué mes recherches pour arriver sur ce [site](https://euler.stephan-brumme.com/160/) qui liste un grand nombre de solutions de ce problème. Par chance, pas besoin de compiler du C++, il y a un script python qui fera très bien l'affaire : [celui de Meng-Gen Tsai](https://raw.githubusercontent.com/Meng-Gen/ProjectEuler/master/160.py) qui va s'appuyer sur le théorème de restes chinois et du module inverse pour trouver les chiffres.

Avec ce script, c'est bon, le flag arrive au bout de quelques secondes : ECSC{eacb38eaa12fef2c0a626a2d5f5a57d28e005bfb}

Pour la partie communication, j'ai utilisé [SacriCat](https://github.com/Sacriyana/SacriCat) qui permet de manipuler facilement les sockets en python3.

Voici un historique de la transaction.

```
PS E:\security\CTF\ecsc\misc\factorial> python.exe solve.py
[2019-05-14 23:19:05][+] challenges.ecsc-teamfrance.fr:3000 - Connected
Step : 0
What is the last digit of 2!
>> 2
Step : 1
What is the last digit of 6!
>> 0
Step : 2
What is the last digit of 80!
>> 0
Step : 3
What is the last digit of 42!
>> 0
Step : 4
What is the last digit of 39!
>> 0
Step : 5
What is the last digit of 6360!
>> 0
Step : 6
What is the last digit of 8746!
>> 0
Step : 7
What is the last digit of 2538!
>> 0
Step : 8
Okay, so there seems to be a lot of zeros...
So now, we remove all the zeros appearing at the end of n! (and only at the end), and consider again the last digits.
For example, the last nonzero digit of 5! is '2', the 2 last nonzero digits of 6! are '72', and the 2 and 3 last nonzero digits of 7! are '04' and '504', respectively.
What are the 2 last nonzero digits of 10!
>> 88
Step : 9
What are the 2 last nonzero digits of 9!
>> 88
Step : 10
What are the 3 last nonzero digits of 20!
>> 664
Step : 11
What are the 3 last nonzero digits of 12!
>> 016
Step : 12
What are the 4 last nonzero digits of 27!
>> 0768
Step : 13
What are the 4 last nonzero digits of 28!
>> 1504
Step : 14
What are the 2 last nonzero digits of 71!
>> 28
Step : 15
What are the 2 last nonzero digits of 60!
>> 96
Step : 16
What are the 3 last nonzero digits of 96!
>> 456
Step : 17
What are the 3 last nonzero digits of 29!
>> 616
Step : 18
What are the 4 last nonzero digits of 35!
>> 5232
Step : 19
What are the 4 last nonzero digits of 23!
>> 7664
Step : 20
What are the 2 last nonzero digits of 8945!
>> 08
Step : 21
What are the 2 last nonzero digits of 7239!
>> 92
Step : 22
What are the 3 last nonzero digits of 4662!
>> 288
Step : 23
What are the 3 last nonzero digits of 3061!
>> 856
Step : 24
What are the 4 last nonzero digits of 5719!
>> 6384
Step : 25
What are the 4 last nonzero digits of 588!
>> 2928
Step : 26
What are the 2 last nonzero digits of 77574162!
>> 92
Step : 27
What are the 2 last nonzero digits of 1736170!
>> 24
Step : 28
What are the 3 last nonzero digits of 45608156!
>> 432
Step : 29
What are the 3 last nonzero digits of 27721344!
>> 696
Step : 30
What are the 4 last nonzero digits of 49556296!
>> 7424
Step : 31
What are the 4 last nonzero digits of 88487708!
>> 8032
Step : 32
What are the 2 last nonzero digits of 468617445604!
>> 76
Step : 33
What are the 2 last nonzero digits of 949904022720!
>> 52
Step : 34
What are the 3 last nonzero digits of 537227676089!
>> 168
Step : 35
What are the 3 last nonzero digits of 165793822012!
>> 624
Step : 36
What are the 4 last nonzero digits of 728843422102!
>> 8064
Step : 37
What are the 4 last nonzero digits of 578879913261!
>> 6224
Step : 38
What are the 2 last nonzero digits of 95353669164969728983687985693734946622770201500848!
>> 44
Step : 39
What are the 2 last nonzero digits of 87993142852930615009890201021646329526014079747468!
>> 12
Step : 40
What are the 2 last nonzero digits of 41731045042162753854706015098870939775222243268648!
>> 44
Step : 41
What are the 3 last nonzero digits of 34743387798278953384024404717437821387161951750210!
>> 576
Step : 42
What are the 3 last nonzero digits of 76233776528158591177331787086364480736148118361690!
>> 416
Step : 43
What are the 3 last nonzero digits of 47997503383061521158527104442709387113846626335396!
>> 488
Step : 44
What are the 4 last nonzero digits of 70020276530360084630965840721508111079018170815766!
>> 6928
Step : 45
What are the 4 last nonzero digits of 19906958759917912695287686180531275599773742027207!
>> 4304
Step : 46
What are the 4 last nonzero digits of 50703352948471767607217312460285577817124521607247!
>> 2048
Step : 47
What are the 4 last nonzero digits of 69158094064668786818924286541050267854196912706386!
>> 0352
Congrats! Here is your flag: ECSC{eacb38eaa12fef2c0a626a2d5f5a57d28e005bfb}
```

