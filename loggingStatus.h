//
// Created by 房诗涵 on 2022/12/19.
//

#ifndef BOOKSTORE_LOGGINGSTATUS_H
#define BOOKSTORE_LOGGINGSTATUS_H

//用vector方便修改信息
#include <vector>
#include <algorithm>
#include <map>
#include "account.h"
#include "book.h"
#include "tokenScanner.h"
#include "error.h"

class LoggingStatus{
public:
    void Su(TokenScanner &tokenScanner,CurrentAccount &user,AccountManager &accountManager);

    //登出
    void Logout(TokenScanner &tokenScanner,CurrentAccount &user);

    //记录用户和选中的图书(删插覆盖）
    void SelectBook(ISBN isbn,const CurrentAccount& user);

    //寻找ID对应的书
    ISBN FindSelected(const ID& id);

    bool Find(const ID& id);
private:
    //登录用户ID栈
    std::vector<ID> IDVector;

    //登录用户权限栈
    std::vector<Privilege> privilegeVector;

    //ID->selectedBook
    std::map<ID,ISBN> selectBook;

    //返回当前账户信息
    CurrentAccount Flush();
};
#endif //BOOKSTORE_LOGGINGSTATUS_H
