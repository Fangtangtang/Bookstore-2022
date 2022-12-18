//
// Created by 房诗涵 on 2022/12/10.
//
//AccountManager: 账户信息管理

//函数 GetKey
#ifndef BOOKSTORE_ACCOUNT_H
#define BOOKSTORE_ACCOUNT_H

#include "linkList.h"
#include <string>

//权限 符号常量 对应{0} {1} {3} {7}
enum Privilege{visitor,customer,clerk,host};

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

};

struct Account {

    Privilege privilege;

    ID userID;

    char name[31]={'\0'};

    char password[31]={'\0'};

    bool operator>(const Account &id) const;

    bool operator==(const Account &id) const;

    bool operator>=(const Account &id) const;

    //获取key
    ID GetKey(ID id) const;

    void Print();

    //修改密码 ATTENTION：若修改成功，需要到文件中覆盖原有信息
    void ChangePassword(std::string &CurrentPassword,std::string &NewPassword);

};

class AccountManager{
public:
    AccountManager();

    ~AccountManager()=default;

    //注册账户 {0} register [UserID] [Password] [Username]
    //游客注册 权限为{1} ID不可和已有的重复
    void Register(std::string &UserID,std::string &Password,std::string & Username);

    //创建帐户 {3} useradd [UserID] [Password] [Privilege] [Username]
    //高权限用户创建低权限账号
    void AddUser(std::string &UserID, std::string &Password,int Privilege ,std::string & Username);

    //删除账户 {7} delete [UserID]
    //仅{7}
    void DeleteUser(std::string &UserID);

    //修改密码 {1} passwd [UserID] ([CurrentPassword])? [NewPassword]
    //如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]（登录栈中用？）
    void ChangePassword(std::string&UserID,std::string&CurrentPassword,std::string&NewPassword);

private:
    //储存账户信息的块状链表
    //类中用（）会和声明函数歧义
    LinkList<ID,ID,Account> accountList{"account_information"};

    //查找帐户
    Account FindAccount(const std::string &UserID);

};
#endif //BOOKSTORE_ACCOUNT_H
