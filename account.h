//
// Created by 房诗涵 on 2022/12/10.
//
//AccountManager: 账户信息管理

//函数 GetKey
#ifndef BOOKSTORE_ACCOUNT_H
#define BOOKSTORE_ACCOUNT_H

#include "linkList.h"
#include "tokenScanner.h"
#include "error.h"

#include <string>

//权限 符号常量 对应 {无} {0} {1} {3} {7}
enum Privilege{none,visitor,customer,clerk,host};

//key:ID
struct ID {
    char userID[31]={'\0'};

    ID();

    ID(char* id);

    ~ID();

    bool operator>(const ID &id) const;

    bool operator==(const ID &id) const;

    bool operator>=(const ID &id) const;

    ID &operator=(const ID &id);

    ID GetIndex();

};

struct CurrentAccount{
    ID userID;
    Privilege privilege=none;
};

struct Account {

    Privilege privilege=none;

    ID userID;

    char name[31]={'\0'};

    char password[31]={'\0'};

    bool operator>(const Account &id) const;

    bool operator==(const Account &id) const;

    bool operator>=(const Account &id) const;

    Account &operator=(const std::pair<Account, bool>& pair);

    //获取key
    ID GetKey(ID id) const ;

//    void Print();

};

class AccountManager{
public:
    AccountManager()=default;

    ~AccountManager()=default;

    //注册账户 {0} register [UserID] [Password] [Username]
    //游客注册 权限为{1} ID不可和已有的重复
    //ID== error
    void Register(TokenScanner &tokenScanner);

    //创建初始用户
    void InitialAccount();

    //创建帐户 {3} useradd [UserID] [Password] [Privilege] [Username]
    //高权限用户创建低权限账号
    void AddUser(TokenScanner &tokenScanner,Privilege &privilege);

    //删除账户 {7} delete [UserID]
    //仅{7}
    void DeleteUser(ID id);

    //修改密码 {1} passwd [UserID] ([CurrentPassword])? [NewPassword]
    //如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]（登录栈中用？）
    void ChangePassword(TokenScanner &tokenScanner,CurrentAccount &user);

    //查找帐户
    std::pair<Account,bool> FindAccount(char* UserID);

private:
    //储存账户信息的块状链表
    //类中用（）会和声明函数歧义
    LinkList<ID,ID,Account> accountList{"account_information"};

    //修改密码
    void Modify(Account account,long iter,char* newPassword);

    //int转privilege
    static Privilege toPrivilege(int i);

    //todo
    //判断信息是否合法（长度等）
    //判断ID是否已经存在
    void CreateAccount(char* UserID, char* Password,Privilege privilege,char* Username);

};
#endif //BOOKSTORE_ACCOUNT_H
