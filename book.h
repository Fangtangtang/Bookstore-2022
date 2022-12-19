//
// Created by 房诗涵 on 2022/12/18.
//
//BookManager: 图书信息管理

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H

#include "linkList.h"
#include "tokenScanner.h"
#include "error.h"
#include <string>
#include <cstring>

struct ISBN {
    char bookISBN[21]={'\0'};

    ISBN();

    ISBN(char* isbn);

    ~ISBN()=default;

    bool operator>(const ISBN &isbn) const;

    bool operator==(const ISBN &isbn) const;

    bool operator>=(const ISBN &isbn) const;

    ISBN &operator=(const ISBN &isbn);

};

struct Name {
    char name[61]={'\0'};

    Name();

    Name(char* name1);

    ~Name()=default;

    bool operator>(const Name &name1) const;

    bool operator==(const Name &name1) const;

    bool operator>=(const Name &name1) const;

    Name &operator=(const Name &name1);

};

struct Author {
    char author[61]={'\0'};

    Author();

    Author(char* author1);

    ~Author()=default;

    bool operator>(const Author &author1) const;

    bool operator==(const Author &author1) const;

    bool operator>=(const Author &author1) const;

    Author &operator=(const Author &author1);

};

//单个keyword
struct Keyword{
    char keyword[61]={'\0'};

    Keyword();

    Keyword(char* keyword1);

    ~Keyword()=default;

    bool operator>(const Keyword &keyword1) const;

    bool operator==(const Keyword &keyword1) const;

    bool operator>=(const Keyword &keyword1) const;

    Keyword &operator=(const Keyword &keyword1);
};

//以name优先排序的key
struct Name_IBSN{
    Name name;

    ISBN bookISBN;

    bool operator>(const Name_IBSN &right) const;

    bool operator==(const Name_IBSN &right) const;

    bool operator>=(const Name_IBSN &right) const;

    Name_IBSN &operator=(const Name_IBSN &right);

    Name GetIndex() const;

};

//以author优先排序的key
struct Author_IBSN{
    Author author;

    ISBN bookISBN;

    bool operator>(const Author_IBSN &right) const;

    bool operator==(const Author_IBSN &right) const;

    bool operator>=(const Author_IBSN &right) const;

    Author_IBSN &operator=(const Author_IBSN &right);

    Author GetIndex() const;
};

//以keyword(单个)优先排序的key
struct Keyword_ISBN{
    Keyword keyword;

    ISBN bookISBN;

    bool operator>(const Keyword_ISBN &right) const;

    bool operator==(const Keyword_ISBN &right) const;

    bool operator>=(const Keyword_ISBN &right) const;

    Keyword_ISBN &operator=(const Keyword_ISBN &right);

    Keyword GetIndex() const;
};

struct Book {

    ISBN bookISBN;

    Name name;

    Author author;

    //基础信息中存完整的keywords
    char keywords[61]={'\0'};

    int quantity=0;

    double price=0;

    bool operator>(const Book &right) const;

    bool operator==(const Book &right) const;

    bool operator>=(const Book &right) const;

    //获取name优先排序的key 用Book中的信息生成对应的key
    Name_IBSN GetKey(Name name1) const;

    Author_IBSN GetKey(Author author1) const;

    Keyword_ISBN GetKey(Keyword keyword1);

    //打印book所有信息
    void Print();

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
    void Buy(TokenScanner &tokenScanner);

    //选择图书 {3} select [ISBN]
    void Select();

    //修改图书信息
    void Modify();

    //图书进货{3} import [Quantity] [TotalCost]
    //修改quantity
    void Import();

private:
    //类中用（）会和声明函数歧义
    //ISBN->图书信息
    LinkList<ISBN,ISBN,Book> bookList{"book_information"};

    //name排序
    LinkList<Name,Name_IBSN,Book> nameList{"book_name"};

    //author排序
    LinkList<Author,Author_IBSN,Book> authorList{"book_author"};

    //单个keyword排序
    //long 对应book在book_information地址
    LinkList<Keyword,Keyword_ISBN,long> keywordList{"book_keyword"};

    //查找
    Book FindBook(const std::string &);

};
#endif //BOOKSTORE_BOOK_H
