//
// Created by 房诗涵 on 2022/12/19.
//
//维护日志信息

#ifndef BOOKSTORE_LOG_H
#define BOOKSTORE_LOG_H

#include <vector>
#include "linkList.h"
#include "tokenScanner.h"
#include "error.h"
#include "account.h"

//一条操作记录
struct Record{
    int num;

    ID userID;

    std::string operation;

    int GetKey(int) const;

    void Print();
};

//一条交易记录
struct Transaction{
    //true 收入
    int num=0;

    bool income_flag= false;

    double amount=0;

    int GetKey(int) const;

};

class TransactionManager{
public:
    //从文件中读入数据
    TransactionManager();

    //将新的count和sum更新到headNode
    ~TransactionManager();

    //初始化
    void InitialTransation();

    //收入记录 （buy）
    void Income(double price);

    //支出记录 （import)
    void Expense(double cast);

    //财务记录查询 {7} show finance ([Count])?
    void ShowFinance(TokenScanner &tokenScanner);

private:
    //历史交易总数
    //最初：初始化清零
    //在文件headnode中记录交易总数，构造时读入，析构时写回文件
    int financeCount=0;

    //历史交易总额
    //在文件headnode中记录，构造时读入，析构时写回文件
    double income=0;
    double expense=0;

    //顺序插入 int为历史插入条数(1-based)
    LinkList<int,int,Transaction> transactionList{"finance_log"};
};

class LogManager{
public:
    LogManager();

    ~LogManager();

    void InitialLog();

    void PrintLog();

    void AddLog(const ID& id,const std::string &str);

private:
    //历史操作总数
    //在文件headnode中记录操作总数，构造时读入，析构时写回文件
    int count=0;

    //int为recode对应的count
    LinkList<int,int,Record> logList{"log"};
};

#endif //BOOKSTORE_LOG_H
