#include "stdafx.h"
#include "std_lib_facilities.h"
#include <iostream>
#include <cstdlib>
#include <map>

using namespace std;

const string MSTR_ACNT_LIST = "Accounts.txt";

class Account
{
public:
	//Constructor/Deconstructor
	Account() { /*cout << "\n\tBuilding an Account";*/ }
	~Account() { /*cout << "\n\tDestroying an Account";*/ }

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
		cout << "error";	//throw? 
	else
		return Month(x);
}

string intmonth_to_str(int x)
{
	switch (x)
	{
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
	case 12:
		return "dec";
		break;
	default: /*cout << "Error converting intmonth to string\n"*/;
	}

	int y = x % 12;
	cout << "mod" << y << endl;

	if (y == 0)
		return "dec";

	switch (y)
	{
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
	case 12:
		return "dec";
		break;
	default: cout << "Error converting intmonth to string\n";
	}
}

Month operator++(Month& m)
{
	m = (m == Month::dec) ? Month::jan : Month(int(m) + 1);
	return m;



	/*if (m == Month::dec)
	{
	m = static_cast<Month>(Month::jan);
	return m;
	}

	//return Month(m + 1);
	m = static_cast<Month>(m + 1);
	return m;*/
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

class Date
{
public:
	Date() { month = Month::jan; day = 1; year = 1900; }	//Initialize empty date as Jan 1, 1900 or 1/1/1900

	Date(Month m, int d, int y)
		:month(m), day(d), year(y)
	{
		if (!isDate(m, y, d)) throw Invalid();		//if not valid throw exception invalid
	};
	class Invalid {};

	bool isDate(int m, int d, int y)	//check if date is valid, right now just return true //make helper function
	{
		return true;
	}


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
	{

	}


	CashLog(Month m, int d, int y)	//initialize with //m, //d, //y, parse these values from string stream
	{
		Date(m, d, y);

		cout << "cashlog created. Date: " << endl;
	}

	int GetMonth() { return date.GetMonth(); }
	int GetYear() { return date.GetYear(); }
	int GetDay() { return date.GetDay(); }

	void SetDay(int n) { date.SetDay(n); }

	void SetValue(double d) { value = d; }
	double GetValue() { return value; }


	string GetDate() { return date.GetMonth() + "/" + date.GetDay() + date.GetYear(); }	//just a string of the date now
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
//void ViewAccounts();
void BuildNewAccount();
void EditMenu(string account);
void ViewEditAccounts();
void CreateCashLog(string account);
void WriteAcntFile(Account Acnt, bool isNew);
void WriteAcntFile(string account, vector<CashLog>Log, bool isNew);



void Test()		//just a place to test things
{

	Month m = Month::oct;

	/*cout << "Month: " << m << endl;
	++m;
	cout << "Month: " << m << endl;
	++m;
	cout << "Month: " << m << endl;
	++m;
	cout << "Month: " << m << endl;
	++m;
	cout << "Month: " << m << endl;
	++m;*/
	cout << "Month: " << m << endl;
	m = m + 3;
	cout << "Month: " << m << endl;


}

int main()
{
	cout << "Welcome to Bien Vendu\n";

	const string MAIN_PROMPT = "Enter a number to select an option: \n\t1. View/Edit Accounts\n\t2. Create New Account\nCtrl - Z to exit program\n";
	cout << MAIN_PROMPT;
	//Will want to merge View/Edit Accounts, no real reason to be seperate

	char n = '0';
	//cin >> n;

	while (cin >> n)	//main menu input loop
	{
		//cout << "Enter a number to select an option: \n";
		//cout << "\t1. View Accounts\n\t2. Create New Account\n\t3. Edit Accounts\n";

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
		//cin.ignore();
	}

	cout << "\nOutside Main Menu\n\n";



	system("PAUSE");



	return 0;
}

void ViewAccounts()
{
	

	return;
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
	cin >> st;

	if (cin.eof()) //if ctrl+z entered return to main menu
		return;

	Account NewAccount;
	NewAccount.SetAccountName(st);
	cout << "New Account Name is " << NewAccount.GetAccountName() << endl;

	//No. of machines for account
	cout << "\tEnter the amount of machines this account has\n";
	int n;
	cin >> n;

	if (cin.eof()) //if ctrl+z entered return to main menu
		return;

	NewAccount.SetNumOfMachines(n);
	cout << "Number of machines for " << NewAccount.GetAccountName() << " is " << NewAccount.GetNumOfMachines() << endl;

	//Write to new account.txt file
	WriteAcntFile(NewAccount, true);			//true means new account
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

														//now do if year are not the same but but months are
														//
														//
														//
	}

	//if (LateLog.GetYear() == EarlyLog.GetYear())	//months are not the same but year is
	//{
	if (EarlyLog.GetDay() > LateLog.GetDay())	//like if earlylog is 3/8/2016 and latelog is 7/5/2016 (partial month)
	{
		//daystosubtract = LateLog.GetDay() - EarlyLog.GetDay();		//subtract these days later
		daystosubtract = EarlyLog.GetDay() - LateLog.GetDay();
		LateLog.SetDay(EarlyLog.GetDay());							//set days to the same
	}

	//cout << "sdfsdfsdfsdfsdfsdfsdf\n";
	cout << "Days to subtract " << daystosubtract << endl;
	int monthsbetween = LateLog.GetMonth() - EarlyLog.GetMonth();	//4
	cout << "monthsbetween " << monthsbetween;
	int currentmonth = EarlyLog.GetMonth();	//gonna go from earlylog to latelog

	cout << "GETMONTH: " << EarlyLog.GetMonth() << endl;


	//yearsbetween
	int yearsbetween = LateLog.GetYear() - EarlyLog.GetYear();
	monthsbetween = monthsbetween + (yearsbetween * 12);
	//yearsbetween


	//int mnthsbtwn = 4;

	//string monthstring = to_string(int_to_month(currentmonth));
	string monthstring = intmonth_to_str(currentmonth);
	cout << "monthstring: " << monthstring << endl;

	int currentyear = EarlyLog.GetYear();

	Month newmonth;

	//	for (int j = 0; j <= yearsbetween;++j)
	//{ 
	//	cout << "once";
	for (int i = 0; i <monthsbetween; ++i)
	{
		//cout << "totaldays" << totaldays << endl;
		cout << Days[monthstring];


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

		cout << "totaldays" << totaldays << endl;
		monthstring = intmonth_to_str(EarlyLog.GetMonth() + (i + 1));	//monthstring is now string of month + (iterations+1)
																		//why can't it move from dec to jan?
																		//cout << monthstring;
		cout << "intmonth" << monthstring << endl;	//goes beyond 12...

		if (monthstring == "jan")
			currentyear++;
	}
	//}
	//ran through up to beginning of LateLog.GetMonth()
	totaldays += LateLog.GetDay();
	//now we have found the days between for perfect months


	//subtract daystosubtract
	totaldays -= daystosubtract;

	return totaldays;
	//}

}

void AnalyzeCashLog(vector<string>Fields, string account)	//basically parsing the date and value of cashlogs right now
{
	//calculate average daily earnings        //and then, average in a year? in a mo?	//will need to bring the value...maybe just create cash log before
	// function call
	stringstream ss;
	//string st = "";
	string month = "";
	string day = "";
	string year = "";
	string value = "";

	stringstream convert;
	double doub = 0.0;

	vector<CashLog>Log;

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
		//try/exception block here (invalid date)

		convert << value;		//start converting string to double, put value into streamstring convert
		convert >> doub;		//put convert into doub

		Log[i].SetValue(doub);		//but why does it round to only use 1 decimal place?
		convert.clear();

		cout << "month " << Log[i].GetMonth() << " day " << Log[i].GetDay() << " year " << Log[i].GetYear() << endl;
		cout << "value " << Log[i].GetValue() << endl;

		ss.clear();
	}

	//so now here we have all the cash logs assembled, as well as their values



	for (int i = 0; i < Log.size(); ++i)
	{
		if (i == 0)		//if first Log entry skip for loop iteration
			continue;

		cout << "i== " << i << endl;


		int daysbetween = DaysBetween(Log[i], Log[i - 1]);

		cout << "There are " << daysbetween << " days between " << Log[i].GetDate() << " and " << Log[i - 1].GetDate() << endl;
	}

	//average daily



	//enumerations
	//operator overload for enums within the cashlog class

	//Log[0]::jan;

	//functions needed potentially: if isLeapYear(), month day overloaded operators

}


void EditMenu(string account)
{
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

		//FUNCTION TO LOOK AT STATS!!!! Right now can analyze Mather and Rivergate...so
		AnalyzeCashLog(Fields, account);



		//New functionality: ask to add new Entries
	}
	else  //File doesn't exist, we must create based on user input
	{
		cout << "There is currently no cash log file for " << account << "\nCreate one? Enter 'y' or 'n'\n";
		char ch = ' ';
		cin >> ch;

		switch (ch)
		{
		case 'y': CreateCashLog(account);
			break;
		case 'n': return;
			break;
		default: cout << "Invalid Input\n";
		}

		if (cin.eof()) //if ctrl+z entered return to main menu
			return;
	}
}

void CreateCashLog(string account)
{
	vector<CashLog>Log;

	//will want to sort by date before saving to .cashlog file and printing results

	string st = "";
	double doub = 0.0;

	cout << "Please enter the date and value for new entry in seperated by whitespace\n";
	cout << "Format should look like MM/DD/YYYY ####.##:\n";
	cout << "Enter '|' twice to stop adding entries and save or Ctrl+Z to exit to main menu without saving\n";
	stringstream ss;
	string month;
	string day;
	string year;

	int m;
	int d;
	int y;

	while (cin >> st >> doub) //terminate?
	{
		//cout << "Please enter the date for new entry as MM/DD/YYYY:\n";   //note: will have to detect invalid input
		Log.push_back(account);


		ss << st;				//put 'date value' string into stringstream ss

		getline(ss, month, '/');		//get month i.e 07

		getline(ss, day, '/');			//get day i.e. 20

		getline(ss, year, ' ');			//get year i.e. 2015

		ss << month;
		ss >> m;
		ss << day;
		ss >> d;
		ss << year;
		ss >> y;

		Log[Log.size() - 1].SetDate(int_to_month(m), d, y);


		//cout << "Please enter the number value of the dated entry as numbers, eg '134.35' \n";
		Log[Log.size() - 1].SetValue(doub);
	}

	if (!(cin >> st >> doub))  //if input does not follow format 'string double' then leave, ask to save or not
	{
		cin.clear();
		cin.ignore();

		cout << "Invalid Input. Reverting back to main menu. Save valid entries so far? 'y' or 'n'\n";
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

		//return;
	}

	if (cin.eof())
	{
		cout << "End" << endl;
		return;
	}


	WriteAcntFile(account, Log, true);
}

void ViewEditAccounts()			//reduce redundancy between EditAccounts() and ViewAccounts()?
{

	cin.clear();
	cin.ignore();


	//Retrieve list of all accounts from master list
	vector<string>accounts = ParseFile(MSTR_ACNT_LIST);

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

void WriteAcntFile(string account, vector<CashLog>Log, bool isNew)  //For CashLog files, will want to add options append? or completely rewrite each time?
{
	//right now isNew always true (always writing a new cashlog)

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
				//return; //return back to main
			}
			else
			{
				//Account Name, Number of Machines, etc
				logfile << Log[i].GetDate() << " " << Log[i].GetValue() << "\n";
				cout << "Entry " << i << ": " << Log[i].GetDate() << " " << Log[i].GetValue() << " saved\n";
			}
		}
		logfile.close();
	}
}


void WriteAcntFile(Account Acnt, bool isNew)		//For Anct txt Files, will want to add options append? or completely rewrite each time?
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
	}
}
