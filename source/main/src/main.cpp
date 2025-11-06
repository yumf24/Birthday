/*
	还没有完成的功能：

   链表存储生日数据
 */

#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip> // 用于 main 函数中的输出
#include "birthclass.h"
#include "myTime.h"

using namespace std;





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
