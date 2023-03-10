//
// Created by 房诗涵 on 2022/12/10.
//
#include "account.h"
#include <string>
#include <cstring>

//ID------------------------------------------------------
//默认构造：全空
ID::ID() {
    memset(userID, 0, sizeof(userID));
}

//由char*初始化
ID::ID(char *id) {
    memset(userID, 0, sizeof(userID));
    strcpy(userID, id);
}

bool ID::operator>(const ID &id) const {
    return strcmp(userID, id.userID) > 0;
}

bool ID::operator<(const ID &id) const {
    return strcmp(userID, id.userID) < 0;
}

bool ID::operator==(const ID &id) const {
    return !strcmp(userID, id.userID);
}

bool ID::operator>=(const ID &id) const {
    return strcmp(userID, id.userID) >= 0;
}

ID &ID::operator=(const ID &id) = default;

ID ID::GetIndex(const ID&) {
    return *this;
}

ID::~ID() = default;

//Account---------------------------------------------------

bool Account::operator>(const Account &id) const {
    return userID>id.userID;
}

bool Account::operator==(const Account &id) const {
    return userID==id.userID;
}

bool Account::operator>=(const Account &id) const {
    return userID>=id.userID;
}

Account &Account::operator=(const std::pair<Account, bool>& pair) {
    *this = pair.first;
    return *this;
}

ID Account::GetKey(const ID& id) const {
    return userID;
}

//AccountManager--------------------------------------------
void AccountManager::InitialAccount() {
    char root[5] = "root", name[5] = "sjtu";
    CreateAccount(root, name, host, root);
}

void AccountManager::Register(TokenScanner &tokenScanner) {
    char userID[31], password[31], name[31];
    memset(userID,0, sizeof(userID));
    memset(password,0, sizeof(password));
    memset(name,0, sizeof(name));
    if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(userID);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(password);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(name,30, true);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) error("Invalid");
    CreateAccount(userID, password, customer, name);
}

void AccountManager::ChangePassword(TokenScanner &tokenScanner, CurrentAccount &user) {
    char userID[31], password1[31], password2[31];
    memset(userID,0, sizeof(userID));
    memset(password1,0, sizeof(password1));
    memset(password2,0, sizeof(password2));
    if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(userID);
    else error("Invalid");
    ID id(userID);
    long iter;
    std::pair<Account, bool> pair = accountList.Find(id, iter);
    if (!pair.second) error("Invalid");
    Account account = pair.first;
    if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(password1);
    else error("Invalid");
    if (user.privilege != host) {
        if (strcmp(password1, pair.first.password) != 0) error("Invalid");
        if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(password2);
        else error("Invalid");
        if (tokenScanner.HasMoreTokens()) error("Invalid");
        //修改密码
        Modify(account, iter, password2);
    }
        //host
    else {
        if (!tokenScanner.HasMoreTokens()) {
            Modify(account, iter, password1);
        } else {
            if (strcmp(password1, pair.first.password) != 0) error("Invalid");
            tokenScanner.SpecialNextToken(password2);
            if (tokenScanner.HasMoreTokens()) error("Invalid");
            Modify(account, iter, password2);
        }
    }
}

std::pair<Account, bool> AccountManager::FindAccount(char *UserID) {
    long iter;
    return accountList.Find(UserID,iter);
}

void AccountManager::Modify(Account account, long iter, char *newPassword) {
    strcpy(account.password, newPassword);
    accountList.WriteValue(account, iter);
}

void AccountManager::AddUser(TokenScanner &tokenScanner, Privilege &privilege) {
    char userID[31], password[31], name[31];
    memset(userID,0, sizeof(userID));
    memset(password,0, sizeof(password));
    memset(name,0, sizeof(name));
    int pri;
    if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(userID);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) tokenScanner.SpecialNextToken(password);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(pri, true);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(name,30, true);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) error("Invalid");
    ID id(userID);
    Privilege newPri = toPrivilege(pri);
    if (newPri >= privilege)error("Invalid");
    CreateAccount(userID, password, newPri, name);
}

Privilege AccountManager::toPrivilege(int i) {
    if (i == 1) return customer;
    if (i == 3) return clerk;
    if (i == 7) return host;
    error("Invalid");
    return none;
}

void AccountManager::CreateAccount(char *UserID, char *password, Privilege privilege, char *name) {
    ID id(UserID);
    long iter;
    std::pair<Account, bool> pair = accountList.Find(id, iter);
    if (pair.second) error("Invalid");//ID已存在
    Account newAccount;
    newAccount.userID = id;
    newAccount.privilege = privilege;
    strcpy(newAccount.password, password);
    strcpy(newAccount.name, name);
    accountList.Insert(id, newAccount);
}

void AccountManager::DeleteUser(const ID& id) {
    if (!accountList.Delete(id)) error("Invalid");
}
