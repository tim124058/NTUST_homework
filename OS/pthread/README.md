# pthread

**程式功能說明**
```
    在main的參數傳入1~3個檔案，然後程式會以多執行緒計算單字出現的次數。
```
  
**執行檔編譯環境：**  
```
    x64 Ubuntu 15.10  
```
  
**編譯指令：**
```
    g++ WordAnal.cpp -o WordAnal -lpthread

    如果有安裝make，可直接使用make。
```
  
**執行方法：**
```
    ./WordAnal [FirstTextFile] [SecondTextFile] [ThirdTextFile]
```
  
