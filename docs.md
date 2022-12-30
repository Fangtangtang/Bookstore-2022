# 书店管理系统开发文档

作者：房诗涵

## 1. 程序功能概述

一个书店管理系统，允许用命令行以合法指令交互。


系统包括：登录状态系统、账户系统、图书系统、日志系统。

程序首次运行时自主执行所需的初始化操作：创建帐户名为 `root`，密码为 `sjtu`，权限为` {7}` 的超级管理员帐户。

程序启动后，根据输入数据执行对应操作，直至读入 EOF 或根据相关指令要求终止运行。



### 1.1 交互方式

各个系统通过系统对应的合法指令操作。

输入数据以换行符或回车符为分隔为若干指令。

单个合法指令由ASCII 字符、空格（多个连续空格效果与一个空格等价；行首行末允许出现多余空格；如无特殊说明禁止省略或另增空格）构成。

指令中第一个部分必须为指令关键词，指令中关键词部分必须与指令格式完全匹配。

在用户输入一条指令后，如果合法则执行对应操作，如果有则输出操作结果；如果指令非法或操作失败则输出`Invalid `。仅有空格的指令合法且无输出内容。

除非有特殊说明，如果输入指令对应的输出内容非空，则结尾有 `\n ` 字符；输出内容为空则不输出任何字符。



合法指令要求：

- `[x]` 表示一串有特定限制的用户自定义字符串；
- `(a|b|c) `表示此处仅能出现`a`,`b`,`c`中其一；
-  `(x)?` 表示此处可以出现零次或一次 `x`；
- `(x)+` 表示此处可以出现一次或多次 `x`。



指令包括：

- 基础指令
  `quit`正常退出系统
  `exit` 正常退出系统



- 帐户系统指令
  `su [UserID] ([Password])?`
  `logout`
  `register [UserID] [Password] [Username]`
  `passwd [UserID] ([CurrentPassword])? [NewPassword]`
  `useradd [UserID] [Password] [Privilege] [Username]`
  `delete [UserID]`



- 图书系统指令
  `show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?`
  `buy [ISBN] [Quantity]`
  `select [ISBN]`
  `modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+`
  `import [Quantity] [TotalCost]`



- 日志系统指令
`show finance ([Count])?`
`log`



### 1.2 系统功能
#### 1.2.1 登录状态系统
##### 1.2.1.1 相关说明
系统支持嵌套登录，即允许多个帐户同时处于登录状态，允许同一帐户同时多次登录。

输入的指令视为最后登录的帐户操作。

退出系统视为登出所有已登录帐户。



##### 1.2.1.2 操作
注：登录状态系统中与其他系统关联大的操作将在其他系统中阐释。



- **登录账户：**使登录帐户变为已登录状态，当前帐户变为该帐户。
  - 如果该帐户不存在则操作失败。
  - 密码错误则操作失败。
  - 如果当前帐户权限等级高于登录帐户则可以省略密码。
  
  


- **注销账户：**撤销最后一次成功执行的`su` 指令效果。
  
  - 如无已登录帐户则操作失败。
  
  


- **选择图书：**以当前帐户选中指定图书（帐户登出后无需保存选中图书情况）。
  - 没有符合条件的图书则创建仅拥有 [ISBN] 信息的新图书。
  - 退出系统视为取消选中图书。




#### 1.2.2 账户系统

书店管理系统服务于 店主（超级管理员），员工（管理员）以及顾客（用户）。

为满足其不同需求，故需帐户系统管理帐户及权限数据，并提供相应功能。



##### 1.2.2.1 相关说明

**权限**

帐户的权限等级有 {7}, {3} 和 {1}；未登录任何帐户的状态下当前帐户权限视为 {0}。

高权限的帐户可以执行需要低权限的指令，反之则不可。

- **店主{7}：**使用 `root` 帐户，可以访问书店系统所有功能，包括日志和帐户管理功能


- **员工{3}：**可以访问修改图书数据相关功能等


- **顾客{1}：**可以注册帐户，查询、购买图书


- **游客{0}：**可以注册帐户



**账户信息**

- **账户名：**除不可见字符以外 ASCII 字符组成；长度不超过30。
- **账户ID：**数字，字母，下划线组成；长度不超过30。
- **权限等级：**长度为1数字。
- **密码：**数字，字母，下划线组成；长度不超过30。



##### 1.2.2.2 操作
- **注册账户：**注册信息如指令所示，权限等级为 {1} 的帐户。
  
  - 如果 `[UserID]`与已注册帐户重复则操作失败。
  
  


- **修改密码：**修改指定帐户的密码。
  - 帐户不存在则操作失败。
  - `[CurrentPassword]`错误则操作失败。
  - 如果当前帐户权限等级为 {7} 则可以省略` [CurrentPassword]`。
  
  


- **创建账户：**创建信息如指令所示的帐户。
  
  - 如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败。
  - 如果 `[UserID]`与已注册帐户重复则操作失败。
  
  


- **删除账户：**删除指定帐户。
  - 如果待删除帐户不存在则操作失败。
  - 如果待删除帐户已登录则操作失败。
  
  


#### 1.2.3图书系统
图书系统提供图书信息、库存信息和图书交易财务记录的相关服务及数据存储功能。

##### 1.2.3.1相关说明

**图书信息**


- **ISBN：**除不可见字符以外 ASCII 字符，长度不超过20。
  - 本系统中任何两本图书不应有相同 [ISBN] 信息。
- **BookName：**除不可见字符和英文双引号以外 ASCII 字符，长度不超过60。
- **Author：**除不可见字符和英文双引号以外 ASCII 字符，长度不超过60。
- **Keyword：**除不可见字符和英文双引号以外 ASCII 字符，长度不超过60。
  - [keyword] 内容以 | 为分隔可以出现多段信息，每段信息长度至少为 1。
- **Quantity：**数字，长度不超过10，数值不超过2'147'483'647。
- **Price：**数字和 `.`，长度不超过13。
- **TotalCast：**数字和 `.`，长度不超过13。



##### 1.2.3.2 操作

- **检索图书：**以 `[ISBN]`字典升序依次输出满足要求的图书信息。
  - 无满足要求的图书时输出空行。
  - 无附加参数时，所有图书均满足要求。
  - 附加参数内容为空、`[Keyword] `中出现多个关键词则操作失败。
  - 每个图书信息输出格式为`[ISBN]\t[BookName]\t[Author]\t[Keyword]\t[Price]\t[库存数量]\n`
  
  


- **购买图书：**购买指定数量的指定图书，减少库存，以浮点数输出购买图书所需的总金额。
  
  - 没有符合条件的图书、购买数量为非正整数则操作失败。
  
  


- **选择图书：**以当前帐户选中指定图书（帐户登出后无需保存选中图书情况）。
  - 没有符合条件的图书则创建仅拥有 [ISBN] 信息的新图书。
  - 退出系统视为取消选中图书。
  
  


- **修改图书信息：**以指令中的信息更新选中图书的信息。
  - 如未选中图书、附加参数内容为空，操作失败。
  - `[keyword]` 包含重复信息段则操作失败。
  - 有重复附加参数为非法指令。
  - 不允许将 ISBN 改为原有的 ISBN。
  
  


- **图书进货：**以指定交易总额购入指定数量的选中图书。
  - 如未选中图书，操作失败。
  - 购入数量为非正整数，操作失败。
  - 交易总额为非正数，操作失败。




#### 1.2.4 日志系统

日志系统负责记录书店管理系统运行过程中的各种数据，提供各类日志信息查询服务




##### 1.2.4.1 相关说明

**日志信息**
- **Count：**交易笔数。数字，长度不超过10，数值不超过2'147'483'647。
- **Recode：**一条操作记录。含记录编号、操作者ID、操作内容。
- **Transaction：**一条交易记录。含交易记录编号、金额、收支标记。




##### 1.2.4.2 操作

- **财务记录查询：**输出最后完成的指定笔数交易总额。
  - 无` Count` 参数时，输出所有交易之总额。不存在交易时认为收入支出均为 `0.00`。
  - `Count` 为 0 时输出空行。
  - `Count` 大于历史交易总笔数时操作失败。
  - 格式为` + [收入] - [支出]\n`
  
  


- **生成日志：**生成日志记录。
  - 内容包括系统操作者ID，操作行为。
  - 若操作失败显示`FAILED`。




## 2. 主体逻辑说明
- 在main函数中定义接入各个系统的对象、设置浮点数输出精度、异常处理。


- 在main函数中通过循环读入指令，main不进行具体指令执行，但在main函数中记录操作日志。

- 利用tokenscanner读入指令，由不同类的对象进行具体操作。若指令非法，抛出异常。


- 通过文件读写来实现数据的输入输出。




## 3. 代码文件结构

**主函数:**
`main.cpp`



**类：**

- 账户类：`account.h` / `account.cpp`
- 登录状态类：`loggingStatus.h` / `loggingStatus.cpp`
- 图书类：`book.h` / `book.cpp`
- 日志类：`log.h` / `log.cpp`
- 块状链表类（模板）：`linkList.h` 
- 指令扫描解析类：`tokenscanner.h`/`tokenscanner.cpp`
- 异常处理类：`error.h`/`error.cpp`



**文件：**
- 账户信息相关文件
- 图书信息相关文件
- 日志文件：财务日志文件、操作日志文件




## 4. 各个类的接口及成员说明

### 4.1 账户系统
负责账户信息相关的维护管理。
#### 4.1.1 权限
```c++
//权限 符号常量 对应 {无} {0} {1} {3} {7}
enum Privilege{ none, visitor, customer, clerk, host};
```
#### 4.1.2 用户ID
作为检索用户的index和key。
```c++
//key:ID
struct ID {

    char userID[31]={'\0'};

    ID();

    ID(char* id);

    ~ID();

    bool operator>(const ID &id) const;

    bool operator<(const ID &id) const;

    bool operator==(const ID &id) const;

    bool operator>=(const ID &id) const;

    ID &operator=(const ID &id);

    ID GetIndex(const ID&);

};
```


#### 4.1.3 当前操作账户

提供登录栈当前操作者的ID、权限信息。
```c++
struct CurrentAccount{

    ID userID;
    
    Privilege privilege=none;
    
};
```


#### 4.1.4 账户

一个账户的全部信息，作为检索和记录中的value。
```c++
struct Account {

    Privilege privilege=none;

    ID userID;

    char name[31]={'\0'};

    char password[31]={'\0'};

    bool operator>(const Account &id) const;

    bool operator==(const Account &id) const;

    bool operator>=(const Account &id) const;

    Account &operator=(const std::pair<Account, bool>& pair);

    //获取key
    ID GetKey(ID id) const ;
    
};
```


#### 4.1.5 账户管理

接受指令，执行指令。
```c++
class AccountManager{
public:

    AccountManager()=default;

    ~AccountManager()=default;

    //注册账户 {0} register [UserID] [Password] [Username]
    //游客注册 权限为{1} ID不可和已有的重复
    void Register(TokenScanner &tokenScanner);

    //创建初始用户
    void InitialAccount();

    //创建帐户 {3} useradd [UserID] [Password] [Privilege] [Username]
    //高权限用户创建低权限账号
    void AddUser(TokenScanner &tokenScanner,Privilege &privilege);

    //删除账户 {7} delete [UserID]
    void DeleteUser(ID id);

    //修改密码 {1} passwd [UserID] ([CurrentPassword])? [NewPassword]
    void ChangePassword(TokenScanner &tokenScanner,CurrentAccount &user);

    //查找帐户
    std::pair<Account,bool> FindAccount(char* UserID);

private:

    //储存账户信息的块状链表
    LinkList<ID,ID,Account> accountList{"account_information"};

    //修改密码
    void Modify(Account account,long iter,char* newPassword);

    //int转privilege
    static Privilege toPrivilege(int i);
    
    //创建新账户
    void CreateAccount(char* UserID, char* Password,Privilege privilege,char* Username);

};

```



### 4.2 登录状态系统

维护登录状态



#### 4.2.1 登录状态

```c++
class LoggingStatus{
public:

    //登录
    void Su(TokenScanner &tokenScanner,CurrentAccount &user,AccountManager &accountManager);

    //登出
    void Logout(TokenScanner &tokenScanner,CurrentAccount &user);

    //记录用户和选中的图书
    void SelectBook(ISBN);

    //寻找当前选中的书
    ISBN FindSelected();

    //修改了图书中的ISBN
    void ChangeISBN(ISBN isbn,ISBN newISBN);

    bool Find(const ID& id);

private:

    //登录用户ID栈
    std::vector<ID> IDVector;

    //登录用户权限栈
    std::vector<Privilege> privilegeVector;

    //ID->selectedBook
    std::vector<ISBN> bookVector;

    //返回当前账户信息
    CurrentAccount Flush();
    
};
```



### 4.3 图书系统

#### 4.3.1 ISBN

图书检索中的index或key。
```c++
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
```


#### 4.3.2 书名

```c++
struct Name {

    char name[61]={'\0'};

    Name();

    explicit Name(char* name1);

    ~Name()=default;

    bool operator>(const Name &name1) const;

    bool operator==(const Name &name1) const;

    bool operator>=(const Name &name1) const;

    Name &operator=(const Name &name1);

};
```


#### 4.3.3 作者

```c++
struct Author {

    char author[61]={'\0'};

    Author();

    explicit Author(char* author1);

    ~Author()=default;

    bool operator>(const Author &author1) const;

    bool operator==(const Author &author1) const;

    bool operator>=(const Author &author1) const;

    Author &operator=(const Author &author1);

};
```


#### 4.3.4 关键词

一个关键词，作为图书检索中的index。
```c++
struct Keyword{

    char keyword[61]={'\0'};

    Keyword();

    explicit Keyword(char* keyword1);

    explicit Keyword(const std::string& keyword1);

    ~Keyword()=default;

    bool operator>(const Keyword &keyword1) const;

    bool operator==(const Keyword &keyword1) const;

    bool operator>=(const Keyword &keyword1) const;

    Keyword &operator=(const Keyword &keyword1);
    
};
```


#### 4.3.5 书名+ISBN

以name优先排序的key。
```c++
struct Name_ISBN{

    Name name;

    ISBN bookISBN;

    bool operator>(const Name_ISBN &right) const;

    bool operator==(const Name_ISBN &right) const;

    bool operator>=(const Name_ISBN &right) const;

    Name_ISBN &operator=(const Name_ISBN &right);

    Name GetIndex(Name) const;

};
```


#### 4.3.6 作者+ISBN

以author优先排序的key。
```c++
struct Author_ISBN{

    Author author;

    ISBN bookISBN;

    bool operator>(const Author_ISBN &right) const;

    bool operator==(const Author_ISBN &right) const;

    bool operator>=(const Author_ISBN &right) const;

    Author_ISBN &operator=(const Author_ISBN &right);

    Author GetIndex(Author) const;
    
};
```


#### 4.3.7 关键词+ISBN

以keyword(单个)优先排序的key
```c++
struct Keyword_ISBN{

    Keyword keyword;

    ISBN bookISBN;

    bool operator>(const Keyword_ISBN &right) const;

    bool operator==(const Keyword_ISBN &right) const;

    bool operator>=(const Keyword_ISBN &right) const;

    Keyword_ISBN &operator=(const Keyword_ISBN &right);

    Keyword GetIndex(Keyword) const;

    Keyword_ISBN GetKey(Keyword) const;

    void Print();
    
};
```


#### 4.3.8 图书

```c++
struct Book {

    ISBN bookISBN;

    Name name;

    Author author;

    //基础信息中存完整的keywords
    char keywords[61]={'\0'};

    int quantity=0;

    double price=0;

    Book &operator=(const std::pair<Book, bool>& pair);

    ISBN GetIndex(ISBN isbn) const;

    Name GetIndex(Name name1) const;

    Author GetIndex(Author author1) const;

    ISBN GetKey(ISBN isbn) const;

    //获取name优先排序的key 用Book中的信息生成对应的key
    Name_ISBN GetKey(Name name1) const;

    Author_ISBN GetKey(Author author1) const;

    //打印book所有信息
    void Print();

};
```


#### 图书管理

接受指令，执行指令。
```c++
class BookManager{
public:

    BookManager()=default;

    ~BookManager()=default;

    //检索图书
    //show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
    //无参数 直接打表
    void Show(TokenScanner &tokenScanner);

    //购买图书 {1} buy [ISBN] [Quantity]
    //减少库存 返回总价
    double Buy(TokenScanner &tokenScanner);

    //选择图书 {3} select [ISBN]
    //返回选中图书的ISBN
    ISBN Select(TokenScanner &tokenScanner);

    //修改当前选中图书信息
    bool Modify(TokenScanner &tokenScanner,Book book,long iter,ISBN &isbn);

    //图书进货{3} import [Quantity] [TotalCost]
    double Import(TokenScanner &tokenScanner,ISBN isbn);

    void GetBook(Book &book,long iter);

     void GetBook(Book &book,ISBN isbn,long &iter);

private:

    //ISBN->图书信息
    LinkList<ISBN,ISBN,Book> bookList{"book_information"};

    //name排序
    LinkList<Name,Name_ISBN,Book> nameList{"book_name"};

    //author排序
    LinkList<Author,Author_ISBN,Book> authorList{"book_author"};

    //单个keyword排序
    //index:Keyword key:Keyword_ISBN value:Keyword_ISBN
    LinkList<Keyword,Keyword_ISBN,Keyword_ISBN> keywordList{"book_keyword"};

    //更新信息
    //传入iter为boolList中地址
    void Update(Book book,long iter);

    //创建仅拥有ISBN的book
    long AddBook(ISBN isbn);

    //将keywords切片为string型 查重
    static void CutKeywords(std::string str,std::vector<std::string>&keywordGroup);

    //重新插入ISBN
    //book:新信息 foreIter:原地址 iter:新地址
    void ReinsertISBN(const Book &book,ISBN foreISBN);

    //重新插入name
    void ReinsertName(const Book &book,Name_ISBN key);

    //重新插入author
    void ReinsertAuthor(const Book &book,Author_ISBN key);

    //重新插入keyword
    //iter!=0 book新位置
    void ReinsertKeyword(const char *foreKeywords,ISBN foreISBN,ISBN isbn,std::vector<std::string>keywordGroup);

    //重写ISBN
    void RewriteISBN(const Book &book,const long &foreIter);

    //重写name
    void RewriteName(const Book &book,const Name_ISBN &key);
    
    //重写author
    void RewriteAuthor(const Book &book,const Author_ISBN &key);

    //将string型keyword转为Keyword
    static Keyword MakeKeyword(const std::string &string);

};

```



### 4.4 日志系统

#### 4.4.1 操作记录
```c++
struct Record{

    int num;

    ID userID;

    char operation[201]={'\0'};

    Record()=default;

    Record(int count,ID id,std::string str);

    ~Record()=default;

    int GetKey(int) const;

    void Print();
    
};
```


#### 4.4.2 交易记录

```c++
//一条交易记录
struct Transaction{

    //true 收入
    int num=0;

    bool income_flag= false;

    double amount=0;

    int GetKey(int) const;

};
```


#### 4.4.3 交易记录管理

```c++
class TransactionManager{
public:

    //从文件中读入数据
    TransactionManager();

    //将新的count和sum更新到headNode
    ~TransactionManager();

    //初始化
    void InitialTransation();

    //收入记录 （buy）
    void Income(double price);

    //支出记录 （import)
    void Expense(double cast);

    //财务记录查询 {7} show finance ([Count])?
    void ShowFinance(TokenScanner &tokenScanner);

private:

    //历史交易总数
    //在文件headnode中记录交易总数，构造时读入，析构时写回文件
    int financeCount=0;

    //历史交易总额
    //在文件headnode中记录，构造时读入，析构时写回文件
    double income=0;
    
    double expense=0;

    //顺序插入 int为历史插入条数
    LinkList<int,int,Transaction> transactionList{"finance_log"};
    
};
```


#### 4.4.4 日志管理

```c++
class LogManager{
public:

    LogManager();

    ~LogManager();

    void InitialLog();

    void PrintLog();

    void AddLog(const ID& id,const std::string &str);

private:

    //历史操作总数
    int count=0;

    //int为recode对应的count
    LinkList<int,int,Record> logList{"log"};
    
};


```



### 4.5 块状链表

辅助信息存取,用于账户信息、图书信息的维护。



#### 4.5.1 头节点
信息储存块,储存首个blocknode位置、系统中的基础信息。
```c++
struct HeadNode {
    long next = 0;
    int count = 0;
    double income = 0;
    double expense = 0;
};
```


#### 4.5.2 LinkList类 模板

index 检索索引 ；key 排序标记 ；value 数组元素
```c++
template<class index, class Key, class Value>
class LinkList {

    static constexpr int blockSize = 1024;
    
public:

    LinkList(const std::string &file_name);

    ~LinkList();

    //打表
    void PrintList();

    //插入value
    long Insert(Key key, Value ele);
    
    //Delete
    bool Delete(Key key); 

    //以index寻找value并打印
    void FindPrint(const index &index1);
    
    //以index寻找value,返回value数组
    std::vector<Value> FindSubList(const index &index1); 

    //基于key寻找value (iter是value地址)
    std::pair<Value, bool> Find(const Key &key, long &iter);
               
    //遍历末count个，返回vector
    std::vector<Value> Traverse(Key startKey);

    //读取一个数组value
    void ReadValue(Value &value, long iter); 

    //向文件中写一个value
    void WriteValue(Value value, const long &iter);

    //读取headNode信息
    HeadNode ReadHeadNode();

    //更新headNode信息
    void WriteHeadNode(int count = 0, double income = 0, double expense = 0);

private:

    //块链节点头部
    //前驱后继 元素数 上下界
    struct Head {
    
        long pre = 0;
        
        long next = 0;
        
        int size = 0;
        
        Key max;
        
        Key min;
        
    };

    //数据块
    struct BlockNode {
    
        Head NodeHead;
        
        Value Array[blockSize];

        //向blocknode中插入
        int Insert(const Key &key, const Value &ele);

        //删去blocknode中的一个
        bool Delete(const Key &key);
        
    };

    HeadNode headNode1;
    
    long head = 0;//头节点位置

    //LinkList读写的文件流对象 在LinkList构造时和相关文件关联
    std::fstream r_w_LinkList;

    //Read iter指向块链节点
    void ReadNode(BlockNode &blockNode, long iter);

    //Read iter指向块链的头部
    void ReadHead(Head &nodeHead, long iter);

    //Write 块链信息
    void WriteNode(BlockNode blockNode, const long &iter);

    //Write 块链头部信息
    void WriteHead(Head nodeHead, const long &iter);

    //BreakNode
    void BreakNode(const long &iter, BlockNode &blockNode);
    
};

```



### 4.6 指令扫描类

一个扫描解释器，将以空格隔开的指令分割为tokens。
```c++

class TokenScanner{
public:

    TokenScanner()=default;

    explicit TokenScanner(const std::string& str);

    ~TokenScanner()=default;

    //除去前导空格
    void Initialize();

    //是否仍有输入
    bool HasMoreTokens() const ;

    //下一个合法token为string
    void NextToken(std::string &str);

    //下一个合法token为char*
    void NextToken(char* token,int validSize,bool allowQuote);

    //特殊要求的char*
    void SpecialNextToken(char* token);
    
    //下一个合法token为int
    void NextToken(int &intNum,bool isPri);

    //下一个合法token为double
    void NextToken(double &doubleNum);

    //解析 -string=
    void TakeType(std::string &str);

    //解析”char*"
    void Quote(char* token);

    //给出剩余部分的string
    std::string ShowRest();

    std::string ShowOperation();
    
private:

    std::string input;
    
    std::string operation;
    
    //input 长度
    int length=0;
    
    //每次截取token的始末位置
    int tokenStart=0,tokenEnd=0;

    //更新token头尾部所在位置
    void UpdatePos();
    
};

```



### 4.7 异常处理类

实现异常处理。



#### 4.7.1 异常
```c++
class ErrorException : public std::exception {
public:

    explicit ErrorException(std::string message);

    std::string getMessage() const;
    
private:

    std::string message;
    
};
```


#### 4.7.2 抛出异常

```c++
void error(std::string message);

```



## 5. 文件存储说明

使用二进制文件储存信息。

所有信息使用块状链表存在文件中，链表的结点大小设计将尽量提高对读写信息的利用效率。




### 5.1 账户信息相关文件
- account_information：账户全部基础信息



### 5.2  图书信息文件
- book_information：图书全部基础信息
- book_ISBN_index：图书ISBN到index的映射结果
- book_name_index：图书name到index的映射结果
- book_author_index：图书author到index的映射结果
- book_keyword_index：图书keyword到index的映射结果



### 5.3 日志文件
- finance_log：财务日志文件
- log：操作日志文件




## 6. 其他补充说明
系统浮点数输入输出精度固定为小数点后两位。

字符相关信息默认值为空，数字相关信息默认值为数值 0。



