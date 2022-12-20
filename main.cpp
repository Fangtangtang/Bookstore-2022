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

void ProcessLine(std::string input,
                 AccountManager &accountManager,
                 BookManager &bookManager,
                 LogManager &logManager,
                 LoggingStatus &loggingStatus,
                 CurrentAccount user);

int main() {
    bool initialize_flag = Initialize();
    //接入系统的对象
    AccountManager accountManager;
    if (initialize_flag) accountManager.InitialAccount();
    BookManager bookManager;
    LogManager logManager;
    //登录栈
    LoggingStatus loggingStatus;
    //当前用户
    CurrentAccount user;
    //浮点数输出精度设置
    std::cout<<std::fixed<<std::setprecision(2);
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            //EOF终止
            if (std::cin.eof()) return 0;
            //todo
            //verify the function
            ProcessLine(input, accountManager, bookManager, logManager, loggingStatus, user);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
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

void ProcessLine(std::string input,
                 AccountManager &accountManager,
                 BookManager &bookManager,
                 LogManager &logManager,
                 LoggingStatus &loggingStatus,
                 CurrentAccount user) {
    TokenScanner tokenScanner(input);
    //读入命令
    std::string cmd;
    bool success = false;
    tokenScanner.NextToken(cmd);
    if (cmd == " ") return;
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
    if(cmd=="buy"){
        bookManager.Buy(tokenScanner);
        success= true;
    }
    if(cmd=="select"){
        bookManager.Select(tokenScanner);
        success= true;
    }
    if(cmd=="modify"){

    }
}