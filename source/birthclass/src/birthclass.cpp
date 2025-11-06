#include <iostream>
#include <ctime>
#include <cmath>
#include <limits> // 用于 numeric_limits
#include <fstream> // 文件读取
#include <sstream> // 行解析
#include "myTime.h"
#include "birthclass.h"

// =======================================================
// Class Birth 实现
// =======================================================

using namespace std;

void Birth::print() const {
     for(int i = 0;i < DataNum; i++) {
         std::cout << allBirth[i].name << std::endl;
         std::cout << allBirth[i].birthYear << "--" << allBirth[i].birthMonth << "--" << allBirth[i].birthDay << std::endl;
         std::cout << "----------------------------------" << std::endl;
     }
}

void Birth::ReadKnowedBirth() {
    // 读取已有的信息
    for(DataNum = 0;DataNum < KnowedNum;DataNum++) {
        allBirth[DataNum] = BirthKnowed[DataNum];
    }
}

bool Birth::Data_Readin(const int operation) {
    if(operation == 2) {
        ReadKnowedBirth();
        return true;
    }
    // operation == 1 时，文件读取逻辑
    if (operation == 1) {
        std::cout << "请输入包含生日数据的txt文件路径: ";
        std::string path;
        std::getline(cin >> std::ws, path); // 读取整行，允许空格

        // 直接使用 ifstream 打开路径（不考虑中文路径特殊处理）
        std::ifstream fin(path, std::ios::in);
        if (!fin.is_open()) {
            std::cerr << "无法打开文件: " << path << std::endl;
            return false;
        }

        DataNum = 0;
        std::string line;
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            // 支持用逗号或空格分隔: name,year,month,day 或 name year month day
            for (char &ch : line) {
                if (ch == ',') ch = ' ';
            }
            std::istringstream iss(line);
            BirthData item;
            if (!(iss >> item.name >> item.birthYear >> item.birthMonth >> item.birthDay)) {
                // 跳过无效行
                continue;
            }
            if (item.birthMonth < 1 || item.birthMonth > 12) continue;
            if (item.birthDay < 1 || item.birthDay > 31) continue; // 粗略校验，后续按具体月份判断

            if (DataNum < sizeof(allBirth)/sizeof(allBirth[0])) {
                allBirth[DataNum++] = item;
            } else {
                std::cerr << "数据已达到最大容量，忽略多余条目。" << std::endl;
                break;
            }
        }

        if (DataNum == 0) {
            std::cerr << "文件中没有读取到有效的生日数据。" << std::endl;
            return false;
        }
        return true;
    }
    return false;
}

/**
 * @brief 计算距离今天最近的是谁的生日
 * * * 核心思路：计算从今天到下一个生日之间的“天数差”，找出天数差最小的人。
 * * 跨年处理：如果生日在今天或之后，天数差 = (生日权重 - 今天权重)。
 * * 如果生日在今天之前，天数差 = (今年剩余天数 - 今天权重) + 明年生日权重。
 * * @return int 最近生日的人在 allBirth 数组中的索引，如果没有数据返回 -1
 */
int Birth::CalcuBirth() const {
    // 1. 基础检查
    if (DataNum < 1) {
        return -1;
    }

    // 2. 获取当前日期
    Date dateNow = getCurrentBeijingDate();
    
    // 3. 计算当前日期的权重 (一年中的第几天)
    const int todayWeight = dateToWeightAccurate(dateNow.year, dateNow.month, dateNow.day);
    const int daysInThisYear = isLeap(dateNow.year) ? 366 : 365;

    // 4. 初始化最小时间差和最近生日的人
    int who = -1; // 存储最近生日的人的索引
    // 使用最大的int值来初始化最小天数差
    int minDaysToBirthday = std::numeric_limits<int>::max(); 

    // 5. 遍历所有人，计算到下一个生日的天数
    for (int i = 0; i < DataNum; ++i) {
        const int birthMonth = allBirth[i].birthMonth;
        const int birthDay = allBirth[i].birthDay;
        
        // 计算生日在今年的权重
        const int birthdayWeightThisYear = dateToWeightAccurate(dateNow.year, birthMonth, birthDay);

        int daysToBirthday;

        if (birthdayWeightThisYear >= todayWeight) {
            // 情况 A: 生日在今天或今天之后 (在今年)
            daysToBirthday = birthdayWeightThisYear - todayWeight;
        } else {
            // 情况 B: 生日已经过了 (在明年)
            // 差值 = (今年剩余天数) + (明年生日天数)
            // 假设明年的天数
            const int daysInNextYear = isLeap(dateNow.year + 1) ? 366 : 365;
            const int birthdayWeightNextYear = dateToWeightAccurate(dateNow.year + 1, birthMonth, birthDay);
            
            // 计算方法： (今年剩余天数) + (明年生日权重)
            // 今年剩余天数 = (今年总天数 - 今天权重)
            daysToBirthday = (daysInThisYear - todayWeight) + birthdayWeightNextYear;
        }
        
        // 排除无效日期，如 2/29 在非闰年
        // 实际上，如果数据是有效的，这步可以省略
        if (birthMonth == 2 && birthDay == 29 && daysToBirthday > 366) {
             // 2月29日生日的人在非闰年时，下一个生日总是在4年后的2月29日，
             // 这里的近似计算可能会略有误差，但对于大多数月份是准确的。
             // 如果需要极致精度，需要更复杂的日期库。
        }


        // 6. 更新最近生日的人
        if (daysToBirthday < minDaysToBirthday) {
            minDaysToBirthday = daysToBirthday;
            who = i;
        } 
        // 7. 处理天数相同时的情况（例如，两个人都是明天生日）
        else if (daysToBirthday == minDaysToBirthday) {
            // 如果天数差相同，保持第一个找到的人 (who 不变)
            // 如果需要按其他规则打破平局（如按名字排序），可以在这里添加逻辑。
        }
    }
    
    // 8. 返回结果
    return who; 
}
