#include <iostream>
#include "account.h"
#include "book.h"
#include "log.h"
#include "loggingStatus.h"
#include "error.h"
#include "tokenScanner.h"
#include <string>

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
    bool success= false;
    tokenScanner.nextToken(cmd);
    if (cmd == " ") return;
    if (cmd == "quit" || cmd == "exit") {
        if (tokenScanner.hasMoreTokens()) error("Invalid");
        exit(0);
    }
    //登录账户
    if (cmd == "su") {
        loggingStatus.Su(tokenScanner, user, accountManager);
        success= true;
    }
    //登出
    if (cmd == "logout") {
       loggingStatus.Logout(tokenScanner,user);
       success= true;
    }
    //注册账户
    if(cmd=="register"){
        accountManager.Register(tokenScanner);
        success= true;
    }
    if(cmd=="passwd"){
        accountManager.ChangePassword(tokenScanner,user);
        success= true;
    }
    if(cmd=="useradd"){
        accountManager.AddUser(tokenScanner,user.privilege);
        success= true;
    }
    if(cmd=="delete"){
        if(user.privilege!=host) error("Invalid");
        accountManager.DeleteUser(tokenScanner);
        success=true;
    }
}