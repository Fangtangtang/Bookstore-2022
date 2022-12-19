//
// Created by 房诗涵 on 2022/12/19.
//

#ifndef BOOKSTORE_LOGGINGSTATUS_H
#define BOOKSTORE_LOGGINGSTATUS_H

//用vector方便修改信息
#include <vector>
#include <algorithm>
#include "account.h"
#include "tokenScanner.h"
#include "error.h"

class LoggingStatus{
public:
    void Su(TokenScanner &tokenScanner,CurrentAccount &user,AccountManager &accountManager);

    //登出
    void Logout(TokenScanner &tokenScanner,CurrentAccount &user);

    bool Find(ID id);
private:
    //登录用户ID栈
    std::vector<ID> IDVector;

    //登录用户权限栈
    std::vector<Privilege> privilegeVector;

    //返回当前账户信息
    CurrentAccount Flush();
};
#endif //BOOKSTORE_LOGGINGSTATUS_H
