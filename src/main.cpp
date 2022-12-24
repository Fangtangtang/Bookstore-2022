#include <iostream>
#include <string>
#include <iomanip>
#include "account.h"
#include "book.h"
#include "log.h"
#include "loggingStatus.h"
#include "error.h"
#include "tokenScanner.h"


//程序首次运行时自主执行所需的初始化操作
bool Initialize();

std::string ProcessLine(const std::string &input,
                        AccountManager &accountManager,
                        BookManager &bookManager,
                        LogManager &logManager,
                        TransactionManager &transactionManager,
                        LoggingStatus &loggingStatus,
                        CurrentAccount &user);

int main() {
//    freopen("my.out", "w", stdout);

    bool initialize_flag = Initialize();
    //接入系统的对象
    AccountManager accountManager;
    BookManager bookManager;
    LogManager logManager;
    TransactionManager transactionManager;
    //***************************
//    accountManager.InitialAccount();
    //***************************
    if (initialize_flag) {
        accountManager.InitialAccount();
//        transactionManager.InitialTransation();
//        logManager.InitialLog();
    }

    //登录栈
    LoggingStatus loggingStatus;
    //当前用户
    CurrentAccount user;
    ID userID;
    //浮点数输出精度设置
    std::cout << std::fixed << std::setprecision(2);
    while (true) {
        std::string operation;
        userID = user.userID;
        try {
            std::string input;
            getline(std::cin, input);
            //EOF终止
            if (std::cin.eof()) return 0;
            //合法操作
            operation = ProcessLine(input, accountManager, bookManager, logManager, transactionManager, loggingStatus,
                                    user);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
        if(operation=="RETURN") return 0;
        if(operation!="BLANK"){
            //向日志添加操作信息
            if (operation.empty()) operation = "FAILED";
            logManager.AddLog(userID, operation);
        }
    }
}

//以”account_information“是否存在为判断依据
bool Initialize() {
    std::fstream test;
    test.open("account_information");
//    std::cout<<"TEST:"<<test.good()<<'\n';
    //文件不存在 初始化
    if (!test.good()) {
        test.close();
        return true;
    } else {
        test.close();
        return false;
    }
}

int c =17, a = 0;

//返回操作语句
std::string ProcessLine(const std::string &input,
                        AccountManager &accountManager,
                        BookManager &bookManager,
                        LogManager &logManager,
                        TransactionManager &transactionManager,
                        LoggingStatus &loggingStatus,
                        CurrentAccount &user) {
    TokenScanner tokenScanner(input);
    tokenScanner.Initialize();
    if(!tokenScanner.HasMoreTokens())return "BLANK";
    //读入命令
    std::string cmd;
    bool success = false;
    tokenScanner.NextToken(cmd);

    --c;
    ++a;
//    if(c==0){
//        std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
//    }
//    std::cout << a << " " << cmd << ": ";

    if(cmd=="clear") {
        loggingStatus.Clear();
    }
    if (cmd == "quit" || cmd == "exit") {
//
//        std::cout<<"\n@@@@@@@@@@@@\n";
//        bookManager.PRINT();
//        std::cout<<"@@@@@@@@@@@@\n\n";

        if (tokenScanner.HasMoreTokens()) error("Invalid");
//        exit(0);
        return "RETURN";
    }
    //登录账户
    if (cmd == "su") {
        loggingStatus.Su(tokenScanner, user, accountManager);
        success = true;
    }
    //登出
    if (cmd == "logout") {
        if (user.privilege == none)error("Invalid");
        loggingStatus.Logout(tokenScanner, user);
        success = true;
    }
    //注册账户
    if (cmd == "register") {
        accountManager.Register(tokenScanner);
        success = true;
    }
    //修改密码
    if (cmd == "passwd") {
        if (user.privilege == none)error("Invalid");
        accountManager.ChangePassword(tokenScanner, user);
        success = true;
    }
    //创建账户
    if (cmd == "useradd") {
        accountManager.AddUser(tokenScanner, user.privilege);
        success = true;
    }
    //删除账户
    if (cmd == "delete") {
        if (user.privilege != host) error("Invalid");
        char userID[31];
        if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(userID);
        else error("Invalid");
        if (tokenScanner.HasMoreTokens()) error("Invalid");
        ID id(userID);
        if (loggingStatus.Find(id))error("Invalid");
        accountManager.DeleteUser(id);
        success = true;
    }
    //购买图书
    if (cmd == "buy") {
        if (user.privilege == none)error("Invalid");
        double price = bookManager.Buy(tokenScanner);
        transactionManager.Income(price);
        success = true;
    }
    if (cmd == "select") {
        if (user.privilege < clerk) error("Invalid");
        //找书或新建
        ISBN isbn = bookManager.Select(tokenScanner);
        //修改用户选书信息
        loggingStatus.SelectBook(isbn);
        success = true;
    }
    if (cmd == "modify") {
        //用户权限
        if (user.privilege < clerk) error("Invalid");
        //选中图书
        ISBN isbn = loggingStatus.FindSelected(),newISBN;
        long iter = 0;
        Book book;
        bookManager.GetBook(book,isbn, iter);
        bool change_ISBN_flag = bookManager.Modify(tokenScanner, book, iter, newISBN);
        if (change_ISBN_flag){
//            loggingStatus.SelectBook(isbn);
            loggingStatus.ChangeISBN(isbn,newISBN);
        }
        success = true;
    }
    if (cmd == "import") {
        if (user.privilege < clerk) error("Invalid");
        ISBN isbn = loggingStatus.FindSelected();
        long iter = 0;
        Book book;
        bookManager.GetBook(book,isbn, iter);
        double cast = bookManager.Import(tokenScanner, isbn);
        transactionManager.Expense(cast);
        success = true;
    }
    if (cmd == "show") {
        if (user.privilege == none)error("Invalid");
        std::string str = tokenScanner.ShowRest();
        if (!tokenScanner.HasMoreTokens() || str[0] == '-') {
            bookManager.Show(tokenScanner);
        } else {
            tokenScanner.NextToken(cmd);
            if (cmd != "finance") error("Invalid");
            if (user.privilege < host) error("Invalid");
            transactionManager.ShowFinance(tokenScanner);
        }
        success = true;
    }
    if (cmd == "log") {
        if (user.privilege < host) error("Invalid");
        if (tokenScanner.HasMoreTokens()) error("Invalid");
        logManager.PrintLog();
        success = true;
    }
//    bookManager.PRINT();
    if (!success) error("Invalid");
    return tokenScanner.ShowOperation();
}