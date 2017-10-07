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
        
    for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) // ���ÿ��Ƿ����и�����
        for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
            if (final[i][j] == value)
                if (!(i == row&&j == col))
                    return false;
    for (int i = 0; i < 9; i++) // �����и����Ƿ����и�����
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
			// TODO: �ڴ�������Դ���
            Core c;
            const int number = 100;
            for (int mode = 1; mode <= 3; mode++)    // ���������Ѷ�
            {
                int (*result)[81] = new int[number][81];
                c.generate(number, mode, result);
                int game[9][9];
                int blanks;
                for (int i = 0; i < number; i++)    // �������ɵ���Ŀ
                {
                    memcpy(game, result[i], sizeof(game));
                    blanks = 0;
                    for (int j = 0; j < 81; j++)
                        Assert::IsTrue(checkValid(game, j / 9, j % 9, blanks), L"�Ϸ��Գ���");
                    switch (mode)
                    {
                    case 1:
                        Assert::IsTrue(blanks >= 20 && blanks <= 30, L"�Ѷ�1�ڿշ�Χ����");
                        break;
                    case 2:
                        Assert::IsTrue(blanks >= 31 && blanks <= 45, L"�Ѷ�2�ڿշ�Χ����");
                        break;
                    case 3:
                        Assert::IsTrue(blanks >= 46 && blanks <= 55, L"�Ѷ�3�ڿշ�Χ����");
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
            // TODO: �ڴ�������Դ���
            Core c;
            const int number = 100, lower = 20, upper = 30;
            int(*result)[81] = new int[number][81];
            c.generate(number, lower, upper, false, result);
            int game[9][9];
            int blanks;
            for (int i = 0; i < number; i++)    // �������ɵ���Ŀ
            {
                memcpy(game, result[i], sizeof(game));
                blanks = 0;
                for (int j = 0; j < 81; j++)
                    Assert::IsTrue(checkValid(game, j / 9, j % 9, blanks), L"�Ϸ��Գ���");
                Assert::IsTrue(blanks >= lower && blanks <= upper, L"�ڿշ�Χ����");
            }
            delete[] result;
        }

        TEST_METHOD(TestGenerate3)
        {
            // TODO: �ڴ�������Դ���
            Core c;
            const int number = 100, lower = 40, upper = 55;
            int(*result)[81] = new int[number][81];
            c.generate(number, lower, upper, true, result);
            int game[9][9];
            int blanks;
            for (int i = 0; i < number; i++)    // �������ɵ���Ŀ
            {
                memcpy(game, result[i], sizeof(game));
                blanks = 0;
                for (int j = 0; j < 81; j++)
                    Assert::IsTrue(checkValid(game, j / 9, j % 9, blanks), L"�Ϸ��Գ���");
                Assert::IsTrue(blanks >= lower && blanks <= upper, L"�ڿշ�Χ����");
                Assert::IsTrue(c.checkUnique(game), L"Ψһ�Գ���");
            }
            delete[] result;
        }

	};

  
}