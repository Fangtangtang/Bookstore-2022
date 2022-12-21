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
    bool initialize_flag = Initialize();
    //接入系统的对象
    AccountManager accountManager;
    BookManager bookManager;
    LogManager logManager;
    TransactionManager transactionManager;
    if (initialize_flag) {
        accountManager.InitialAccount();
//        transactionManager.InitialTransation();
//        logManager.InitialLog();
    }
    //登录栈
    LoggingStatus loggingStatus;
    //当前用户
    CurrentAccount user;
    //浮点数输出精度设置
    std::cout << std::fixed << std::setprecision(2);
    while (true) {
        std::string operation;
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
        //向日志添加操作信息
        if (operation.empty()) operation = "FAILED";
        logManager.AddLog(user.userID, operation);
    }
}

//以”account_information“是否存在为判断依据
bool Initialize() {
    std::fstream test;
    test.open("account_information");
    //文件不存在 初始化
    if (!test.good()) {
        test.close();
        return true;
    } else {
        test.close();
        return false;
    }
}

//返回操作语句
std::string ProcessLine(const std::string &input,
                        AccountManager &accountManager,
                        BookManager &bookManager,
                        LogManager &logManager,
                        TransactionManager &transactionManager,
                        LoggingStatus &loggingStatus,
                        CurrentAccount &user) {
    TokenScanner tokenScanner(input);
    //读入命令
    std::string cmd;
    bool success = false;
    tokenScanner.NextToken(cmd);
    if (cmd == " ") return "";
    if (cmd == "quit" || cmd == "exit") {
        if (tokenScanner.HasMoreTokens()) error("Invalid");
        exit(0);
    }
    //登录账户
    if (cmd == "su") {
        loggingStatus.Su(tokenScanner, user, accountManager);
        success = true;
    }
    //登出
    if (cmd == "logout") {
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
        char *userID;
        if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(userID);
        else error("Invalid");
        if (tokenScanner.HasMoreTokens()) error("Invalid");
        ID id(userID);
        if (loggingStatus.Find(id))error("Invalid");
        accountManager.DeleteUser(id);
        success = true;
    }
    //购买图书
    if (cmd == "buy") {
        double price = bookManager.Buy(tokenScanner);
        transactionManager.Income(price);
        success = true;
    }
    if (cmd == "select") {
        if (user.privilege <= clerk) error("Invalid");
        //找书或新建
        std::pair<ISBN, long> pair = bookManager.Select(tokenScanner);
        //修改用户选书信息
        loggingStatus.SelectBook(pair);
        success = true;
    }
    if (cmd == "modify") {
        //用户权限
        if (user.privilege <= clerk) error("Invalid");
        //选中图书
        std::pair<ISBN, long> pair = loggingStatus.FindSelected();
        bookManager.Modify(tokenScanner, pair);
        success = true;
    }
    if (cmd == "import") {
        if (user.privilege <= clerk) error("Invalid");
        std::pair<ISBN, long> pair = loggingStatus.FindSelected();
        double cast = bookManager.Import(tokenScanner, pair);
        transactionManager.Expense(cast);
        success = true;
    }
    if (cmd == "show") {
        std::string str = tokenScanner.ShowRest();
        if (!tokenScanner.HasMoreTokens() || str[0] == '-') {
            bookManager.Show(tokenScanner);
        } else {
            tokenScanner.NextToken(cmd);
            if (cmd != "finance") error("Invalid");
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
    if (!success) error("Invalid");
    return tokenScanner.ShowOperation();
}