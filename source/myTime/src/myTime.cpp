#include <ctime>
#include "myTime.h"



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
