#!/usr/bin/env python3
import sys
from socket import *

port = 0
IP = ""

#開始socket
def socketStart():
    global IP, port
    try:
        cs = socket(AF_INET, SOCK_STREAM)
        cs.connect((IP, port))
        message = cs.recv(1024).decode()
        print(message)
        while True:
            cmd = input("> ")
            if len(cmd) == 0: continue
            cs.send(cmd.encode())           #send command
            if cmd == "exit": break         #close connect
            msg = cs.recv(1024).decode()
            print(msg)

            # get file
            if cmd[:4] == "get " and msg != "file not found":
                filename = msg.split('\n')[0].split(" : ")[1]
                size = int(msg.split('\n')[1].split(" : ")[1])
                cs.send(b"OK")

                # receive file
                content = b""
                while True:
                    tmp = cs.recv(size)
                    size -= len(tmp)
                    content += tmp
                    if(size<=0): break

                with open(filename,"wb") as fd:
                    fd.write(content)
                print("檔案傳輸成功!")

        cs.close()
        print("連線已關閉...")
    except KeyboardInterrupt:           #ctrl+c 關閉程式
        print("結束程式...")
        sys.exit(0)
    except Exception as e:
        sys.exit(1)


def main():
    global IP, port
    usage = "Usage : python3 %s <IP address> <port num> " % sys.argv[0]

    try:
        #檢查參數個數
        if len(sys.argv) != 3: raise Exception("argv error")
        
        #檢查IP
        IP = sys.argv[1]
        tmp = IP.split(".")
        if(len(tmp) != 4): raise Exception("ip error")
        for i in tmp:
            if not(0<=int(i)<=255): raise Exception("ip error")

        #檢查port
        port = int(sys.argv[2])
        if not( 0 <= port < 65536 ): raise Exception("port error")
    except Exception as e:
        print("[ERROR] : " ,e)
        print(usage)
        sys.exit(1)

    socketStart()


if __name__ == "__main__":
    main()
