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
#include <vector>
#include <algorithm>

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

    Keyword(const std::string& keyword1);

    ~Keyword()=default;

    bool operator>(const Keyword &keyword1) const;

    bool operator==(const Keyword &keyword1) const;

    bool operator>=(const Keyword &keyword1) const;

    Keyword &operator=(const Keyword &keyword1);
};

//以name优先排序的key
struct Name_ISBN{
    Name name;

    ISBN bookISBN;

    bool operator>(const Name_ISBN &right) const;

    bool operator==(const Name_ISBN &right) const;

    bool operator>=(const Name_ISBN &right) const;

    Name_ISBN &operator=(const Name_ISBN &right);

    Name GetIndex(Name) const;

};

//以author优先排序的key
struct Author_ISBN{
    Author author;

    ISBN bookISBN;

    bool operator>(const Author_ISBN &right) const;

    bool operator==(const Author_ISBN &right) const;

    bool operator>=(const Author_ISBN &right) const;

    Author_ISBN &operator=(const Author_ISBN &right);

    Author GetIndex(Author) const;
};

//以keyword(单个)优先排序的key
struct Keyword_ISBN{
    Keyword keyword;

    ISBN bookISBN;

    bool operator>(const Keyword_ISBN &right) const;

    bool operator==(const Keyword_ISBN &right) const;

    bool operator>=(const Keyword_ISBN &right) const;

    Keyword_ISBN &operator=(const Keyword_ISBN &right);

    Keyword GetIndex(Keyword) const;
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

    ISBN GetIndex(ISBN isbn) const;

    Name GetIndex(Name name1) const;

    Author GetIndex(Author author1) const;

    ISBN GetKey(ISBN isbn) const;

    //获取name优先排序的key 用Book中的信息生成对应的key
    Name_ISBN GetKey(Name name1) const;

    Author_ISBN GetKey(Author author1) const;

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

    Keyword_ISBN GetKey(Keyword keywordIsbn) const;

    Keyword GetIndex(Keyword)const;

    void Print();
};

class BookManager{
public:
    BookManager()=default;

    ~BookManager()=default;

    //检索图书
    //show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
    //无参数 直接打表
    //需要4个函数满足不同key的检索
    void Show(TokenScanner &tokenScanner);

    //购买图书 {1} buy [ISBN] [Quantity]
    //减少库存
    //返回总价
    double Buy(TokenScanner &tokenScanner);

    //选择图书 {3} select [ISBN]
    //返回选中图书的ISBN和位置
    std::pair<ISBN,long> Select(TokenScanner &tokenScanner);

    //修改当前选中图书信息
    //修改ISBN影响排序 delete add 且修改keyword文件
    //选择某些文件覆盖 某些重插
    //修改keyword需把原先从文件中删干净
    //分成多个修改函数
    //modify_flag
    void Modify(TokenScanner &tokenScanner,std::pair<ISBN,long> pair);

    //图书进货{3} import [Quantity] [TotalCost]
    //修改quantity(in ISBN,name,author)
    double Import(TokenScanner &tokenScanner,std::pair<ISBN,long> pair);

private:
    //类中用（）会和声明函数歧义
    //ISBN->图书信息
    LinkList<ISBN,ISBN,Book> bookList{"book_information"};

    //name排序
    LinkList<Name,Name_ISBN,Book> nameList{"book_name"};

    //author排序
    LinkList<Author,Author_ISBN,Book> authorList{"book_author"};

    //单个keyword排序
    //index:Keyword key:Keyword_ISBN value:keyword+ISBN+location
    LinkList<Keyword,Keyword_ISBN,BookLocation> keywordList{"book_keyword"};

//    //查找
//    Book FindBook(const std::string &);

    //更新信息 ATTENTION：若修改成功，需要到文件中覆盖原有信息
    //传入iter为boolList中地址
    //增减库存 不动keyword文件
    void Update(Book book,long iter);

    //创建仅拥有ISBN的book 只需修改bookList
    void AddBook(ISBN isbn);

    //将keywords切片为string型 查重
    static void CutKeywords(std::string str,std::vector<std::string>&keywordGroup);

    //book:新信息 foreIter:原地址 iter:新地址
    void ReinsertISBN(const Book &book,const long &foreIter,long &iter);

    void ReinsertName(const Book &book,Name_ISBN key);

    void ReinsertAuthor(const Book &book,Author_ISBN key);

    //iter!=0 book新位置
    void ReinsertKeyword(const long &iter,const char *foreKeywords,ISBN foreISBN,ISBN isbn,std::vector<std::string>keywordGroup);

    void RewriteISBN(const Book &book,const long &foreIter);

    void RewriteName(const Book &book,const Name_ISBN &key);

    void RewriteAuthor(const Book &book,const Author_ISBN &key);

    //将string型keyword转为Keyword
    static Keyword MakeKeyword(const std::string &string);

};
#endif //BOOKSTORE_BOOK_H
