import account.*;
import java.util.*;
import java.text.SimpleDateFormat;
import java.text.ParseException;

public class Application {
    static void TEST_CheckingAccount(Account a){
        System.out.println("---------------- TEST_CheckingAccount ----------------");
        System.out.println("## 開設帳戶時的金額 : " + a.balance() + " ##");
        System.out.println("# 領出-100元");
        withdrawFromAccount(a,-100,parseDate("2016/10/10"));
        System.out.println("# 存入1000元");
        depositIntoAccount(a,1000,parseDate("2016/10/10"));
        System.out.println("# 領出2000元");
        withdrawFromAccount(a,2000,parseDate("2016/10/10"));
        System.out.println("# 在1年後領出1000");
        withdrawFromAccount(a,1000,parseDate("2017/10/10"));
        System.out.println("---------------- END ----------------\n\n");
    }
    static void TEST_SavingAccount(Account a){
        System.out.println("---------------- TEST_CheckingAccount ----------------");
        System.out.println("## 開設帳戶時的金額 : " + a.balance() + " ##");
        System.out.println("# 領出-100元");
        withdrawFromAccount(a,-100,parseDate("2016/10/10"));
        System.out.println("# 領出1300元");
        withdrawFromAccount(a,1300,parseDate("2016/10/10"));
        System.out.println("# 存入100元");
        depositIntoAccount(a,100,parseDate("2016/10/10"));
        System.out.println("# 存入100元");
        depositIntoAccount(a,100,parseDate("2016/10/10"));
        System.out.println("# 領出100元");
        withdrawFromAccount(a,100,parseDate("2016/10/10"));
        System.out.println("# 領出100元，會扣$1手續費");
        withdrawFromAccount(a,100,parseDate("2016/10/10"));
        System.out.println("# 在3個月後領出100");
        withdrawFromAccount(a,100,parseDate("2017/1/15"));
        System.out.println("---------------- END ----------------\n\n");
    }
    static void TEST_CDAccount(Account a){
        System.out.println("---------------- TEST_CheckingAccount ----------------");
        System.out.println("## 開設帳戶時的金額 : " + a.balance() + " ##");
        System.out.println("# 存入100元");
        depositIntoAccount(a,100,parseDate("2016/10/10"));
        System.out.println("# 開帳5個月後領出200元，因為在12個月內，所以還有$250手續費");
        withdrawFromAccount(a,200,parseDate("2017/3/18"));
        System.out.println("# 開帳5個月後領出500元，因為在12個月內，所以還有$250手續費，但因為手續費而導致餘額不足");
        withdrawFromAccount(a,500,parseDate("2017/3/18"));
        System.out.println("# 開帳2年後領出300元");
        withdrawFromAccount(a,300,parseDate("2018/10/10"));
        System.out.println("---------------- END ----------------\n\n");
    }
    static void TEST_LoanAccount(Account a){
        System.out.println("---------------- TEST_CheckingAccount ----------------");
        System.out.println("## 開設帳戶時的金額 : " + a.balance() + " ##");
        System.out.println("# 領出100元");
        withdrawFromAccount(a,100,parseDate("2016/10/10"));
        System.out.println("# 存入500元");
        depositIntoAccount(a,500,parseDate("2016/10/10"));
        System.out.println("# 存入-100元");
        depositIntoAccount(a,-100,parseDate("2016/10/10"));
        System.out.println("# 5個月後存入300元");
        depositIntoAccount(a,300,parseDate("2017/3/10"));
        System.out.println("---------------- END ----------------\n\n");
    }

    public static void main( String args []) {
		Account[] accountList = new Account[4];
		
		// buid 4 different accounts in the same array
		accountList[0] = new CheckingAccount("John Smith", 1500.0,parseDate("2016/10/10"));
		accountList[1] = new SavingAccount("William Hurt", 1200.0,parseDate("2016/10/10"));
		accountList[2] = new CDAccount("Woody Allison", 1000.0,parseDate("2016/10/10"));
		accountList[3] = new LoanAccount("Judi Foster", -1500.0,parseDate("2016/10/10"));

        //測試4種Account
        TEST_CheckingAccount(accountList[0]);
        TEST_SavingAccount(accountList[1]);
        TEST_CDAccount(accountList[2]);
        TEST_LoanAccount(accountList[3]);
    }

    //print error message
    static void stdExceptionPrinting(Exception e, double balance) {
        System.out.println("EXCEPTION: Banking system throws a " + e.getClass() +
                           " with message: \n\t" +
                           "MESSAGE: " + e.getMessage());
        System.out.println("\tAccount balance remains $" + balance + "\n");
    }

    //依據dateString設定Date，並回傳Date物件
    static Date parseDate(String dateString){
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy/MM/dd");
        try{
            return formatter.parse(dateString);
        }catch(ParseException e){
            System.out.println("Date parse error");
            return null;
        }
    }

    //存錢進Account a
    static void depositIntoAccount(Account a, double amount, Date d){
        try {
            double ret = a.deposit(amount, d);
            System.out.println ("Account <" + a.name() + "> now has $" + ret + " balance\n");
        } catch (Exception e) {
            stdExceptionPrinting(e, a.balance());   
        }
    }

    //從Account a中領錢
    static void withdrawFromAccount(Account a, double amount, Date d){
        try {
            double ret = a.withdraw(amount, d);
            System.out.println ("Account <" + a.name() + "> now has $" + ret + " balance\n");
        } catch (Exception e) {
            stdExceptionPrinting(e, a.balance());   
        }
    }

    //計算Account a中的利息
    static void computeAccountInterest(Account a, Date d){
        try {
            double ret = a.computeInterest(d);
            System.out.println ("Account <" + a.name() + "> now has $" + ret + " balance\n");
        } catch (Exception e) {
            stdExceptionPrinting(e, a.balance());   
        }
    }

    //計算Account a中的利息，並回傳balance
    static double AccountBalance(Account a, Date d){
        try {
            double ret = a.balance(d);
            System.out.println ("Account <" + a.name() + "> now has $" + ret + " balance\n");
            return ret;
        } catch (Exception e) {
            stdExceptionPrinting(e, a.balance());   
            return -1;      //error
        }
    }
}          