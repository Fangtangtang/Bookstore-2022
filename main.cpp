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

//添加初始用户
void AddInitialAccount(AccountManager &accountManager);

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
    if (initialize_flag) AddInitialAccount(accountManager);
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

void AddInitialAccount(AccountManager &accountManager) {
    char root[5] = "root", name[5] = "sjtu";
    accountManager.AddUser(root, name, host, root);
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
    tokenScanner.nextToken(cmd);
    if (cmd == " ") return;
    if (cmd == "quit" || cmd == "exit") {
        if (tokenScanner.hasMoreTokens()) error("Invalid");
        exit(0);
    }
    //登录账户
    if (cmd == "su") {
//        loggingStatus.Su
        char *userID, *password;
        tokenScanner.nextToken(userID);
        //输入ID对应的account
        Account suAccount;
        std::pair<Account, bool> pair = accountManager.FindAccount(userID);
        if (!pair.second) error("Invalid");
        suAccount = pair.first;
        //输入了密码
        if (tokenScanner.hasMoreTokens()) {
            tokenScanner.nextToken(password);
            //密码错误
            if (password != suAccount.password) error("Invalid");
            if (tokenScanner.hasMoreTokens()) error("Invalid");
            //登录成功 加入登录栈
            //更新当前user
            loggingStatus.Login(suAccount);
            user.privilege = suAccount.privilege;
            user.userID = suAccount.userID;
            return;
        }
        //未输入密码
        if (user.privilege > suAccount.privilege) {
            //登录成功 加入登录栈
            //更新当前user
            loggingStatus.Login(suAccount);
            user.privilege = suAccount.privilege;
            user.userID = suAccount.userID;
            return;
        }
        error("Invalid");
    }
    //登出
    if (cmd == "logout") {
        if (tokenScanner.hasMoreTokens()) error("Invalid");
        //当前无登录用户
        if (user.privilege == none) error("Invalid");
        loggingStatus.Logout();
        //更新当前user
        user = loggingStatus.Flush();
        return;
    }
    //注册账户
    if(cmd=="register"){
        char *userID,*password,*name;
        if(tokenScanner.hasMoreTokens()) tokenScanner.nextToken(userID);
        else error("Invalid");
        if(tokenScanner.hasMoreTokens()) tokenScanner.nextToken(password);
        else error("Invalid");
        if(tokenScanner.hasMoreTokens()) tokenScanner.nextToken(name);
        else error("Invalid");
        if(tokenScanner.hasMoreTokens()) error("Invalid");
    }
}
