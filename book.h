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

    ISBN GetIndex();

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

    //基于key做比较 重载实际不需要
    bool operator>(const Book &right) const;

    bool operator==(const Book &right) const;

    bool operator>=(const Book &right) const;

    Book &operator=(const std::pair<Book, bool>& pair);

    ISBN GetKey(ISBN isbn) const;

    //获取name优先排序的key 用Book中的信息生成对应的key
    Name_IBSN GetKey(Name name1) const;

    Author_IBSN GetKey(Author author1) const;

//    Keyword_ISBN GetKey(Keyword keyword1);

    //打印book所有信息
    void Print();

};

//存于“book_keyword"中的元素，
//包含单个keyword ISBN location
struct BookLocation{

    Keyword keyword;

    ISBN bookISBN;

    long location=0;

    //基于key做比较 重载实际不需要
    bool operator>(const BookLocation &right) const;

    bool operator==(const BookLocation &right) const;

    bool operator>=(const BookLocation &right) const;

    BookLocation &operator=(const std::pair<BookLocation,bool> &pair);

    Keyword_ISBN GetKey(Keyword_ISBN keywordIsbn) const;

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
    void Select(TokenScanner &tokenScanner);

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
    //index:Keyword key:Keyword_ISBN value:keyword+ISBN+location
    LinkList<Keyword,Keyword_ISBN,BookLocation> keywordList{"book_keyword"};

    //查找
    Book FindBook(const std::string &);

    //更新信息 ATTENTION：若修改成功，需要到文件中覆盖原有信息
    //传入iter为boolList中地址
    //增减库存 不动keyword文件
    void Update(Book book,long iter);

    //创建仅拥有ISBN的book 只需修改bookList
    void AddBook(ISBN isbn);
};
#endif //BOOKSTORE_BOOK_H
