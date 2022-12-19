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
bool Name_IBSN::operator>(const Name_IBSN &right) const {
    if (name > right.name || (name == right.name && bookISBN > right.bookISBN))return true;
    else return false;
}

//ISBN唯一
bool Name_IBSN::operator==(const Name_IBSN &right) const {
    return bookISBN == right.bookISBN;
}

bool Name_IBSN::operator>=(const Name_IBSN &right) const {
    return !(right > *this);
}

Name_IBSN &Name_IBSN::operator=(const Name_IBSN &right) = default;

Name Name_IBSN::GetIndex() const {
    return name;
}

//Author_IBSN--------------------------------------------------

bool Author_IBSN::operator>(const Author_IBSN &right) const {
    if (author > right.author || (author == right.author && bookISBN > right.bookISBN))return true;
    else return false;
}

bool Author_IBSN::operator==(const Author_IBSN &right) const {
    return bookISBN == right.bookISBN;
}

bool Author_IBSN::operator>=(const Author_IBSN &right) const {
    return !(right > *this);
}

Author_IBSN &Author_IBSN::operator=(const Author_IBSN &right) = default;

Author Author_IBSN::GetIndex() const {
    return author;
}

//Keyword_ISBN--------------------------------------------------

bool Keyword_ISBN::operator>(const Keyword_ISBN &right) const {
    if (keyword > right.keyword || (keyword == right.keyword && bookISBN > right.bookISBN))return true;
    else return false;
}

bool Keyword_ISBN::operator==(const Keyword_ISBN &right) const {
    return bookISBN == right.bookISBN;
}

bool Keyword_ISBN::operator>=(const Keyword_ISBN &right) const {
    return !(right > *this);
}

Keyword_ISBN &Keyword_ISBN::operator=(const Keyword_ISBN &right) = default;

Keyword Keyword_ISBN::GetIndex() const {
    return keyword;
}

//Book---------------------------------------------------------
bool Book::operator>(const Book &right) const {
    return bookISBN>right.bookISBN;
}

bool Book::operator==(const Book &right) const {
    return bookISBN==right.bookISBN;
}

bool Book::operator>=(const Book &right) const {
    return bookISBN>=right.bookISBN;
}

Name_IBSN Book::GetKey(Name name1) const {
    Name_IBSN tmp;
    tmp.name=name;
    tmp.bookISBN=bookISBN;
    return tmp;
}

Author_IBSN Book::GetKey(Author author1) const {
    Author_IBSN tmp;
    tmp.author=author;
    tmp.bookISBN=bookISBN;
    return tmp;
}

Keyword_ISBN Book::GetKey(Keyword keyword1) {
    Keyword_ISBN tmp;
    tmp.keyword=
}


