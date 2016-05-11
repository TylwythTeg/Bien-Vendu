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
