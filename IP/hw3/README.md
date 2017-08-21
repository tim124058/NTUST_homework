# Homework3

## 檔案說明
	firewall.cpp : 依據rule將封包過濾後輸出
	rule.txt     : 給firewall看的規則檔

## 編譯說明
**測試環境、編譯器版本**  
	
	x64 Ubuntu 15.10 : g++ version 5.2.1   
	OS X El Capitan  : Apple LLVM version 8.0.0 (clang-800.0.38)


**編譯方式**  
	
	$ g++ -std=c++11 firewall.cpp -o firewall -lpcap


## 執行說明  
	第一個參數傳入rule檔案  
	第二個參數傳入要過濾的pcap檔  
	第三個參數為過濾後輸出的pcap檔  
	$ ./firewall <rule.txt> <input.pcap> <output.pcap>
	

## rule檔案說明
	firewall會根據此rule檔來過濾封包，只要Source和Dest的IP,Port一樣，
	此封包就會被丟棄，不管是TCP或UDP。
	
	rule的格式為：
	<Source IP> <Dest IP> <Source port> <Dest port>
	每個字之間以空白分割，請勿使用tab。
	以上為單一個rule，若有多個rule請用換行分割。
	
