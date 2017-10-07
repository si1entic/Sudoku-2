#include "Input.h"

void Input::handle(const int argc, char *argv[], int result[][81])
{
    switch (argc)
    {
    case 3:
        if (strcmp(argv[1], "-c") == 0)
        {
            int number = getNumber(argv[2]);
            if (number == 0)
                handleError("����[-c number]Ӧ����0<number<=1000000");
            core.generate(number, 0, 0, false, result);
            Output::output(number, result);
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            ifstream fp(argv[2]);
            if (!fp.is_open())
                handleError(string(argv[2]) + "�޷��򿪣�");
            int count = 0;
            char c;
            while (fp.get(c))
                if (isdigit(c))
                {
                    result[count / 81][count % 81] = c-'0';
                    count++;
                }
            fp.close();
            int number = count / 81;
            for (int i = 0; i < number; i++)
                core.solve(result[i], result[i]);
            Output::output(number, result);
        }
        else if (strcmp(argv[1], "-n") == 0)
            handleError("����-n������������(-m/-r/-u)ʹ��");
        else if (strcmp(argv[1], "-m") == 0)
            handleError("����-m����-nʹ��");
        else if (strcmp(argv[1], "-r") == 0)
            handleError("����-r����-nʹ��");
        else if (strcmp(argv[1], "-u") == 0)
            handleError("����-u����-nʹ��");
        else
            handleError("������ʽ����");
        break;
    case 4:
        if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-u") == 0))
        {
            int number = getNumber(argv[2]);
            if(number==0||number>10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            core.generate(number, 20, 50, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-u") == 0) && (strcmp(argv[2], "-n") == 0))
        {
            int number = getNumber(argv[3]);
            if (number == 0 || number>10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            core.generate(number, 20, 50, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) || (strcmp(argv[2], "-r") == 0))
            handleError("����-r����-nʹ��");
        else if ((strcmp(argv[1], "-u") == 0) || (strcmp(argv[3], "-u") == 0))
            handleError("����-u����-nʹ��");
        break;
    case 5:
        if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-m") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int mode = getNumber(argv[4]);
            if (mode == 0 || mode > 3)
                handleError("����[-m mode]Ӧ����0<number<=3");
            core.generate(number, mode, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-r") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[4]);
            if(lower<20||lower>=upper||upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            core.generate(number, lower, upper, false, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-m") == 0) && (strcmp(argv[3], "-n") == 0))
        {
            int mode = getNumber(argv[2]);
            if (mode == 0 || mode > 3)
                handleError("����[-m mode]Ӧ����0<number<=3");
            int number = getNumber(argv[4]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            core.generate(number, mode, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) && (strcmp(argv[3], "-n") == 0))
        {
            int lower, upper;
            getRange(lower, upper, argv[2]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            int number = getNumber(argv[4]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            core.generate(number, lower, upper, false, result);
            Output::output(number, result);
        }
        else
            handleError("������ʽ����");
        break;
    case 6:
        if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-r") == 0) &&
            (strcmp(argv[5], "-u") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[4]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[4], "-r") == 0) &&
            (strcmp(argv[3], "-u") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[5]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) && (strcmp(argv[3], "-n") == 0) &&
            (strcmp(argv[5], "-u") == 0))
        {
            int number = getNumber(argv[4]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[2]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) && (strcmp(argv[3], "-u") == 0) &&
            (strcmp(argv[4], "-n") == 0))
        {
            int number = getNumber(argv[5]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[2]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-u") == 0) && (strcmp(argv[2], "-n") == 0) &&
            (strcmp(argv[4], "-r") == 0))
        {
            int number = getNumber(argv[3]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[5]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-u") == 0) && (strcmp(argv[2], "-r") == 0) &&
            (strcmp(argv[4], "-n") == 0))
        {
            int number = getNumber(argv[5]);
            if (number == 0 || number > 10000)
                handleError("����[-n number]Ӧ����0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[3]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else
            handleError("������ʽ����");
    default:
        handleError("������ʽ����");
        break;
    }
}

int Input::getNumber(const string & s){
    int size = s.size();
    if (size > 7)
        return 0;
    for (int i = 0; i < size; i++) {
        if (isdigit(s[i]))
            continue;
        else
            return 0;
    }
    return stoi(s);
}

void Input::getRange(int & lower, int & upper, const string & s)  // 20~55
{
    if (s.size() != 5 || s[2] != '~')
        handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
    string a = s.substr(0, 2), b = s.substr(3, 2);
    for (int i = 0; i < 2; i++)
        if (!(isdigit(a[i])&&isdigit(b[i])))
            handleError("����[-r a~b]Ӧ����20<=a<=b<=55");
    lower = stoi(a);
    upper = stoi(b);
    return;
}

void Input::handleError(const string & s)
{
    cout << s << endl;
    exit(1);
}
