/*
   未完成的功能：使用txt文件读取生日

   链表存储生日数据
 */

#include <string>
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <limits> // 用于 numeric_limits
#include <iomanip> // 用于 main 函数中的输出
#include <fstream> // 文件读取
#include <sstream> // 行解析

using namespace std;

// 存储生日信息的结构体
struct BirthData {
    string name;
    unsigned int birthYear;
    unsigned int birthMonth; // 注意：Month的首字母大写
    unsigned int birthDay;   // 注意：Day的首字母大写
};

struct Date {
    int year;
    int month;
    int day;
};

// =======================================================
// 日期时间辅助函数 (用于精确计算天数)
// =======================================================

/**
 * @brief 判断某年是否是闰年
 */
bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * @brief 计算某年某月的天数
 */
int daysInMonth(int year, int month) {
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeap(year)) {
        return 29;
    }
    return days[month];
}

/**
 * @brief 将月/日转换为一年中的“权重”（精确计算，考虑闰年）
 * @param year 年份 (用于判断闰年)
 * @param month 月份
 * @param day 日期
 * @return int 一年中的第几天 (1-366)
 */
int dateToWeightAccurate(int year, int month, int day) {
    int weight = 0;
    for (int m = 1; m < month; ++m) {
        weight += daysInMonth(year, m);
    }
    weight += day;
    return weight;
}


// =======================================================
// 时间获取函数
// =======================================================

Date getCurrentBeijingDate() {
    // 1. 获取当前时间（Unix 时间戳，从纪元开始经过的秒数）
    time_t now = time(NULL);

    // 2. 将时间戳转换为本地时间（struct tm 结构体）
    tm* ltm = localtime(&now);

    Date currentDate;

    // 3. 将 struct tm 中的信息填充到自定义的 Date 结构体中
    currentDate.year = 1900 + ltm->tm_year;
    currentDate.month = 1 + ltm->tm_mon;
    currentDate.day = ltm->tm_mday;

    return currentDate;
}

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

// =======================================================
// Class Birth 实现
// =======================================================

class Birth {
private:
    struct BirthData allBirth[20];
    unsigned int DataNum = 0;
    void ReadKnowedBirth();
    
public:
    
    bool Data_Readin(const int operation); // 根据operation的值，使用文件读取，还是内置数据读取

    int CalcuBirth() const; // 计算距离今天最近的是谁的生日 (返回索引)

    const BirthData& getData(int index) const { return allBirth[index]; }

    void print() const {
        for(int i = 0;i < DataNum; i++) {
            std::cout << allBirth[i].name << std::endl;
            std::cout << allBirth[i].birthYear << "--" << allBirth[i].birthMonth << "--" << allBirth[i].birthDay << std::endl;
            std::cout << "----------------------------------" << std::endl;
        }
    }
};

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


// =======================================================
// 主函数
// =======================================================

int main() {

    // 因为我用的是wsl，所以需要强制编码为utf-8
    #ifdef _WIN32
        system("chcp 65001");
    #endif


    Birth test;

    // 选择数据来源
    std::cout << "请选择数据来源: 1=从txt文件读取, 2=使用内置数据" << std::endl;
    int source = 2;
    cin >> source;
    if (source != 1 && source != 2) source = 2;

    if (test.Data_Readin(source)) {
        std::cout << "数据读取成功！" << std::endl;
    } else {
        std::cout << "数据读取失败！" << std::endl;
        if (source == 1) {
            std::cout << "将尝试使用内置数据..." << std::endl;
            if (test.Data_Readin(2)) {
                std::cout << "已切换至内置数据。" << std::endl;
            } else {
                std::cout << "内置数据读取也失败，程序结束。" << std::endl;
                return 1;
            }
        } else {
            return 1;
        }
    }

    // 2. 打印当前日期
    Date dateTest = getCurrentBeijingDate();
    std::cout << "\n----------------------------------" << std::endl;
    std::cout << "当前日期 (北京时间): " << dateTest.year << "-"
              << std::setw(2) << std::setfill('0') << dateTest.month << "-"
              << std::setw(2) << std::setfill('0') << dateTest.day << std::endl;
    std::cout << "----------------------------------\n" << std::endl;

    std::cout << "请输入一个操作:" << endl;
	std::cout << "1表示打印所有生日信息, 2表示计算当前谁的生日最近,3表示退出程序" << endl;

	int opera = 0;
	cin >> opera;

	while(opera != 3) {

		if(opera == 1){
			test.print();
		}
		else if (opera == 2) {
			// 3. 计算最近生日
			int nearestIndex = test.CalcuBirth();

			if (nearestIndex != -1) {
				const BirthData &bd = test.getData(nearestIndex);
				std::cout << "最近生日的人是: " << bd.name << std::endl;
				std::cout << "生日日期: " 
						<< std::setw(2) << std::setfill('0') << bd.birthMonth 
						<< "月" 
						<< std::setw(2) << std::setfill('0') << bd.birthDay 
						<< "日" << std::endl;
			} else {
				std::cout << "没有有效的数据来计算最近生日。" << std::endl;
			}
		}
		else {
			std::cout << "非法输入" << std::endl;
		}
		std::cout << "请输入一个操作:" << endl;
		std::cout << "1表示打印所有生日信息, 2表示计算当前谁的生日最近,3表示退出程序" << std::endl;
		cin >> opera;
	}


    return 0;
}
