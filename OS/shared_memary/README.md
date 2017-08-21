# shared_memary
<br>
執行檔編譯環境:<br>
x64 ubuntu 15.10<br>
<br>
<br>
編譯指令：<br>
gcc ProcessA.c -o ProcessA -lrt<br>
gcc ProcessB.c -o ProcessB -lrt<br>
<br>
<br>
執行步驟：<br>
只需執行ProcessA，<br>
輸入一個整數，會借由shared memory，將輸入的數字傳到ProcessB，<br>
ProcessB會將入的數字處理後，放回shared memory，<br>
再由ProcessA讀取，並顯示出來。<br>
<br>
<br>
輸入正整數：回傳此正整數的二進位。<br>
輸入-1：回傳前面所有輸入轉成二進位後，1的個數最多的資料。<br>
輸入-2：回傳前面所有輸入轉成二進位後，1的個數最少的資料。<br>


