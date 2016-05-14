#ifndef ACCOUNT_H
#define ACCOUNT_H


#include "Log.h"
#include <string>
#include <vector>



const std::string MSTR_ACNT_LIST = "Accounts.txt";

class Account
{
public:
	Account(std::string str);
	std::string getName() const { return name; }
	int getNumOfMachines() const { return numOfMachines; }
	void setNumOfMachines(int n) { numOfMachines = n; }




private:
	std::string name;
	int numOfMachines;
	std::vector<Log> entries;

};

Account buildAccountFromFile(std::string file);
void printAccounts();
bool masterFile();
bool account(std::string account);
bool accounts();
void writeMasterFile(std::string accountName);
void removeFromMaster(std::string account);
void writeAccountFile(const Account& newAccount);
void deleteAccount(std::string account);

#endif
