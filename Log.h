#ifndef Log_H
#define Log_H

#include "Date.h"
#include <vector>
#include <string>


//using namespace std;


#pragma once

class Log
{
public:
	Log() { accountName = ""; }
	Log(std::string str) { accountName = str; }

	std::string getAccountName() const { return accountName; }
	Date getDate() const { return date; }
	double getValue() const { return value; }

	int getMonth() const { return date.month; }
	int getDay() const { return date.day; }
	int getYear() const { return date.year; }

	std::string getDateString() const;
	void setDate(int m, int d, int y);

	void setValue(double doub) { value = doub; }
	void setAccountName(std::string str) { accountName = str; }

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
	std::string accountName;
	Date date;
	double value;

};

bool cashLogFile(std::string account);
std::vector<std::string> parseFile(std::string file);
Log getLogFromString(std::string st);
std::vector<Log> getLogsFromFile(std::string file);
bool cashLogFile(std::string account);
bool logDateExists(const Date& date, const std::vector<Log>& log); //check if Date entry exists in vector of Log entries
void printLogs(std::string account);
void analyzeCashLog(std::string account);
void writeLogFile(std::string account, std::vector<Log>& log); //note, only change of vector is chronological sorting
void deleteLogEntry(Log userEntry);

#endif
