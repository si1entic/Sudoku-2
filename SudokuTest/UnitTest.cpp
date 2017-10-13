#include "stdafx.h"
#include "../Project3/Core.h"
#include <iostream>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;
using namespace std;

const int MAXROW = 9 * 9 * 9;
const int MAXCOL = 1 + 9 * 9 * 4;
const int NUM = MAXCOL + MAXROW * 4;	// 总元素个数,  
                                        // 第一个为Head元素,接着9*9*4个为列标元素,最后9*9*9*4个为“1”元素
int g_left[NUM], g_right[NUM], g_up[NUM], g_down[NUM];    // 每个元素的4个方向分量（相当于链表中的箭头）
int g_col[NUM];        // 记录每个元素的列标元素
int g_row[NUM];        // 记录每个元素所在的01矩阵行数
int g_size[MAXCOL];    // 记录每列的“1”元素个数
int g_head[MAXROW];    // 记录每行第一个“1”元素
int g_no;				// 元素编号

void init()
{
    memset(g_left, 0, sizeof(g_left));
    memset(g_right, 0, sizeof(g_right));
    memset(g_up, 0, sizeof(g_up));
    memset(g_down, 0, sizeof(g_down));
    memset(g_col, 0, sizeof(g_col));
    memset(g_row, 0, sizeof(g_row));
    memset(g_size, 0, sizeof(g_size));
    memset(g_head, 0, sizeof(g_head));
}

void insertNode(int col, int no)     /* 将no号元素添加到col列里 */
{
    /* 在元素col和Down[col]之间插入no */
    g_up[g_down[col]] = no;
    g_down[no] = g_down[col];
    g_up[no] = col;
    g_down[col] = no;
    g_size[col]++;
    g_col[no] = col;
}

void remove(int col)
{
    /* 移除col列标元素 */
    g_left[g_right[col]] = g_left[col];
    g_right[g_left[col]] = g_right[col];
    /* 移除col列“1”元素与其同行元素 */
    for (int i = g_down[col]; i != col; i = g_down[i])   // 向下删
    {
        for (int j = g_right[i]; j != i; j = g_right[j]) // 向右删
        {
            g_up[g_down[j]] = g_up[j];
            g_down[g_up[j]] = g_down[j];
            g_size[g_col[j]]--;
        }
    }
}

void restore(int col)
{
    /* 恢复col列标元素 */
    g_left[g_right[col]] = g_right[g_left[col]] = col;
    /* 恢复col列“1”元素与其同行元素 */
    for (int i = g_up[col]; i != col; i = g_up[i])   // 恢复顺序与移除顺序相反
    {
        for (int j = g_left[i]; j != i; j = g_left[j])
        {
            g_up[g_down[j]] = g_down[g_up[j]] = j;
            g_size[g_col[j]]++;
        }
    }
}

void link(int final[9][9])
{
    /* 链接列标元素 */
    for (int i = 0; i < MAXCOL; i++)
    {
        g_left[i] = i - 1;
        g_right[i] = i + 1;
        g_up[i] = g_down[i] = i;
        g_row[i] = 0;
        g_col[i] = i;
        g_size[i] = 0;
    }
    /* 链接Head元素 */
    g_left[0] = MAXCOL - 1;
    g_right[MAXCOL - 1] = 0;
    /* 链接“1”元素 */
    g_no = MAXCOL;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++) // 遍历9x9数独
        {
            int k = final[i][j];
            if (k)
            {
                for (int t = 1; t <= 4; t++)  // 每个非0数字会在01矩阵中产生4个“1”元素
                {
                    g_left[g_no + t] = g_no + t - 1;
                    g_right[g_no + t] = g_no + t + 1;
                    g_row[g_no + t] = i * 81 + j * 9 + k - 1;
                }
                g_left[g_no + 1] = g_no + 4;
                g_right[g_no + 4] = g_no + 1;
                g_head[g_row[g_no + 1]] = g_no + 1;
                /* 将4个元素插入列链中 */
                insertNode(i * 9 + j + 1, g_no + 1);
                insertNode(81 + i * 9 + k, g_no + 2);
                insertNode(81 * 2 + j * 9 + k, g_no + 3);
                insertNode(81 * 3 + (i / 3 * 3 + j / 3) * 9 + k, g_no + 4);
                g_no += 4;
            }
            else  // 该位置为0，即待填
            {
                for (int k = 1; k <= 9; k++)        // 构造9种填法
                {
                    for (int t = 1; t <= 4; t++)    // 生成并链接它们的元素
                    {
                        g_left[g_no + t] = g_no + t - 1;
                        g_right[g_no + t] = g_no + t + 1;
                        g_row[g_no + t] = i * 81 + j * 9 + k - 1;
                    }
                    g_left[g_no + 1] = g_no + 4;
                    g_right[g_no + 4] = g_no + 1;
                    g_head[g_row[g_no + 1]] = g_no + 1;
                    insertNode(i * 9 + j + 1, g_no + 1);
                    insertNode(81 + i * 9 + k, g_no + 2);
                    insertNode(81 * 2 + j * 9 + k, g_no + 3);
                    insertNode(81 * 3 + (i / 3 * 3 + j / 3) * 9 + k, g_no + 4);
                    g_no += 4;
                }
            }
        }
    }
}

bool findSolutions(int final[9][9], int select, int& solution)
{
    if (select > 81)
    {
        if (++solution == 1) // 已求出的解个数
            return false;
        throw(1);
    }
    /* 遍历列标元素，选一个元素最少的列（回溯率低） */
    int col = 0;
    int min = INT_MAX;
    for (int i = g_right[0]; i != 0; i = g_right[i])
    {
        int size = g_size[i];
        if (size == 0)
            return false;
        if (size < min) {
            min = size;
            col = i;
        }
    }
    remove(col);
    /* 遍历该列各“1”元素,逐行依次尝试,失败则恢复已移除的结点 */
    for (int i = g_down[col]; i != col; i = g_down[i])
    {
        int row = g_row[i];
        final[row / 81][row % 81 / 9] = row % 9 + 1;    // 根据该元素的行填入数独
        for (int j = g_right[i]; j != i; j = g_right[j])    // 移除与该元素同行元素的列 
            remove(g_col[j]);
        if (findSolutions(final, select + 1, solution))    // 已选行数+1，递归
            return true;
        for (int j = g_left[i]; j != i; j = g_left[j])    // 返回false则恢复刚才的移除，顺序相反
            restore(g_col[j]);
    }
    restore(col);
    return false;
}

bool checkUnique(int final[9][9])
{
    init();
    link(final);
    int select = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int k = final[i][j];
            if (k)  // 代表数独题目已给数字的行必选
            {
                int row = i * 81 + j * 9 + k - 1;
                select++;
                remove(g_col[g_head[row]]);     // 移除该行首元素及其相关元素
                for (int u = g_right[g_head[row]]; u != g_head[row]; u = g_right[u])
                    remove(g_col[u]);         // 移除该行其他元素及其相关元素
            }
        }
    }
    int solution = 0;
    try
    {
        findSolutions(final, select + 1, solution);
        return true;
    }
    catch (int)
    {
        return false;
    }
}


bool checkValid(int final[9][9], int row, int col, int& blanks)
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

namespace SudokuTest
{
	[TestClass]
	public ref class UnitTest
	{
	private:
		TestContext^ testContextInstance;

	public: 
		/// <summary>
		///获取或设置测试上下文，该上下文提供
		///有关当前测试运行及其功能的信息。
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		// 编写测试时，可以使用以下附加特性: 
		//
		// 在运行类中的第一个测试之前使用 ClassInitialize 运行代码
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		// 在类中的所有测试都已运行之后使用 ClassCleanup 运行代码
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//在运行每个测试之前，使用 TestInitialize 来运行代码
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//在每个测试运行完之后，使用 TestCleanup 来运行代码
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 

		[TestMethod]
		void TestGenerate1()
		{
            Core c;
            const int number = 100;
            for (int mode = 1; mode <= 3; mode++)    // 遍历三个难度
            {
                int result[number][81];
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
            }
		};

        [TestMethod]
        void TestGenerate2()
        {
            Core c;
            const int number = 100, lower = 20, upper = 30;
            int result[number][81];
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
        };

        [TestMethod]
        void TestGenerate3()
        {
            Core c;
            const int number = 100, lower = 40, upper = 55;
            int result[number][81];
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
                Assert::IsTrue(checkUnique(game), L"唯一性出错");
            }
        };

        [TestMethod]
        void TestSolve()
        {
            Core c;
            int puzzle[1][81];
            int final[9][9];
            int blanks = 0;

            c.generate(1, 1, puzzle);
            Assert::IsTrue(c.solve(puzzle[0], puzzle[0]), L"求解失败");
            memcpy(final, puzzle, sizeof(final));
            for (int j = 0; j < 81; j++)
                Assert::IsTrue(checkValid(final, j / 9, j % 9, blanks), L"合法性出错");

            c.generate(1, 2, puzzle);
            Assert::IsTrue(c.solve(puzzle[0], puzzle[0]), L"求解失败");
            memcpy(final, puzzle, sizeof(final));
            for (int j = 0; j < 81; j++)
                Assert::IsTrue(checkValid(final, j / 9, j % 9, blanks), L"合法性出错");

            c.generate(1, 3, puzzle);
            Assert::IsTrue(c.solve(puzzle[0], puzzle[0]), L"求解失败");
            memcpy(final, puzzle, sizeof(final));
            for (int j = 0; j < 81; j++)
                Assert::IsTrue(checkValid(final, j / 9, j % 9, blanks), L"合法性出错");

            c.generate(1, 20, 55, false, puzzle);
            Assert::IsTrue(c.solve(puzzle[0], puzzle[0]), L"求解失败");
            memcpy(final, puzzle, sizeof(final));
            for (int j = 0; j < 81; j++)
                Assert::IsTrue(checkValid(final, j / 9, j % 9, blanks), L"合法性出错");

            c.generate(1, 50, 55, true, puzzle);
            Assert::IsTrue(c.solve(puzzle[0], puzzle[0]), L"求解失败");
            memcpy(final, puzzle, sizeof(final));
            for (int j = 0; j < 81; j++)
                Assert::IsTrue(checkValid(final, j / 9, j % 9, blanks), L"合法性出错");
        };

        [TestMethod]
        void TestNumberException()
        {
            Core c;
            int result[1][81];
            try
            {
                c.generate(-1, 1, result);  // number传入-1
                Assert::Fail(L"number范围出错");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(INT_MAX, 30, 40, true, result); // number传入最大int值
                Assert::Fail(L"number范围出错");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            }
        };

        [TestMethod]
        void TestModeException()
        {
            Core c;
            int result[1][81];
            try
            {
                c.generate(1, 0, result); // mode传入0
                Assert::Fail(L"mode范围出错");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(1, 4, result); // mode传入4
                Assert::Fail(L"mode范围出错");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            }
        };

        [TestMethod]
        void TestRangeException()
        {
            Core c;
            int result[1][81];
            try
            {
                c.generate(1, -1, 20, false, result); // lower传入-1
                Assert::Fail(L"range范围出错");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(1, 50, 40, false, result); // 传入的lower比upper大
                Assert::Fail(L"range范围出错");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(1, 20, 56, false, result); // upper传入56
                Assert::Fail(L"range范围出错");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            }
        };

        [TestMethod]
        void TestValidException()
        {
            Core c;
            int result[1][81];
            c.generate(1, 3, result);
            result[0][0] = result[0][1] = 1;
            try
            {
                c.solve(result[0], result[0]); // 传入非法数独
                Assert::Fail(L"解出非法数独");
            }
            catch (const std::exception& e)
            {
                cout << e.what() << endl;
            };
        };
	};
}
