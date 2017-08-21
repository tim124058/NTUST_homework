#!/usr/bin/env python3
import sys
from socket import *
import os

port = 0
files = []


#開始socket
def socketStart():
    global port, files
    Help="""請輸入指令 : 
  help 顯示幫助訊息
  ls 列出有哪些檔案可以下載
  get filename 下載檔案(get %num)
  exit 結束
  """
    ss = socket(AF_INET, SOCK_STREAM)
    ss.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    ss.bind(('' , port))
    ss.listen(5)
    print("等待連線...\n")
    while True:
        try:
            c_sock, c_addr = ss.accept()
            print("Client ", c_addr ," 已連線" )
            c_sock.send(("連線成功...\n\n" + Help).encode())

            #command line
            while True:
                cmd = c_sock.recv(1024).decode()
                if cmd: print(c_addr , " : cmd = " + repr(cmd))

                if cmd[:4] == "get " :              #send file
                    filename = cmd[4:]
                    if filename in (files + ["%"+"%d"%i for i in range(len(files))]):
                        if filename[0] == "%" :
                            filename = files[int(filename[1:])]
                        with open(filename,"rb") as fd:
                            content = fd.read()
                            c_sock.send(("file : " + filename + "\nsize : " + str(len(content)) + "\n").encode())
                            c_sock.recv(10)
                            c_sock.send(content)
                    else:
                        c_sock.send("file not found".encode())
                elif cmd == 'help':                 #help message
                    c_sock.send(Help.encode())
                elif cmd == 'ls':                   #list file
                    msg = ""
                    for i in range(len(files)):
                        msg += "%" + str(i) + " : " + files[i] + "\n"
                    c_sock.send(msg.encode())
                elif cmd == 'exit':                 #close connect
                    break
                else:
                    c_sock.send("command not found".encode())

            c_sock.close()
            print("Client ", c_addr ," 連線已關閉" )
        except KeyboardInterrupt:           #ctrl+c 關閉程式
            print("結束程式...")
            break
        except:                             #client使用ctrl+c
            print("Client ", c_addr ," 連線非預期結束" )


def main():
    global port, files
    usage = "Usage : python3 %s <port num> <file1> <file2> ..." % sys.argv[0]
    try:
        #檢查參數個數
        if len(sys.argv) < 3: raise Exception("argv error")

        #檢查port
        port = int(sys.argv[1])
        if not( 0 <= port < 65536 ): raise Exception("port error")

        #檢查檔案
        files = sys.argv[2:]
        for i in files:
            if not os.path.exists(i): raise Exception("%s : file not found" % i )
        #檢查是否為圖片
        # import imghdr
        # for i in files:
            # if imghdr.what(i) == None: raise
    except Exception as e:
        print("[ERROR] : " ,e)
        print(usage)
        sys.exit(1)

    socketStart()


if __name__ == "__main__":
    main()
