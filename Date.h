#ifndef Date_H
#define Date_H

#include <iostream>
#include <string>
#include <map>
using namespace std;


enum Month { invalid, jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };
Month operator++(Month& m);
Month operator+(Month m, int n);
Month monthFromInt(int x);
string monthStringFromInt(int x);

struct Date
{
	Month month;
	int day;
	int year;
};

bool isDate(const Date& date);
bool leapYear(int year);
int daysBetween(const Date& lateDate, const Date& earlyDate);	//Find amount of days beetween two cash logs


#endif
