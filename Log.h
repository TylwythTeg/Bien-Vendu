#pragma once
//#include "stdafx.h"
#include "Date.h"
#include "std_lib_facilities.h" //for to_string?
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


#pragma once

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
		int m = date.month;
		return (std::to_string(m) + "/" + std::to_string(date.day) + "/" + std::to_string(date.year));
	}

	void setDate(int m, int d, int y)
	{
		date.month = monthFromInt(m);
		date.day = d;
		date.year = y;
	}
	void setValue(double doub) { value = doub; }
	void setAccountName(string str) { accountName = str; }

	friend bool operator<(Log log1, Log log2)
	{
		//different years
		if (log1.getYear() < log2.getYear())
			return true;
		else if (log1.getYear() > log2.getYear())
			return false;

		//same year, different months
		if (log1.getMonth() < log2.getMonth())
			return true;
		else if (log1.getMonth() > log2.getMonth())
			return false;

		//same year, same month, different days
		if (log1.getDay() < log2.getDay())
			return true;
		if (log1.getDay() > log2.getDay())
			return false;
		else				//same date
			return false;
	}

private:
	string accountName;
	Date date;
	double value;

};
/////////////////////////////////////////////////////////////////////////
//////////////////////
bool cashLogFile(string account)
{
	ifstream logfile(account + ".CashLog");
	if (logfile)
		return true;
	else
		return false;
}

vector<string> parseFile(string file)
{
	ifstream filestream(file);

	if (!filestream)
	{
		cout << "Can't open " << file << endl;
	}
	else
	{
		vector<string>fields;
		string st;

		for (int i = 0; !filestream.eof(); ++i)
		{
			getline(filestream, st);

			if (filestream.eof())
				break;
			fields.push_back(st);
		}

		filestream.close();
		return fields;
	}

}

Log getEntryFromString(string st)
{
	//until change to just date functions will ignore double as they desire
	double value = 0.0;

	stringstream ss;
	stringstream convert;
	string month;
	string day;
	string year;
	string val;

	ss << st;

	getline(ss, month, '/');
	getline(ss, day, '/');
	getline(ss, year, ' ');
	getline(ss, val, '\n');

	convert << val;
	convert >> value;

	int m = atoi(month.c_str());
	int d = atoi(day.c_str());
	int y = atoi(year.c_str());

	Log log;
	log.setDate(monthFromInt(m), d, y); //int_to_month(m)
	log.setValue(value);

	return log;

}

////////////////



vector<Log> getEntriesFromFile(std::string file)
{
	vector<Log> log;
	vector<string>fields = parseFile(file);
	for (int i = 0; i < fields.size(); ++i)
	{
		Log entry = getEntryFromString(fields[i]);
		log.push_back(entry);
	}
	return log;
}
bool cashLogFile(string account);
