#pragma once

#include "stdafx.h"

using namespace std;

class Core
{
public:
    void generate(int number, int mode, int result[][81]);
    void generate(int number, int lower, int upper, bool unique, int result[][81]);
    bool solve(int puzzle[81], int solution[81]);
private:
    void generateFinal(int final[9][9]);
    void shuffle(int array[9]);
    bool fillRow(int final[9][9], int row);
    bool fill(int final[9][9], int row, int col);
    bool checkValid(int final[9][9], int row, int col, int value);
    void digUnique(int final[9][9], int blanks);
    void digRandom(int final[9][9], int blanks);
    bool checkUnique(int final[9][9]);
    void init();
    void link(int final[9][9]);
    void insertNode(int col, int no);
    void remove(int col);
    void restore(int col);
    bool findOneSolution(int final[9][9], int select);
    bool findSolutions(int final[9][9], int select, int& solution);
};

