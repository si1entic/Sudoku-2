#include "stdafx.h"
#include "Input.h"

using namespace std;


int main(int argc, char *argv[]){
    srand((unsigned)time(NULL));
    int(*result)[81] = new int[1000000][81];
    Input inputHandler;
    inputHandler.handle(argc, argv, result);
    delete[] result;
	return 0;
}
