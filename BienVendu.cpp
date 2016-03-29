#include "stdafx.h"
#include "std_lib_facilities.h"
#include <iostream>

using namespace std;

class Account
{
	public:
		//Constructor/Deconstructor
		Account() { /*cout << "\n\tBuilding an Account";*/ }
		~Account() { /*cout << "\n\tDestroying an Account";*/  }

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

void ViewAccounts()           //change function to LOAD accounts? Purpose.
{
	//Parse and Load master accounts list
	ifstream accountslist("Accounts.txt");

	vector<string>accounts;
	char letter;
	string current_account = "";
	for (int i = 0; !accountslist.eof(); i++)
	{
		accountslist.get(letter);

		if (letter != ',')
		{
			current_account += letter;
		}
		else
		{
			accounts.push_back(current_account);
			current_account = "";
		}
	}
	accountslist.close();


	//list accounts Run through all accounts from accounts vector
	cout << "\nAccounts List: ";
	for (int i = 0; i < accounts.size(); i++)
	{
		cout << "\n" << accounts[i] << ": \n\t"; //will print say "Atherton.txt"                //note will print : once completed all accounts.txt
		
		//open accounts.txt file
		//build class from txt file
		//print attributes

		ifstream account(accounts[i]); //open say atherton.txt  ifstream account("Atherton.txt");
		if(!account)
		{
			cout << "Error opening file" << endl;
			return;
		}
		else
		{
			Account Acnt; //start building account class member from atherton.txt

			char chrctr; //to store info from txt one character at a time
			string current = ""; //string to store information between commas ','
			int file_movement = 0; //keep track of where we are in the file, how many fields of data we have passed through and collected etc

			for (int s = 0; !account.eof(); s++)
			{
				account.get(chrctr);

				if (chrctr != ',') //if not comma keep building the current string to hold information from field
				{
					current += chrctr;
				}
				else				//if comma, end of field, use information to build class member accordingly
				{
					if (file_movement == 0)
					{
						Acnt.SetAccountName(current);
						cout << accounts[i] << " name set as " << Acnt.GetAccountName();
						current = ""; //reset current string to null
						file_movement++;
					}
					else if (file_movement == 1)
					{
						Acnt.SetNumOfMachines(atoi(current.c_str()));
						file_movement--; //since last field right now set back to our starting point 0

						cout << "\n\t" << Acnt.GetAccountName() << " has " << Acnt.GetNumOfMachines() << " machines\n";
						current = ""; //reset current string to null
						account.close();
						break;
					}
				}
			}
		}
	}
}

void BuildNewAccount()
{
	//Name the account
	cout << "\tEnter a name for the new account\n";
	string st;
	cin >> st;

	Account NewAccount;
	NewAccount.SetAccountName(st);
	cout << "New Account Name is " << NewAccount.GetAccountName() << endl;

	//No. of machines for account
	cout << "\tEnter the amount of machines this account has\n";
	int n;
	cin >> n;
	NewAccount.SetNumOfMachines(n);
	cout << "Number of machines for " << NewAccount.GetAccountName() << " is " << NewAccount.GetNumOfMachines() << endl;

	//Write account information to .txt file
	ofstream writer(NewAccount.GetAccountName() + ".txt");

	if (!writer)
	{
		cout << "Error opening file\n" << endl;
		//return; //return back to main
	}
	else
	{
		//Account Name, Number of Machines, etc
		writer << NewAccount.GetAccountName() << "," << NewAccount.GetNumOfMachines() << ",";
		writer.close();
		cout << NewAccount.GetAccountName() << " saved\n";

		//Write to Master Accounts list
		ofstream accountslist("Accounts.txt", ios::app);
		if (!writer)
		{
			cout << "Error opening file" << endl;
			return;
		}
		else
		{
			accountslist << NewAccount.GetAccountName() << ".txt,";
			accountslist.close();
			cout << "Master accounts list saved\n";
		}
	}
}

int main()
{
	cout << "Welcome to Bien Vendu";
	cout << "\n\nEnter a number to select an option: ";
	cout << "\n\t1. View Accounts\n\t2. Create New Account\n";

	char n = '0';
	cin >> n;

	switch (n)
	{
		case '1': ViewAccounts();
			break;
		case '2': BuildNewAccount();
			break;
		default: cout << "Invalid Input\n";
	}
	
	cout << "Back To Main\n";



	system("PAUSE");



	return 0;
}
