cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        src/main.cpp
        src/account.cpp
        src/tokenScanner.cpp
        src/book.cpp
        src/log.cpp
        src/loggingStatus.cpp
        )
