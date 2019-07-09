import binascii
import base64
import dpkt
import os
from Crypto.Cipher import ARC4
import struct

def rc4(k, d):
    d = binascii.unhexlify(d).decode().encode('latin-1')
    cipher = ARC4.new(k.encode())
    return binascii.unhexlify(cipher.decrypt(d))

filename='sinkhole_capture.pcap'
list_files = []
i = 0
file_data = b''

dirName = 'recover_files'
if not os.path.exists(dirName):
    os.mkdir(dirName)

for ts, pkt in dpkt.pcap.Reader(open(filename,'rb')):
    eth = dpkt.ethernet.Ethernet(pkt)
    ip = eth.data
    tcp = ip.data
    ack = tcp.ack
    if len(tcp.data) > 0:
        try:
            http = dpkt.http.Request(tcp.data)
            if 'panel.php' in http.uri:
                encoded_data = base64.b64decode(http.headers['content-md5'])
                # print(ack, type(encoded_data))
                data = rc4(str(ack),encoded_data)
                file_data += data
                if len(data) < 16:
                    if not list_files:
                        list_files = file_data.decode().split('\n')
                    else:
                        with open(dirName+"/"+list_files[i].replace('/','_'),'wb') as file_i:
                            file_i.write(file_data)
                        file_data = b''
                        i += 1
        except Exception as e:
            pass

