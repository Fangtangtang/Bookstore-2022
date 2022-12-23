//
// Created by 房诗涵 on 2022/12/18.
//

#include "book.h"

//ISBN------------------------------------------
ISBN::ISBN() {
    memset(bookISBN, 0, sizeof(bookISBN));
}

ISBN::ISBN(char *isbn) {
    strcpy(bookISBN, isbn);
}

bool ISBN::operator>(const ISBN &isbn) const {
    return strcmp(bookISBN, isbn.bookISBN) > 0;
}

bool ISBN::operator==(const ISBN &isbn) const {
    return !strcmp(bookISBN, isbn.bookISBN);
}

bool ISBN::operator>=(const ISBN &isbn) const {
    return strcmp(bookISBN, isbn.bookISBN) >= 0;
}

ISBN &ISBN::operator=(const ISBN &isbn) = default;

ISBN ISBN::GetIndex() {
    return *this;
}

//Name--------------------------------------------
Name::Name() {
    memset(name, 0, sizeof(name));
}

Name::Name(char *name1) {
    strcpy(name, name1);
}

bool Name::operator>(const Name &name1) const {
    return strcmp(name, name1.name) > 0;
}

bool Name::operator==(const Name &name1) const {
    return !strcmp(name, name1.name);
}

bool Name::operator>=(const Name &name1) const {
    return strcmp(name, name1.name) >= 0;
}

Name &Name::operator=(const Name &name1) = default;

//Author-------------------------------------------

Author::Author() {
    memset(author, 0, sizeof(author));
}

Author::Author(char *author1) {
    strcpy(author, author1);
}

bool Author::operator>(const Author &author1) const {
    return strcmp(author, author1.author) > 0;
}

bool Author::operator==(const Author &author1) const {
    return !strcmp(author, author1.author);
}

bool Author::operator>=(const Author &author1) const {
    return strcmp(author, author1.author) >= 0;
}

Author &Author::operator=(const Author &author1) = default;

//Keyword---------------------------------------------

Keyword::Keyword() {
    memset(keyword, 0, sizeof(keyword));
}

Keyword::Keyword(char *keyword1) {
    strcpy(keyword, keyword1);
}

Keyword::Keyword(const std::string &keyword1) {
    const char *tmp = keyword1.c_str();
    strcpy(keyword, tmp);
}

bool Keyword::operator>(const Keyword &keyword1) const {
    return strcmp(keyword, keyword1.keyword) > 0;
}

bool Keyword::operator==(const Keyword &keyword1) const {
    return !strcmp(keyword, keyword1.keyword);
}

bool Keyword::operator>=(const Keyword &keyword1) const {
    return strcmp(keyword, keyword1.keyword) >= 0;
}

Keyword &Keyword::operator=(const Keyword &keyword1) = default;

//Name_IBSN----------------------------------------------
//name优先排序 IBSN次优先
bool Name_ISBN::operator>(const Name_ISBN &right) const {
    if (name > right.name || (name == right.name && bookISBN > right.bookISBN))return true;
    else return false;
}

//ISBN唯一
bool Name_ISBN::operator==(const Name_ISBN &right) const {
    return bookISBN == right.bookISBN;
}

bool Name_ISBN::operator>=(const Name_ISBN &right) const {
    return !(right > *this);
}

Name_ISBN &Name_ISBN::operator=(const Name_ISBN &right) = default;

Name Name_ISBN::GetIndex(Name) const {
    return name;
}

//Author_IBSN--------------------------------------------------

bool Author_ISBN::operator>(const Author_ISBN &right) const {
    if (author > right.author || (author == right.author && bookISBN > right.bookISBN))return true;
    else return false;
}

bool Author_ISBN::operator==(const Author_ISBN &right) const {
    return bookISBN == right.bookISBN;
}

bool Author_ISBN::operator>=(const Author_ISBN &right) const {
    return !(right > *this);
}

Author_ISBN &Author_ISBN::operator=(const Author_ISBN &right) = default;

Author Author_ISBN::GetIndex(Author) const {
    return author;
}

//Keyword_ISBN--------------------------------------------------

bool Keyword_ISBN::operator>(const Keyword_ISBN &right) const {
    if (keyword > right.keyword || (keyword == right.keyword && bookISBN > right.bookISBN))return true;
    else return false;
}

bool Keyword_ISBN::operator==(const Keyword_ISBN &right) const {
    return (bookISBN == right.bookISBN && keyword == right.keyword);
}

bool Keyword_ISBN::operator>=(const Keyword_ISBN &right) const {
    return !(right > *this);
}

Keyword_ISBN &Keyword_ISBN::operator=(const Keyword_ISBN &right) = default;

Keyword Keyword_ISBN::GetIndex(Keyword) const {
    return keyword;
}

Keyword_ISBN Keyword_ISBN::GetKey(Keyword) const {
    return *this;
}

void Keyword_ISBN::Print() {
    std::cout << bookISBN.bookISBN << '\t' << keyword.keyword << '\n';
}

//Book---------------------------------------------------------
bool Book::operator>(const Book &right) const {
    return bookISBN > right.bookISBN;
}

bool Book::operator==(const Book &right) const {
    return bookISBN == right.bookISBN;
}

bool Book::operator>=(const Book &right) const {
    return bookISBN >= right.bookISBN;
}

Book &Book::operator=(const std::pair<Book, bool> &pair) {
    *this = pair.first;
    return *this;
}

ISBN Book::GetKey(ISBN isbn) const {
    return bookISBN;
}

Name_ISBN Book::GetKey(Name name1) const {
    Name_ISBN tmp;
    tmp.name = name;
    tmp.bookISBN = bookISBN;
    return tmp;
}

Author_ISBN Book::GetKey(Author author1) const {
    Author_ISBN tmp;
    tmp.author = author;
    tmp.bookISBN = bookISBN;
    return tmp;
}

void Book::Print() {
    std::string s_bookISBN = bookISBN.bookISBN, s_name = name.name, s_author = author.author, s_keywords = keywords;
    std::cout << s_bookISBN << '\t' << s_name << '\t' << s_author << '\t' << s_keywords << '\t'
              << price << '\t' << quantity << '\n';
}

ISBN Book::GetIndex(ISBN isbn) const {
    return bookISBN;
}

Name Book::GetIndex(Name name1) const {
    return name;
}

Author Book::GetIndex(Author author1) const {
    return author;
}

//BookLocation-----------------------------------------
//
//bool BookLocation::operator>(const BookLocation &right) const {
//    return bookISBN > right.bookISBN;
//}
//
//bool BookLocation::operator==(const BookLocation &right) const {
//    return bookISBN == right.bookISBN;
//}
//
//bool BookLocation::operator>=(const BookLocation &right) const {
//    return bookISBN >= right.bookISBN;
//}
//
//BookLocation &BookLocation::operator=(const std::pair<BookLocation, bool> &pair) {
//    *this = pair.first;
//    return *this;
//}
//
//Keyword_ISBN BookLocation::GetKey(Keyword keywordIsbn) const {
//    Keyword_ISBN tmp;
//    tmp.keyword = keyword;
//    tmp.bookISBN = bookISBN;
//    return tmp;
//}
//
//Keyword BookLocation::GetIndex(Keyword) const {
//    return keyword;
//}
//
//void BookLocation::Print() {
//    std::string s_bookISBN = bookISBN.bookISBN, s_keywords = keyword.keyword;
//    std::cout << s_bookISBN << '\t' << s_keywords << '\n';
//}

//BookManager-----------------------------------

void BookManager::Show(TokenScanner &tokenScanner) {
    //无附加 遍历bookList

//    std::cout << "BEFOREEEEE    SHOWWWWWWWWWWWW: \n";
//    PRINT();
//    std::cout << "SHOWWWWWWWWWWWW: \n\n";

    if (!tokenScanner.HasMoreTokens()) {
        bookList.PrintList();
        return;
    }
    bool success = false;
    std::string type;
    tokenScanner.TakeType(type);
    char token[61];
    memset(token, 0, sizeof(token));
    long iter;
    if (type == "ISBN") {
        tokenScanner.NextToken(token);
        ISBN isbn(token);
        std::pair<Book, bool> pair = bookList.Find(isbn, iter);
        if (pair.second) pair.first.Print();
        else std::cout << '\n';
        success = true;
    }
    if (type == "name") {
        tokenScanner.Quote(token);
        Name index(token);
        nameList.FindPrint(index);
        success = true;
    }
    if (type == "author") {

        tokenScanner.Quote(token);
        Author index(token);
        authorList.FindPrint(index);
        success = true;
    }
    if (type == "keyword") {

//        std::cout << "KEEEEEEEEEEEEEEEEEEEEEEY: \n";
//        bookList.PrintList();
//keywordList.PrintList();
//        std::cout << "KEEEEEEEEEEEEEEEEEEEEEEY: \n\n";
        tokenScanner.Quote(token);
        //构造时判断token是否合法
        Keyword index(token);
        //keyword对应bookLocation集合
        std::vector<Keyword_ISBN> bookLocationGroup = keywordList.FindSubList(index);
        long bookIter;
        if (bookLocationGroup.empty()) std::cout << '\n';
        else {
            //遍历vector 找书 Print
            for (auto &i: bookLocationGroup) {
                long iter1;
                std::pair<Book, bool> pair = bookList.Find(i.bookISBN, iter1);
                pair.first.Print();
            }
        }
        success = true;
    }
//    std::cout << "AFTER    SHOWWWWWWWWWWWW: \n";
//    PRINT();
//    std::cout << "SHOWWWWWWWWWWWW: \n\n";

    if (!success) error("Invalid");
    if (tokenScanner.HasMoreTokens()) error("Invalid");
}

double BookManager::Buy(TokenScanner &tokenScanner) {
    char bookISBN[21];
    memset(bookISBN, 0, sizeof(bookISBN));
    int quantity;
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(bookISBN);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(quantity);
    else error("Invalid");
    if (quantity == 0) error("Invalid");
    if (tokenScanner.HasMoreTokens()) error("Invalid");
    double sum = 0;
    long iter;
    std::pair<Book, bool> pair = bookList.Find(bookISBN, iter);
    if (!pair.second) error("Invalid");//不存在
    Book buyBook = pair.first;
    //库存充足？
    if (buyBook.quantity < quantity) error("Invalid");
    sum = 1.0 * quantity * buyBook.price;
    std::cout << sum << '\n';
    //减少库存
    buyBook.quantity -= quantity;
    //写回文件（多个文件都要修改）
    Update(buyBook, iter);
    return sum;
}

void BookManager::Update(Book book, long iter) {
    //覆盖bookList中原有信息
    bookList.WriteValue(book, iter);
    //nameList
    Name_ISBN nameIbsn;
    nameIbsn.name = book.name;
    nameIbsn.bookISBN = book.bookISBN;
    RewriteName(book,nameIbsn);
//    nameList.Find(nameIbsn, iter);
//    nameList.WriteValue(book, iter);
    //authorList
    Author_ISBN authorIbsn;
    authorIbsn.author = book.author;
    authorIbsn.bookISBN = book.bookISBN;
    RewriteAuthor(book,authorIbsn);
//    authorList.Find(authorIbsn, iter);
//    authorList.WriteValue(book, iter);
}

ISBN BookManager::Select(TokenScanner &tokenScanner) {
    char isbn[21];
    memset(isbn, 0, sizeof(isbn));
    long iter;
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(isbn);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens())error("Invalid");
    ISBN bookISBN(isbn);
    //不存在 新建
    if (!bookList.Find(bookISBN, iter).second)
        iter = AddBook(bookISBN);

//    std::cout<<"SELE: \n";
//    bookList.PrintList();
//    std::cout<<"SELE: \n\n";

    return bookISBN;
}

long BookManager::AddBook(ISBN isbn) {
    Book newBook;
    newBook.bookISBN = isbn;
    return bookList.Insert(isbn, newBook);
}
void BookManager::GetBook(Book &book,long iter) {
    bookList.ReadValue(book,iter);
}

void BookManager::GetBook(Book &book,ISBN isbn, long &iter) {
    std::pair<Book, bool> pair = bookList.Find(isbn, iter);
    if (!pair.second) error("Invalid");
    book=pair.first;
}


bool BookManager::Modify(TokenScanner &tokenScanner, Book book, long foreIter, ISBN &isbn) {
    if (!tokenScanner.HasMoreTokens()) error("Invalid");

//    std::cout << "\nBEFORE";
//    PRINT();

    Book modify = book;//修改后信息
    std::string type, str;
    char token[61];
    memset(token, 0, sizeof(token));
    std::vector<std::string> keywordGroup;
    //是否已经有该类修改
    bool change_ISBN = false;
    bool change_name = false;
    bool change_author = false;
    bool change_keyword = false;
    bool change_price = false;
    //重新插入
    bool reinsert_ISBN_flag = false;
    bool reinsert_name_flag = false;
    bool reinsert_author_flag = false;
    bool reinsert_keywords_flag = false;
    //改写
    bool rewrite_ISBN_flag = false;
    bool rewrite_name_flag = false;
    bool rewrite_author_flag = false;
    //收集modify信息
    while (tokenScanner.HasMoreTokens()) {
        //修改信息类型
        tokenScanner.TakeType(type);
        if (type == "ISBN") {
            if (change_ISBN) error("Invalid");
            tokenScanner.NextToken(token);
            ISBN newISBN(token);
            //不可重复
            long iter;
            if (bookList.Find(newISBN, iter).second)error("Invalid");
//            if (newISBN == book.bookISBN)
            modify.bookISBN = newISBN;
            //设置已修改
            change_ISBN = true;
            isbn = newISBN;
            //设置flag
            reinsert_ISBN_flag = true;
            reinsert_name_flag = true;
            reinsert_author_flag = true;
            reinsert_keywords_flag = true;
        }
        if (type == "name") {
            if (change_name) error("Invalid");
            tokenScanner.Quote(token);
            Name newName(token);
            modify.name = newName;
            change_name = true;
            reinsert_name_flag = true;
            rewrite_ISBN_flag = true;
            rewrite_author_flag = true;
        }
        if (type == "author") {
            if (change_author)error("Invalid");
            tokenScanner.Quote(token);
            Author newAuthor(token);
            modify.author = newAuthor;
            change_author = true;
            reinsert_author_flag = true;
            rewrite_ISBN_flag = true;
            rewrite_name_flag = true;
        }
        //keywords整段 在修改keyword文件时切片
        if (type == "keyword") {
            if (change_keyword) error("Invalid");
            tokenScanner.Quote(token);
            //keywords过长
            if (strlen(token) > 60) error("Invalid");
            str = token;
            //keywords切片查重
            CutKeywords(str, keywordGroup);
            strcpy(modify.keywords, token);
            change_keyword = true;
            rewrite_ISBN_flag = true;
            rewrite_name_flag = true;
            rewrite_author_flag = true;
            reinsert_keywords_flag = true;
        }
        if (type == "price") {
            if (change_price)error("Invalid");
            tokenScanner.NextToken(modify.price);
            change_price = true;
            rewrite_ISBN_flag = true;
            rewrite_name_flag = true;
            rewrite_author_flag = true;
        }
//
//        std::cout<<"BEFORE    MODYFYYYYYYYYYY: \n";
//        bookList.PrintList();
//        std::cout<<"MODIFYYYYYYYYYY: \n\n";

    }
    //原书
//    Book selected = bookList.ReadValue(pair.second);
    long iterISBN = foreIter;//book_information中位置
    // 重插或重写
    if (reinsert_ISBN_flag) {
        ReinsertISBN(modify, book.bookISBN);
    } else {
        if (rewrite_ISBN_flag) RewriteISBN(modify, foreIter);
    }
    Name_ISBN nameKey;
    nameKey.name = book.name;
    nameKey.bookISBN = book.bookISBN;
    if (reinsert_name_flag) {
        ReinsertName(modify, nameKey);
    } else {
        if (rewrite_name_flag) RewriteName(modify, nameKey);
    }
    Author_ISBN authorKey;
    authorKey.author = book.author;
    authorKey.bookISBN = book.bookISBN;
    if (reinsert_author_flag) {
        ReinsertAuthor(modify, authorKey);
    } else {
        if (rewrite_author_flag) RewriteAuthor(modify, authorKey);
    }
    char *foreKeywords = book.keywords;
    if (reinsert_keywords_flag) ReinsertKeyword(foreKeywords, book.bookISBN, book.bookISBN, keywordGroup);

//    std::cout << "AFTER    MODYFYYYYYYYYYY: \n";
//    keywordList.PrintList();
//    PRINT();
//    std::cout << "MODIFYYYYYYYYYY: \n\n";


    return change_ISBN;
}

void BookManager::CutKeywords(std::string str, std::vector<std::string> &keywordGroup) {
    if (str.empty()) return;
    int start = 0;
    std::string keyword;
    int end = 0;
    while (true) {
        while (str[end] != '|' && end < str.length()) {
            ++end;
        }
        keyword = str.substr(start, end - start);
        if (std::find(keywordGroup.begin(), keywordGroup.end(), keyword) != keywordGroup.end())
            error("Invalid");
        keywordGroup.push_back(keyword);
        if (end >= str.length()) break;
        ++end;
        start = end;
    }
}

void BookManager::ReinsertISBN(const Book &book, ISBN foreISBN) {
    bookList.Delete(foreISBN);//优化空间 直接由foreIter
    bookList.Insert(book.bookISBN, book);
    long iter = 0;
    bookList.Find(book.bookISBN, iter);//优化空间 直接从insert获取？
}

void BookManager::ReinsertName(const Book &book, Name_ISBN key) {
    nameList.Delete(key);
    key.name = book.name;
    key.bookISBN = book.bookISBN;
    nameList.Insert(key, book);
}

void BookManager::ReinsertAuthor(const Book &book, Author_ISBN key) {
    authorList.Delete(key);
    key.author = book.author;
    key.bookISBN = book.bookISBN;
    authorList.Insert(key, book);
}

void BookManager::ReinsertKeyword(const char *foreKeywords,
                                  ISBN foreISBN, ISBN isbn,
                                  std::vector<std::string> keywordGroup) {
    //根据foreKeywords删去原有的所有keyword
    //切出keyword
    std::string str = foreKeywords;
    std::vector<std::string> foreKeywordGroup;
    CutKeywords(str, foreKeywordGroup);
    Keyword keyword;
    Keyword_ISBN key;
    //循环删去
    while (!foreKeywordGroup.empty()) {
        keyword = MakeKeyword(foreKeywordGroup.back());
        foreKeywordGroup.pop_back();
        //makeKey
        key.keyword = keyword;
        key.bookISBN = foreISBN;
        //delete
        keywordList.Delete(key);
    }
    //添加新的keyword
    while (!keywordGroup.empty()) {
        keyword = MakeKeyword(keywordGroup.back());
        keywordGroup.pop_back();
        key.keyword = keyword;
        key.bookISBN = isbn;
        keywordList.Insert(key, key);
    }
}

Keyword BookManager::MakeKeyword(const std::string &string) {
    const char *tmp = string.c_str();
    Keyword keyword;
    strcpy(keyword.keyword, tmp);
    return keyword;
}

void BookManager::RewriteISBN(const Book &book, const long &foreIter) {
    bookList.WriteValue(book, foreIter);
}

void BookManager::RewriteName(const Book &book, const Name_ISBN &key) {
    long iter;
    std::pair<Book, bool>pair=nameList.Find(key, iter);
    if(pair.second) nameList.WriteValue(book, iter);
}

void BookManager::RewriteAuthor(const Book &book, const Author_ISBN &key) {
    long iter;
    std::pair<Book, bool>pair=authorList.Find(key, iter);
    if(pair.second) authorList.WriteValue(book, iter);
}

double BookManager::Import(TokenScanner &tokenScanner, ISBN isbn) {
    int quantity;
    double totalCast;
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(quantity);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) tokenScanner.NextToken(totalCast);
    else error("Invalid");
    if (tokenScanner.HasMoreTokens()) error("Invalid");
    if (quantity == 0 || totalCast == 0) error("Invalid");
    //图书信息
    long iter;
    std::pair<Book, bool> pair = bookList.Find(isbn, iter);
    if (!pair.second) error("Invalid");

//    PRINT();

    Book book = pair.first;
    book.quantity += quantity;
    //重写
    RewriteISBN(book, iter);
    Name_ISBN key1;
    key1.name = book.name;
    key1.bookISBN = book.bookISBN;
    RewriteName(book, key1);
    Author_ISBN key2;
    key2.author = book.author;
    key2.bookISBN = book.bookISBN;
    RewriteAuthor(book, key2);
//    PRINT();
    return totalCast;
}
