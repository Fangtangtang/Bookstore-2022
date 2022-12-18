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

void TokenScanner::setInput(const std::string &str) {
    input = str;
    length = str.length();
    tokenStart = tokenEnd = 0;
}

bool TokenScanner::hasMoreTokens() {
    if (tokenEnd >= length) return false;
    else return true;
}

void TokenScanner::nextToken(char *token) {
    //前导空格
    while (input[tokenStart] == ' ') ++tokenStart;
    tokenEnd=tokenStart;
    while (input[tokenEnd] != ' ') {
        if(tokenEnd==length-1) break;
        ++tokenEnd;
    }
    std::string str = input.substr(tokenStart, tokenEnd);
    const char *tmp = str.c_str();
    strcpy(token, tmp);
    updatePos();
}

void TokenScanner::nextToken(int &intNum) {
    intNum = 0;
    while (input[tokenEnd] != ' ') {
        intNum *= 10;
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0') {
            intNum += (input[tokenEnd] - '0');
            ++tokenEnd;
            if(tokenEnd==length) return;
        } else error("Invalid");
    }
    updatePos();
}

void TokenScanner::nextToken(double &doubleNum) {
    doubleNum = 0;
    //整数部分
    while (input[tokenEnd] != ' ') {
        if (input[tokenEnd] == '.') {
            ++tokenEnd;
            break;
        }
        doubleNum *= 10;
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0') {
            doubleNum += (input[tokenEnd] - '0');
            ++tokenEnd;
            if(tokenEnd==length) return;
        } else error("Invalid");
    }
    double mul = 0.1;
    while (input[tokenEnd] != ' ') {
        if (input[tokenEnd] - '0' >= 0 && input[tokenEnd] - '0') {
            doubleNum += (input[tokenEnd] - '0') * mul;
            mul *= 0.1;
            ++tokenEnd;
            if(tokenEnd==length) return;
        } else error("Invalid");
    }
    updatePos();
}

//private
void TokenScanner::updatePos() {
    tokenStart = tokenEnd + 1;
    while (input[tokenStart] == ' ') {
        ++tokenStart;
        if (tokenStart == length) break;
    }
    tokenEnd = tokenStart;
}


