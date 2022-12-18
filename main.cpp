#include <iostream>
#include "account.h"
#include "error.h"
#include "tokenScanner.h"
#include <string>

//todo
//add the objects
void processLine(std::string ,AccountManager& ,BookManager&,LogManager&);

//main:循环读取cmd 退出或接入相应系统
//应对各种异常输入 getline + substr 读取（tokenscanner？）
int main() {
    //todo
    //接入系统的对象
    while (true) {
        try{
            std::string input;
            getline(std::cin, input);
            //todo
            //verify the function
            processLine(input, );
        }catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
}

void processLine(std::string input,){
    TokenScanner tokenScanner(input);

}
