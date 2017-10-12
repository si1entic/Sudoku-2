#include "QtGuiApplication2.h"
#include <QtWidgets/QApplication>
#include "Input.h"
int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	QtGuiApplication2 w;
	w.show();
	
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
	return a.exec();
}
