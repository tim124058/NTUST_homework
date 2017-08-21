/*
 *  Derived class: CheckingAccount
 *
 *  Description:
 *      Interest is computed daily; there's no fee for
 *      withdraw; there is a minimum balance of $1000.
 */
package account;
import java.util.*;
                          
public class CheckingAccount extends Account{    
    public CheckingAccount(String s, double firstDeposit, Date firstDate) {
        super(s,firstDeposit,firstDate);
    }	

    public double deposit(double amount, Date depositDate) throws BankingException {
        computeInterest(depositDate);
        if ( amount <= 0 ) {
            throw new BankingException ("交易金額必須大於零,checking account name:" + accountName);
        }else {
            accountBalance += amount;   
            return(accountBalance);     
        }                                           
    }
    
    public double withdraw(double amount, Date withdrawDate) throws BankingException {
        computeInterest(withdrawDate);
        if ( amount <= 0 ) {
            throw new BankingException ("交易金額必須大於零,checking account name:" + accountName);
        } else if ((accountBalance  - amount) < 1000) {    // minimum balance is 1000, raise exception if violated
            throw new BankingException ("錢不夠領，最小餘額為$1000,checking account name:" + accountName);
        } else {
            accountBalance -= amount;	
            return(accountBalance); 	
        }                                        	
    }
    
    public double computeInterest (Date interestDate) throws BankingException {
        interestDate = ((interestDate != null)?interestDate:new Date());
        if (interestDate.before(lastInterestDate)) {
            throw new BankingException ("Invalid date to compute interest for account name" + accountName);                            	
        }
        
        int numberOfDays = (int) ((interestDate.getTime() 
                                   - lastInterestDate.getTime())
                                   / 86400000.0);
        System.out.println("Number of days since last interest is " + numberOfDays);
        double interestEarned = (double) numberOfDays / 365.0 *
                                      accountInterestRate * accountBalance;
        System.out.println("Interest earned is " + interestEarned); 
        lastInterestDate = interestDate;
        accountBalance += interestEarned;
        return(accountBalance);                            
    }  	
}        