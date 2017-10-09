#include "stdafx.h"
#include "../Sudoku-2/Core.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

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
            PuzzleSovlver ps;
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
                Assert::IsTrue(ps.checkUnique(game), L"唯一性出错");
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
            catch (NumberException& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(INT_MAX, 30, 40, true, result); // number传入最大int值
                Assert::Fail(L"number范围出错");
            }
            catch (NumberException& e)
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
            catch (ModeException& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(1, 4, result); // mode传入4
                Assert::Fail(L"mode范围出错");
            }
            catch (ModeException& e)
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
            catch (RangeException& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(1, 50, 40, false, result); // 传入的lower比upper大
                Assert::Fail(L"range范围出错");
            }
            catch (RangeException& e)
            {
                cout << e.what() << endl;
            }
            try
            {
                c.generate(1, 20, 56, false, result); // upper传入56
                Assert::Fail(L"range范围出错");
            }
            catch (RangeException& e)
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
            catch (ValidException& e)
            {
                cout << e.what() << endl;
            };
        };
	};
}
