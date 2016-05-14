#include "Account.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>

Account::Account(string str)
{
	name = str;

	if (cashLogFile(name + ".CashLog"))
		vector<Log> entries = getLogsFromFile(name + ".CashLog");
}

Account buildAccountFromFile(string file)
{
	vector<string>fields = parseFile(file);

	Account acnt(fields[0]);
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
		cout << "\t" << acnt.getName() << " has " << acnt.getNumOfMachines() << " machines\n";
	}

	cout << endl;
}

bool masterFile()
{
	if (!(ifstream(MSTR_ACNT_LIST).good()))
		return false;

	return true;
}
bool account(string account)
{
	if (!masterFile())
		return false;

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
		return false;

	return true;
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

		masterlist << accountName << "\n";
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

		masterlist << accountName << "\n";
		masterlist.close();
		cout << "Master accounts list saved\n";
	}
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

void writeAccountFile(const Account& newAccount)
{
	ofstream writer(newAccount.getName() + ".txt");

	if (!writer)
	{
		cout << "Error opening file\n" << endl;
		return;
	}

	//Account Name, Number of Machines, etc
	writer << newAccount.getName() << "\n" << newAccount.getNumOfMachines() << "\n";
	writer.close();
	cout << newAccount.getName() << " saved\n";

	writeMasterFile(newAccount.getName());


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
