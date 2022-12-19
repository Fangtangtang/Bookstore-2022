//
// Created by 房诗涵 on 2022/12/19.
//

#ifndef BOOKSTORE_LOGGINGSTATUS_H
#define BOOKSTORE_LOGGINGSTATUS_H

//用vector方便修改信息
#include <vector>
#include "account.h"
#include "tokenScanner.h"

struct CurrentAccount{
    ID userID;
    Privilege privilege=none;
};

class LoggingStatus{
public:
    void Su()

    //登出
    void Logout();

    //返回当前账户信息
    CurrentAccount Flush();
private:
    //登录用户ID栈
    std::vector<ID> IDStack;

    //登录用户权限栈
    std::vector<Privilege> privilegeStack;

    //登录
    void Login(Account);
};
#endif //BOOKSTORE_LOGGINGSTATUS_H
