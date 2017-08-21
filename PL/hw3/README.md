# 程式語言 programming assignment 1

## 編譯和執行方式
```
	直接執行(UNIX-like)：
	$ ./compileAndRun.sh
	
	或在compileAndRun.sh同層目錄下執行：
	javac -sourcepath src -classpath bin -d bin src/Application.java 
	java -classpath bin Application
```

## 程式說明

### Application
可以開啟Checking、Saving、CD、Loan Account，並且可對這幾種Account進行：
deposit、withdraw、balance、compute_interest等操作。  
在每次操作時都會**先計算利息**。  

**main**：內主要是在測試各個帳戶是否正確。  
**parseDate**：回傳一個由"year/month/day"字串為格式的Date物件。  

以下4個function針對某個Account操作，第一個參數指定要操作的帳戶；且都有Date參數，指定操作日期，若日期填入null代表使用預設時間(執行時的時間)：  

* **depositIntoAccount**：將錢存入某個帳戶。  
* **withdrawFromAccount**：從某個帳戶中領出錢。  
* **computeAccountInterest**：計算帳戶的利息。   
* **AccountBalance**：回傳某帳戶的餘額。 

### Account
為所有帳戶的父類別，每個帳戶都有：  
**帳戶名稱**、**餘額**、**利率**、**開設帳戶日期**、**最後計算利息日期**等屬性。  
**name**：回傳帳戶名稱。  
**balance**：回傳餘額。  
  
以下3個function在繼承Account時都必須實作出來：  

 * **deposit**：存錢並計算利息
 * **withdraw**：領錢並計算利息
 * **computeInterest**：計算利息

### CheckingAccount
利息是以**天**來計算，沒有手續費，但餘額最少必須為$1000。 

### SavingAccount
利息是以**月**來計算，除了每個月的前3次，領錢或存錢都要$1的手續費，餘額最少為$0(不能為負數)。
  
### CDAccount
利息是以**月**來計算，開完帳戶後就不能再存錢進去，在1年內領錢都要$250的手續費，1年後則停止計算利息。

### LoanAccount
利息是以**月**來計算，除了每個月的前3次，存錢要$1的手續費，且不能領錢(借更多錢)，大部分都跟SavingAccount一樣，只是balance是負的。  
