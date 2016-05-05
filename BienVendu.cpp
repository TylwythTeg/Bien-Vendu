#include "stdafx.h"
#include "std_lib_facilities.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <cstdio>
#include <tuple> 

using namespace std;

const string MSTR_ACNT_LIST = "Accounts.txt";

void viewEditAccounts();
void menuNewAccount();
void menuEditAccount(string account);
void menuCreateCashLog(string account);
void menuEditCashLog(string account);
void menuAddEntries(string account);
void menuMain();
void menuDeleteAccount(string account);
void menuDeleteByDate(string account);
void askToCreate();
void askForAccount();

//util
bool account(string account);
bool cashLogFile(string account);
bool masterFile();
bool accounts();
bool isDate(int m, int d, int y);
bool leapYear(int year);
bool logDateExists(int m, int d, int y, vector<CashLog> Log);
Account buildAccountFromFile(string account);
void printAccounts();
void printLogs(string account);
void analyzeCashLog(string account);
int daysBetween(CashLog lateLog, CashLog earlyLog);
void deleteAccount(string account);
void deleteLogEntry(int m,int d,int y, Log);
void removeFromMaster(string account);
void writeAccountFile(Account newAccount);
void writeLogFile(string account, vector<CashLog>Log);
void writeMasterFile(string accountName);
vector<string> parseFile(string file);
tuple<int, int, int, double>getLogFromString(string st);


int main()
{

	menuMain();
	
	return 0;
}

void menuMain()
{
	cout << "Welcome to Bien Vendu\n";

	const string MAIN_PROMPT = "Enter a number to select an option: \n\t1. View/Edit Accounts\n\t2. Create New Account\nCtrl - Z to exit program\n";
	cout << MAIN_PROMPT;

	char n;
	while (cin >> n)
	{
		switch (n)
		{
		case '1': viewEditAccounts();
			cout << MAIN_PROMPT;
			break;
		default: cout << "Invalid Input. Try again\n";
		}

		//So typing something like '42' won't have the program respond with default case followed by case '2'
		cin.clear();
	}
}

void viewEditAccounts()
{
	cin.clear();
	cin.ignore();

	if ( !masterFile() || !accounts() )
	{
		cout << "There were no accounts found\n";
		askToCreate();
		return;
	}

	printAccounts();
	askForAccount();

}

void askToCreate()
{
	cout << "Do you want to create a new account? 'y' or 'n'\n";

	char n;
	while (cin >> n)
	{
		switch (n)
		{
		case 'y': 
			menuNewAccount();
			break;
		case 'n':
			return;
			break;
		default: cout << "Invalid Input. Try again\n";
		}

		cin.clear();
	}
	return;
}

void menuDeleteAccount(string account)
{
	cout << "Are you sure you want to delete " << account << "? 'y' or 'n'\n";

	char ch = ' ';

	while (cin >> ch)
	{
		if (ch == 'y')
			deleteAccount(account);

		else if (ch == 'n' || cin.eof())
			return;
		else
			cout << "Invalid Input\n";
	}
}

void askForAccount()
{
	vector<string>accounts = parseFile(MSTR_ACNT_LIST);

	cout << "Type the name of an Account to Edit (Preserve Capitalization):\n"
		<< "or type 'new' to create a new account:\n";
		string st = "";

	bool validAccount = false;
	while (!validAccount)	//input validation loop
	{
		cin >> st;

		for (int i = 0; i < accounts.size(); i++)
		{
			if (accounts[i] == st)	//input matches an Account, continue
			{
				menuEditAccount(accounts[i]);
				validAccount = true;
				break;
			}
		}

		if (!validAccount && !(cin.eof()))	//if no valid account and user didn't input ctrl+z then ask keep asking
		{
			if ((st == "new") || (st == "New") || (st == "NEW"))
				menuNewAccount();
			else
				cout << "No such account '" << st << "' found. Try Again\n";
		}
		else if (!validAccount && cin.eof())	//if user input ctrl+z return back to main menu
		{
			cin.clear();
			return;
		}
	}
}

void menuNewAccount() {
	//Name the account, eg "Atherton"
	cout << "\tEnter a name for the new account\n";
	string st;
	while (true)
	{
		cin >> st;

		if (!account(st) && st != "new")	//create function for case neutrality
			break;
		else if (st == "new")
		{
			cout << "Invalid name\n";
		}
		else
			cout << "Account already exists\n";

		if (cin.eof()) //if ctrl+z entered return to main menu
			return;
	}

	Account newAccount;
	newAccount.setAccountName(st);
	cout << "New Account Name is " << newAccount.getAccountName() << endl;
	
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

	if (cin.eof())
		return;

	newAccount.setNumOfMachines(n);
	cout << "Number of machines for " << newAccount.getAccountName() << " is " << newAccount.getNumOfMachines() << endl;

	void writeAccountFile(newAccount);
}

void menuEditAccount(string account)
{
	cout << "1. View/Edit/Create Cash Log File\n2. Delete Account\n";

	char ch = ' ';


	while (cin >> ch)
	{
		if (ch == '1')
		{ 
			menuEditCashLog(account);
			return;
		}
		if (ch == '2')
		{
			menuDeleteAccount(account);
			return;
		}
		else
			cout << "Invalid Input\n";
	}

	if (cin.eof())
		return;
}

void menuCreateCashLog(string account)
{
	vector<CashLog>Log;

	//will want to sort by date before saving to .cashlog file and printing results

	cout << "\nPlease enter the date and value for new entry in seperated by whitespace\n";
	cout << "Format should look like MM/DD/YYYY ####.##:\n";
	cout << "Ctrl+Z to exit to main menu with or without saving\n";

	int m, d, y;
	string st = "";
	double doub = 0.0;
	while (cin >> st >> doub) //terminate?
	{
		tuple<int, int, int, double> tupleLog = getLogFromString(st);

		d = get<0>(tupleLog);
		m = get<1>(tupleLog);
		y = get<2>(tupleLog);

		if (!isDate(m, d, y))
		{
			cout << "Invalid Date. Try again:\n";
			cin.clear();
		}
		if (logDateExists(m, d, y, Log))
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

	writeLogFile(account, Log);
}

void menuEditCashLog(string account)
{
	if (cashLogFile(account))	//file exists, read and continue
	{
		printLogs(account);

		vector<string>fields = parseFile(account + ".CashLog");////////////////

		cout << "1. Amounts made per day\n2. Add entries\n3. Delete entry by date\n";
		//vector<CashLog> Log;//////////////////
		char ch = ' ';
		while (cin >> ch)
		{
			switch (ch)
			{
			case '1':
				if (fields.size() <= 1)
					cout << "Insufficient number of entries\n";
				else
					analyzeCashLog(account);
				return;
				break;
			case '2':
				menuAddEntries(account);
				return;
				break;
			case '3':
				menuDeleteByDate(account);
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

		while (cin >> ch)
		{
			switch (ch)
			{
			case 'y': createCashLog(account);
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

void menuAddEntries(string account)
{
	//get user date input and value
	//check against m/d/y fields to determine if date exists
	double value;
	
	//entries from upcoming user input
	vector<CashLog> Log;
	int m, d, y;


	//Store entries both from user input and those existing in logfile in cashlog vector
	vector<string>fields = parseFile(account + ".CashLog");
	for (int i = 0; i < fields.size; ++i)
	{
		tuple<int, int, int, double> tupleLog = getLogFromString(fields[i]);
		m = get<0>(tupleLog);
		d = get<1>(tupleLog);
		y = get<2>(tupleLog);
		value = get<3>(tupleLog);

		Log[i].setValue(value);


		Log.push_back(account);
		Log[i].setDate(int_to_month(m), d, y);
	}

	cout << "Enter entries as ##/##/#### ####.##\n";

	string st;
	double doub;
	while (std::cin >> st >> doub) //terminate?
	{	
		tuple<int, int, int, double> tupleLog = getLogFromString(st);

		d = get<0>(tupleLog);
		m = get<1>(tupleLog);
		y = get<2>(tupleLog);

		if (!isDate(m, d, y))
		{
			cout << "Invalid Date. Try again:\n";
			std::cin.clear();
		}
		if (logDateExists(m, d, y,Log))	//true means cashlog exists
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

	//write cash log
	writeLogFile(account, Log);
}

//util
bool masterFile()
{
	if (!(ifstream(MSTR_ACNT_LIST).good()))
	{
		return false;
	}
	return true;
}
bool account(string account)
{
	//check if master file exists
	if (!(ifstream(MSTR_ACNT_LIST).good()))
		return false;

	//check file fields
	vector<string>fields = parseFile(MSTR_ACNT_LIST);

	for (int i = 0; i < fields.size(); ++i)
	{
		if (fields[i] == account)
			return true;
	}
	return false;
}

bool cashLogFile(string account)
{
	//does logfile exist?
	ifstream logfile(account + ".CashLog");
	if (logfile)	
		return true;
	else
		return false;
}

bool accounts()
{
	vector<string>accounts = parseFile(MSTR_ACNT_LIST);

	if (accounts.empty())
	{
		return false;
	}

	return true;
}

bool isDate(int m, int d, int y)
{
	if (m >= 1 && m <= 12)
	{
		if (d <= 31 && d >= 1)
		{
			if ((m == 2 && d <= 28) || (m == 2 && d == 29 && leapYear(y)))
				return true;

			if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
				return true;

			if ((m == 4 || m == 6 || m == 9 || m == 11) && d <= 30)
				return true;
		}
	}
	return false;
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

bool logDateExists(int m, int d, int y, vector<CashLog> Log)
{
	for (int i = 0; i < Log.size(); ++i)
	{
		if ((Log[i].getMonth() == m) && (Log[i].getDay() == d) && (Log[i].getYear() == y))
		{
			//found duplicate in log vector
			return true;
		}
	}
	return false;
}

Account buildAccountFromFile(string account)
{
	Vector<string>fields = parseFile(account);

	Account acnt;
	acnt.setAccountName(fields[0]);
	acnt.setNumOfMachines(atoi(fields[1].c_str()));
	return acnt;
}

void printAccounts()
{
	vector<string>accounts = parseFile(MSTR_ACNT_LIST);

	cout << "\nAccounts List: \n\n";

	for (int i = 0; i < accounts.size(); ++i)
	{
		Account acnt = buildAccountFromFile(accounts[i] + ".txt");  //Build account from .txt...for instance one loop pass through will build Atherton from Atherton.txt
		cout << "\t" << acnt.getAccountName() << " has " << acnt.getNumOfMachines() << " machines\n";
	}

	cout << endl;
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

	vector<CashLog>Log;
	vector<string>fields = parseFile(account + ".CashLog");
	int m, d, y;
	double value;

	for (int i = 0; i < fields.size(); ++i)
	{
		tuple<int, int, int, double> tupleLog = getLogFromString(fields[i]);
		m = get<0>(tupleLog);
		d = get<1>(tupleLog);
		y = get<2>(tupleLog);
		value = get<3>(tupleLog);


		Log.push_back(account);
		Log[i].setDate(int_to_month(m), d, y);		

		Log[i].setValue(value);

		cout << "\t" << Log[i].getMonth() << "/" << Log[i].getDay() << "/" << Log[i].getYear() << ": $" << Log[i].getValue() << endl;

		///////////Calculate days between and money made per day/////////
		if (i == 0)		//if first Log entry skip for loop iteration
		{
			cout << endl;
			continue;
		}

		int daysbetween = daysBetween(Log[i], Log[i - 1]);

		cout << "\tDays between last log date: " << daysbetween << endl;
		cout << "\tThe money made per day was " << Log[i].getValue() / daysbetween << endl << endl;

		daily.push_back(daysbetween);
		///////////Calculate days between and money made per day//////////
	}

	//average daily
	vector<double>dailyValue;
	double sum = 0;
	for (int i = 0; i < dailyValue.size(); ++i)
		sum += dailyValue[i];

	cout << "The average money made per day is " << sum / dailyValue.size() << endl << endl;

	return;
}

int daysBetween(CashLog lateLog, CashLog earlyLog)	//Find amount of days beetween two cash logs
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

	string monthstring = intmonth_to_str(currentmonth);

	int currentyear = earlyLog.getYear();

	Month newmonth;

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

		monthstring = intmonth_to_str(earlyLog.getMonth() + (i + 1));	//monthstring is now string of month + (iterations+1)															

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

void deleteAccount(string account)
{
	if (remove((account + ".txt").c_str()) != 0) 
	{
	 
		cout << "Failed to delete " << account << "..." << endl;
		return;
	}
	else
		cout << account << " file deleted\n";

	remove((account + ".CashLog").c_str());

	removeFromMaster(account);
}

void deleteLogEntry(int m,int d,int y, Log)
{
	const string CSH_LOG = Log.GetAccountName() + ".CashLog";
	vector<string>fields = parseFile(CSH_LOG);

	for (int i = 0; i < Log.size(); ++i)
	{
		if ((Log[i].getMonth() == m) && (Log[i].getDay() == d) && (Log[i].getYear() == y))
		{
			//found duplicate in log vector
			ofstream new_cashlog("templog.txt");

			if (!new_cashlog)
			{
				cout << "Failed to delete entry\n";
				return;
			}
			
			vector<string>fields = parseFile(CSH_LOG);
			if (fields.empty())	//no entries just delete the cashlog file
			{
				if (remove((CSH_LOG).c_str()))
					cout << "Failed to delete Cash Log File\n";
				else
				{
					cout << "Entry deleted\n";
					cout << "Cash Log deleted\n";
				}
				return;
			}

			for (int i = 0; i < fields.size(); ++i)
			{
				new_cashlog << fields[i] << endl;
			}

			//now delete account.cashlog
			//rename templog.txt to account.cashlog
			if (remove((CSH_LOG).c_str()) != 0)
				cout << "Failed to delete entry\n" << endl;
			else
				cout << "Entry deleted\n";

			new_cashlog.close();
			rename("templog.txt", CSH_LOG.c_str());
			return;
			
		}
	}


}

void menuDeleteByDate(string account)
{
	cout << "Enter date as ##/##/#### to delete entry\n";
	const string CSH_LOG = account + ".CashLog";

	vector<CashLog>Log;
	int m, d, y;
	double value;

	//Store entries both from user input and those existing in logfile in cashlog vector
	vector<string>fields = parseFile(CSH_LOG);
	for (int i = 0; i < fields.size; ++i)
	{
		tuple<int, int, int, double> tupleLog = getLogFromString(fields[i]);
		m = get<0>(tupleLog);
		d = get<1>(tupleLog);
		y = get<2>(tupleLog);
		value = get<3>(tupleLog);

		Log[i].setValue(value);


		Log.push_back(account);
		Log[i].setDate(int_to_month(m), d, y);
	}

	string st;
	while (cin >> st)
	{
		if (cin.eof())
			return;

		tuple<int, int, int, double> inputDate = getLogFromString(st);

		m = get<0>(inputDate);
		d = get<1>(inputDate);
		y = get<2>(inputDate);

		if (logDateExists(m, d, y, Log))
		{
			deleteLogEntry(m, d, y, Log);
			return;
		}

		cout << "No such date found. Try again\n";
	}

	if (cin.eof())
		return;

}

void removeFromMaster(string account)
{
	/*
	Delete entry in master account file
	by loading entries into vector
	deleting appropriate entry, rewriting to temp.txt
	delete master account file, and rename temp.txt to master account file name
	*/
	vector<string>fields = parseFile(MSTR_ACNT_LIST);

	ofstream new_master("temp.txt");
	for (int i = 0; i < fields.size(); ++i)
	{
		if (fields[i] == account)
			fields.erase(fields.begin() + i);
	}

	if (!new_master)
	{
		cout << "Error updating master account file. Account not fully deleted.\n";
		return;
	}

	for (int i = 0; i < fields.size(); ++i)
	{
		if (!new_master)
		{
			cout << "Error updating master account file. Account not deleted.\n" << endl;
			return;
		}
		else
		{
			new_master << fields[i] << endl;
		}
	}
	new_master.close();
	remove(MSTR_ACNT_LIST.c_str());
	rename("temp.txt", MSTR_ACNT_LIST.c_str());
}

void writeAccountFile(Account newAccount)
{
	ofstream writer(newAccount.getAccountName() + ".txt");

	if (!writer)
	{
		cout << "Error opening file\n" << endl;
		return;
	}

	//Account Name, Number of Machines, etc
	writer << newAccount.getAccountName() << "\n" << newAccount.getNumOfMachines() << "\n";
	writer.close();
	cout << newAccount.GetAccountName() << " saved\n";

	writeMasterFile(newAccount.getAccountName());


}

void writeLogFile(string account, vector<CashLog>Log)
{
	const string CSH_LOG = account + ".CashLog";

	//sort logs by date
	if (Log.size() > 1)
	{
		sort(Log.begin(), Log.end());
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
			for (int i = 0; i < Log.size(); ++i)
			{
				new_cashlog << Log[i].getDate() << " " << Log[i].getValue() << "\n";
			}

			//now delete account.cashlog
			//rename templog.txt to account.cashlog
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
		for (int i = 0; i < Log.size(); ++i)
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
				logfile << Log[i].getDate() << " " << Log[i].getValue() << "\n";
				cout << "Entry " << i << ": " << Log[i].getDate() << " " << Log[i].getValue() << " saved\n";
			}
			
		}
		cout << "All entries saved...\n";
		logfile.close();
	}
}

void writeMasterFile(string accountName)
{

	if (masterFile())
	{
		ofstream masterlist(MSTR_ACNT_LIST, ios::app);
		if (!masterlist)
		{
			cout << "Error opening file" << endl;
			return;
		}

		masterlist << newAccount.GetAccountName() << "\n";      //now saves in master list as Atherton\nRuthfield\nJohn, etc instead of Atherton.txt
		masterlist.close();
		cout << "Master accounts list saved\n";
	}
	else
	{ 
		ofstream masterlist(MSTR_ACNT_LIST);
		if (!masterlist)
		{
			cout << "Error opening file" << endl;
			return;
		}

		masterlist << newAccount.GetAccountName() << "\n";      //now saves in master list as Atherton\nRuthfield\nJohn, etc instead of Atherton.txt
		masterlist.close();
		cout << "Master accounts list saved\n";
	}
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

tuple<int, int, int, double>getLogFromString(string st)
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

	int d = atoi(day.c_str());
	int m = atoi(month.c_str());
	int y = atoi(year.c_str());

	return make_tuple(m, d, y, value);

}
