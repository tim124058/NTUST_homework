/*
 *  Derived class: CDAccount
 *
 *  Description:
 *      monthly interest; fixed amount and duration (e.g., you can open
 *      1 12-month CD for $5000; for the next 12 months you can't deposit
 *      anything and withdrawals cost a  $250 fee); at the end of the
 *      duration the interest payments stop and you can withdraw no fee.
 */
package account;
import java.util.*;
import java.text.SimpleDateFormat;
                          
public class CDAccount extends Account{  
    private int remainInterest;         //剩餘的利息次數
    public CDAccount(String s, double firstDeposit, Date firstDate) {
        super(s,firstDeposit,firstDate); 
        remainInterest = 12;
    }   

    public double deposit(double amount, Date depositDate) throws BankingException {
        throw new BankingException ("不能存款");                                               
    }
    
    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        computeInterest(withdrawDate);
        if ( amount <= 0 ) {
            throw new BankingException ("交易金額必須大於零");
        } else if ((accountBalance  - amount) < 0) {    // minimum balance is 0, raise exception if violated
            throw new BankingException ("錢不夠領,CD account name:" + accountName);
        } else {
            if(remainInterest > 0) amount += 250;
            if ((accountBalance  - amount) < 0) 
                throw new BankingException ("加上手續費後，錢不夠領,CD account name:" + accountName);
            
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
        System.out.println("Number of Months since last interest is " + numberOfMonths);
        if(numberOfMonths > remainInterest){
            numberOfMonths = remainInterest;
            remainInterest = 0;
        }else{
            remainInterest -= numberOfMonths;
        }

        System.out.println("有效利息月數為：" + numberOfMonths);
        double interestEarned = (double) numberOfMonths / 12.0 *
                                      accountInterestRate * accountBalance;
        System.out.println("Interest earned is " + interestEarned); 
        lastInterestDate = interestDate;
        accountBalance += interestEarned;
        return(accountBalance);                            
    }
}        
