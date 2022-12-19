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
bool Name_IBSN::operator>(const Name_IBSN &id) const {
    return false;
}

bool Name_IBSN::operator==(const Name_IBSN &id) const {
    return false;
}

bool Name_IBSN::operator>=(const Name_IBSN &id) const {
    return false;
}

Name_IBSN &Name_IBSN::operator=(const Name_IBSN &id) =default;

Name Name_IBSN::GetIndex() {
    return name;
}
