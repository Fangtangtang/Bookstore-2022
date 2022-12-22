//
// Created by 房诗涵 on 2022/12/18.
//

#include "tokenScanner.h"
#include "error.h"
#include <string>
#include <cstring>

TokenScanner::TokenScanner(const std::string &str) {
    input = str;
    length = str.length();
    tokenStart = tokenEnd = 0;
}

void TokenScanner::SetInput(const std::string &str) {
    input = str;
    length = str.length();
    tokenStart = tokenEnd = 0;
}

bool TokenScanner::HasMoreTokens() {
    if (tokenEnd >= length) return false;
    else return true;
}

void TokenScanner::NextToken(std::string &str) {
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if(tokenStart==length-1) break;
    }
    tokenEnd=tokenStart;
    while (input[tokenEnd] != ' ') {
        ++tokenEnd;
        if(tokenEnd==length) break;
    }
    str = input.substr(tokenStart, tokenEnd-tokenStart);
    operation=operation+str+" ";
    UpdatePos();
}
void TokenScanner::NextToken(char *token) {
    //前导空格
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if(tokenStart==length-1) break;
    }
    tokenEnd=tokenStart;
    while (input[tokenEnd] != ' ') {
        ++tokenEnd;
        if(tokenEnd==length) break;
    }
    std::string str = input.substr(tokenStart, tokenEnd-tokenStart);
    operation=operation+str+" ";
    const char *tmp = str.c_str();
    strcpy(token, tmp);
    UpdatePos();
}

void TokenScanner::NextToken(int &intNum) {
    intNum = 0;
    while (input[tokenEnd] != ' ') {
        intNum *= 10;
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0'<10) {
            intNum += (input[tokenEnd] - '0');
            ++tokenEnd;
            if(tokenEnd==length) break;
        } else error("Invalid");
    }
    std::string str = input.substr(tokenStart, tokenEnd-tokenStart);
    operation=operation+str+" ";
    UpdatePos();
}

void TokenScanner::NextToken(double &doubleNum) {
    doubleNum = 0;
    //整数部分
    while (input[tokenEnd] != ' ') {
        if (input[tokenEnd] == '.') {
            ++tokenEnd;
            break;
        }
        doubleNum *= 10;
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0'<10) {
            doubleNum += (input[tokenEnd] - '0');
            ++tokenEnd;
            if(tokenEnd==length) break;
        } else error("Invalid");
    }
    double mul = 1;
    while (tokenEnd<length&&input[tokenEnd] != ' ') {
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0'<10) {
            mul *= 0.1;
            doubleNum += (input[tokenEnd] - '0') * mul;
            ++tokenEnd;
            if(tokenEnd==length) break;
        } else error("Invalid");
    }
    std::string str = input.substr(tokenStart, tokenEnd-tokenStart);
    operation=operation+str+" ";
    UpdatePos();
}

void TokenScanner::TakeType(std::string &str) {
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if(tokenStart==length-1) break;
    }
    if(input[tokenStart]!='-') error("Invalid");
    ++tokenStart;
    tokenEnd=tokenStart;
    while (input[tokenEnd] != '='&&input[tokenEnd] !=' ') {
        if(tokenEnd==length-1) error("Invalid");
        ++tokenEnd;
    }
    if(input[tokenEnd]!='=') error("Invalid");
    str = input.substr(tokenStart, tokenEnd-tokenStart);
    operation=operation+"-"+str+"=";
    UpdatePos();
}

void TokenScanner::Quote(char *token) {
     if(input[tokenStart]!='"')  error("Invalid");
     ++tokenStart;
     tokenEnd=tokenStart;
    while (input[tokenEnd] != '"'&&input[tokenEnd] !=' ') {
        if(tokenEnd==length-1) error("Invalid");
        ++tokenEnd;
    }
    if(input[tokenEnd]!='"') error("Invalid");
    std::string str = input.substr(tokenStart, tokenEnd-tokenStart);
    operation=operation+'"'+ str+'"'+" ";
    const char *tmp = str.c_str();
    strcpy(token, tmp);
    UpdatePos();
}

std::string TokenScanner::ShowRest() {
    return input.substr(tokenStart,length);
}

std::string TokenScanner::ShowOperation() {
    return operation;
}

//private
void TokenScanner::UpdatePos() {
    if(tokenEnd>=input.size()) return;
    tokenStart = tokenEnd + 1;
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if (tokenStart == length) break;
    }
    tokenEnd = tokenStart;
}






