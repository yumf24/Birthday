#ifndef __TIME_H
#define __TIME_H

struct Date {
	int year;
	int month;
	int day;
};

bool isLeap(int year); 
int dateToWeightAccurate(int year, int month, int day); 
Date getCurrentBeijingDate(); 

#endif
