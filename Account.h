#pragma once

#include <string>

class Account
{
	public:
		Account(string str)
		{ 
			name = str; 

			int m, d, y;
			double value;
			if (cashLogFile(name + ".CashLog"))
			{
				vector<string>fields = parseFile(name + ".CashLog");	//functionize this
				vector<Log> entries;

				for (int i = 0; i < fields.size; ++i)
					entries.push_back(getEntryFromString(fields[i]));
			}
		
		}

		string getName() { return name; }
		int getNumOfMachines() { return numOfMachines; }

		void setNumOfMachines(int n) { numOfMachines = n; }

		


	private:
		string name;
		int numOfMachines;
		vector<Log> entries;
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
		Log() { accountName = ""; }
		Log(string str) { accountName = str; }

		string getAccountName() { return accountName; }
		Date getDate() { return date; }
		double getValue() { return value; }

		int getMonth() { return date.month; }
		int getDay() { return date.day; }
		int getYear() { return date.year; }

		string getDateString()
		{
			return (to_string(date.month) + "/" + to_string(date.day) + "/" + to_string(date.year));
		}

		void setDate(int m, int d, int y)
		{
			date.month = m;
			date.day = d;
			date.year = y;
		}
		void setValue(double doub) { value = doub; }
		void setAccountName(string str) { accountName = str; }

	private:
		string accountName;
		Date date;
		double value;

};
