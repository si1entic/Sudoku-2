#include "Core.h"

const int maxrow = 9 * 9 * 9;
const int maxcol = 1 + 9 * 9 * 4;
const int num = maxcol + maxrow * 4;	// 总元素个数,  
                                        // 第一个为Head元素,接着9*9*4个为列标元素,最后9*9*9*4个为“1”元素
int Left[num], Right[num], Up[num], Down[num];    // 每个元素的4个方向分量（相当于链表中的箭头）
int Col[num];        // 记录每个元素的列标元素
int Row[num];        // 记录每个元素所在的01矩阵行数
int Size[maxcol];    // 记录每列的“1”元素个数
int Head[maxrow];    // 记录每行第一个“1”元素
int no;				// 元素编号

/* 生成number个难度为mode的题目，放入result中 */
void Core::generate(int number, int mode, int result[][81])
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < number; i++)
    {
        int final[9][9];
        generateFinal(final);
        int blanks;
        switch (mode)
        {
        case 1:
            blanks = (rand() % (30 - 20 + 1)) + 20;
            break;
        case 2:
            blanks = (rand() % (45 - 31 + 1)) + 31;
            break;
        case 3:
            blanks = (rand() % (55 - 46 + 1)) + 46;
            break;
        default:
            cout << "mode is wrong" << endl;
            exit(1);
        }
        digRandom(final, blanks);
        memcpy(result[i], final, sizeof(final));
    }
}

void Core::generate(int number, int lower, int upper, bool unique, int result[][81])
{/*@ REQUIRES:lower>=20 && upper>=lower && upper<=55 */
    for (int i = 0; i < number; i++)
    {
        int final[9][9];
        generateFinal(final);
        int blanks = blanks = (rand() % (upper - lower + 1)) + lower;
        if(unique)
            digUnique(final, blanks);
        else
            digRandom(final, blanks);
        memcpy(result[i], final, sizeof(final));
    }
}

bool Core::solve(int puzzle[81], int solution[81])
{
    int final[9][9];
    memcpy(final, puzzle, sizeof(final));
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
                remove(Col[Head[row]]);     // 移除该行首元素及其相关元素
                for (int u = Right[Head[row]]; u != Head[row]; u = Right[u])
                    remove(Col[u]);         // 移除该行其他元素及其相关元素
            }
        }
    }
    if(findOneSolution(final,select+1))
        return true;
    return false;
}

/* 随机生成1个终盘 */
void Core::generateFinal(int final[9][9])
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
void Core::shuffle(int array[9]) 
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
bool Core::fillRow(int final[9][9], int row)
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
bool Core::fill(int final[9][9], int row, int col)
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
bool Core::checkValid(int final[9][9], int row, int col, int value)
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

void Core::digUnique(int final[9][9], int blanks)
{
    int copy[9][9];
    while(blanks--)
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
            if (checkUnique(copy))
            {
                final[row][col] = 0;
                break;
            }
        }
    }
}

void Core::digRandom(int final[9][9], int blanks)
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

bool Core::checkUnique(int final[9][9])
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
                remove(Col[Head[row]]);     // 移除该行首元素及其相关元素
                for (int u = Right[Head[row]]; u != Head[row]; u = Right[u])
                    remove( Col[u]);         // 移除该行其他元素及其相关元素
            }
        }
    }
    int solution = 0;
    if(findSolutions(final, select + 1, solution))
        return false;
    return true;
}

void Core::init()
{
    memset(Left, 0, sizeof(Left));
    memset(Right, 0, sizeof(Right));
    memset(Up, 0, sizeof(Up));
    memset(Down, 0, sizeof(Down));
    memset(Col, 0, sizeof(Col));
    memset(Row, 0, sizeof(Row));
    memset(Size, 0, sizeof(Size));
    memset(Head, 0, sizeof(Head));
}

void Core::link(int final[9][9])
{
    /* 链接列标元素 */
    for (int i = 0; i < maxcol; i++) 
    {
        Left[i] = i - 1;
        Right[i] = i + 1;
        Up[i] = Down[i] = i;
        Row[i] = 0;
        Col[i] = i;
        Size[i] = 0;
    }
    /* 链接Head元素 */
    Left[0] = maxcol - 1;
    Right[maxcol - 1] = 0;
    /* 链接“1”元素 */
    no = maxcol;
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) // 遍历9x9数独
        {    
            int k = final[i][j];
            if (k) 
            {
                for (int t = 1; t <= 4; t++)  // 每个非0数字会在01矩阵中产生4个“1”元素
                {   
                    Left[no + t] = no + t - 1;
                    Right[no + t] = no + t + 1;
                    Row[no + t] = i * 81 + j * 9 + k - 1;
                }
                Left[no + 1] = no + 4;
                Right[no + 4] = no + 1;
                Head[Row[no + 1]] = no + 1;
                /* 将4个元素插入列链中 */
                insertNode(i * 9 + j + 1, no + 1);
                insertNode(81 + i * 9 + k, no + 2);
                insertNode(81 * 2 + j * 9 + k, no + 3);
                insertNode(81 * 3 + (i / 3 * 3 + j / 3) * 9 + k, no + 4);
                no += 4;
            }
            else  // 该位置为0，即待填
            {   
                for (int k = 1; k <= 9; k++)        // 构造9种填法
                {   
                    for (int t = 1; t <= 4; t++)    // 生成并链接它们的元素
                    {   
                        Left[no + t] = no + t - 1;
                        Right[no + t] = no + t + 1;
                        Row[no + t] = i * 81 + j * 9 + k - 1;
                    }
                    Left[no + 1] = no + 4;
                    Right[no + 4] = no + 1;
                    Head[Row[no + 1]] = no + 1;
                    insertNode(i * 9 + j + 1, no + 1);
                    insertNode(81 + i * 9 + k, no + 2);
                    insertNode(81 * 2 + j * 9 + k, no + 3);
                    insertNode(81 * 3 + (i / 3 * 3 + j / 3) * 9 + k, no + 4);
                    no += 4;
                }
            }
        }
    }
}

void Core::insertNode(int col, int no)     /* 将no号元素添加到col列里 */
{
    /* 在元素col和Down[col]之间插入no */
    Up[Down[col]] = no;
    Down[no] = Down[col];
    Up[no] = col;
    Down[col] = no;
    Size[col]++;
    Col[no] = col;
} 

void Core::remove(int col)
{
    /* 移除col列标元素 */
    Left[Right[col]] = Left[col];
    Right[Left[col]] = Right[col];
    /* 移除col列“1”元素与其同行元素 */
    for (int i = Down[col]; i != col; i = Down[i])   // 向下删
    {    
        for (int j = Right[i]; j != i; j = Right[j]) // 向右删
        {   
            Up[Down[j]] = Up[j];
            Down[Up[j]] = Down[j];
            Size[Col[j]]--;
        }
    }
}

void Core::restore(int col)
{
    /* 恢复col列标元素 */
    Left[Right[col]] = Right[Left[col]] = col;
    /* 恢复col列“1”元素与其同行元素 */
    for (int i = Up[col]; i != col; i = Up[i])   // 恢复顺序与移除顺序相反
    {      
        for (int j = Left[i]; j != i; j = Left[j]) 
        {
            Up[Down[j]] = Down[Up[j]] = j;
            Size[Col[j]]++;
        }
    }
}

bool Core::findOneSolution(int final[9][9], int select)
{
    if (select > 81)    // 已选够
        return true;
    /* 遍历列标元素，选一个元素最少的列（回溯率低） */
    int col = 0;
    int min = INT_MAX;
    for (int i = Right[0]; i != 0; i = Right[i]) 
    {
        int size = Size[i];
        if (size == 0)
            return false;
        if (size < min) {
            min = size;
            col = i;
        }
    }
    remove(col);
    /* 遍历该列各“1”元素,逐行依次尝试,失败则恢复已移除的结点 */
    for (int i = Down[col]; i != col; i = Down[i]) 
    {
        int row = Row[i];
        final[row / 81][row % 81 / 9] = row % 9 + 1;    // 根据该元素的行填入数独
        for (int j = Right[i]; j != i; j = Right[j])    // 移除与该元素同行元素的列 
            remove(Col[j]);
        if (findOneSolution(final, select + 1))    // 已选行数+1，递归
            return true;
        for (int j = Left[i]; j != i; j = Left[j])    // 返回false则恢复刚才的移除，顺序相反
            restore(Col[j]);
    }
    restore(col);
    return false;
}

bool Core::findSolutions(int final[9][9], int select, int& solution)
{
    if (select > 81)    
    {
        if (++solution > 1) // 已求出的解个数
            return true;
        return false;
    }
       
    /* 遍历列标元素，选一个元素最少的列（回溯率低） */
    int col = 0;
    int min = INT_MAX;
    for (int i = Right[0]; i != 0; i = Right[i])
    {
        int size = Size[i];
        if (size == 0)
            return false;
        if (size < min) {
            min = size;
            col = i;
        }
    }
    remove(col);
    /* 遍历该列各“1”元素,逐行依次尝试,失败则恢复已移除的结点 */
    for (int i = Down[col]; i != col; i = Down[i])
    {
        int row = Row[i];
        final[row / 81][row % 81 / 9] = row % 9 + 1;    // 根据该元素的行填入数独
        for (int j = Right[i]; j != i; j = Right[j])    // 移除与该元素同行元素的列 
            remove(Col[j]);
        if (findSolutions(final, select + 1, solution))    // 已选行数+1，递归
            return true;
        for (int j = Left[i]; j != i; j = Left[j])    // 返回false则恢复刚才的移除，顺序相反
            restore(Col[j]);
    }
    restore(col);
    return false;
}







