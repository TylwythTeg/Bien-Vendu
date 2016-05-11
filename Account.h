#pragma once


#include "Log.h"
#include "std_lib_facilities.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <map>
#include <cstdio>

using namespace std;


class Account
{
public:
	Account(string str)
	{
		name = str;

		if (cashLogFile(name + ".CashLog"))
			vector<Log> entries = getEntriesFromFile(name + ".CashLog");
	}
	string getName() { return name; }
	int getNumOfMachines() { return numOfMachines; }

	void setNumOfMachines(int n) { numOfMachines = n; }




private:
	string name;
	int numOfMachines;
	vector<Log> entries;

};
