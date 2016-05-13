#pragma once

#include <iostream>
#include <string>
using namespace std;


enum Month { invalid, jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

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


struct Date
{
	Month month;
	int day;
	int year;
};

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

bool isDate(Date date)
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
