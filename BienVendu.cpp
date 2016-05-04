#include "stdafx.h"
#include "std_lib_facilities.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <cstdio>

using namespace std;

const string MSTR_ACNT_LIST = "Accounts.txt";

void viewEditAccounts();
void menuNewAccount();
void menuMain();
void askToCreate();

//util
bool account(string account);
bool masterFile();
bool accounts();
void writeAccountFile(Account newAccount);
void writeMasterFile(string accountName);
vector<string> parseFile(string file);

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

	if (!masterFile())
	{
		cout << "No accounts found\n";
		askToCreate();
		

		return;
	}

	if (!accounts())
	{
		cout << "No accounts found \n";
		askToCreate();
		

		return;
	}

	//Retrieve list of all accounts from master list
	vector<string>accounts = parseFile(MSTR_ACNT_LIST);

	

}

void askToCreate()
{
	cout << "Do you want to create a new account? 'y' or 'no'\n";

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

void menuNewAccount() {
	//Name the account, eg "Atherton"
	cout << "\tEnter a name for the new account\n";
	string st;
	while (true)
	{
		cin >> st;

		if (!account(st))
			break;
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
	//check if file exists
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

bool accounts()
{
	vector<string>accounts = parseFile(MSTR_ACNT_LIST);

	if (accounts.empty())
	{
		return false;
	}

	return true;
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

	writeMasterFile(newAccount.getAccuountName());


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



