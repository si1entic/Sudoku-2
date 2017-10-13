#include "Core.h"

const int MAXROW = 9 * 9 * 9;
const int MAXCOL = 1 + 9 * 9 * 4;
const int NUM = MAXCOL + MAXROW * 4;	// ��Ԫ�ظ���,  
                                        // ��һ��ΪHeadԪ��,����9*9*4��Ϊ�б�Ԫ��,���9*9*9*4��Ϊ��1��Ԫ��
int g_left[NUM], g_right[NUM], g_up[NUM], g_down[NUM];    // ÿ��Ԫ�ص�4������������൱�������еļ�ͷ��
int g_col[NUM];        // ��¼ÿ��Ԫ�ص��б�Ԫ��
int g_row[NUM];        // ��¼ÿ��Ԫ�����ڵ�01��������
int g_size[MAXCOL];    // ��¼ÿ�еġ�1��Ԫ�ظ���
int g_head[MAXROW];    // ��¼ÿ�е�һ����1��Ԫ��
int g_no;				// Ԫ�ر��

/* ����number���Ѷ�Ϊmode����Ŀ������result�� */
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
            if (k)  // ����������Ŀ�Ѹ����ֵ��б�ѡ
            {
                int row = i * 81 + j * 9 + k - 1;
                select++;
                ps.remove(g_col[g_head[row]]);     // �Ƴ�������Ԫ�ؼ������Ԫ��
                for (int u = g_right[g_head[row]]; u != g_head[row]; u = g_right[u])
                    ps.remove(g_col[u]);         // �Ƴ���������Ԫ�ؼ������Ԫ��
            }
        }
    }
    if (!ps.findOneSolution(final, select + 1))     // �����޽�
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

/* �������1������ */
void FinalMaker::make(int final[9][9])
{
    for (int i = 0; i<9; i++)
        for (int j = 0; j<9; j++)
            final[i][j] = 0;
    for (int i = 0; i < 9; i++) // ���һ�Ų���check��ֱ���������
        final[0][i] = i + 1;
    shuffle(final[0]);
    while (!fillRow(final, 1));
    return;
}

/* ������Ҹ����� */
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

/* ÿ����һ�У��ݹ�������� */
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

/* ÿ����һ�񣬵ݹ�����һ�� */
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

/* ���ø��ܷ���������� */
bool FinalMaker::checkValid(int final[9][9], int row, int col, int value)
{
    for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) // ���ÿ��Ƿ����и�����
        for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
            if (final[i][j] == value)
                return false; 
    for (int i = 0; i < 9; i++) // �����и����Ƿ����и�����
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
            if (k)  // ����������Ŀ�Ѹ����ֵ��б�ѡ
            {    
                int row = i * 81 + j * 9 + k - 1;
                select++;
                remove(g_col[g_head[row]]);     // �Ƴ�������Ԫ�ؼ������Ԫ��
                for (int u = g_right[g_head[row]]; u != g_head[row]; u = g_right[u])
                    remove( g_col[u]);         // �Ƴ���������Ԫ�ؼ������Ԫ��
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
            for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) // ���ÿ��Ƿ����и�����
                for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
                    if (final[i][j] == value)
                        if (!(i == row&&j == col))
                            return false;
            for (int i = 0; i < 9; i++)                         // �����и����Ƿ����и�����
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
    /* �����б�Ԫ�� */
    for (int i = 0; i < MAXCOL; i++) 
    {
        g_left[i] = i - 1;
        g_right[i] = i + 1;
        g_up[i] = g_down[i] = i;
        g_row[i] = 0;
        g_col[i] = i;
        g_size[i] = 0;
    }
    /* ����HeadԪ�� */
    g_left[0] = MAXCOL - 1;
    g_right[MAXCOL - 1] = 0;
    /* ���ӡ�1��Ԫ�� */
    g_no = MAXCOL;
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) // ����9x9����
        {    
            int k = final[i][j];
            if (k) 
            {
                for (int t = 1; t <= 4; t++)  // ÿ����0���ֻ���01�����в���4����1��Ԫ��
                {   
                    g_left[g_no + t] = g_no + t - 1;
                    g_right[g_no + t] = g_no + t + 1;
                    g_row[g_no + t] = i * 81 + j * 9 + k - 1;
                }
                g_left[g_no + 1] = g_no + 4;
                g_right[g_no + 4] = g_no + 1;
                g_head[g_row[g_no + 1]] = g_no + 1;
                /* ��4��Ԫ�ز��������� */
                insertNode(i * 9 + j + 1, g_no + 1);
                insertNode(81 + i * 9 + k, g_no + 2);
                insertNode(81 * 2 + j * 9 + k, g_no + 3);
                insertNode(81 * 3 + (i / 3 * 3 + j / 3) * 9 + k, g_no + 4);
                g_no += 4;
            }
            else  // ��λ��Ϊ0��������
            {   
                for (int k = 1; k <= 9; k++)        // ����9���
                {   
                    for (int t = 1; t <= 4; t++)    // ���ɲ��������ǵ�Ԫ��
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

void PuzzleSovlver::insertNode(int col, int no)     /* ��no��Ԫ����ӵ�col���� */
{
    /* ��Ԫ��col��Down[col]֮�����no */
    g_up[g_down[col]] = no;
    g_down[no] = g_down[col];
    g_up[no] = col;
    g_down[col] = no;
    g_size[col]++;
    g_col[no] = col;
} 

void PuzzleSovlver::remove(int col)
{
    /* �Ƴ�col�б�Ԫ�� */
    g_left[g_right[col]] = g_left[col];
    g_right[g_left[col]] = g_right[col];
    /* �Ƴ�col�С�1��Ԫ������ͬ��Ԫ�� */
    for (int i = g_down[col]; i != col; i = g_down[i])   // ����ɾ
    {    
        for (int j = g_right[i]; j != i; j = g_right[j]) // ����ɾ
        {   
            g_up[g_down[j]] = g_up[j];
            g_down[g_up[j]] = g_down[j];
            g_size[g_col[j]]--;
        }
    }
}

void PuzzleSovlver::restore(int col)
{
    /* �ָ�col�б�Ԫ�� */
    g_left[g_right[col]] = g_right[g_left[col]] = col;
    /* �ָ�col�С�1��Ԫ������ͬ��Ԫ�� */
    for (int i = g_up[col]; i != col; i = g_up[i])   // �ָ�˳�����Ƴ�˳���෴
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
    if (select > 81)    // ��ѡ��
        return true;
    /* �����б�Ԫ�أ�ѡһ��Ԫ�����ٵ��У������ʵͣ� */
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
    /* �������и���1��Ԫ��,�������γ���,ʧ����ָ����Ƴ��Ľ�� */
    for (int i = g_down[col]; i != col; i = g_down[i]) 
    {
        int row = g_row[i];
        final[row / 81][row % 81 / 9] = row % 9 + 1;    // ���ݸ�Ԫ�ص�����������
        for (int j = g_right[i]; j != i; j = g_right[j])    // �Ƴ����Ԫ��ͬ��Ԫ�ص��� 
            remove(g_col[j]);
        if (findOneSolution(final, select + 1))    // ��ѡ����+1���ݹ�
            return true;
        for (int j = g_left[i]; j != i; j = g_left[j])    // ����false��ָ��ղŵ��Ƴ���˳���෴
            restore(g_col[j]);
    }
    restore(col);
    return false;
}

bool PuzzleSovlver::findSolutions(int final[9][9], int select, int& solution)
{
    if (select > 81)    
    {
        if (++solution == 1) // ������Ľ����
            return false; 
        throw(1);
    }
    /* �����б�Ԫ�أ�ѡһ��Ԫ�����ٵ��У������ʵͣ� */
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
    /* �������и���1��Ԫ��,�������γ���,ʧ����ָ����Ƴ��Ľ�� */
    for (int i = g_down[col]; i != col; i = g_down[i])
    {
        int row = g_row[i];
        final[row / 81][row % 81 / 9] = row % 9 + 1;    // ���ݸ�Ԫ�ص�����������
        for (int j = g_right[i]; j != i; j = g_right[j])    // �Ƴ����Ԫ��ͬ��Ԫ�ص��� 
            remove(g_col[j]);
        if (findSolutions(final, select + 1, solution))    // ��ѡ����+1���ݹ�
            return true;
        for (int j = g_left[i]; j != i; j = g_left[j])    // ����false��ָ��ղŵ��Ƴ���˳���෴
            restore(g_col[j]);
    }
    restore(col);
    return false;
}

NumberException::NumberException() :exception("������Ŀ����numberӦ��1~10000��Χ��")
{
}

ModeException::ModeException() : exception("�����Ѷ�modeӦ��1~3��Χ��")
{
}

RangeException::RangeException() : exception("�ڿո�������lower������upperӦ����0<=lower<=upper<=55")
{
}

ValidException::ValidException() : exception("�Ƿ�������Ŀ�޷����")
{
}






