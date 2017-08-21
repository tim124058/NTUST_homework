/*
 *  Derived class: LoanAccount
 *
 *  Description:
 *      like a saving account, but the balance is "negative" (you owe
 *      the bank money, so a deposit will reduce the amount of the loan);
 *      you can't withdraw (i.e., loan more money) but of course you can 
 *      deposit (i.e., pay off part of the loan).
 */
package account;
import java.util.*;
import java.text.SimpleDateFormat;
                          
public class LoanAccount extends Account{    
    private int monthTransactions;          //一個月做了幾次交易
    private boolean sameMonth;              //這次跟lastInterestDate是否在同一個月內
    public LoanAccount(String s, double loanAmount, Date firstDate) {
        super(s,loanAmount,firstDate);  
        sameMonth = false;
        monthTransactions = 0;
    }  

    public double deposit(double amount, Date depositDate) throws BankingException {
        computeInterest(depositDate);
        if ( amount <= 0 ) {
            throw new BankingException ("交易金額必須大於零,loan account name:" + accountName);
        }else {
            if (sameMonth) monthTransactions++;         //跟上次交易是同一個月
            if (monthTransactions > 3) amount -= 1;     //這個月已經超過3次
            if (amount <= 0){
                monthTransactions--;                    //交易失敗
                throw new BankingException ("扣掉手續費後，交易金額小於等於零,loan account name:" + accountName);
            }
            accountBalance += amount;   
            return(accountBalance);     
        }                                          
    }
    
    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        throw new BankingException ("不能提款");                                       
    }
    
    public double computeInterest (Date interestDate) throws BankingException {
        interestDate = ((interestDate != null)?interestDate:new Date());
        if (interestDate.before(lastInterestDate)) {
            throw new BankingException ("Invalid date to compute interest for account name" + accountName);                             
        }
        
        SimpleDateFormat dateFormat = new SimpleDateFormat("MM");
        int numberOfMonths = (Integer.parseInt(dateFormat.format(interestDate)) - Integer.parseInt(dateFormat.format(lastInterestDate)));
        dateFormat = new SimpleDateFormat("yyyy");
        numberOfMonths += (Integer.parseInt(dateFormat.format(interestDate)) - Integer.parseInt(dateFormat.format(lastInterestDate)))*12;
        
        if (numberOfMonths == 0) {
            sameMonth = true;    
        }
        else{ 
            sameMonth = false;
            monthTransactions = 0;
        }
        
        System.out.println("Number of Months since last interest is " + numberOfMonths);
        double interestEarned = (double) numberOfMonths / 12.0 *
                                      accountInterestRate * accountBalance;
        System.out.println("Interest earned is " + interestEarned); 
        lastInterestDate = interestDate;
        accountBalance += interestEarned;
        return(accountBalance);                            
    }    
}        