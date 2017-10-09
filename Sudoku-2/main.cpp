#include "Input.h"

using namespace std;

int main(int argc, char *argv[]){
    int(*result)[81] = new int[1000000][81];
    Input inputHandler;
    try
    {
        inputHandler.handle(argc, argv, result);
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
        exit(1);
    }
    delete[] result;
	return 0;
}
