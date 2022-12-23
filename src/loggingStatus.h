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
    void Clear(){
        IDVector.clear();
        privilegeVector.clear();
        bookVector.clear();
    }

    void Su(TokenScanner &tokenScanner,CurrentAccount &user,AccountManager &accountManager);

    //登出
    void Logout(TokenScanner &tokenScanner,CurrentAccount &user);

    //记录用户和选中的图书
    void SelectBook(ISBN);

    //寻找当前选中的书
    ISBN FindSelected();

    //修改了图书中的ISBN
    void ChangeISBN(ISBN isbn,ISBN newISBN);

    bool Find(const ID& id);
private:
    //登录用户ID栈
    std::vector<ID> IDVector;

    //登录用户权限栈
    std::vector<Privilege> privilegeVector;

    //ID->selectedBook
    std::vector<ISBN> bookVector;

    //返回当前账户信息
    CurrentAccount Flush();
};
#endif //BOOKSTORE_LOGGINGSTATUS_H
