#ifndef __BIRTHCLASS_H
#define __BIRTHCLASS_H

#include <string>


// 存储生日信息的结构体
struct BirthData {
	std::string name;
    unsigned int birthYear;
    unsigned int birthMonth; // 注意：Month的首字母大写
    unsigned int birthDay;   // 注意：Day的首字母大写
};


// =======================================================
// 全局常量和数据
// =======================================================
const int KnowedNum = 6; // 存储已经知道的生日信息个数

const struct BirthData BirthKnowed[KnowedNum] = {
    {"ymf", 2005, 5, 9}, 
    {"lyq", 2005, 10, 11},
    {"hzw", 2005, 10, 30},
    {"cd", 2005, 11, 22},
    {"ymx", 2006, 3, 11},
    {"chj", 2005, 10, 14}
}; // 存储已经知道的生日数据




class Birth {
private:
    struct BirthData allBirth[20];
    unsigned int DataNum = 0;
    void ReadKnowedBirth();
    
public:
    
    bool Data_Readin(const int operation); // 根据operation的值，使用文件读取，还是内置数据读取

    int CalcuBirth() const; // 计算距离今天最近的是谁的生日 (返回索引)

    const BirthData& getData(int index) const { return allBirth[index]; }

    void print() const;
};

#endif
