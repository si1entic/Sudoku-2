#pragma once

#include "Core.h"
#include "Exception.h"
#include "Output.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Input {
public:
	void handle(const int argc, char *argv[],int result[][81]);

private:
	int getNumber(const string &s);
    void getRange(int &lower, int &upper,const string &s);
    void error(const string &str);
};

