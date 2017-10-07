#include "Output.h"

static char str[163000001];    // (18 * 9 + 1)*1000000+1
static int index = 0;

void Output::output(int number, int result[][81])
{
    for (int i = 0; i < number; i++)
    {
        for (int j = 0; j < 81; j++)
        {
            str[index++] = result[i][j] + '0';
            str[index++] = ' ';
            if ((j + 1) % 9 == 0)
                str[index++] = '\n';
        }
        str[index++] = '\n';
    }
    FILE* fp = fopen("sudoku.txt", "wt");
    if (fp == NULL)  
    {
        cout << "无法打开sudoku.txt文件" << endl;
        exit(1);  
    }
    fwrite(str, 1, index, fp);
    fclose(fp);
}
