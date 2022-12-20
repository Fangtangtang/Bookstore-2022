//
// Created by 房诗涵 on 2022/12/19.
//
#include "loggingStatus.h"

void LoggingStatus::Su(TokenScanner &tokenScanner, CurrentAccount &user, AccountManager &accountManager) {
    char *userID, *password;
    tokenScanner.NextToken(userID);
    //输入ID对应的account
    Account suAccount;
    std::pair<Account, bool> pair = accountManager.FindAccount(userID);
    if (!pair.second) error("Invalid");
    suAccount = pair.first;
    //输入了密码
    if (tokenScanner.HasMoreTokens()) {
        tokenScanner.NextToken(password);
        //密码错误
        if (password != suAccount.password) error("Invalid");
        if (tokenScanner.HasMoreTokens()) error("Invalid");
        //登录成功 加入登录栈
        IDVector.push_back(suAccount.userID);
        privilegeVector.push_back(suAccount.privilege);
        //更新当前user
        user.privilege = suAccount.privilege;
        user.userID = suAccount.userID;
        return;
    }
    //未输入密码
    if (user.privilege > suAccount.privilege) {
        //登录成功 加入登录栈
        IDVector.push_back(suAccount.userID);
        privilegeVector.push_back(suAccount.privilege);
        //更新当前user
        user.privilege = suAccount.privilege;
        user.userID = suAccount.userID;
        return;
    }
    error("Invalid");
}

void LoggingStatus::Logout(TokenScanner &tokenScanner, CurrentAccount &user) {
    if (tokenScanner.HasMoreTokens()) error("Invalid");
    //当前无登录用户
    if (IDVector.empty()) error("Invalid");
    //修改登录栈
    IDVector.pop_back();
    privilegeVector.pop_back();
    //更新当前user
    user = Flush();
}

void LoggingStatus::SelectBook(ISBN isbn, const CurrentAccount& user) {
     auto iter=selectBook.find(user.userID);
     //用户有选过书 删掉
     if(iter!=selectBook.end()) selectBook.erase(iter);
     //用户当前选择
     selectBook[user.userID]=isbn;
}

ISBN LoggingStatus::FindSelected(const ID& id) {
    auto iter=selectBook.find(id);
    //有选中
    if(iter!=selectBook.end()) return iter->second;
    else error("Invalid");
}


CurrentAccount LoggingStatus::Flush() {
    CurrentAccount tmp;
    if (!IDVector.empty()) {
        tmp.privilege = privilegeVector.back();
        tmp.userID = IDVector.back();
    } else {
        tmp.privilege = none;
    }
    return tmp;
}

bool LoggingStatus::Find(const ID& id) {
    return (std::find(IDVector.begin(), IDVector.end(), id) != IDVector.end());
}



