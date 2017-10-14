#include "Sudoku.h"
Sudoku::Sudoku(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    this->setFixedSize(390, 400);
    mTimer = new QTimer(this);

    easyOpenAction = new QAction(tr("Easy"), this);
    easyOpenAction->setStatusTip(tr("Choose Easy model"));
    connect(easyOpenAction, &QAction::triggered, this, &Sudoku::easyOpen);
    normalOpenAction = new QAction(tr("Normal"), this);
    normalOpenAction->setStatusTip(tr("Choose Normal model"));
    connect(normalOpenAction, &QAction::triggered, this, &Sudoku::normalOpen);
    hardOpenAction = new QAction(tr("Hard"), this);
    hardOpenAction->setStatusTip(tr("Choose Hard model"));
    connect(hardOpenAction, &QAction::triggered, this, &Sudoku::hardOpen);
    recordAction = new QAction(tr("Record"), this);
    recordAction->setStatusTip(tr("Record"));
    connect(recordAction, &QAction::triggered, this, &Sudoku::record);

    helpAction = new QAction(tr("Help"), this);
    helpAction->setStatusTip(tr("Help"));
    connect(helpAction, &QAction::triggered, this, &Sudoku::help);

    QMenu *menuNew = menuBar()->addMenu(tr("&New"));
    menuNew->addAction(easyOpenAction);
    menuNew->addAction(normalOpenAction);
    menuNew->addAction(hardOpenAction);
    menuNew->addAction(recordAction);

    QMenu *menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->addAction(helpAction);

    midLayout = new QGridLayout;
    topLayout = new QGridLayout;
    mainLayout = new QGridLayout;

    mainLayout->addLayout(topLayout, 0, 0);
    mainLayout->addLayout(midLayout, 1, 0);

    lb_model = new QLabel(this);
    lb_model->setText("Easy");
    topLayout->addWidget(lb_model, 0, 0, 0);
    btn_start = new QPushButton(this);
    btn_start->setText("start");
    connect(btn_start, SIGNAL(clicked()), this, SLOT(startGame()));
    topLayout->addWidget(btn_start, 0, 1, 0);

    lb_timer = new QLabel(this);
    lb_timer->setText("0");
    lb_timer->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    topLayout->addWidget(lb_timer, 0, 3, 0);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(displayTimer()));

    QRegExp rx("[1-9]");

    midLayout->setVerticalSpacing(0);
    midLayout->setHorizontalSpacing(0);
    for (int i = 0; i < 81; i++) {
        sudo[i] = new MineLineEdit(this);
        sudo[i]->createActions();
        sudo[i]->setMaxLength(1);
        sudo[i]->setMaximumWidth(40);
        sudo[i]->setMaximumHeight(40);
        sudo[i]->setAlignment(Qt::AlignHCenter);
        sudo[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sudo[i]->setValidator(new QRegExpValidator(rx, sudo[i]));
        sudo[i]->setReadOnly(true);

        sudo[i]->setAccessibleName(QString::number(i));
        sudo[i]->setFont(QFont("Timers", 14, QFont::Bold));
        sudo[i]->setStyleSheet(
            "background-color: #66cccc;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }

    tipAction = new QAction(tr("Easy"), this);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++) 
        {
            midLayoutIn[i][j] = new QGridLayout();
            midLayoutIn[i][j]->setMargin(2);
            midLayout->addLayout(midLayoutIn[i][j], i, j, 0);
        }
    }
    for (int i = 0; i < 81; i++)
    {
        midLayoutIn[i / 9 / 3][i % 9 / 3]->addWidget(sudo[i], i / 9, i % 9, 0);
        connect(sudo[i], SIGNAL(tip_clicked()), this, SLOT(tipClick()));
        connect(sudo[i], SIGNAL(textChanged(const QString&)), this, SLOT(sudoTableEdit()));
    }

    this->centralWidget()->setLayout(mainLayout);

    //Help
    helpLayout = new QWidget();
    helpLayout->setWindowTitle(tr("Help"));
    QLabel *lb_help = new QLabel(helpLayout);
    lb_help->setGeometry(QRect(328, 240, 329, 27 * 4));  //四倍行距
    lb_help->setWordWrap(true);
    lb_help->setAlignment(Qt::AlignTop);
    lb_help->adjustSize();
    lb_help->setText(QStringLiteral("说明\n本程序为9*9数独游戏。分为简单、普通、困难三个难度。\n提示功能：\n\
在需要提示的格子处右键，点击菜单中的tip"));

    QGridLayout *qGrid1 = new QGridLayout(helpLayout);
    qGrid1->addWidget(lb_help, 0, 0, 0);
    helpLayout->setFixedSize(250, 120);
    helpLayout->setLayout(qGrid1);

    //Record
    recordLayout = new QWidget();
    QString *qstr1 = new QString();
    QString *qstr2 = new QString();
    QString *qstr3 = new QString();
    readInit("1", *qstr1);
    readInit("2", *qstr2);
    readInit("3", *qstr3);
    recordLayout->setWindowTitle(tr("Record"));
    recordEasy = new QLabel(recordLayout);
    recordNormal = new QLabel(recordLayout);
    recordHard = new QLabel(recordLayout);
    recordEasyGrade = new QLabel(recordLayout);
    recordNormalGrade = new QLabel(recordLayout);
    recordHardGrade = new QLabel(recordLayout);
    recordEasy->setText("    Easy   : ");
    recordNormal->setText("    Normal : ");
    recordHard->setText("    Hard   : ");
    recordEasyGrade->setText(*qstr1 + "s");
    recordNormalGrade->setText(*qstr2 + "s");
    recordHardGrade->setText(*qstr3 + "s");
    delete qstr1, qstr2, qstr3;

    btn_resetHistory = new QPushButton(recordLayout);
    connect(btn_resetHistory, SIGNAL(clicked()), this, SLOT(resetHistory()));
    btn_resetHistory->setText(tr("Reset"));
    btn_resetHistory->setMaximumWidth(100);

    recordLayoutIn = new QGridLayout(recordLayout);
    recordLayoutIn->addWidget(recordEasy, 0, 0, 1, 2, 0);
    recordLayoutIn->addWidget(recordNormal, 1, 0, 1, 2, 0);
    recordLayoutIn->addWidget(recordHard, 2, 0, 1, 2, 0);
    recordLayoutIn->addWidget(recordEasyGrade, 0, 2, 1, 2, 0);
    recordLayoutIn->addWidget(recordNormalGrade, 1, 2, 1, 2, 0);
    recordLayoutIn->addWidget(recordHardGrade, 2, 2, 1, 2, 0);
    recordLayoutIn->addWidget(btn_resetHistory, 3, 1, 1, 2, 0);

    recordLayout->setFixedSize(220, 120);

    recordLayout->setLayout(recordLayoutIn);
}


void Sudoku::resetHistory()
{
    writeInit("1", "99999");
    writeInit("2", "99999");
    writeInit("3", "99999");
    recordEasyGrade->setText("99999s");
    recordNormalGrade->setText("99999s");
    recordHardGrade->setText("99999s");

}

void Sudoku::easyOpen()
{
    stopmTimer();
    timeTimer = 0;
    lb_timer->setText("0");
    model = 1;
    lb_model->setText("Easy");
    btn_start->setText("start");
    for (int i = 0; i < 81; i++) {
        sudo[i]->setText("");
        sudo[i]->setReadOnly(true);
        sudo[i]->setStyleSheet(
            "background-color: #66cccc;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }
}

void Sudoku::normalOpen() {
    stopmTimer();
    timeTimer = 0;
    lb_timer->setText("0");
    model = 2;
    lb_model->setText("Normal");
    btn_start->setText("start");
    for (int i = 0; i < 81; i++) {
        sudo[i]->setText("");
        sudo[i]->setReadOnly(true);
        sudo[i]->setStyleSheet(
            "background-color: #66cccc;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }
}

void Sudoku::hardOpen() {
    stopmTimer();
    timeTimer = 0;
    lb_timer->setText("0");
    model = 3;
    lb_model->setText("Hard");
    btn_start->setText("start");
    for (int i = 0; i < 81; i++) {
        sudo[i]->setText("");
        sudo[i]->setReadOnly(true);
        sudo[i]->setStyleSheet(
            "background-color: #66cccc;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }
}

void Sudoku::record() {
    QString *qstr1 = new QString();
    QString *qstr2 = new QString();
    QString *qstr3 = new QString();
    readInit("1", *qstr1);
    readInit("2", *qstr2);
    readInit("3", *qstr3);
    recordEasyGrade->setText(*qstr1 + "s");
    recordNormalGrade->setText(*qstr2 + "s");
    recordHardGrade->setText(*qstr3 + "s");

    recordLayout->show();
}

void Sudoku::startGame() {
    btn_start->setText("restart");
    lb_timer->setText("0");
    sudoku.generate(1, model, result);
    startmTimer();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            sudoTable[i][j] = result[0][i * 9 + j];
        }
    }
    for (int i = 0; i < 81; i++) {
        if (result[0][i] != 0)
        {
            sudo[i]->setText(QString::number(result[0][i]));
            sudo[i]->setReadOnly(true);
            sudo[i]->setStyleSheet(
                "background-color: #66cccc;"
                "selection-color: yellow;"
                "selection-background-color: blue;");
        }
        else
        {
            sudo[i]->setText("");
            sudo[i]->setReadOnly(false);
            sudo[i]->setStyleSheet(
                "background-color: white;"
                "selection-color: white;"
                "selection-background-color: white;");
        }
    }
}

void Sudoku::tipClick() {
    MineLineEdit *mle = qobject_cast<MineLineEdit*>(sender());
    int i = mle->accessibleName().toInt();
    int solution[81];
    bool f = false;
    try {
        f = sudoku.solve(result[0], solution);
    }
    catch (const std::exception&) {
        QMessageBox::information(this, tr("tip"), tr("Already Wrong"));
        return;
    }
    if (f) 
    {
        mle->setText(QString::number(solution[i]));
        result[0][i] = solution[i];
        sudoTable[i / 9][i % 9] = solution[i];
        mle->setStyleSheet("color: blue;");
    }
    else {
        QMessageBox::information(this, tr("tip"), tr("Already Wrong"));
    }
}

bool checkValid(int final[9][9], int row, int col, int value)
{
    for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) // 检测该块是否已有该数字
        for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
        {
            if (i == row&&j == col)
                continue;
            if (final[i][j] == value)
                return false;
        }
    for (int i = 0; i < 9; i++) // 检测该行该列是否已有该数字
        if ((i != col&&final[row][i] == value) || (i != row&&final[i][col] == value))
            return false;
    return true;
}

void Sudoku::sudoTableEdit() 
{
    MineLineEdit *mle = qobject_cast<MineLineEdit*>(sender());
    if (mle->isReadOnly()) 
        return;
    int i = mle->accessibleName().toInt();
    int x = mle->text().toInt();
    sudoTable[i / 9][i % 9] = 0;
    if (checkValid(sudoTable, i / 9, i % 9, x))
    {
        result[0][i] = mle->text().toInt();
        sudoTable[i / 9][i % 9] = mle->text().toInt();
        mle->setStyleSheet("color: black;");
    }
    else
    {
        result[0][i] = mle->text().toInt();
        sudoTable[i / 9][i % 9] = mle->text().toInt();
        mle->setStyleSheet("color: red;");
    }
    if (isfilled(sudoTable)) 
    {
        stopmTimer();
        QString *qstr = new QString();
        readInit(QString::number(model), *qstr);
        if (qstr->toInt() > timeTimer)
        {
            writeInit(QString::number(model), QString::number(timeTimer));
            QMessageBox::information(this, tr("Congratulations"), tr("You Win! New Record!"));
        }
        else
            QMessageBox::information(this, tr("Congratulations"), tr("You Win!"));
        for (int i = 0; i < 81; i++)
            sudo[i]->setReadOnly(true);
    }
}

bool Sudoku::isfilled(int sudoTable[9][9])
{
    for (int i = 0; i < 81; i++)
    {
        int value = sudoTable[i / 9][i % 9];
        if (value == 0 || !checkValid(sudoTable, i / 9, i % 9, value))
            return false;
    }
    return true;
}

void Sudoku::startmTimer() 
{
    timeTimer = 0;
    mTimer->start(1000);
}
void Sudoku::stopmTimer() 
{
    mTimer->stop();
}


void Sudoku::displayTimer() 
{
    timeTimer += 1;
    if (timeTimer == 99999) {
        lb_timer->setText("99999");
        return;
    }
    QString qstr = QString::number(timeTimer);
    lb_timer->setText(qstr);
}

void Sudoku::writeInit(QString key, QString value)
{
    QString path = "sudo.ini";

    //创建配置文件操作对象  
    QSettings *config = new QSettings(path, QSettings::IniFormat);
    //将信息写入配置文件  
    config->beginGroup("history");
    config->setValue(key, value);
    config->endGroup();
    delete config;
}

void Sudoku::readInit(QString key, QString &value) {
    QString path = "sudo.ini";

    //创建配置文件操作对象  
    QSettings *config = new QSettings(path, QSettings::IniFormat);
    //读取配置信息  
    value = config->value(QString("history/") + key, QVariant(QString::number(99999))).toString();

    delete config;
}

void Sudoku::help()
{
    helpLayout->show();
}