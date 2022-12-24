//
// Created by 房诗涵 on 2022/12/24.
//
#include "error.h"
void error(std::string message) {
    throw ErrorException(message);
}