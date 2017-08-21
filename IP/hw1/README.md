# Homework1

### 檔案說明
```
server.py : python3版本的server，只支援單人連線。
MP_server.py : python3版本的server，可以同時多個使用者連線。
client.py : python3版本的client。

server.cpp : c++版本的server，只支援單人連線。
MP_server.py : c++版本的server，可以同時多個使用者連線。
client.cpp : c++版本的client。

語言間可互通(可用python的client連c++的server 或 c++的client連python的server)
```


### 測試環境、編譯器版本
```
x64 Ubuntu 15.10
OS X El Capitan

python3
gcc version 5.2.1
```


### 編譯說明
```
python版的可直接執行。

c++版編譯指令: 
    $ g++ -std=c++11 server.cpp -o server
    $ g++ -std=c++11 client.cpp -o client
```


### 執行說明
**執行步驟：**
```
1. 先執行server.py，第1個參數為要listen的port，第2個以後的參數為要傳輸的檔案
    $ python3 server.py <port num> <file1> <file2> ...      # python3
    或
    $ ./server <port num> <file1> <file2> ...               # c++
    
2. 執行client.py，第1個參數為server的IP，第二個參數為server的port
    $ python3 client.py <IP address> <port num>             # python3
    或
    $ ./client <IP address> <port num>                      # c++

3. client連上server後，可使用的指令：
    > help 顯示幫助訊息
    > ls 列出所有可下載的檔案
    > get filename 下載檔案(get %num)
    > exit 結束程式
```

**example :**
```
1. server
    $ python3 server.py 12345 img_1.jpg img_2.jpg img_3.jpeg
    
2. client
    $ python3 client.py 127.0.0.1 12345
    > ls
    %0 : img_1.jpg
    %1 : img_2.jpg
    %2 : img_3.jpeg
    > get %0                            # client下載img_1.jpg
    > get img_3.jpeg                    # client下載img_3.jpeg
    > exit 
    
3. 關閉server：ctrl+c
```

