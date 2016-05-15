#include "Date.h"
#include <iostream>
#include <map>

using namespace std;

Month operator++(Month& m)
{
	m = (m == Month::dec) ? Month::jan : Month(int(m) + 1);
	return m;
}

Month operator+(Month m, int n)
{
	for (int i = 0; i < n; ++i)
		++m;

	return m;
}

Month monthFromInt(int x)
{
	if (x<Month::jan || x>Month::dec)
	{
		return Month(invalid);
		cout << "Error converting int to month";
	}
	else
		return Month(x);
}

string monthStringFromInt(int x)
{
	int y = x % 12;

	switch (y)
	{
	case 0:
		return "dec";
		break;
	case 1:
		return "jan";
		break;
	case 2:
		return "feb";
		break;
	case 3:
		return "mar";
		break;
	case 4:
		return "apr";
		break;
	case 5:
		return "may";
		break;
	case 6:
		return "jun";
		break;
	case 7:
		return "jul";
		break;
	case 8:
		return "aug";
		break;
	case 9:
		return "sep";
		break;
	case 10:
		return "oct";
		break;
	case 11:
		return "nov";
		break;
	default: cout << "Error converting intmonth to string\n";
	}
}

bool leapYear(int year)
{
	if (year % 4 == 0)
	{
		if (year % 100 == 0)
		{
			if (year % 400 == 0)
			{
				return true;
			}
			return false;
		}
		return true;
	}
	return false;
}

bool isDate(const Date& date)
{

	switch (date.month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if (date.day >= 1 && date.day <= 31)
			return true;
		return false;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if (date.day >= 1 && date.day <= 30)
			return true;
		return false;
		break;
	case 2:
		if ((date.day <= 28) || (date.day == 29 && leapYear(date.year)))
			return true;
		return false;
		break;
	default:
		return false;
		break;
	}

}

int daysBetween(const Date& lateDate, const Date& earlyDate)	//Find amount of days beetween two cash logs
{
	int daystosubtract = 0;
	int totaldays = 0;

	map<string, int>Days;
	Days["jan"] = 31;
	Days["feb"] = 28;
	Days["mar"] = 31;
	Days["apr"] = 30;
	Days["may"] = 31;
	Days["jun"] = 30;
	Days["jul"] = 31;
	Days["aug"] = 31;
	Days["sep"] = 30;
	Days["oct"] = 31;
	Days["nov"] = 30;
	Days["dec"] = 31;

	if ((lateDate.month == earlyDate.month) && (lateDate.year == earlyDate.year))
		return lateDate.day - earlyDate.day;	//mo and day are same so return simple difference of days

	if (earlyDate.day > lateDate.day)	//like if earlylog is 3/8/2016 and latelog is 7/5/2016 (partial month)
	{
		daystosubtract = earlyDate.day - lateDate.day;	//subtract these days later
														//will act as if LateLog.GetDay==EarlyLog.GetDay
	}

	int monthsbetween = lateDate.month - earlyDate.month;	//4
	int currentmonth = earlyDate.month;	//gonna go from earlylog to latelog

										//yearsbetween
	int yearsbetween = lateDate.year - earlyDate.year;
	monthsbetween = monthsbetween + (yearsbetween * 12);
	//yearsbetween

	string monthstring = monthStringFromInt(currentmonth);

	int currentyear = earlyDate.year;

	//Month newmonth;

	for (int i = 0; i <monthsbetween; ++i)
	{
		if (i == 0)	//if first month we must subtract days from out starting point
		{
			totaldays += (Days[monthstring] - earlyDate.day); //31-8 = 23 total days
		}
		if (i != 0)
		{
			totaldays += (Days[monthstring]);
		}

		if ((monthstring == "feb") && leapYear(currentyear))
			totaldays++;

		monthstring = monthStringFromInt(earlyDate.month + (i + 1));	//monthstring is now string of month + (iterations+1)															

		if (monthstring == "jan")
			currentyear++;	//this may cause problems if we start out on january
	}
	//ran through up to beginning of LateLog.GetMonth()
	totaldays += earlyDate.day;
	//now we have found the days between for perfect months

	//subtract daystosubtract
	totaldays -= daystosubtract;

	return totaldays;
}
