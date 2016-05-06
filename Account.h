#pragma once

#include <string>

class Account
{
	public:
		Account(string str){ name = str; }

		string getName() { return name; }
		int getNumOfMachines() { return numOfMachines; }

		void setNumOfMachines(int n) { numOfMachines = n; }


	private:
		string name;
		int numOfMachines;
		vector<Log> Entries;
};

struct Date
{
	int month;
	int day;
	int year;
};

class Log
{
	public:
		Log() {}
		Log(string str) { accountName = str; }

		string getAccountName() { return accountName; }
		Date getDate() { return date; }
		double getValue() { return value; }

		int getMonth() { return date.month; }
		int getDay() { return date.day; }
		int getYear() { return date.year; }

		void setDate(int m, int d, int y)
		{
			date.month = m;
			date.day = d;
			date.year = y;
		}

	private:
		string accountName;
		Date date;
		double value;

};
