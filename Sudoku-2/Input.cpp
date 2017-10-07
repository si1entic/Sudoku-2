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
                handleError("参数[-c number]应满足0<number<=1000000");
            core.generate(number, 0, 0, false, result);
            Output::output(number, result);
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            ifstream fp(argv[2]);
            if (!fp.is_open())
                handleError(string(argv[2]) + "无法打开！");
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
            handleError("参数-n请结合其他参数(-m/-r/-u)使用");
        else if (strcmp(argv[1], "-m") == 0)
            handleError("参数-m请结合-n使用");
        else if (strcmp(argv[1], "-r") == 0)
            handleError("参数-r请结合-n使用");
        else if (strcmp(argv[1], "-u") == 0)
            handleError("参数-u请结合-n使用");
        else
            handleError("参数格式错误！");
        break;
    case 4:
        if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-u") == 0))
        {
            int number = getNumber(argv[2]);
            if(number==0||number>10000)
                handleError("参数[-n number]应满足0<number<=10000");
            core.generate(number, 20, 50, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-u") == 0) && (strcmp(argv[2], "-n") == 0))
        {
            int number = getNumber(argv[3]);
            if (number == 0 || number>10000)
                handleError("参数[-n number]应满足0<number<=10000");
            core.generate(number, 20, 50, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) || (strcmp(argv[2], "-r") == 0))
            handleError("参数-r请结合-n使用");
        else if ((strcmp(argv[1], "-u") == 0) || (strcmp(argv[3], "-u") == 0))
            handleError("参数-u请结合-n使用");
        break;
    case 5:
        if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-m") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int mode = getNumber(argv[4]);
            if (mode == 0 || mode > 3)
                handleError("参数[-m mode]应满足0<number<=3");
            core.generate(number, mode, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-r") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[4]);
            if(lower<20||lower>=upper||upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            core.generate(number, lower, upper, false, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-m") == 0) && (strcmp(argv[3], "-n") == 0))
        {
            int mode = getNumber(argv[2]);
            if (mode == 0 || mode > 3)
                handleError("参数[-m mode]应满足0<number<=3");
            int number = getNumber(argv[4]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            core.generate(number, mode, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) && (strcmp(argv[3], "-n") == 0))
        {
            int lower, upper;
            getRange(lower, upper, argv[2]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            int number = getNumber(argv[4]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            core.generate(number, lower, upper, false, result);
            Output::output(number, result);
        }
        else
            handleError("参数格式错误！");
        break;
    case 6:
        if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[3], "-r") == 0) &&
            (strcmp(argv[5], "-u") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[4]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-n") == 0) && (strcmp(argv[4], "-r") == 0) &&
            (strcmp(argv[3], "-u") == 0))
        {
            int number = getNumber(argv[2]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[5]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) && (strcmp(argv[3], "-n") == 0) &&
            (strcmp(argv[5], "-u") == 0))
        {
            int number = getNumber(argv[4]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[2]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-r") == 0) && (strcmp(argv[3], "-u") == 0) &&
            (strcmp(argv[4], "-n") == 0))
        {
            int number = getNumber(argv[5]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[2]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-u") == 0) && (strcmp(argv[2], "-n") == 0) &&
            (strcmp(argv[4], "-r") == 0))
        {
            int number = getNumber(argv[3]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[5]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else if ((strcmp(argv[1], "-u") == 0) && (strcmp(argv[2], "-r") == 0) &&
            (strcmp(argv[4], "-n") == 0))
        {
            int number = getNumber(argv[5]);
            if (number == 0 || number > 10000)
                handleError("参数[-n number]应满足0<number<=10000");
            int lower, upper;
            getRange(lower, upper, argv[3]);
            if (lower<20 || lower >= upper || upper>55)
                handleError("参数[-r a~b]应满足20<=a<=b<=55");
            core.generate(number, lower, upper, true, result);
            Output::output(number, result);
        }
        else
            handleError("参数格式有误！");
    default:
        handleError("参数格式有误！");
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
        handleError("参数[-r a~b]应满足20<=a<=b<=55");
    string a = s.substr(0, 2), b = s.substr(3, 2);
    for (int i = 0; i < 2; i++)
        if (!(isdigit(a[i])&&isdigit(b[i])))
            handleError("参数[-r a~b]应满足20<=a<=b<=55");
    lower = stoi(a);
    upper = stoi(b);
    return;
}

void Input::handleError(const string & s)
{
    cout << s << endl;
    exit(1);
}
