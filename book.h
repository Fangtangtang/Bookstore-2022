//
// Created by 房诗涵 on 2022/12/18.
//
//BookManager: 图书信息管理

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H

#include "linkList.h"
#include <string>

struct ISBN {
    char bookISBN[21]={'\0'};

    ISBN();

    ISBN(char* id);

    ~ISBN();

    bool operator>(const ISBN &id) const;

    bool operator==(const ISBN &id) const;

    bool operator>=(const ISBN &id) const;

    ISBN &operator=(const ISBN &id);

};

struct Name {
    char name[61]={'\0'};

    Name();

    Name(char* id);

    ~Name();

    bool operator>(const Name &id) const;

    bool operator==(const Name &id) const;

    bool operator>=(const Name &id) const;

    Name &operator=(const Name &id);

};

struct Author {
    char author[61]={'\0'};

    Author();

    Author(char* id);

    ~Author();

    bool operator>(const Author &id) const;

    bool operator==(const Author &id) const;

    bool operator>=(const Author &id) const;

    Author &operator=(const Author &id);

};

struct Book {

    ISBN bookISBN;

    Name name;

    Author author;

    char keyword[61]={'\0'};

    int quantity=0;

    double price=0;

    //基于不同key插入时 在同key大小比较基于IBSN
    //double_index linklist?
    bool operator>(const Book &id) const;

    bool operator==(const Book &id) const;

    bool operator>=(const Book &id) const;

    //修改信息 ATTENTION：若修改成功，需要到文件中覆盖原有信息
    void Modify(std::string &CurrentPassword,std::string &NewPassword);

};

class BookManager{
public:
    BookManager();

    ~BookManager()=default;

    //检索图书
    //show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
    //无参数 直接打表
    //需要4个函数满足不同key的检索
    void Show();

    //购买图书 {1} buy [ISBN] [Quantity]
    //减少库存
    void Buy();

    //选择图书 {3} select [ISBN]
    void Select();

    //修改图书信息
    void Modify();

private:
    //储存账户信息的块状链表
    //类中用（）会和声明函数歧义
    LinkList<I,Account> accountList{"account_information"};

    std::fstream io_account_information;

//    std::fstream io_account_index;

    //查找帐户
    Book FindBook(const std::string &UserID);


};
#endif //BOOKSTORE_BOOK_H
