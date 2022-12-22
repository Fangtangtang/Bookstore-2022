//
// Created by 房诗涵 on 2022/12/10.
//

#ifndef BOOKSTORE_LINKLIST_H
#define BOOKSTORE_LINKLIST_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

//头节点（信息储存块）
struct HeadNode {
    long next = 0;
    int count = 0;
    double income = 0;
    double expense = 0;
};

//LinkList类 模板
//index 检索索引 ；key 排序标记 ；value 数组元素
//key: GetIndex(index)
//value:GetKey(index indexSign) 重载函数 获取元素基于index的key index作为函数选择标记
template<class index, class Key, class Value>
class LinkList {
    static constexpr int blockSize = 1024;
public:
    LinkList(const std::string &file_name) {

        r_w_LinkList.open(file_name);

//        r_w_LinkList.open(file_name, std::fstream::trunc);

        if (!r_w_LinkList.good()) {
            //create headNode

            r_w_LinkList.open(file_name, std::ios::out);
            r_w_LinkList.close();
            r_w_LinkList.open(file_name);

//            r_w_LinkList.open("1", std::ios::out);
//            r_w_LinkList.close();
//            r_w_LinkList.open("1");

            headNode1.next = 0;
            r_w_LinkList.seekp(0);//将指针定位到文件开头
            head = r_w_LinkList.tellp();
            //从文件开头写HeadNode
            r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
        }
        r_w_LinkList.seekg(head);
        r_w_LinkList.read(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));

//        std::cout<<file_name<<" "<<headNode1.next<<" "<<headNode1.count<<'\n';
    }

    ~LinkList() {
//        std::cout<<"breakdown\n";
        r_w_LinkList.seekp(head);
        r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
    }

    void PrintList() {
        BlockNode blockNode;
        long iter = headNode1.next;//第一个节点
        while (iter != 0) {
//            std::cout << "####\n";
            blockNode = ReadNode(iter);
            for (int i = 0; i < blockNode.NodeHead.size; ++i)
                blockNode.Array[i].Print();
//            std::cout << '\n';
            iter = blockNode.NodeHead.next;
        }
    }

    long Insert(Key key, Value ele) {
        //插入地址
        long location = 0;
        int num = 0;
        bool break_flag = false;//是否break
        bool insert_flag = false;
        BlockNode blockNode;
        Head head1, head2;
        long iter = headNode1.next;
        //第一次插元素
        if (headNode1.next == 0) {
            r_w_LinkList.seekp(0, std::ios::end);//the end of the file
            iter = r_w_LinkList.tellp();
            blockNode.NodeHead.pre = head;
            headNode1.next = iter;
            num = blockNode.Insert(key, ele);//不可能裂块
            r_w_LinkList.seekp(head);
            r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
            WriteNode(blockNode, iter);
        }
            //之后插元素
        else {
            head2 = ReadHead(iter);
            if (head2.max > key) {
                blockNode = ReadNode(iter);
                num = blockNode.Insert(key, ele);
                insert_flag = true;
                if (blockNode.NodeHead.size == blockSize)break_flag = true;
                if (break_flag) {
                    BreakNode(iter, blockNode);
                }
                WriteNode(blockNode, iter);
            }
            if (!insert_flag) {
                iter = head2.next;
                while (iter != 0) {//向后遍历
                    head1 = head2;
                    head2 = ReadHead(iter);
                    if (key >= head1.min && head2.min >= key) {
                        if (head1.max > key) {
                            iter = head2.pre;
                            blockNode = ReadNode(iter);
                            num = blockNode.Insert(key, ele);//插入当前块的array
                            if (blockNode.NodeHead.size == blockSize)break_flag = true;
                            if (break_flag) {
                                BreakNode(iter, blockNode);
                            }
                            WriteNode(blockNode, iter);
                        }
                        if (key > head1.max) {
                            blockNode = ReadNode(iter);
                            num = blockNode.Insert(key, ele);//插入后一块的array
                            if (blockNode.NodeHead.size == blockSize)break_flag = true;
                            if (break_flag) {
                                BreakNode(iter, blockNode);
                            }
                            WriteNode(blockNode, iter);
                        }
                        break;
                    }
                    iter = head2.next;
                }
                if (iter == 0) {
                    //曾对尾插做过一些愚蠢的多余特判 导致会在same index顺序插入时不断加新块！！！！
                    if (head1.next != 0)iter = head1.next;
                    else iter = headNode1.next;
                    blockNode = ReadNode(iter);
                    num = blockNode.Insert(key, ele);//插入后一块的array
                    if (blockNode.NodeHead.size == blockSize)break_flag = true;
                    if (break_flag) BreakNode(iter, blockNode);
                    WriteNode(blockNode, iter);
                }
            }
        }
        location = iter + sizeof(Head) + num * sizeof(Value);
        return location;
    }

    //Delete
    bool Delete(Key key) {
        long iter = headNode1.next;
        BlockNode blockNode;
        Head head1;
        while (iter != 0) {
            head1 = ReadHead(iter);
            if (key >= head1.min && head1.max >= key)break;
            iter = head1.next;
        }
        if (iter == 0) return false;
        blockNode = ReadNode(iter);
        bool b = blockNode.Delete(key);
        if (!b) return false;
        if (blockNode.NodeHead.size < blockSize / 2) {
            Head preHead, nextHead;
            BlockNode preNode, nextNode;
            if (blockNode.NodeHead.pre != 0) {
                preHead = ReadHead(blockNode.NodeHead.pre);
            }
            if (blockNode.NodeHead.next != 0) {
                nextHead = ReadHead(blockNode.NodeHead.next);
            }
            if (blockNode.NodeHead.size == 0) {
                if (blockNode.NodeHead.pre == 0) {
                    headNode1.next = blockNode.NodeHead.next;
                    r_w_LinkList.seekp(head);
                    r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
                    if (blockNode.NodeHead.next != 0) {
                        nextHead.pre = head;
                        WriteHead(nextHead, blockNode.NodeHead.next);
                    }
                } else {
                    preHead.next = blockNode.NodeHead.next;
                    WriteHead(preHead, blockNode.NodeHead.pre);
                    if (blockNode.NodeHead.next != 0) {
                        nextHead.pre = blockNode.NodeHead.pre;
                        WriteHead(nextHead, blockNode.NodeHead.next);
                    }
                }
                return true;
            }
            index indexSign;
            if (preHead.size > blockSize / 2) {
                preNode = ReadNode(blockNode.NodeHead.pre);
                blockNode.Insert(preNode.NodeHead.max, preNode.Array[preNode.NodeHead.size - 1]);

                preNode.Delete(preNode.Array[preNode.NodeHead.size - 1].GetKey(indexSign));
                WriteNode(preNode, blockNode.NodeHead.pre);
                WriteNode(blockNode, iter);
                return true;
            }
            if (nextHead.size > blockSize / 2) {
                nextNode = ReadNode(blockNode.NodeHead.next);
                blockNode.Insert(nextNode.NodeHead.min, nextNode.Array[0]);
                nextNode.Delete(nextNode.Array[0].GetKey(indexSign));
                WriteNode(nextNode, blockNode.NodeHead.next);
                WriteNode(blockNode, iter);
                return true;
            }
            if (blockNode.NodeHead.next != 0) {
                nextNode = ReadNode(blockNode.NodeHead.next);
                preNode = ReadNode(blockNode.NodeHead.pre);
                CombineNode(preNode, blockNode, nextNode);
                return true;
            }
        }
        WriteNode(blockNode, iter);
        return true;
    }

    void FindPrint(const index &index1) {
        long iter = headNode1.next;
        Head head1;
        while (iter != 0) {
            head1 = ReadHead(iter);
            if (index1 >= head1.min.GetIndex(index1) && head1.max.GetIndex(index1) >= index1)break;
            iter = head1.next;
        }
        if (iter == 0) {
            std::cout << "\n";
            return;
        }
        BlockNode blockNode = ReadNode(iter);
        int i = 0;
        while (!(index1 == blockNode.Array[i].GetIndex(index1))) {
            ++i;
            if (i == blockNode.NodeHead.size) {
                std::cout << "\n";
                return;//没有元素
            }
        }
        while (true) {
            blockNode.Array[i].Print();
//            std::cout << blockNode.Array[i].GetValue() << " ";
            ++i;
            if (i == blockNode.NodeHead.size) {//结束一个块
                blockNode = ReadNode(blockNode.NodeHead.next);
                i = 0;
            }
            if (!(index1 == blockNode.Array[i].GetIndex(index1))) return;//找完
        }
    }

    //以index寻找元素
    //返回value数组
    std::vector<Value> FindSubList(const index &index1) {
        std::vector<Value> vec;
        long iter = headNode1.next;
        Head head1;
        while (iter != 0) {
            head1 = ReadHead(iter);
            if (index1 >= head1.min.GetIndex(index1) && head1.max.GetIndex(index1) >= index1)break;
            iter = head1.next;
        }
        if (iter == 0) {
            return vec;
        }
        BlockNode blockNode = ReadNode(iter);
        int i = 0;
        while (!(index1 == blockNode.Array[i].GetIndex(index1))) {
            ++i;
            if (i == blockNode.NodeHead.size) {
                return vec;//没有元素
            }
        }
        while (true) {
            vec.push_back(blockNode.Array[i]);
            ++i;
            if (i == blockNode.NodeHead.size) {//结束一个块
                blockNode = ReadNode(blockNode.NodeHead.next);
                i = 0;
            }
            if (!(index1 == blockNode.Array[i].GetIndex(index1))) return vec;//找完
        }
    }

    //基于key寻找value
    //iter是value地址
    std::pair<Value, bool> Find(const Key &key, long &iter) {
        iter = headNode1.next;
        Head head1;
        Value value;
        index indexSign;
        while (iter != 0) {
            head1 = ReadHead(iter);
            if (key >= head1.min && head1.max >= key)break;
            iter = head1.next;
        }
        if (iter == 0) {
            return std::make_pair(value, false);
        }
        BlockNode blockNode = ReadNode(iter);
        int i = 0;
        while (!(key == (blockNode.Array[i].GetKey(indexSign)))) {
            ++i;
            if (i == blockNode.NodeHead.size) {
                return std::make_pair(value, false);
            }
        }
        //数组元素地址
        iter = iter + sizeof(Head) + i * sizeof(Value);
        return std::make_pair(blockNode.Array[i], true);
    }

    //遍历末count个，返回vector
    std::vector<Value> Traverse(Key startKey) {
        std::vector<Value> eleGroup;
        long iter = headNode1.next;
        Head head1;
        while (iter != 0) {
            head1 = ReadHead(iter);
            if (startKey >= head1.min && head1.max >= startKey)break;
            iter = head1.next;
        }
        BlockNode blockNode = ReadNode(iter);
        int i = 0;
        index indexSign;
        while (!(startKey == blockNode.Array[i].GetKey(indexSign))) {
            ++i;
        }
        while (true) {
            eleGroup.push_back(blockNode.Array[i]);
            ++i;
            if (i == blockNode.NodeHead.size) {//结束一个块
                if (blockNode.NodeHead.next == 0) return eleGroup;
                blockNode = ReadNode(blockNode.NodeHead.next);
                i = 0;
            }
        }
    }

    //读取一个数组元素
    Value ReadValue(long iter) {
        Value value;
        r_w_LinkList.seekg(iter);
        r_w_LinkList.read(reinterpret_cast<char *>(&value), sizeof(value));
        return value;
    }

    void WriteValue(Value value, const long &iter) {
        r_w_LinkList.seekp(iter);
        r_w_LinkList.write(reinterpret_cast<char *> (&value), sizeof(Value));
    }

    //读取headNode信息
    HeadNode ReadHeadNode() {
        return headNode1;
    }

    //更新headNode信息
    void WriteHeadNode(int count = 0, double income = 0, double expense = 0) {
        headNode1.count = count;
        headNode1.income = income;
        headNode1.expense = expense;
        r_w_LinkList.seekp(head);
        r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
    }

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

        //Insert
        int Insert(const Key &key, const Value &ele) {
            int i = 0;
            index indexSign;
            while (key > Array[i].GetKey(indexSign) && i < NodeHead.size) {
                ++i;
            }
            if (i < NodeHead.size && key == Array[i].GetKey(indexSign)) return 1024;
            int move = NodeHead.size;
            while (move != i) {
                Array[move] = Array[move - 1];
                --move;
            }
            Array[i] = ele;
            ++NodeHead.size;
            if (i == 0) NodeHead.min = key;
            if (i == NodeHead.size - 1) NodeHead.max = key;
            return i;
        }

        //Delete
        bool Delete(const Key &key) {
            int i = 0;
            index indexSign;
            while (i < NodeHead.size) {
                if (Array[i].GetKey(indexSign) == key) break;
                ++i;
            }
            if (i == NodeHead.size) return false;
            --NodeHead.size;
            for (; i < NodeHead.size; ++i) {
                Array[i] = Array[i + 1];
            }
            Value eleNull;
            Array[NodeHead.size] = eleNull;
            NodeHead.min = Array[0].GetKey(indexSign);
            NodeHead.max = Array[NodeHead.size - 1].GetKey(indexSign);
            return true;
        }
    };

    HeadNode headNode1;
    long head = 0;//头节点位置

    //LinkList读写的文件流对象 在LinkList构造时和相关文件关联
    std::fstream r_w_LinkList;

    //Read iter指向块链节点
    BlockNode ReadNode(long iter) {
        BlockNode blockNode;
        r_w_LinkList.seekg(iter);
        r_w_LinkList.read(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
        return blockNode;
    }

    //Read iter指向块链的头部
    Head ReadHead(long iter) {
        Head nodeHead;
        r_w_LinkList.seekg(iter);
        r_w_LinkList.read(reinterpret_cast<char *> (&nodeHead), sizeof(Head));
        return nodeHead;
    }


    //Write 块链信息
    void WriteNode(BlockNode blockNode, const long &iter) {
        r_w_LinkList.seekp(iter);
        r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    }

    //Write 块链头部信息
    void WriteHead(Head nodeHead, const long &iter) {
        r_w_LinkList.seekp(iter);
        r_w_LinkList.write(reinterpret_cast<char *> (&nodeHead), sizeof(Head));
    }


    //BreakNode
    void BreakNode(const long &iter, BlockNode &blockNode) {
        //获取新空间
        r_w_LinkList.seekp(0, std::ios::end);//the end of the file
        long newIter = r_w_LinkList.tellp();
        BlockNode newBlock;
        Value ele;
        for (int i = blockSize / 2; i < blockSize; ++i) {
            newBlock.Array[i - blockSize / 2] = blockNode.Array[i];
            blockNode.Array[i] = ele;
        }
        blockNode.NodeHead.size = newBlock.NodeHead.size = blockSize / 2;
        newBlock.NodeHead.max = blockNode.NodeHead.max;
        index indexSign;
        blockNode.NodeHead.max = blockNode.Array[blockSize / 2 - 1].GetKey(indexSign);
        newBlock.NodeHead.min = newBlock.Array[0].GetKey(indexSign);
        newBlock.NodeHead.next = blockNode.NodeHead.next;
        newBlock.NodeHead.pre = iter;
        blockNode.NodeHead.next = newIter;
        if (newBlock.NodeHead.next != 0) {
            Head head1 = ReadHead(newBlock.NodeHead.next);
            head1.pre = newIter;
            WriteHead(head1, newBlock.NodeHead.next);
        }
        WriteNode(newBlock, newIter);
    }

    //CombineNode并入后一块
    void CombineNode(BlockNode &pre, BlockNode &blockNode, BlockNode &next) {
        for (int i = next.NodeHead.size; i >= 0; --i) {
            next.Array[blockNode.NodeHead.size + i] = next.Array[i];
        }
        for (int i = 0; i < blockNode.NodeHead.size; ++i) {
            next.Array[i] = blockNode.Array[i];
        }
        next.NodeHead.size += blockNode.NodeHead.size;
        next.NodeHead.min = blockNode.NodeHead.min;
        if (blockNode.NodeHead.pre != 0) {
            pre.NodeHead.next = blockNode.NodeHead.next;
            next.NodeHead.pre = blockNode.NodeHead.pre;
            WriteNode(pre, next.NodeHead.pre);
            WriteNode(next, pre.NodeHead.next);
        } else {
//            HeadNode headNode;
            headNode1.next = blockNode.NodeHead.next;
            next.NodeHead.pre = head;
            WriteNode(next, headNode1.next);
            r_w_LinkList.seekp(head);
            r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
        }
    }
};

#endif //BOOKSTORE_LINKLIST_H
