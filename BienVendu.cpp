
#include "Account.h"
#include <iostream>
#include <vector>

using namespace std;

void menuMain();
void viewEditAccounts();
void menuNewAccount();
void menuEditAccount(string account);
void menuCreateCashLog(string account);
void menuEditCashLog(string account);
void menuAddEntries(string account);
void menuDeleteAccount(string account);
void menuDeleteByDate(string account);
void askToCreate();
void askForAccount();

int main()
{

	menuMain();
	
	return 0;
}

void menuMain()
{
	cout << "Welcome to Bien Vendu\n";

	const string MAIN_PROMPT = "Enter a number to select an option: \n\t1. View/Edit Accounts\nCtrl - Z to exit program\n";
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
		{ 
			deleteAccount(account);
			return;
		}

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

	Account newAccount(st);
	cout << "New Account Name is " << newAccount.getName() << endl;
	
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
	cout << "Number of machines for " << newAccount.getName() << " is " << newAccount.getNumOfMachines() << endl;

	writeAccountFile(newAccount);
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
	vector<Log>log;

	cout << "\nPlease enter the date and value for new entry in seperated by whitespace\n";
	cout << "Format should look like MM/DD/YYYY ####.##:\n";
	cout << "Ctrl+Z to exit to main menu with or without saving\n";

	string st = "";
	double doub = 0.0;
	while (cin >> st >> doub) //terminate?
	{
		Log userEntry = getLogFromString(st);
		userEntry.setValue(doub);

		if (!isDate(userEntry.getDate()))
		{
			cout << "Invalid Date. Try again:\n";
			cin.clear();
		}
		else if (logDateExists(userEntry.getDate(), log))
		{
			cout << "Date already exists\n";
			cin.clear();
		}
		else
		{ 
			log.push_back(userEntry);
		}

	}

	if (!(cin >> st >> doub))  //if input does not follow format 'string double' then leave, ask to save or not
	{
		if (cin.eof())
		{
			if (log.empty())
				return;
		}
		else
			cout << "Invalid Input. Reverting back to main menu...";

		if (log.empty())
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
		case 'y':
			writeLogFile(account, log);
			break;
		case 'n': 
			return;
			break;
		default: cout << "Reverting without saving\n";	return;
		}
	}
}

void menuEditCashLog(string account)
{
	if (cashLogFile(account))	//file exists, read and continue
	{
		printLogs(account);

		vector<string>fields = parseFile(account + ".CashLog");

		cout << "1. Amounts made per day\n2. Add entries\n3. Delete entry by date\n";
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
			case 'y': menuCreateCashLog(account);
				return;
				break;
			case 'n': 
				return;
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
	//check if valid date and if date entry already exists
	//save when done
	

	vector<Log> log = getLogsFromFile(account + ".CashLog");
	int fileEntriesNum = log.size();

	cout << "Enter entries as ##/##/#### ####.##\n";

	string st;
	double value;
	while (std::cin >> st >> value)
	{
		Log userEntry = getLogFromString(st);
		userEntry.setValue(value);

		if (!isDate(userEntry.getDate()))
		{
			cout << "Invalid Date. Try again:\n";
			std::cin.clear();
		}
		else if (logDateExists(userEntry.getDate(),log))
		{
			cout << "Date already exists\n";
			std::cin.clear();
		}
		else
		{
			log.push_back(userEntry);
		}
	}

	if (!(std::cin >> st >> value))  //if input does not follow format 'string double' then leave, ask to save or not
	{
		if (std::cin.eof())
		{
			if (log.size() == fileEntriesNum)
				return;
		}
		else
			cout << "Invalid Input. Reverting back to main menu...";

		if (log.size() == fileEntriesNum)
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
		case 'y':
			writeLogFile(account, log);			
			break;
		case 'n':
			return;
			break;
		default: cout << "Reverting without saving\n";	return;
		}
	}
}

void menuDeleteByDate(string account)
{
	cout << "Enter date as ##/##/#### to delete entry\n";
	const string CSH_LOG = account + ".CashLog";

	vector<Log>log = getLogsFromFile(CSH_LOG);

	string st;
	while (cin >> st)
	{
		if (cin.eof())
			return;

		Log userEntry = getLogFromString(st);
		userEntry.setAccountName(account);

		if (logDateExists(userEntry.getDate(), log))
		{
			deleteLogEntry(userEntry);
			return;
		}

		cout << "No such date found. Try again\n";
	}

	if (cin.eof())
		return;

}


