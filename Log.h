#pragma once

#include "Date.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


#pragma once

class Log
{
public:
	Log() { accountName = ""; }
	Log(string str) { accountName = str; }

	string getAccountName() const { return accountName; }
	Date getDate() const { return date; }
	double getValue() const { return value; }

	int getMonth() const { return date.month; }
	int getDay() const { return date.day; }
	int getYear() const { return date.year; }

	string getDateString() const
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

bool logDateExists(const Date& date, const vector<Log>& log) //check if Date entry exists in vector of Log entries
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

		int daysbetween = daysBetween(log[i].getDate(), log[i - 1].getDate());

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

void writeLogFile(string account, vector<Log>& log) //note, only change of vector is chronological sorting
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
void deleteLogEntry(Log userEntry)
{
	const string CSH_LOG = userEntry.getAccountName() + ".CashLog";

	vector<string>fields = parseFile(CSH_LOG);
	vector<Log> log = getLogsFromFile(CSH_LOG);

	for (int i = 0; i < log.size(); ++i)
	{
		if ((log[i].getMonth() == userEntry.getMonth()) && (log[i].getDay() == userEntry.getDay()) && (log[i].getYear() == userEntry.getYear()))
		{
			//found duplicate in log vector, remove string
			fields.erase(fields.begin() + i);

			ofstream new_cashlog("templog.txt");

			if (!new_cashlog)
			{
				cout << "Failed to delete entry\n";
				return;
			}

			if (log.size() == 1) //only the one entry, delete file
			{
				if (remove((CSH_LOG).c_str()) != 0)
					cout << "Failed to delete entryn";
				else
					cout << "File deleted\n";

				return;
			}

			for (int i = 0; i < fields.size(); ++i)
			{
				new_cashlog << fields[i] << endl;
			}

			//now delete account.cashlog and replace with templog
			if (remove((CSH_LOG).c_str()) != 0)
			{
				cout << "Failed to delete entry\n" << endl;
				return;
			}

			new_cashlog.close();
			rename("templog.txt", CSH_LOG.c_str());
			cout << "Entry deleted\n";
			return;

		}
	}


}
