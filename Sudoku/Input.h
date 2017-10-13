#pragma once

#include "../Core/Core.h"
#include "Output.h"
#include "Sudoku.h"
#include <QtWidgets/QApplication>
#include <qdebug.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Input {
public:
	void handle(int argc, char *argv[],int result[][81]);

private:
	int getNumber(const string &s);
    void getRange(int &lower, int &upper,const string &s);
    void error(const string &str);
};

