//
// Created by 房诗涵 on 2022/12/18.
//

#ifndef BOOKSTORE_TOKENSCANNER_H
#define BOOKSTORE_TOKENSCANNER_H

#include <string>
#include <cctype>
class TokenScanner{
public:
    TokenScanner()=default;

    explicit TokenScanner(const std::string& str);

    ~TokenScanner()=default;

    void Initialize();

    //是否仍有输入
    bool HasMoreTokens() ;

    //下一个合法token为string
    void NextToken(std::string &str);

    //下一个合法token为char*
    void NextToken(char* token,int validSize,bool allowQuote);

    //特殊要求的char*
    void SpecialNextToken(char* token);
    //下一个合法token为int
    void NextToken(int &intNum);

    //下一个合法token为double
    void NextToken(double &doubleNum);

    //解析 -string=
    void TakeType(std::string &str);

    //解析”char*"
    void Quote(char* token);

    //给出剩余部分的string
    std::string ShowRest();

    std::string ShowOperation();
private:
    std::string input;
    std::string operation;
    //input 长度
    int length=0;
    //每次截取token的始末位置
    int tokenStart=0,tokenEnd=0;

    //更新token头尾部所在位置
    void UpdatePos();
};

#endif //BOOKSTORE_TOKENSCANNER_H
