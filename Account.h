#ifndef ACCOUNT_H
#define ACCOUNT_H


#include "Log.h"
#include <string>
#include <vector>


using namespace std;
const string MSTR_ACNT_LIST = "Accounts.txt";

class Account
{
public:
	Account(string str);
	string getName() const { return name; }
	int getNumOfMachines() const { return numOfMachines; }
	void setNumOfMachines(int n) { numOfMachines = n; }




private:
	string name;
	int numOfMachines;
	vector<Log> entries;

};

Account buildAccountFromFile(string file);
void printAccounts();
bool masterFile();
bool account(string account);
bool accounts();
void writeMasterFile(string accountName);
void removeFromMaster(string account);
void writeAccountFile(const Account& newAccount);
void deleteAccount(string account);

#endif
