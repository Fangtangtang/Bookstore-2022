//
// Created by 房诗涵 on 2022/12/18.
//
//异常处理类(copy)
#ifndef BOOKSTORE_ERROR_H
#define BOOKSTORE_ERROR_H

#include <string>
#include <exception>

class ErrorException : public std::exception {
public:
    explicit ErrorException(std::string message) {
        this->message = message;
    }

    std::string getMessage() const{
        return message;
    }

private:
    std::string message;
};

//------------------------------------------------------------------------------------------------
//调用：error("Invalid");
// catch (ErrorException &ex) {
//        std::cout << ex.getMessage() << std::endl;
// }
//static
void error(std::string message);
//{
//    throw ErrorException(message);
//}

#endif //BOOKSTORE_ERROR_H
