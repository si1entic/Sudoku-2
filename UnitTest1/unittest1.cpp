#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Sudoku-2/Core.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool checkValid(int final[9][9], int row, int col,int& blanks)
{
    int value = final[row][col];
    if (value == 0)
    {
        blanks++;
        return true;
    }
        
    for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) // 检测该块是否已有该数字
        for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
            if (final[i][j] == value)
                if (!(i == row&&j == col))
                    return false;
    for (int i = 0; i < 9; i++) // 检测该行该列是否已有该数字
        if ((i != col&&final[row][i] == value) || (final[i][col] == value&&i != row))
            return false;
    return true;
}

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestGenerate1)
		{
			// TODO: 在此输入测试代码
            Core c;
            const int number = 100;
            for (int mode = 1; mode <= 3; mode++)    // 遍历三个难度
            {
                int (*result)[81] = new int[number][81];
                c.generate(number, mode, result);
                int game[9][9];
                int blanks;
                for (int i = 0; i < number; i++)    // 遍历生成的题目
                {
                    memcpy(game, result[i], sizeof(game));
                    blanks = 0;
                    for (int j = 0; j < 81; j++)
                        Assert::IsTrue(checkValid(game, j / 9, j % 9, blanks), L"合法性出错");
                    switch (mode)
                    {
                    case 1:
                        Assert::IsTrue(blanks >= 20 && blanks <= 30, L"难度1挖空范围出错");
                        break;
                    case 2:
                        Assert::IsTrue(blanks >= 31 && blanks <= 45, L"难度2挖空范围出错");
                        break;
                    case 3:
                        Assert::IsTrue(blanks >= 46 && blanks <= 55, L"难度3挖空范围出错");
                        break;
                    default:
                        break;
                    }
                }
                delete [] result;
            }
		}

        TEST_METHOD(TestGenerate2)
        {
            // TODO: 在此输入测试代码
            Core c;
            const int number = 100, lower = 20, upper = 30;
            int(*result)[81] = new int[number][81];
            c.generate(number, lower, upper, false, result);
            int game[9][9];
            int blanks;
            for (int i = 0; i < number; i++)    // 遍历生成的题目
            {
                memcpy(game, result[i], sizeof(game));
                blanks = 0;
                for (int j = 0; j < 81; j++)
                    Assert::IsTrue(checkValid(game, j / 9, j % 9, blanks), L"合法性出错");
                Assert::IsTrue(blanks >= lower && blanks <= upper, L"挖空范围出错");
            }
            delete[] result;
        }

        TEST_METHOD(TestGenerate3)
        {
            // TODO: 在此输入测试代码
            Core c;
            const int number = 100, lower = 40, upper = 55;
            int(*result)[81] = new int[number][81];
            c.generate(number, lower, upper, true, result);
            int game[9][9];
            int blanks;
            for (int i = 0; i < number; i++)    // 遍历生成的题目
            {
                memcpy(game, result[i], sizeof(game));
                blanks = 0;
                for (int j = 0; j < 81; j++)
                    Assert::IsTrue(checkValid(game, j / 9, j % 9, blanks), L"合法性出错");
                Assert::IsTrue(blanks >= lower && blanks <= upper, L"挖空范围出错");
                Assert::IsTrue(c.checkUnique(game), L"唯一性出错");
            }
            delete[] result;
        }

	};

  
}