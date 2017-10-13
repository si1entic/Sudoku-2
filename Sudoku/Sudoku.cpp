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


    QMenu *menuNew = menuBar()->addMenu(tr("&New"));
    menuNew->addAction(easyOpenAction);
    menuNew->addAction(normalOpenAction);
    menuNew->addAction(hardOpenAction);
    menuNew->addAction(recordAction);

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
    btn_submit = new QPushButton(this);
    connect(btn_submit, SIGNAL(clicked()), this, SLOT(open()));
    topLayout->addWidget(btn_submit, 0, 2, 0);
    lb_timer = new QLabel(this);
    lb_timer->setText("0");
    lb_timer->setAlignment(Qt::AlignRight);
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
        //		sudo[i]->setContextMenuPolicy(Qt::NoContextMenu);
        sudo[i]->setValidator(new QRegExpValidator(rx, sudo[i]));
        sudo[i]->setReadOnly(true);

        sudo[i]->setAccessibleName(QString::number(i));
        sudo[i]->setStyleSheet(
            "background-color: yellow;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }

    tipAction = new QAction(tr("Easy"), this);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            midLayoutIn[i][j] = new QGridLayout(this);
            midLayoutIn[i][j]->setMargin(1);
            midLayout->addLayout(midLayoutIn[i][j], i, j, 0);
        }
    }
    for (int i = 0; i < 81; i++) {
        midLayoutIn[i / 9 / 3][i % 9 / 3]->addWidget(sudo[i], i / 9, i % 9, 0);
        connect(sudo[i], SIGNAL(tip_clicked()), this, SLOT(tipClick()));
        connect(sudo[i], SIGNAL(textChanged(const QString&)), this, SLOT(sudoTableEdit()));
    }

    this->centralWidget()->setLayout(mainLayout);


}



void Sudoku::open()
{
    QMessageBox::information(this, tr("Information"), tr("Open"));
}

void Sudoku::paintEvent(QPaintEvent *event)
{
    /*QPainter p(this);
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::black);
    p.setPen(pen);
    for (int i = 3; i<8; i += 3)
    {
    int x1 = (sudo[i]->geometry().left() + sudo[i - 1]->geometry().right()) / 2;
    int y1 = sudo[i]->geometry().top();
    int y2 = sudo[8*9+i]->geometry().bottom();
    p.drawLine(x1 + 1, y1 + 2 , x1 + 1, y2 );
    }
    for (int i = 3; i<8; i += 3)
    {
    int y1 = (sudo[i*9]->geometry().top() + sudo[(i - 1)*9]->geometry().bottom()) / 2;
    int x1 = sudo[i*9]->geometry().left();
    int x2 = sudo[i*9+8]->geometry().right();
    p.drawLine(x1 + 2, y1 + 1, x2 - 2, y1 + 1);
    }
    */
}



void Sudoku::easyOpen() {
    model = 1;
    lb_model->setText("Easy");
    btn_start->setText("start");
    for (int i = 0; i < 81; i++) {
        sudo[i]->setText("");
        sudo[i]->setReadOnly(true);
        sudo[i]->setStyleSheet(
            "background-color: yellow;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }
}
void Sudoku::normalOpen() {
    model = 2;
    lb_model->setText("Normal");
    btn_start->setText("start");
    for (int i = 0; i < 81; i++) {
        sudo[i]->setText("");
        sudo[i]->setReadOnly(true);
        sudo[i]->setStyleSheet(
            "background-color: yellow;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }
}
void Sudoku::hardOpen() {
    model = 3;
    lb_model->setText("Hard");
    btn_start->setText("start");
    for (int i = 0; i < 81; i++) {
        sudo[i]->setText("");
        sudo[i]->setReadOnly(true);
        sudo[i]->setStyleSheet(
            "background-color: yellow;"
            "selection-color: yellow;"
            "selection-background-color: blue;");
    }
}
void Sudoku::record() {
    QString *qstr;
    readInit("1", *qstr);
    QMessageBox::information(this, tr("Congratulations"), *qstr);
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
                "background-color: yellow;"
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
    qDebug() << "tip clicked:" << i;
    int solution[81];
    bool f = false;
    try {
        f = sudoku.solve(result[0], solution);
    }
    catch (const std::exception&) {
        QMessageBox::information(this, tr("tip"), tr("Already Wrong"));
        return;
    }

    if (f) {
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

    if (mle->isReadOnly()) {
        return;
    }

    int i = mle->accessibleName().toInt();
    int x = mle->text().toInt();
    qDebug() << "sudo edit:" << i << " text:" << x;
    if (checkValid(sudoTable, i / 9, i % 9, x))
    {
        result[0][i] = mle->text().toInt();
        sudoTable[i / 9][i % 9] = mle->text().toInt();
        mle->setStyleSheet("color: black;");
        qDebug() << "yes";
    }
    else
    {
        result[0][i] = mle->text().toInt();
        sudoTable[i / 9][i % 9] = mle->text().toInt();
        mle->setStyleSheet("color: red;");
        qDebug() << "no";
    }
    if (isfilled(sudoTable)) 
    {
        qDebug() << "isfilled";
        QMessageBox::information(this, tr("Congratulations"), tr("You Win! Time consume:"));
        qDebug() << timeTimer;
        writeInit(QString::number(model), QString::number(timeTimer));
        stopmTimer();
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

void Sudoku::startmTimer() {
    timeTimer = 0;
    mTimer->start(1000);
}
void Sudoku::stopmTimer() {
    mTimer->stop();
}


void Sudoku::displayTimer() {
    timeTimer += 1;
    QString qstr = QString::number(timeTimer);
    setStrLength(&qstr, 3);
    lb_timer->setText(qstr);
}


void Sudoku::setStrLength(QString *qstr, int len) {
    if (qstr->length() < len) {
        qstr->insert(0, "0");
    }
}

void Sudoku::writeInit(QString key, QString value) {
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
    value = QString("");
    QString path = "sudo.ini";

    //创建配置文件操作对象  
    QSettings *config = new QSettings(path, QSettings::IniFormat);

    //读取配置信息  
    value = config->value(QString("history/") + key).toString();
    delete config;
}