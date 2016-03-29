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

class CashLog
{
	public:
		CashLog(string str)
			:Date(str) {}
		CashLog(string str, double val)
			:Date(str), Value(val) {}
		~CashLog() {}
		

	//Member Methods
	string GetDate() { return Date; }
	void SetDate(string st) { Date = st; }

	double GetValue() { return Value; }
	void SetValue(double n) { Value = n; }

	private:
		string Date;
		double Value;

};


//functions
vector<string> LoadMasterAccountsList();
Account BuildAccountFromFile(string account_file);
void ViewAccounts();
void BuildNewAccount();
void EditMenu(string account);
void EditAccounts();
void CreateCashLog(string account);




int main()
{
	cout << "Welcome to Bien Vendu";
	cout << "\n\nEnter a number to select an option: ";
	cout << "\n\t1. View Accounts\n\t2. Create New Account\n\t3. Edit Accounts\n";

	char n = '0';
	cin >> n;

	switch (n)
	{
		case '1': ViewAccounts();
			break;
		case '2': BuildNewAccount();
			break;
		case '3': EditAccounts();
			break;
		default: cout << "Invalid Input\n";
	}
	
	cout << "Back To Main\n";



	system("PAUSE");



	return 0;
}

vector<string> LoadMasterAccountsList()				//Retrieve list of all accounts from master file and store in vector
{
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

	return accounts;
}

Account BuildAccountFromFile(string account_file)
{
	ifstream account(account_file); //open say atherton.txt  ifstream account("Atherton.txt");

	if (!account)
	{
		cout << "Error opening file" << endl;
		//return;
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
					//cout << accounts[i] << " name set as " << Acnt.GetAccountName();
					current = ""; //reset current string to null
					file_movement++;
				}
				else if (file_movement == 1)
				{
					Acnt.SetNumOfMachines(atoi(current.c_str()));
					file_movement--; //since last field right now set back to our starting point 0
									 //cout << "\n\t" << Acnt.GetAccountName() << " has " << Acnt.GetNumOfMachines() << " machines\n";
					current = ""; //reset current string to null
					account.close();
					//break;
					return Acnt;
				}
			}
		}
	}
}


void ViewAccounts()
{
	//list of all accounts.txt
	vector<string>accounts = LoadMasterAccountsList();

	//Run through all accounts from master list
	cout << "\nAccounts List: ";

	for (int i = 0; i < accounts.size(); i++)
	{
		cout << "\n" << accounts[i] << ": \n\t"; //will print say "Atherton"......note will print : once completed all accounts.txt
		Account Acnt = BuildAccountFromFile(accounts[i] + ".txt");  //Build account from .txt...for instance one loop pass through will build Atherton from Atherton.txt
		cout << "\n\t" << Acnt.GetAccountName() << " has " << Acnt.GetNumOfMachines() << " machines\n";
	}
}

void BuildNewAccount()
{
	//Name the account, eg "Atherton"
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

	//Write account information to .txt file, eg "Atherton.txt"
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
			accountslist << NewAccount.GetAccountName() << ",";      //now saves in master list as Atherton,Ruthfield,John, etc instead of Atherton.txt
			accountslist.close();
			cout << "Master accounts list saved\n";
		}
	}
}

void EditMenu(string account)
{
	//Account Acnt = BuildAccountFromFile(account + ".txt");
	ifstream logfile(account + ".CashLog");

	//note: will want to detect no of machines and select from list of them

	//check if CashLog file for account exists: Account.CashLog, eg "Atherton.CashLog"
	if (logfile)
	{
		//file already exists, READ IT

		vector<CashLog>Log;
		char chrctr; //to store info from txt one character at a time
		string current = ""; //string to store information between commas ','
		int field_movement = 0;  //how many data fields have we accumulated up to this point?

		for (int s = 0; !logfile.eof(); ++s)				//ParseText function? GetField?
		{
			logfile.get(chrctr);

			if (logfile.eof())
				break;

			if (field_movement == 4)
				cout << field_movement << field_movement;

			if (chrctr != ',') //if not comma, we are data collecting
			{
				if (chrctr == ' ') //but if whitespace then that means date is done collecting, so set date and reset current string
				{
					//date done collecting
					//Log[s].SetDate(current); push back log and set date
					Log.push_back(CashLog(current));
					current = "";
				}
				else                //not whitespace so keep building current string
				{
					current += chrctr;
				}
			}
			else if (chrctr == ',')                 //comma, ascertain that we have logged date and may now log value
			{
				//end of field, store current value in created Latest Log entry
				double doub;
				stringstream ss; //convert string to double
				ss << current;
				ss >> doub;
				Log[field_movement].SetValue(doub);
				current = "";
				field_movement++;
			}
		}

		logfile.close();

		for (int s = 0; s<Log.size(); s++)				//print all cashlog entries for account
		{
			cout << "\nEntry " << s << ": " << Log[s].GetDate() << " : $" << Log[s].GetValue();
		}


	}
	else
	{
		//file does not exist, time to CREATE IT, WRITE IT BASED ON USER INPUT
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


	}
}

void CreateCashLog(string account)
{
	//ofstream logfile(account + ".CashLog");
	
	//Account Acnt;
	vector<CashLog>Log;

	//will want to sort by date before saving to .cashlog file and printing results

	string st = "";
	double doub = 0.0;

	cout << "Please enter the date and value for new entry in seperated by whitespace\n";
	cout << "Format should look like MM/DD/YYYY ####.##:\n";
	cout << "Enter '|' to terminate entry";
	while (cin>>st>>doub) //terminate?
	{
		//cout << "Please enter the date for new entry as MM/DD/YYYY:\n";   //note: will have to detect invalid input
		Log.push_back(CashLog(st));

		//cout << "Please enter the number value of the dated entry as numbers, eg '134.35' \n";
		Log[Log.size() - 1].SetValue(doub);

	}


	ofstream logfile(account + ".CashLog");
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
			logfile << Log[i].GetDate() << " " << Log[i].GetValue() << ",";
			cout << "Entry " << i << ": " << Log[i].GetDate() << " " << Log[i].GetValue() << " saved\n";
		}
	}
	logfile.close();

}

void EditAccounts()			//reduce redundancy between EditAccounts() and ViewAccounts()?
{
	//list of all accounts.txt
	cout << "Accounts: \n";
	vector<string>accounts = LoadMasterAccountsList();

	for (int i = 0; i < accounts.size(); i++)
	{
		cout << accounts[i] << ": \n\t";
	}

	cout << "Type the name of an Account to Edit (Preserve capitalization):\n";
	string st = "";

	cin >> st;

	for (int i = 0; i < accounts.size(); i++)
	{
		if (accounts[i] == st)
		{
			EditMenu(accounts[i]);
			break;
		}
	}
}
