#pragma once

#include "stdafx.h"
#include "Core.h"
#include "Output.h"

using namespace std;

class Core;

class Input {
public:
	void handle(const int argc, char *argv[],int result[][81]);

private:
    Core core;
	int getNumber(const string &s);
    void getRange(int &lower, int &upper,const string &s);
    void handleError(const string &s);
};

