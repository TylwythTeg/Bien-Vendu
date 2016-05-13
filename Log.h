#pragma once
//#include "stdafx.h"
#include "Date.h"
#include "std_lib_facilities.h" //for to_string?
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

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

Log getLogFromString(string st)
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



vector<Log> getLogsFromFile(std::string file)
{
	vector<Log> log;
	vector<string>fields = parseFile(file);
	for (int i = 0; i < fields.size(); ++i)
	{
		Log entry = getLogFromString(fields[i]);
		log.push_back(entry);
	}
	return log;
}
bool cashLogFile(string account);

bool logDateExists(Date date, vector<Log> log) //check if Date entry exists in vector of Log entries
{

	for (int i = 0; i < log.size(); ++i)
	{
		if ((log[i].getMonth() == date.month) && (log[i].getDay() == date.day) && (log[i].getYear() == date.year))
			return true;
	}
	return false;
}

void printLogs(string account)
{
	vector<string>fields = parseFile(account + ".CashLog");

	for (int i = 0; i < fields.size(); ++i)
	{
		cout << "Entry [" << i << "]: " << fields[i] << endl;

	}
}

//The function parameters should strictly accept two dates and function should be moved to Date.h
int daysBetween(Log lateLog, Log earlyLog)	//Find amount of days beetween two cash logs
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

	if ((lateLog.getMonth() == earlyLog.getMonth()) && (lateLog.getYear() == earlyLog.getYear()))
	{
		return lateLog.getDay() - earlyLog.getDay();	//mo and day are same so return simple difference of days
	}

	if (earlyLog.getDay() > lateLog.getDay())	//like if earlylog is 3/8/2016 and latelog is 7/5/2016 (partial month)
	{
		daystosubtract = earlyLog.getDay() - lateLog.getDay();	//subtract these days later
																//will act as if LateLog.GetDay==EarlyLog.GetDay
	}

	int monthsbetween = lateLog.getMonth() - earlyLog.getMonth();	//4
	int currentmonth = earlyLog.getMonth();	//gonna go from earlylog to latelog

											//yearsbetween
	int yearsbetween = lateLog.getYear() - earlyLog.getYear();
	monthsbetween = monthsbetween + (yearsbetween * 12);
	//yearsbetween

	string monthstring = monthStringFromInt(currentmonth);

	int currentyear = earlyLog.getYear();

	//Month newmonth;

	for (int i = 0; i <monthsbetween; ++i)
	{
		if (i == 0)	//if first month we must subtract days from out starting point
		{
			totaldays += (Days[monthstring] - earlyLog.getDay()); //31-8 = 23 total days
		}
		if (i != 0)
		{
			totaldays += (Days[monthstring]);
		}

		if ((monthstring == "feb") && leapYear(currentyear))
			totaldays++;

		monthstring = monthStringFromInt(earlyLog.getMonth() + (i + 1));	//monthstring is now string of month + (iterations+1)															

		if (monthstring == "jan")
			currentyear++;
	}
	//ran through up to beginning of LateLog.GetMonth()
	totaldays += earlyLog.getDay();
	//now we have found the days between for perfect months

	//subtract daystosubtract
	totaldays -= daystosubtract;

	return totaldays;
}

void analyzeCashLog(string account)
{
	//calculate average daily earnings        
	//and then total average

	vector<Log>log;
	vector<string>fields = parseFile(account + ".CashLog");
	vector<double>dailyValue;
	for (int i = 0; i < fields.size(); ++i)
	{
		Log entry = getLogFromString(fields[i]);

		log.push_back(entry);

		cout << "\t" << log[i].getMonth() << "/" << log[i].getDay() << "/" << log[i].getYear() << ": $" << log[i].getValue() << endl;

		///////////Calculate days between and money made per day/////////
		if (i == 0)		//if first Log entry skip for loop iteration
		{
			cout << endl;
			continue;
		}

		int daysbetween = daysBetween(log[i], log[i - 1]);

		cout << "\tDays between last log date: " << daysbetween << endl;
		cout << "\tThe money made per day was " << log[i].getValue() / daysbetween << endl << endl;

		dailyValue.push_back(daysbetween);
		///////////Calculate days between and money made per day//////////
	}

	//average daily

	double sum = 0;
	for (int i = 0; i < dailyValue.size(); ++i)
		sum += dailyValue[i];

	cout << "The average money made per day is " << sum / dailyValue.size() << endl << endl;

	return;
}

void writeLogFile(string account, vector<Log>log)
{
	const string CSH_LOG = account + ".CashLog";

	//sort logs by date
	if (log.size() > 1)
	{
		sort(log.begin(), log.end());
	}

	//log file already exists
	if (cashLogFile(account))
	{
		//write to temp, delete account + ".CashLog", rename temp to account + ."CashLog"
		ofstream new_cashlog("templog.txt");

		if (!new_cashlog)
		{
			cout << "Failed to save logfile. Aborting without saving...\n";
			return;
		}
		else
		{
			for (int i = 0; i < log.size(); ++i)
			{
				new_cashlog << log[i].getDateString() << " " << log[i].getValue() << "\n";
			}

			//now delete account.cashlog and replace with temp
			if (remove((account + ".CashLog").c_str()) != 0)
			{
				cout << "Failed to update cashlog file. Aborting without saving...\n" << endl;
				return;
			}
			else
			{
				new_cashlog.close();
				rename("templog.txt", CSH_LOG.c_str());
				cout << "New entries saved\n";
				return;
			}
		}

	}


	//logfile doesn't exist
	ofstream logfile(CSH_LOG);

	if (!logfile)
	{
		cout << "Error opening file. Aborting without saving...\n" << endl;
	}
	else
	{
		for (int i = 0; i < log.size(); ++i)
		{
			//save and print

			if (!logfile)
			{
				cout << "Error opening file. Aborting without saving...\n" << endl;
				return;
			}
			else
			{
				//Account Name, Number of Machines, etc
				logfile << log[i].getDateString() << " " << log[i].getValue() << "\n";
				cout << "Entry " << i << ": " << log[i].getDateString() << " " << log[i].getValue() << " saved\n";
			}

		}
		cout << "All entries saved...\n";
		logfile.close();
	}
}
