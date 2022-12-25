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

void TokenScanner::Initialize() {
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if (tokenStart == length) break;
    }
    tokenEnd = tokenStart;
}

bool TokenScanner::HasMoreTokens() const {
    if (tokenEnd >= length) return false;
    else return true;
}

void TokenScanner::NextToken(std::string &str) {
//    while (input[tokenStart] == ' ') {
//        ++tokenStart;
//        if (tokenStart == length - 1) break;
//    }
//    tokenEnd = tokenStart;
    while (input[tokenEnd] != ' ') {
        if(!std::isprint(input[tokenEnd]))error("Invalid");
        ++tokenEnd;
        if (tokenEnd == length) break;
    }
    str = input.substr(tokenStart, tokenEnd - tokenStart);
    operation = operation + str + " ";
    UpdatePos();
}

void TokenScanner::NextToken(char *token, int validSize, bool allowQuote) {
    //前导空格
//    while (input[tokenStart] == ' ') {
//        ++tokenStart;
//        if (tokenStart == length - 1) break;
//    }
//    tokenEnd = tokenStart;
    while (input[tokenEnd] != ' ') {
        if(!std::isprint(input[tokenEnd]))error("Invalid");
        if (!allowQuote) {
            if (input[tokenEnd] == '"')error("Invalid");
        }
        ++tokenEnd;
        if (tokenEnd - tokenStart > validSize)error("Invalid");
        if (tokenEnd == length) break;
    }
//    if (tokenEnd - tokenStart > validSize)error("Invalid");
    std::string str = input.substr(tokenStart, tokenEnd - tokenStart);
    operation = operation + str + " ";
    const char *tmp = str.c_str();
    strcpy(token, tmp);
    UpdatePos();
}

void TokenScanner::SpecialNextToken(char *token) {
//    while (input[tokenStart] == ' ') {
//        ++tokenStart;
//        if (tokenStart == length - 1) break;
//    }
//    tokenEnd = tokenStart;
    while (input[tokenEnd] != ' ') {
        if ((input[tokenEnd] - '0' < 10 && input[tokenEnd] - '0' >= 0) || input[tokenEnd] == '_' ||
            (input[tokenEnd] - 'a' >= 0 && input[tokenEnd] - 'a' < 26) ||
            (input[tokenEnd] - 'A' >= 0 && input[tokenEnd] - 'A' < 26)) {
            ++tokenEnd;
            if (tokenEnd == length) break;
        } else error("Invalid");
    }
    if (tokenEnd - tokenStart > 30)error("Invalid");
    std::string str = input.substr(tokenStart, tokenEnd - tokenStart);
    operation = operation + str + " ";
    const char *tmp = str.c_str();
    strcpy(token, tmp);
    UpdatePos();
}

void TokenScanner::NextToken(int &intNum) {
    intNum = 0;
    long long num=0;
    while (input[tokenStart]=='0'){
        if(tokenStart==length-1) break;
        ++tokenStart;
    }
    tokenEnd=tokenStart;
    while (input[tokenEnd] != ' ') {
        num *= 10;
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0' < 10) {
            num += (input[tokenEnd] - '0');
            ++tokenEnd;
            if (tokenEnd == length) break;
        } else error("Invalid");
    }
    if (num > 2147483647)error("Invalid");
    intNum=int(num);
    std::string str = input.substr(tokenStart, tokenEnd - tokenStart);
    operation = operation + str + " ";
    UpdatePos();
}

void TokenScanner::NextToken(double &doubleNum) {
    doubleNum = 0;
    //前导0
    while (input[tokenStart]=='0'){
        if(tokenStart==length-1) break;
        ++tokenStart;
    }
    tokenEnd=tokenStart;
    //整数部分
    while (input[tokenEnd] != ' ') {
        if (input[tokenEnd] == '.') {
            ++tokenEnd;
            break;
        }
        doubleNum *= 10;
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0' < 10) {
            doubleNum += (input[tokenEnd] - '0');
            ++tokenEnd;
            if (tokenEnd == length) break;
        } else error("Invalid");
    }
    double mul = 1;
    int count=0;
    while (tokenEnd < length && input[tokenEnd] != ' ') {
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0' < 10) {
            mul *= 0.1;
            doubleNum += (input[tokenEnd] - '0') * mul;
            ++count;
            if(count ==3)error("Invalid");
            ++tokenEnd;
            if (tokenEnd == length) break;
        } else error("Invalid");
    }
    if (tokenEnd - tokenStart > 13) error("Invalid");
    std::string str = input.substr(tokenStart, tokenEnd - tokenStart);
    operation = operation + str + " ";
    UpdatePos();
}

void TokenScanner::TakeType(std::string &str) {
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if (tokenStart == length - 1) break;
    }
    if (input[tokenStart] != '-') error("Invalid");
    ++tokenStart;
    tokenEnd = tokenStart;
    while (input[tokenEnd] != '=' && input[tokenEnd] != ' ') {
        if (tokenEnd == length - 1) error("Invalid");
        ++tokenEnd;
    }
    if (input[tokenEnd] != '=') error("Invalid");
    if (input[tokenEnd+1] == ' ') error("Invalid");
    str = input.substr(tokenStart, tokenEnd - tokenStart);
    operation = operation + "-" + str + "=";
    UpdatePos();
}

void TokenScanner::Quote(char *token) {
    if (input[tokenStart] != '"') error("Invalid");
    ++tokenStart;
    tokenEnd = tokenStart;
    while (input[tokenEnd] != '"' && input[tokenEnd] != ' ') {
        if(!std::isprint(input[tokenEnd]))error("Invalid");
        if(input[tokenEnd] == '"'){
            if(tokenEnd!=length-1&&input[tokenEnd+1]!=' ') error("Invalid");
        }
        if (tokenEnd == length - 1) error("Invalid");
        ++tokenEnd;
    }
    if (input[tokenEnd] != '"') error("Invalid");
    if (tokenEnd - tokenStart > 60||tokenEnd-tokenStart==0)error("Invalid");
    std::string str = input.substr(tokenStart, tokenEnd - tokenStart);
    operation = operation + '"' + str + '"' + " ";
    const char *tmp = str.c_str();
    strcpy(token, tmp);
    UpdatePos();
}

std::string TokenScanner::ShowRest() {
    return input.substr(tokenStart, length);
}

std::string TokenScanner::ShowOperation() {
    return operation;
}

//private
void TokenScanner::UpdatePos() {
    if (tokenEnd >= length-1){
        ++tokenEnd;
        return;
    }
    tokenStart = tokenEnd + 1;
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if (tokenStart == length) break;
    }
    tokenEnd = tokenStart;
}








