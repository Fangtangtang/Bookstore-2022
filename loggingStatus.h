//
// Created by 房诗涵 on 2022/12/19.
//

#ifndef BOOKSTORE_LOGGINGSTATUS_H
#define BOOKSTORE_LOGGINGSTATUS_H

//用vector方便修改信息
#include <vector>
#include <algorithm>
#include "account.h"
#include "book.h"
#include "tokenScanner.h"
#include "error.h"

class LoggingStatus{
public:
    void Su(TokenScanner &tokenScanner,CurrentAccount &user,AccountManager &accountManager);

    //登出
    void Logout(TokenScanner &tokenScanner,CurrentAccount &user);

    //记录用户和选中的图书
    void SelectBook(std::pair<ISBN,long> pair);

    //寻找当前选中的书
    std::pair<ISBN,long>  FindSelected();

    bool Find(const ID& id);
private:
    //登录用户ID栈
    std::vector<ID> IDVector;

    //登录用户权限栈
    std::vector<Privilege> privilegeVector;

    //ID->selectedBook
    std::vector<std::pair<ISBN,long>> bookVector;

    //返回当前账户信息
    CurrentAccount Flush();
};
#endif //BOOKSTORE_LOGGINGSTATUS_H
