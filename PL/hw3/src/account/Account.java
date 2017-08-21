package account;
import java.util.*;

class BankingException extends Exception {
    BankingException () { super(); }
    BankingException (String s) { super(s); }
} 

public abstract class Account {
    // protected variables to store commom attributes for every bank accounts	
    protected String accountName;           //帳戶名稱
    protected double accountBalance;        //餘額
    protected double accountInterestRate;   //利率   
    protected Date openDate;                //開設帳戶日期   
    protected Date lastInterestDate;        //最後計算利息日期
    
    public Account(String s, double firstDeposit, Date firstDate){
        accountName = s;
        accountBalance = firstDeposit;
        accountInterestRate = 0.12;
        openDate = ((firstDate != null)?firstDate:new Date());
        lastInterestDate = openDate;  
    }

    //return account的名字
    public String name() {
    	return(accountName);	
    }	
    
    //return balance並計算利息
    public double balance(Date interestDate) throws BankingException {
        computeInterest(interestDate);
        return(accountBalance);
    }
    //return balance但不計算利息
    public double balance() {
        return(accountBalance);
    }

    //存錢(存入amount元)並計算利息。若沒設定日期，預設為現在時間
    public abstract double deposit(double amount, Date depositDate) throws BankingException;    
    
    //領錢(領出amount元)並計算利息。若沒設定日期，預設為現在時間
    public abstract double withdraw(double amount, Date withdrawDate) throws BankingException;
    
    //計算利息。openDate~interestDate的利息
    public abstract double computeInterest(Date interestDate) throws BankingException;
}
   