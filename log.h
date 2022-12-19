//
// Created by 房诗涵 on 2022/12/19.
//
//维护日志信息

#ifndef BOOKSTORE_LOG_H
#define BOOKSTORE_LOG_H

#include "linkList.h"

//一条操作记录
struct Record{
    //todo
};

//一条交易记录
struct Transaction{
    //true 收入
    bool income_flag= false;

    double amount=0;
};

class TransactionManager{
public:
    TransactionManager();

    ~TransactionManager();

    //财务记录查询 {7} show finance ([Count])?
    void ShowFinance(int count);

    //财务记录查询 {7} show finance
    void Sum();

    //添加交易记录
    void AddTransaction();

private:
    //历史交易总数
    //在文件headnode中记录交易总数，构造时读入，析构时写回文件
    int financeCount=0;

    //历史交易总额
    //在文件headnode中记录，构造时读入，析构时写回文件
    double sum=0;

    //顺序插入 int为历史插入条数
    LinkList<int,int,Transaction> transactionList{"finance_log"};
};

class LogManager{
public:
    LogManager();

    ~LogManager();

    void PrintLog();

private:
    //历史操作总数
    //在文件headnode中记录操作总数，构造时读入，析构时写回文件
    int count=0;

    //int为recode对应的count
    LinkList<int,int,Record> logList{"log"};
};

#endif //BOOKSTORE_LOG_H
