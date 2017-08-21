# Homework2

## 檔案說明

```
server.cpp : 傳送資料給clent
client.cpp : 連線到server，並接收server傳送的資料
libTCP.h,libTCP.cpp : packet的struct和一些對packet做處理的function
```

## 編譯說明
**測試環境、編譯器版本**

```
x64 Ubuntu 15.10 : g++ version 5.2.1
OS X El Capitan  : Apple LLVM version 8.0.0 (clang-800.0.38)
```



**編譯方式**

```
直接使用：
$ make
或是：
$ g++ -std=c++11 server.cpp libTCP.cpp -o server
$ g++ -std=c++11 client.cpp libTCP.cpp -o client
```

## 執行說明
**執行步驟：**

```
1. 先執行server，第1個參數為要bind的port，第2個為要傳送給client的封包數。
   執行後開始等待client連線。
    $ server <port> <num of packets>
    
2. 再執行client，第1個參數為server的IP，第二個參數為server的port。
    $ ./client <IP> <port>

3. 執行client後，client丟給server連線請求封包，開始three-way handshake。

4. 連線完後server傳送資料給client，若有封包遺失，會因Timeout或
   Three duplicate acks而重傳資料，因此可以確保資料完整傳輸。

5. server確認送完資料後，傳送fin封包給client，且開始terminate連線。

6. 結束連線後client關閉程式，而server繼續等待其他client連線。
```

**執行範例**

```
1. server
	$ ./server 12345 30
2. client
	$ ./client 127.0.0.1 12345
3. 關閉server：ctrl+c
```

**輸出說明：**

```
1. server或client收到或送出封包時，都會印出封包的seq、ack number，
   且前面若是Receive：則代表收到封包，若是Send：則代表自己送出封包。
2. 若出現 "...discard" 代表前一個Receive的封包被丟棄不做處理或回應。
3. three-way handshake、transmission、terminate時都會顯示開始、結束或錯誤訊息並以[]包起來。
4. Timeout或Three duplicate acks時server會顯示訊息並以#為開頭。
```

**其他 :**

```
1. server第一次送出的seqence number為預設10，client為20。
2. 若要測試Three duplicate，請將client第3行的 DEBUG = 0 改成1，並重新編譯。
3. 若要測試timeout，請將client第3行的 DEBUG = 0 改成2，並重新編譯。
```
