/*
 *  Derived class: SavingAccount
 *
 *  Description:
 *      monthly interest; fee of $1 for every transaction, 
 *      except the first three per month are free; no minimum balance.
 */
package account;
import java.util.*;
import java.text.SimpleDateFormat;
                          
public class SavingAccount extends Account{    
    private int monthTransactions;          //一個月做了幾次交易
    private boolean sameMonth;              //這次跟lastInterestDate是否在同一個月內
    public SavingAccount(String s, double firstDeposit, Date firstDate) {
        super(s,firstDeposit,firstDate);
        sameMonth = false;
        monthTransactions = 0;
    }	

    public double deposit(double amount, Date depositDate) throws BankingException {
        computeInterest(depositDate);
        if ( amount <= 0 ) {
            throw new BankingException ("交易金額必須大於零,saving account name:" + accountName);
        }else {
            if (sameMonth) monthTransactions++;         //跟上次交易是同一個月
            if (monthTransactions > 3) amount -= 1;     //這個月已經超過3次
            if (amount <= 0){
                monthTransactions--;                    //交易失敗
                throw new BankingException ("扣掉手續費後，交易金額小於等於零,saving account name:" + accountName);
            }
            accountBalance += amount;   
            return(accountBalance);     
        }                                           
    }
    
    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        computeInterest(withdrawDate);
        if ( amount <= 0 ) {
            throw new BankingException ("交易金額必須大於零,saving account name:" + accountName);
        } else if ((accountBalance  - amount) < 0) {    // minimum balance is 0, raise exception if violated
            throw new BankingException ("錢不夠領,saving account name:" + accountName);
        } else {
            if (sameMonth) monthTransactions++;         //跟上次交易是同一個月
            if (monthTransactions > 3) amount += 1;     //這個月已經超過3次
            if ((accountBalance  - amount) < 0) {       // minimum balance is 0, raise exception if violated
                monthTransactions--;  
                throw new BankingException ("加上手續費後，錢不夠領,saving account name:" + accountName);
            }
            accountBalance -= amount;	
            return(accountBalance); 	
        }                                        	
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