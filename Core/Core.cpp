#include "Core.h"

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

/* 生成number个难度为mode的题目，放入result中 */
void Core::generate(int number, int mode, int result[][81])
{
    srand((unsigned)time(NULL));
    if (number < 1 || number > 10000)
        throw NumberException();
    if(mode<1||mode>3)
        throw ModeException();
    for (int i = 0; i < number; i++)
    {
        int final[9][9];
        fm.make(final);
        int blanks;
        switch (mode)
        {
        case 1:
            blanks = (rand() % (30 - 20 + 1)) + 20;
            break;
        case 2:
            blanks = (rand() % (45 - 31 + 1)) + 31;
            break;
        default:
            blanks = (rand() % (55 - 46 + 1)) + 46;
            break;
        }
        hollowRandom(final, blanks);
        memcpy(result[i], final, sizeof(final));
    }
}

void Core::generate(int number, int lower, int upper, bool unique, int result[][81])
{
    srand((unsigned)time(NULL));
    if (number < 1 || number > 1000000)
        throw NumberException();
    if (lower > upper || upper > 55 || lower < 0)
       throw RangeException();
    for (int i = 0; i < number; i++)
    {
        int final[9][9];
        fm.make(final);
        int blanks = blanks = (rand() % (upper - lower + 1)) + lower;
        if(unique)
            hollowUnique(final, blanks);
        else
            hollowRandom(final, blanks);
        memcpy(result[i], final, sizeof(final));
    }
}

bool Core::solve(int puzzle[81], int solution[81])
{
    srand((unsigned)time(NULL));
    int final[9][9];
    memcpy(final, puzzle, sizeof(final));
    if (!ps.checkValid(final))
        throw ValidException();
    ps.init();
    ps.link(final);
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
                ps.remove(g_col[g_head[row]]);     // 移除该行首元素及其相关元素
                for (int u = g_right[g_head[row]]; u != g_head[row]; u = g_right[u])
                    ps.remove(g_col[u]);         // 移除该行其他元素及其相关元素
            }
        }
    }
    if (!ps.findOneSolution(final, select + 1))     // 数独无解
        return false;
    memcpy(solution, final, sizeof(final));
    return true;
}

void Core::hollowUnique(int final[9][9], int blanks)
{
    int copy[9][9];
    while (blanks--)
    {
        int random, row, col;
        while (true)
        {
            memcpy(copy, final, sizeof(copy));
            random = rand() % 81;
            row = random / 9;
            col = random % 9;
            if (final[row][col] == 0)
                continue;
            copy[row][col] = 0;
            if (ps.checkUnique(copy))
            {
                final[row][col] = 0;
                break;
            }
        }
    }
}

void Core::hollowRandom(int final[9][9], int blanks)
{
    for (int i = 0; i < blanks; i++)
    {
        int random, row, col;
        while (true)
        {
            random = rand() % 81;
            row = random / 9;
            col = random % 9;
            if (final[row][col] == 0)
                continue;
            final[row][col] = 0;
            break;
        }
    }
}

/* 随机生成1个终盘 */
void FinalMaker::make(int final[9][9])
{
    for (int i = 0; i<9; i++)
        for (int j = 0; j<9; j++)
            final[i][j] = 0;
    for (int i = 0; i < 9; i++) // 填第一排不用check，直接随机即可
        final[0][i] = i + 1;
    shuffle(final[0]);
    while (!fillRow(final, 1));
    return;
}

/* 随机打乱该数组 */
void FinalMaker::shuffle(int array[9])
{
    int temp, index;
    for (int i = 0; i < 9; i++) {	
        index = rand() % 9;
        temp = array[index];
        array[index] = array[i];
        array[i] = temp;
    }
}

/* 每次填一行，递归填完九行 */
bool FinalMaker::fillRow(int final[9][9], int row)
{
    if (row > 8)
        return true;
    if (fill(final, row, 0))
        if (fillRow(final, row + 1))
            return true;
    for (int i = 0; i < 9; i++)
        final[row][i] = 0;
    return false;
}

/* 每次填一格，递归填完一行 */
bool FinalMaker::fill(int final[9][9], int row, int col)
{
    if (col > 8)
        return true;
    int array[9] = { 1,2,3,4,5,6,7,8,9 };
    shuffle(array);
    for (int i = 0; i < 9; i++)
    {
        if (checkValid(final, row, col, array[i]))
        {
            final[row][col] = array[i];
            if(fill(final, row, col + 1))
                return true;
        }
    }
    return false;
}

/* 检测该格能否填入该数字 */
bool FinalMaker::checkValid(int final[9][9], int row, int col, int value)
{
    for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) // 检测该块是否已有该数字
        for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
            if (final[i][j] == value)
                return false; 
    for (int i = 0; i < 9; i++) // 检测该行该列是否已有该数字
        if (final[row][i] == value || final[i][col] == value)
            return false;
    return true;
}

bool PuzzleSovlver::checkUnique(int final[9][9])
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
                    remove( g_col[u]);         // 移除该行其他元素及其相关元素
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

bool PuzzleSovlver::checkValid(int final[9][9])
{
    for (int row = 0; row < 9; row++)
        for (int col = 0; col < 9; col++)
        {
            int value = final[row][col];
            if (value == 0)
                continue;
            for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) // 检测该块是否已有该数字
                for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
                    if (final[i][j] == value)
                        if (!(i == row&&j == col))
                            return false;
            for (int i = 0; i < 9; i++)                         // 检测该行该列是否已有该数字
                if ((i != col&&final[row][i] == value) || (final[i][col] == value&&i != row))
                    return false;
        }
    return true;
}

void PuzzleSovlver::init()
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

void PuzzleSovlver::link(int final[9][9])
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

void PuzzleSovlver::insertNode(int col, int no)     /* 将no号元素添加到col列里 */
{
    /* 在元素col和Down[col]之间插入no */
    g_up[g_down[col]] = no;
    g_down[no] = g_down[col];
    g_up[no] = col;
    g_down[col] = no;
    g_size[col]++;
    g_col[no] = col;
} 

void PuzzleSovlver::remove(int col)
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

void PuzzleSovlver::restore(int col)
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

bool PuzzleSovlver::findOneSolution(int final[9][9], int select)
{
    if (select > 81)    // 已选够
        return true;
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
        if (findOneSolution(final, select + 1))    // 已选行数+1，递归
            return true;
        for (int j = g_left[i]; j != i; j = g_left[j])    // 返回false则恢复刚才的移除，顺序相反
            restore(g_col[j]);
    }
    restore(col);
    return false;
}

bool PuzzleSovlver::findSolutions(int final[9][9], int select, int& solution)
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

NumberException::NumberException() :exception("数独题目个数number应在1~10000范围内")
{
}

ModeException::ModeException() : exception("数独难度mode应在1~3范围内")
{
}

RangeException::RangeException() : exception("挖空个数下限lower与上限upper应满足0<=lower<=upper<=55")
{
}

ValidException::ValidException() : exception("非法数独题目无法求解")
{
}






