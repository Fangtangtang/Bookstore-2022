//
// Created by 房诗涵 on 2022/12/18.
//

#ifndef BOOKSTORE_TOKENSCANNER_H
#define BOOKSTORE_TOKENSCANNER_H

#include <string>

class TokenScanner{
public:
    TokenScanner()=default;

    explicit TokenScanner(const std::string& str);

    ~TokenScanner()=default;

    void setInput(const std::string& str);

    //是否仍有输入
    bool hasMoreTokens() ;

    //下一个合法token为string
    void nextToken(std::string &str);

    //下一个合法token为char*
    void nextToken(char* token);

    //下一个合法token为int
    void nextToken(int &intNum);

    //下一个合法token为double
    void nextToken(double &doubleNum);


private:
    std::string input;
    //input 长度
    int length=0;
    //每次截取token的始末位置
    int tokenStart=0,tokenEnd=0;

    //更新token头尾部所在位置
    void updatePos();
};

#endif //BOOKSTORE_TOKENSCANNER_H
