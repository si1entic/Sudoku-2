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
            if (k)  // ����������Ŀ�Ѹ����ֵ��б�ѡ
            {
                int row = i * 81 + j * 9 + k - 1;
                select++;
                remove(g_col[g_head[row]]);     // �Ƴ�������Ԫ�ؼ������Ԫ��
                for (int u = g_right[g_head[row]]; u != g_head[row]; u = g_right[u])
                    remove(g_col[u]);         // �Ƴ���������Ԫ�ؼ������Ԫ��
            }
        }
    }
    if(findOneSolution(final,select+1))
        return true;
    return false;
}

/* �������1������ */
void Core::generateFinal(int final[9][9])
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

/* ÿ����һ�У��ݹ�������� */
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

/* ÿ����һ�񣬵ݹ�����һ�� */
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

/* ���ø��ܷ���������� */
bool Core::checkValid(int final[9][9], int row, int col, int value)
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
    if(findSolutions(final, select + 1, solution))
        return false;
    return true;
}

void Core::init()
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

void Core::link(int final[9][9])
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

void Core::insertNode(int col, int no)     /* ��no��Ԫ����ӵ�col���� */
{
    /* ��Ԫ��col��Down[col]֮�����no */
    g_up[g_down[col]] = no;
    g_down[no] = g_down[col];
    g_up[no] = col;
    g_down[col] = no;
    g_size[col]++;
    g_col[no] = col;
} 

void Core::remove(int col)
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

void Core::restore(int col)
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

bool Core::findOneSolution(int final[9][9], int select)
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

bool Core::findSolutions(int final[9][9], int select, int& solution)
{
    if (select > 81)    
    {
        if (++solution > 1) // ������Ľ����
            return true;
        return false;
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







