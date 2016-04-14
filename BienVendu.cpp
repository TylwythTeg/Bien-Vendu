#include "stdafx.h"
#include "std_lib_facilities.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <cstdio>

using namespace std;

const string MSTR_ACNT_LIST = "Accounts.txt";

class Account
{
public:
	//Constructor/Deconstructor
	Account() {}
	~Account() {}

	//Member Methods
	string GetAccountName() { return AccountName; }
	void SetAccountName(string st) { AccountName = st; }

	int GetNumOfMachines() { return NumOfMachines; }
	void SetNumOfMachines(int n) { NumOfMachines = n; }

private:
	string AccountName;
	int NumOfMachines;
	vector<double>cash;
};

enum Month { jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

Month int_to_month(int x)
{
	if (x<Month::jan || x>Month::dec)
		cout << "Error converting int to month";
	else
		return Month(x);
}

string intmonth_to_str(int x)
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

Month operator++(Month& m)
{
	m = (m == Month::dec) ? Month::jan : Month(int(m) + 1);
	return m;
}

Month operator+(Month m, int n)
{
	for (int i = 0; i < n; ++i)
	{
		++m;
	}
	return m;
}

bool IsLeapYear(int year)
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

bool isDate(int m, int d, int y)
{
	if (m >= 1 && m <= 12)
	{
		if (d <= 31 && d >= 1)
		{
			if ((m == 2 && d <= 28) || (m == 2 && d == 29 && IsLeapYear(y)))
				return true;

			if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
				return true;

			if ((m == 4 || m == 6 || m == 9 || m == 11) && d <= 30)
				return true;
		}
	}
	return false;
}

class Date
{
public:
	Date() { month = Month::jan; day = 1; year = 1900; }	//empty date

	Date(Month m, int d, int y)
		:month(m), day(d), year(y)
	{};
	
	int GetMonth() { return month; }
	int GetDay() { return day; }
	int GetYear() { return year; }

	void SetMonth(Month m) { month = m; }
	void SetDay(int d) { day = d; }
	void SetYear(int y) { year = y; }

private:
	Month month;
	int day;
	int year;
};

class CashLog
{
public:
	CashLog(string st)
	{}

	CashLog(Month m, int d, int y)	//initialize with //m, //d, //y, parse these values from string stream
	{
		Date(m, d, y);
	}

	friend bool operator<(CashLog Log1, CashLog Log2)
	{
		//different years
		if (Log1.GetYear() < Log2.GetYear())			
			return true;
		else if (Log1.GetYear() > Log2.GetYear())
			return false;

		//same year, different months
		if (Log1.GetMonth() < Log2.GetMonth())
			return true;
		else if (Log1.GetMonth() > Log2.GetMonth())
			return false;

		//same year, same month, different days
		if (Log1.GetDay() < Log2.GetDay())				
			return true;
		if (Log1.GetDay() > Log2.GetDay())
			return false;
		else				//same date
			return false;
	}

	int GetMonth() { return date.GetMonth(); }
	int GetYear() { return date.GetYear(); }
	int GetDay() { return date.GetDay(); }

	void SetDay(int n) { date.SetDay(n); }

	void SetValue(double d) { value = d; }
	double GetValue() { return value; }

	string GetDate()
	{ 
	 return (to_string(date.GetMonth()) + "/" + to_string(date.GetDay()) + "/" + to_string(date.GetYear()));
	}

	void SetDate(Month m, int d, int y)
	{
		date.SetMonth(m);
		date.SetDay(d);
		date.SetYear(y);
	}

private:
	Date date;
	double value;
};

//declare functions
vector<string> ParseFile(string file);
Account BuildAccountFromFile(string account_file);
bool DeleteAccount(string account);
void BuildNewAccount();
void EditMenu(string account);
void ViewEditAccounts();
void CreateCashLog(string account);
void WriteAcntFile(Account Acnt, bool isNew, bool master_exists);
void WriteAcntFile(string account, vector<CashLog>Log, bool isNew);
bool AccountExists(string account);
bool DateExists(int m, int d, int y, vector<CashLog> Log, string account, bool CashLogExists);

int main()
{
	cout << "Welcome to Bien Vendu\n";

	const string MAIN_PROMPT = "Enter a number to select an option: \n\t1. View/Edit Accounts\n\t2. Create New Account\nCtrl - Z to exit program\n";
	cout << MAIN_PROMPT;

	char n = '0';

	while (cin >> n)	//main menu input loop
	{
		switch (n)
		{
		case '1': ViewEditAccounts();
			cout << MAIN_PROMPT;
			break;
		case '2': BuildNewAccount();
			cout << MAIN_PROMPT;
			break;
		default: cout << "Invalid Input. Try again\n";
		}

		//So typing something like '42' won't have the program respond with default case followed by case '2'
		cin.clear();
	}
	return 0;
}

vector<string> ParseFile(string file)
{
	ifstream filestream(file);

	if (!filestream)
	{
		cout << "Can't open " << file << endl;
	}
	else
	{
		vector<string>Fields;
		string st;

		for (int i = 0; !filestream.eof(); ++i)
		{
			getline(filestream, st);

			if (filestream.eof())
				break;
			Fields.push_back(st);
		}

		filestream.close();
		return Fields;
	}

}

Account BuildAccountFromFile(string account_file)
{
	Vector<string>Fields = ParseFile(account_file);

	Account Acnt;
	Acnt.SetAccountName(Fields[0]);
	Acnt.SetNumOfMachines(atoi(Fields[1].c_str()));
	return Acnt;
}


void BuildNewAccount()
{
	//Name the account, eg "Atherton"
	cout << "\tEnter a name for the new account\n";
	string st;
	while(true)
	{ 
		cin >> st;

		if (!AccountExists(st))
			break;
		else
			cout << "Account already exists\n";
		if (cin.eof()) //if ctrl+z entered return to main menu
			return;
	}

	Account NewAccount;
	NewAccount.SetAccountName(st);
	cout << "New Account Name is " << NewAccount.GetAccountName() << endl;

	//No. of machines for account
	cout << "\tEnter the amount of machines this account has\n";

	int n;
	while (!(cin >> n))
	{
		if (cin.eof()) //if ctrl+z entered return to main menu
			return;

		cout << "Input not an integer. Try again.\n";
		cin.clear();
		cin.ignore();
	}

	if (cin.eof()) //if ctrl+z entered return to main menu
		return;

	NewAccount.SetNumOfMachines(n);
	cout << "Number of machines for " << NewAccount.GetAccountName() << " is " << NewAccount.GetNumOfMachines() << endl;

	bool master_exists;

	ifstream master(MSTR_ACNT_LIST);
	if (!master.good())
		master_exists = false;
	else
		master_exists = true;

	master.close();

	system("PAUSE");

	//Write to new account.txt file
	WriteAcntFile(NewAccount, true,master_exists);			//true means new account
}

int DaysBetween(CashLog LateLog, CashLog EarlyLog)	//Find amount of days beetween two cash logs
{
	int daystosubtract = 0;
	int totaldays = 0;
	//vector<Month>months;

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

	if ((LateLog.GetMonth() == EarlyLog.GetMonth()) && (LateLog.GetYear() == EarlyLog.GetYear()))
	{
		return LateLog.GetDay() - EarlyLog.GetDay();	//mo and day are same so return simple difference of days
	}
	
	if (EarlyLog.GetDay() > LateLog.GetDay())	//like if earlylog is 3/8/2016 and latelog is 7/5/2016 (partial month)
	{		
		daystosubtract = EarlyLog.GetDay() - LateLog.GetDay();	//subtract these days later
		//will act as if LateLog.GetDay==EarlyLog.GetDay
	}

	int monthsbetween = LateLog.GetMonth() - EarlyLog.GetMonth();	//4
	int currentmonth = EarlyLog.GetMonth();	//gonna go from earlylog to latelog

	//yearsbetween
	int yearsbetween = LateLog.GetYear() - EarlyLog.GetYear();
	monthsbetween = monthsbetween + (yearsbetween * 12);
	//yearsbetween

	string monthstring = intmonth_to_str(currentmonth);

	int currentyear = EarlyLog.GetYear();

	Month newmonth;
	
	for (int i = 0; i <monthsbetween; ++i)
	{
		if (i == 0)	//if first month we must subtract days from out starting point
		{
			totaldays += (Days[monthstring] - EarlyLog.GetDay()); //31-8 = 23 total days
		}
		if (i != 0)
		{
			totaldays += (Days[monthstring]);
		}

		if ((monthstring == "feb") && IsLeapYear(currentyear))
			totaldays++;

		monthstring = intmonth_to_str(EarlyLog.GetMonth() + (i + 1));	//monthstring is now string of month + (iterations+1)															

		if (monthstring == "jan")
			currentyear++;
	}
	//ran through up to beginning of LateLog.GetMonth()
	totaldays += EarlyLog.GetDay();
	//now we have found the days between for perfect months

	//subtract daystosubtract
	totaldays -= daystosubtract;

	return totaldays;
}

void AnalyzeCashLog(vector<string>Fields, string account)	//basically parsing the date and value of cashlogs right now
{
	//calculate average daily earnings        //and then, average in a year? in a mo?	//will need to bring the value...maybe just create cash log before
	// function call
	stringstream ss;
	string month = "";
	string day = "";
	string year = "";
	string value = "";

	stringstream convert;
	double doub = 0.0;

	vector<CashLog>Log;
	vector<double>daily;

	for (int i = 0; i < Fields.size(); ++i)
	{
		ss << Fields[i];				//put 'date value' string into stringstream ss

		getline(ss, month, '/');		//get month i.e 07

		getline(ss, day, '/');			//get day i.e. 20

		getline(ss, year, ' ');			//get year i.e. 2015
		getline(ss, value, '\n');				//get value i.e. 200.50
												//cout << "value == " << value << endl;

												//try/exception block here (invalid date)
												//Log.push_back(CashLog(account));		//before would create cash log with arbitrary date string, but lets create empty nextline
		Log.push_back(account);
		Log[i].SetDate(int_to_month(atoi(month.c_str())), atoi(day.c_str()), atoi(year.c_str()));

		convert << value;		//start converting string to double, put value into streamstring convert
		convert >> doub;		//put convert into doub

		Log[i].SetValue(doub);		//but why does it round to only use 1 decimal place?
		convert.clear();

		cout << "\t" << Log[i].GetMonth() << "/" << Log[i].GetDay() << "/" << Log[i].GetYear() << ": $" << Log[i].GetValue() << endl;

		ss.clear();

		///////////Calculate days between and money made per day/////////
		if (i == 0)		//if first Log entry skip for loop iteration
		{
			cout << endl;
			continue;
		}

		int daysbetween = DaysBetween(Log[i], Log[i - 1]);

		cout << "\tDays between last log date: " << daysbetween << endl;
		cout << "\tThe money made per day was " << Log[i].GetValue() / daysbetween << endl << endl;

		daily.push_back(daysbetween);
		///////////Calculate days between and money made per day//////////
	}

	//average daily
	double sum = 0;
	for (int i = 0; i < daily.size(); ++i)
		sum += daily[i];

	cout << "The average money made per day is " << sum / daily.size() << endl << endl;
		
	return;
}

bool DeleteAccount(string account)
{
	cout << "Are you sure you want to delete " << account << "? 'y' or 'n'\n";

	char ch = ' ';

	while (cin >> ch)
	{
		if (ch == 'y')
		{
			cout << "deleted\n";

			if (remove((account + ".txt").c_str()) != 0)
				cout << "Failed to delete " << account << endl;
			else
				cout << account << " deleted\n";

			remove((account + ".CashLog").c_str());

			//Delete entry in master account file
			//by loading entries into vector
			//deleting appropriate entry, rewriting to temp.txt
			//delete master account file, and rename temp.txt to master account file name
			vector<string>Fields = ParseFile(MSTR_ACNT_LIST);
			ofstream new_master("temp.txt");
			for (int i = 0; i < Fields.size(); ++i)
			{
				if (Fields[i] == account)
					Fields.erase(Fields.begin()+i);
			}

			if (!new_master)
			{
				cout << "Error updating master account file\n";
				return false;
			}
			else
			{
				for (int i = 0; i < Fields.size(); ++i)
				{
					if (!new_master)
					{
						cout << "Error updating master account file\n" << endl;
						return false;
					}
					else
					{
						new_master << Fields[i] << endl;
					}
				}
				new_master.close();
				remove(MSTR_ACNT_LIST.c_str());
				rename("temp.txt", MSTR_ACNT_LIST.c_str());
				return true;
			}

		}
		else if (ch == 'n')
			return false;
		else if (cin.eof())
			return false;
		else
			cout << "Invalid Input\n";
	}	
}

void DeleteByDate(vector<string>Fields,string account)
{
	cout << "Enter date as ##/##/#### to delete entry\n";
	const string CSH_LOG = account + ".CashLog";

	string st = "";
	stringstream ss;
	string fmonth;
	string fday;
	string fyear;

	string umonth;
	string uday;
	string uyear;

	int fm;
	int fd;
	int fy;

	int um;
	int ud;
	int uy;

	stringstream fs;

	while (cin >> st)
	{
		
		ss << st;						//put 'date value' string into stringstream ss
		getline(ss, umonth, '/');		//get month i.e 07
		getline(ss, uday, '/');			//get day i.e. 20
		getline(ss, uyear);			//get year i.e. 2015

		for (int i = 0; i < Fields.size(); ++i)
		{
			fs << Fields[i];
			getline(fs, fmonth, '/');		//get month i.e 07
			getline(fs, fday, '/');			//get day i.e. 20
			getline(fs, fyear, ' ');			//get year i.e. 2015
			fs.ignore(10000);
			fs.clear();

			um = atoi(umonth.c_str());
			ud = atoi(uday.c_str());
			uy = atoi(uyear.c_str());

			fm = atoi(fmonth.c_str());
			fd = atoi(fday.c_str());
			fy = atoi(fyear.c_str());

			if ((um == fm) && (ud == fd) && (uy == fy))
			{
				//delete entry and rewrite new cashlog
				Fields.erase(Fields.begin() + i);

				ofstream new_cashlog("templog.txt");

				if (!new_cashlog)
					cout << "Failed to delete entry\n";
				else
				{
					if (Fields.empty())	//no entries just delete the cashlog file
					{
						if (remove((account + ".CashLog").c_str()))
							cout << "Failed to delete Cash Log File\n";
						else
						{ 
							cout << "Entry deleted\n";
							cout << "Cash Log deleted\n";
						}
						return;
					}

					for (int i = 0; i < Fields.size(); ++i)
					{
						new_cashlog << Fields[i] << endl;
					}

					//now delete account.cashlog
					//rename templog.txt to account.cashlog
					if (remove((account + ".CashLog").c_str()) != 0)
						cout << "Failed to delete entry\n" << endl;
					else
						cout << "Entry deleted\n";

					new_cashlog.close();
					rename("templog.txt", CSH_LOG.c_str());
					return;
				}
			}

		}

		cout << "No such date found. Try again\n";
	}

	if (cin.eof())
		return;
	
}

void AddEntries(vector<string>Fields, vector<CashLog> Log, string account)
{
	//get user date input and value
	//check against m/d/y fields to determine if date exists

	stringstream ss;
	stringstream convert;
	double doub;
	string st = "";
	string month;
	string day;
	string year;
	string value;
	int d;
	int m;
	int y;

	cout << "Enter entries as ##/##/#### ####.##\n";

	while (std::cin >> st >> doub) //terminate?
	{
		ss << st;				//put 'date value' string into stringstream ss

		getline(ss, month, '/');		//get month i.e 07

		getline(ss, day, '/');			//get day i.e. 20

		getline(ss, year, ' ');			//get year i.e. 2015

		d = atoi(day.c_str());
		m = atoi(month.c_str());
		y = atoi(year.c_str());

		if (!isDate(m, d, y))
		{
			cout << "Invalid Date. Try again:\n";
			std::cin.clear();
		}
		if (DateExists(m, d, y, Log, account, true))	//true means cashlog exists
		{
			cout << "Date already exists\n";
			std::cin.clear();
		}
		else
		{
			Log.push_back(account);
			Log[Log.size() - 1].SetDate(int_to_month(m), d, y);
			Log[Log.size() - 1].SetValue(doub);
		}

		ss.clear();
	}

	if (!(std::cin >> st >> doub))  //if input does not follow format 'string double' then leave, ask to save or not
	{
		if (std::cin.eof())
		{
			if (Log.empty())
				return;
		}
		else
			cout << "Invalid Input. Reverting back to main menu...";

		if (Log.empty())
		{
			cout << endl;
			std::cin.clear();
			std::cin.ignore();
			return;
		}
		else
			cout << "Save valid entries so far? 'y' or 'n'\n";

		std::cin.clear();

		char ch = ' ';
		std::cin >> ch;

		switch (ch)
		{
		case 'y': //yes, go ahead and write
			if (Log.empty())	return;			//if Log vector is empty there is NOTHING TO WRITE, IGNORE USER AND RETURN TO MAIN MENU
			break;
		case 'n': return;
			break;
		default: cout << "Reverting without saving\n";	return;
		}
	}

	for (int i = 0; i < Fields.size(); ++i)	//add cashlog stuff to log, then sort all of it
	{
		ss << Fields[i];				//put 'date value' string into stringstream ss

		getline(ss, month, '/');		//get month i.e 07
		getline(ss, day, '/');			//get day i.e. 20
		getline(ss, year, ' ');			//get year i.e. 2015
		getline(ss, value, '\n');				//get value i.e. 200.50

		d = atoi(day.c_str());
		m = atoi(month.c_str());
		y = atoi(year.c_str());

		convert << value;		//start converting string to double, put value into streamstring convert
		convert >> doub;

		Log.push_back(account);
		Log[Log.size() - 1].SetDate(int_to_month(m), d, y);
		Log[Log.size() - 1].SetValue(doub);

		ss.clear();
	}

	//sort
	if (Log.size() > 1)
	{
		sort(Log.begin(), Log.end());
	}

	//write cash log
	WriteAcntFile(account, Log, false);
}

void EditMenu(string account)
{
	cout << "1. View/Edit/Create Cash Log File\n2. Delete Account\n";

	char ch = ' ';


	while (cin >> ch)
	{
		if (ch == '1')
			break;
		if (ch == '2')
		{ 
			if(DeleteAccount(account))
				return;
			cout << "1. View/Edit/Create Cash Log File\n2. Delete Account\n";
		}
		else
			cout << "Invalid Input\n";
	}

	if (cin.eof()) //if ctrl+z entered return to main menu
		return;

	//view/edit/create cashlog


	ifstream logfile(account + ".CashLog");
	if (logfile)	//file already exists, we must read it
	{
		logfile.close();

		//create vector of fields, eg Fields[i] = 07/24/2015 132.50
		vector<string>Fields = ParseFile(account + ".CashLog");

		//print entries
		for (int i = 0; i < Fields.size(); ++i)
		{
			cout << "Entry [" << i << "]: " << Fields[i] << endl;

		}

		cout << "1. Amounts made per day\n2. Add entries\n3. Delete entry by date\n";
		vector<CashLog> Log;
		while (cin >> ch)
		{
			switch (ch)
			{
			case '1':
				if (Fields.size() <= 1)
					cout << "Insufficient number of entries\n";
				else
					AnalyzeCashLog(Fields, account); 
				return;
				break;
			case '2':
				AddEntries(Fields,Log, account);
				return;
				break;
			case '3': 
				DeleteByDate(Fields, account);
				return;
				break;
			//case 'n': return;
				//break;
			default: cout << "Invalid Input\n";
			}
		}

		if (cin.eof()) //if ctrl+z entered return to main menu
			return;
	}
	else  //File doesn't exist, we must create based on user input
	{
		cout << "There is currently no cash log file for " << account << "\nCreate one? Enter 'y' or 'n'\n";
		char ch = ' ';

		while(cin >> ch)
		{ 
			switch (ch)
			{
			case 'y': CreateCashLog(account);
				return;
				break;
			case 'n': return;
				break;
			default: cout << "Invalid Input\n";
			}
		}

		if (cin.eof()) //if ctrl+z entered return to main menu
			return;
	}
}

bool AccountExists(string account)
{

	if (!(ifstream(MSTR_ACNT_LIST).good()))
		return false;

	vector<string>Fields = ParseFile(MSTR_ACNT_LIST);

	for (int i = 0; i < Fields.size(); ++i)
	{
		if (Fields[i] == account)
			return true;
	}
	return false;
}

bool DateExists(int m, int d, int y, vector<CashLog> Log, string account, bool CashLogExists)
{
	//check if date exists either in cashlog file or previous user input
	if(CashLogExists)
	{ 
		Vector<string>Fields = ParseFile(account + ".CashLog");
		stringstream ss;
		string month;
		string day;
		string year;
		string value;

		for (int i = 0; i < Fields.size(); ++i)	//check cashlog
		{
			ss << Fields[i];				//put 'date value' string into stringstream ss

			getline(ss, month, '/');		//get month i.e 07
			getline(ss, day, '/');			//get day i.e. 20
			getline(ss, year, ' ');			//get year i.e. 2015
			getline(ss, value, '\n');				//get value i.e. 200.50

			if ((atoi(month.c_str()) == m) && (atoi(day.c_str()) == d) && (atoi(year.c_str()) == y))
			{
				//Found duplicate date in cashlog
				return true;
			}
		}
	}
	for (int i = 0; i < Log.size(); ++i)
	{
		if ((Log[i].GetMonth() == m) && (Log[i].GetDay() == d) && (Log[i].GetYear() == y))
		{
			//found duplicate in recent user entry
			return true;
		}
	}
	return false;
}

void CreateCashLog(string account)
{
	vector<CashLog>Log;

	//will want to sort by date before saving to .cashlog file and printing results

	string st = "";
	double doub = 0.0;

	cout << "\nPlease enter the date and value for new entry in seperated by whitespace\n";
	cout << "Format should look like MM/DD/YYYY ####.##:\n";
	cout << "Ctrl+Z to exit to main menu with or without saving\n";
	stringstream ss;
	string month;
	string day;
	string year;

	int m;
	int d;
	int y;

	while (cin >> st >> doub) //terminate?
	{
		ss << st;				//put 'date value' string into stringstream ss

		getline(ss, month, '/');		//get month i.e 07

		getline(ss, day, '/');			//get day i.e. 20

		getline(ss, year, ' ');			//get year i.e. 2015

		d = atoi(day.c_str());
		m = atoi(month.c_str());
		y = atoi(year.c_str());

		if (!isDate(m, d, y))
		{
			cout << "Invalid Date. Try again:\n";
			cin.clear();
		}
		if (DateExists(m, d, y,Log,account,false))	//false means creating new cashlog
		{
			cout << "Date already exists\n";
			cin.clear();
		}
		else
		{
		Log.push_back(account);
		Log[Log.size() - 1].SetDate(int_to_month(m), d, y);
		Log[Log.size() - 1].SetValue(doub);
		}

		ss.clear();
	}

	if (!(cin >> st >> doub))  //if input does not follow format 'string double' then leave, ask to save or not
	{
		if (cin.eof())
		{
			if (Log.empty())
				return;
		}
		else
			cout << "Invalid Input. Reverting back to main menu...";

		if (Log.empty())
		{
			cout << endl;
			cin.clear();
			cin.ignore();
			return;
		}
		else
			cout << "Save valid entries so far? 'y' or 'n'\n";

		cin.clear();

		char ch = ' ';
		cin >> ch;

		switch (ch)
		{
		case 'y': //yes, go ahead and write
			if (Log.empty())	return;			//if Log vector is empty there is NOTHING TO WRITE, IGNORE USER AND RETURN TO MAIN MENU
			break;
		case 'n': return;
			break;
		default: cout << "Reverting without saving\n";	return;
		}
	}

	if(Log.size() > 1)
	{
		sort(Log.begin(), Log.end());
	}

	WriteAcntFile(account, Log, true);
}

void ViewEditAccounts()
{

	cin.clear();
	cin.ignore();

	if (!(ifstream(MSTR_ACNT_LIST).good()))
	{
		cout << "No accounts found\n";
		return;
	}

	//Retrieve list of all accounts from master list
	vector<string>accounts = ParseFile(MSTR_ACNT_LIST);

	if (accounts.empty())
	{
		cout << "No accounts found\n";
		return;
	}

	//Run through build and print all accounts from list
	cout << "\nAccounts List: \n\n";

	for (int i = 0; i < accounts.size(); ++i)
	{
		Account Acnt = BuildAccountFromFile(accounts[i] + ".txt");  //Build account from .txt...for instance one loop pass through will build Atherton from Atherton.txt
		cout << "\t" << Acnt.GetAccountName() << " has " << Acnt.GetNumOfMachines() << " machines\n";
	}

	cout << endl;

	cout << "Type the name of an Account to Edit (Preserve Capitalization):\n";
	string st = "";

	bool validAccount = false;

	while (!validAccount)	//input validation loop
	{
		cin >> st;

		for (int i = 0; i < accounts.size(); i++)
		{
			if (accounts[i] == st)	//input matches an Account, continue
			{
				EditMenu(accounts[i]);
				validAccount = true;
				break;
			}
		}

		if (!validAccount && !(cin.eof()))	//if no valid account and user didn't input ctrl+z then ask keep asking
		{
			cout << "No such account '" << st << "' found. Try Again\n";
		}
		else if (!validAccount && cin.eof())	//if user input ctrl+z return back to main menu
		{
			cin.clear();
			//cin.ignore();
			return;
		}
	}
}

void WriteAcntFile(string account, vector<CashLog>Log, bool isNew)  //For CashLog files
{
	const string CSH_LOG = account + ".CashLog";

	if (!isNew)
	{
		//write to temp, delete account + ".CashLog", rename temp to account + ."CashLog"
		ofstream new_cashlog("templog.txt");

		if (!new_cashlog)
			cout << "Failed to delete entry\n";
		else
		{
			for (int i = 0; i < Log.size(); ++i)
			{
				new_cashlog << Log[i].GetDate() << " " << Log[i].GetValue() << "\n";
			}

			//now delete account.cashlog
			//rename templog.txt to account.cashlog
			if (remove((account + ".CashLog").c_str()) != 0)
			{ 
				cout << "Failed to update cashlog file\n" << endl;
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

	ofstream logfile(account + ".CashLog");

	if (!logfile)
	{
		cout << "Error opening file\n" << endl;
		//return; //return back to main
	}
	else
	{
		for (int i = 0; i < Log.size(); ++i)
		{
			//save and print

			if (!logfile)
			{
				cout << "Error opening file\n" << endl;
				return; //return back to main
			}
			else
			{
				//Account Name, Number of Machines, etc
				logfile << Log[i].GetDate() << " " << Log[i].GetValue() << "\n";
				cout << "Entry " << i << ": " << Log[i].GetDate() << " " << Log[i].GetValue() << " saved\n";
			}
			cout << "Entries saved\n";
		}
		logfile.close();
	}
}


void WriteAcntFile(Account Acnt, bool isNew, bool master_exists)  //For writing account files
{
	ofstream writer(Acnt.GetAccountName() + ".txt");

	if (!writer)
	{
		cout << "Error opening file\n" << endl;
		//return; //return back to main
	}
	else
	{
		//Account Name, Number of Machines, etc
		writer << Acnt.GetAccountName() << "\n" << Acnt.GetNumOfMachines() << "\n";
		writer.close();
		cout << Acnt.GetAccountName() << " saved\n";

		if (isNew)  //if new account append to master list
		{
			//Add to Master Accounts list

			if(master_exists)
			{ 
				ofstream accountslist(MSTR_ACNT_LIST, ios::app);
				if (!writer)
				{
					cout << "Error opening file" << endl;
					return;
				}
				else
				{
					accountslist << Acnt.GetAccountName() << "\n";      //now saves in master list as Atherton\nRuthfield\nJohn, etc instead of Atherton.txt
					accountslist.close();
					cout << "Master accounts list saved\n";
				}
			}
			else
			{
				ofstream accountslist(MSTR_ACNT_LIST);
				if (!writer)
				{
					cout << "Error opening file" << endl;
					return;
				}
				else
				{
					accountslist << Acnt.GetAccountName() << "\n";      //now saves in master list as Atherton\nRuthfield\nJohn, etc instead of Atherton.txt
					accountslist.close();
					cout << "Master accounts list saved\n";
				}
			}
		}
	}
}
