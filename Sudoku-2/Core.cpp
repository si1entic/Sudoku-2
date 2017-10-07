#include "Core.h"

const int maxrow = 9 * 9 * 9;
const int maxcol = 1 + 9 * 9 * 4;
const int num = maxcol + maxrow * 4;	// ��Ԫ�ظ���,  
                                        // ��һ��ΪHeadԪ��,����9*9*4��Ϊ�б�Ԫ��,���9*9*9*4��Ϊ��1��Ԫ��
int Left[num], Right[num], Up[num], Down[num];    // ÿ��Ԫ�ص�4������������൱�������еļ�ͷ��
int Col[num];        // ��¼ÿ��Ԫ�ص��б�Ԫ��
int Row[num];        // ��¼ÿ��Ԫ�����ڵ�01��������
int Size[maxcol];    // ��¼ÿ�еġ�1��Ԫ�ظ���
int Head[maxrow];    // ��¼ÿ�е�һ����1��Ԫ��
int no;				// Ԫ�ر��

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
                remove(Col[Head[row]]);     // �Ƴ�������Ԫ�ؼ������Ԫ��
                for (int u = Right[Head[row]]; u != Head[row]; u = Right[u])
                    remove(Col[u]);         // �Ƴ���������Ԫ�ؼ������Ԫ��
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
                remove(Col[Head[row]]);     // �Ƴ�������Ԫ�ؼ������Ԫ��
                for (int u = Right[Head[row]]; u != Head[row]; u = Right[u])
                    remove( Col[u]);         // �Ƴ���������Ԫ�ؼ������Ԫ��
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
    /* �����б�Ԫ�� */
    for (int i = 0; i < maxcol; i++) 
    {
        Left[i] = i - 1;
        Right[i] = i + 1;
        Up[i] = Down[i] = i;
        Row[i] = 0;
        Col[i] = i;
        Size[i] = 0;
    }
    /* ����HeadԪ�� */
    Left[0] = maxcol - 1;
    Right[maxcol - 1] = 0;
    /* ���ӡ�1��Ԫ�� */
    no = maxcol;
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) // ����9x9����
        {    
            int k = final[i][j];
            if (k) 
            {
                for (int t = 1; t <= 4; t++)  // ÿ����0���ֻ���01�����в���4����1��Ԫ��
                {   
                    Left[no + t] = no + t - 1;
                    Right[no + t] = no + t + 1;
                    Row[no + t] = i * 81 + j * 9 + k - 1;
                }
                Left[no + 1] = no + 4;
                Right[no + 4] = no + 1;
                Head[Row[no + 1]] = no + 1;
                /* ��4��Ԫ�ز��������� */
                insertNode(i * 9 + j + 1, no + 1);
                insertNode(81 + i * 9 + k, no + 2);
                insertNode(81 * 2 + j * 9 + k, no + 3);
                insertNode(81 * 3 + (i / 3 * 3 + j / 3) * 9 + k, no + 4);
                no += 4;
            }
            else  // ��λ��Ϊ0��������
            {   
                for (int k = 1; k <= 9; k++)        // ����9���
                {   
                    for (int t = 1; t <= 4; t++)    // ���ɲ��������ǵ�Ԫ��
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

void Core::insertNode(int col, int no)     /* ��no��Ԫ����ӵ�col���� */
{
    /* ��Ԫ��col��Down[col]֮�����no */
    Up[Down[col]] = no;
    Down[no] = Down[col];
    Up[no] = col;
    Down[col] = no;
    Size[col]++;
    Col[no] = col;
} 

void Core::remove(int col)
{
    /* �Ƴ�col�б�Ԫ�� */
    Left[Right[col]] = Left[col];
    Right[Left[col]] = Right[col];
    /* �Ƴ�col�С�1��Ԫ������ͬ��Ԫ�� */
    for (int i = Down[col]; i != col; i = Down[i])   // ����ɾ
    {    
        for (int j = Right[i]; j != i; j = Right[j]) // ����ɾ
        {   
            Up[Down[j]] = Up[j];
            Down[Up[j]] = Down[j];
            Size[Col[j]]--;
        }
    }
}

void Core::restore(int col)
{
    /* �ָ�col�б�Ԫ�� */
    Left[Right[col]] = Right[Left[col]] = col;
    /* �ָ�col�С�1��Ԫ������ͬ��Ԫ�� */
    for (int i = Up[col]; i != col; i = Up[i])   // �ָ�˳�����Ƴ�˳���෴
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
    if (select > 81)    // ��ѡ��
        return true;
    /* �����б�Ԫ�أ�ѡһ��Ԫ�����ٵ��У������ʵͣ� */
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
    /* �������и���1��Ԫ��,�������γ���,ʧ����ָ����Ƴ��Ľ�� */
    for (int i = Down[col]; i != col; i = Down[i]) 
    {
        int row = Row[i];
        final[row / 81][row % 81 / 9] = row % 9 + 1;    // ���ݸ�Ԫ�ص�����������
        for (int j = Right[i]; j != i; j = Right[j])    // �Ƴ����Ԫ��ͬ��Ԫ�ص��� 
            remove(Col[j]);
        if (findOneSolution(final, select + 1))    // ��ѡ����+1���ݹ�
            return true;
        for (int j = Left[i]; j != i; j = Left[j])    // ����false��ָ��ղŵ��Ƴ���˳���෴
            restore(Col[j]);
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
    /* �������и���1��Ԫ��,�������γ���,ʧ����ָ����Ƴ��Ľ�� */
    for (int i = Down[col]; i != col; i = Down[i])
    {
        int row = Row[i];
        final[row / 81][row % 81 / 9] = row % 9 + 1;    // ���ݸ�Ԫ�ص�����������
        for (int j = Right[i]; j != i; j = Right[j])    // �Ƴ����Ԫ��ͬ��Ԫ�ص��� 
            remove(Col[j]);
        if (findSolutions(final, select + 1, solution))    // ��ѡ����+1���ݹ�
            return true;
        for (int j = Left[i]; j != i; j = Left[j])    // ����false��ָ��ղŵ��Ƴ���˳���෴
            restore(Col[j]);
    }
    restore(col);
    return false;
}







