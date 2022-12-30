//
// Created by 房诗涵 on 2022/12/19.
//

#include "log.h"

//Record--------------------------------------------------------
Record::Record(int count, ID id, std::string str) {
    memset(operation, 0, sizeof(operation));
    num = count;
    userID = id;
    const char *tmp = str.c_str();
    strcpy(operation, tmp);
}

int Record::GetKey(int) const {
    return num;
}

void Record::Print() {
    std::cout<<"("<< num<<")" << userID.userID << '\n' << operation << "\n";
}

//Transaction--------------------------------------------------
int Transaction::GetKey(int) const {
    return num;
}

//TransactionManager-------------------------------------------
//用linkList默认的构造函数构造list
TransactionManager::TransactionManager() {
    HeadNode headNode = transactionList.ReadHeadNode();
    financeCount = headNode.count;
    income = headNode.income;
    expense = headNode.expense;
}

TransactionManager::~TransactionManager() {
    transactionList.WriteHeadNode(financeCount, income, expense);
}

void TransactionManager::InitialTransation() {
    financeCount = 0;
    income = 0;
    expense = 0;
}

void TransactionManager::Income(double price) {
    ++financeCount;
    income += price;
    Transaction transaction;
    transaction.num = financeCount;
    transaction.income_flag = true;
    transaction.amount = price;
    transactionList.Insert(financeCount, transaction);
}

void TransactionManager::Expense(double cast) {
    ++financeCount;
    expense += cast;
    Transaction transaction;
    transaction.num = financeCount;
    transaction.income_flag = false;
    transaction.amount = cast;
    transactionList.Insert(financeCount, transaction);
}

void TransactionManager::ShowFinance(TokenScanner &tokenScanner) {
    if (!tokenScanner.HasMoreTokens()) {
        std::cout << "+ " << income << " - " << expense << '\n';
        return;
    }
    int count;
    tokenScanner.NextToken(count, false);
    if (tokenScanner.HasMoreTokens()) error("Invalid");
    if (count == 0) {
        std::cout << '\n';
        return;
    }
    if (count > financeCount) error("Invalid");
    //最后count笔交易
    std::vector<Transaction> transactionGroup = transactionList.Traverse(financeCount - count + 1);
    double in = 0, out = 0;
    Transaction transaction;
    while (!(transactionGroup.empty())) {
        transaction = transactionGroup.back();
        if (transaction.income_flag) {
            in += transaction.amount;
        } else {
            out += transaction.amount;
        }
        transactionGroup.pop_back();
    }
    std::cout << "+ " << in << " - " << out << '\n';
}

//LogManager---------------------------------------------------------
LogManager::LogManager() {
    HeadNode headNode = logList.ReadHeadNode();
    count = headNode.count;
}

LogManager::~LogManager() {
    logList.WriteHeadNode(count);
}

void LogManager::InitialLog() {
    count = 0;
}

void LogManager::PrintLog() {
    logList.PrintList();
}

void LogManager::AddLog(const ID &id, const std::string &str) {
    ++count;
    Record record(count, id, str);
    logList.Insert(count, record);
}

