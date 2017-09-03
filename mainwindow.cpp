#include "mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSound>

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(500, 800);

    //background
    QPixmap pix;
    pix.load("../../../img/background.jpg");
    pix.scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pix));
    this->setPalette(palette);

    pix.load("../../../img/clock.png");
    m_clock = new QLabel(this);
    m_clock->setGeometry(25, 42, 40, 40);
    m_clock->setPixmap(pix);
    m_clock->setScaledContents(true);

    m_timer = new SudoTimer(this);
    m_timer->setGeometry(60, 35, 100, 60);

    QFont f;
    f.setPixelSize(25);
    m_diff = new QLabel("难度:", this);
    m_diff->setFont(f);
    m_diff->setGeometry(165, 30, 60, 60);

    m_list = new QComboBox(this);
    m_list->setStyleSheet("QComboBox{width: 80px; height: 20px; border-radius: 5px; border: 5px; font-size: 20pt;background-color: orange}"
                          "QComboBox QAbstractItemView::item {selection-color: black}");
    m_list->setGeometry(235, 45, 80, 30);

    m_start = new QPushButton(tr("开始"), this);
    m_start->setGeometry(350, 35, 50, 50);
    connect(m_start, SIGNAL(clicked(bool)), this, SLOT(_start()));
    m_start->setStyleSheet("QPushButton{color: white; background:#FFB90F; border-radius:5px; font-size: 20px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:white; color: #FFB90F;}");

    m_pause = new QPushButton(tr("停止"), this);
    m_pause->setGeometry(410, 35, 50, 50);
    connect(m_pause, SIGNAL(clicked(bool)), this, SLOT(_pause()));
    m_pause->setStyleSheet("QPushButton{color: white; background:#FFB90F; border-radius:5px; font-size: 20px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:white; color: #FFB90F;}");

    for (int i = 0; i < 10; i++)
    {
        char s[10];
        sprintf(s, "%d", i);
        m_list->addItem(s);
    }

    connect(m_list, SIGNAL(currentIndexChanged(int)), this, SLOT(_set_model_difficulty(int)));

    m_signal_mapper = new QSignalMapper(this);

    int deep2 = 600; 
    for (int i = 0; i < 9 ; i++)                 //(600, 700)
    {
        char s[10];
        sprintf(s, "%d", i + 1);
        m_figure[i] = new QPushButton(tr(s), this);
        m_figure[i]->setStyleSheet("QPushButton{color: #FF4500; background:#FFB90F; border-radius:5px; font-size: 30px}"
                                   "QPushButton:hover{background-color:white; color: #FFB90F;}");
        m_figure[i]->setGeometry(52 * i + 20, deep2, 47, 47);

        //connect
        connect(m_figure[i], SIGNAL(clicked(bool)), m_signal_mapper, SLOT(map()));
        m_signal_mapper->setMapping(m_figure[i], i + 1);
    }

    int weight = 50, height = 50;
    int deep3 = 670;

    m_clean = new QPushButton(tr("删除"), this);
    m_clean->setStyleSheet("QPushButton{color: white; background:#FFB90F; border-radius:5px; font-size: 20px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:white; color: #FFB90F;}");
    m_clean->setGeometry(200, deep3, 60, 50);
    connect(m_clean, SIGNAL(clicked(bool)), m_signal_mapper, SLOT(map()));
    m_signal_mapper->setMapping(m_clean, 0);
    connect(m_signal_mapper, SIGNAL(mapped(int)), this, SLOT(_set_model_num(int)));

    m_forward = new QPushButton(tr(""), this);
    QIcon ico("../../../img/next.png");
    m_forward->setGeometry(50, deep3, weight, height);
    m_forward->setIcon(ico);
    m_forward->setIconSize(QSize(50, 50));
    m_forward->setFlat(true);
    connect(m_forward, SIGNAL(clicked(bool)), this, SLOT(_forward()));


    m_back = new QPushButton(tr(""), this);
    QIcon ico2("../../../img/forward.png");
    m_back->setGeometry(130, deep3, weight, height);
    m_back->setIcon(ico2);
    m_back->setIconSize(QSize(50, 50));
    m_back->setFlat(true);
    connect(m_back, SIGNAL(clicked(bool)), this, SLOT(_back()));

    m_sign = new QPushButton(tr("标记"), this);
    m_sign->setStyleSheet("QPushButton{color: white; background:#FFB90F; border-radius:5px; font-size: 20px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:white; color: #FFB90F;}");
    m_sign->setGeometry(270, deep3, 60, 50);
    connect(m_sign, SIGNAL(clicked(bool)), this, SLOT(_sign()));

    m_read = new QPushButton(tr("清除"), this);
    m_read->setStyleSheet("QPushButton{color: white; background:#FFB90F; border-radius:5px; font-size: 20px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:white; color: #FFB90F;}");
    m_read->setGeometry(340, deep3, 60, 50);
    connect(m_read, SIGNAL(clicked(bool)), this, SLOT(_clean()));


    m_solve = new QPushButton(tr("求解"), this);
    m_solve->setStyleSheet("QPushButton{color: white; background:#FFB90F; border-radius:5px; font-size: 20px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:white; color: #FFB90F;}");
    m_solve->setGeometry(410, deep3, 60, 50);
    connect(m_solve, SIGNAL(clicked(bool)), this, SLOT(_solve()));

    update();
}


void mainWindow::paintEvent(QPaintEvent *ev) //(100, 600), 9 * 9
{
    QPainter p(this);

    QPen pen(Qt::black, 5);
    p.setPen(pen);

    int x1, x2, y1, y2;
    int interval = 150;
    int top = 120, left = 25;

    for (int i = 0; i < 4; i++)
    {
        x1 = left;
        x2 = left + 3 * interval;
        y1 = top + i * interval;
        y2 = y1;
        p.drawLine(x1, y1, x2, y2);
    }

    for (int i = 0; i < 4; i++)
    {
        x1 = left + interval * i;
        x2 = x1;
        y1 = top;
        y2 = top + interval * 3;
        p.drawLine(x1, y1, x2, y2);
    }

    pen.setWidth(3);
    p.setPen(pen);

    for (int i= 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            x1 = left;
            x2 = left + 3 * interval;
            y1 = top + i * interval + (j + 1) * interval / 3;
            y2 = y1;
            p.drawLine(x1, y1, x2, y2);
        }
    }
    for (int i= 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            y1 = top;
            y2 = top + 3 * interval;
            x1 = left + i * interval + (j + 1) * interval / 3;
            x2 = x1;
            p.drawLine(x1, y1, x2, y2);
        }
    }

    //paint the highlight
    _paint_highlight();

    // put in numbers
   _paint_number();

   //paint the sign
   _paint_sign();

   //paint the chosen row and column
   _paint_chosen();

}

void mainWindow::_paint_highlight()
{
    QPainter p(this);
    int interval = 50;
    int top = 120, left = 25;
    p.setBrush(QColor("#ADFF2F"));

    for (int i = 0; i < 9; i++)
        for (int j= 0; j < 9; j++)
        {
            if (m_model._get_if_highlight(i, j))
            {
                p.drawRect(left + interval * i, top + interval * j, interval - 2, interval - 2);
            }
        }

}

void mainWindow::_paint_number()  //draw number from model
{
    QPainter p(this);

    QFont font("Helvetica");
    font.setPixelSize(30);

    p.setFont(font);

    int top = 140, left = 25, margin = 15, interval = 50;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            char s[10];
            std::vector<int> num = m_model._get_data(i, j);
            if (num.size() == 1)
            {
                font.setPixelSize(30);
                p.setFont(font);
                if (num[0] == 0)
                    sprintf(s, " ");
                else
                    sprintf(s, "%d", num[0]);
                if (m_model._fixed(i, j))
                    p.setPen(QColor("#919191"));
                else
                    p.setPen(Qt::black);
                p.drawText(left + interval * i + margin, top + interval * j + margin, s);
            }
            else if (num.size() > 1)
            {
                font.setPixelSize(12);
                p.setFont(font);
                p.setPen(Qt::black);

                int x = left + interval * i + margin - 10, y = top + interval * j + margin - 20;
                for (int k = 0; k < num.size(); k++)
                {
                     int xi = (num[k] - 1) % 3;
                     int yi = (num[k] - 1) / 3;
                     sprintf(s, "%d", num[k]);
                     p.drawText(x + xi * 15, y + yi * 15, s);
                }
            }
        }
}

void mainWindow::_paint_chosen() //paint the chosen row and column
{
    if (m_model._get_if_chosen())
    {
        int index = m_model._get_chosen_index();
        int x = index % 9;
        int y = index / 9;

        QPainter p(this);
        QPen pen(QColor("#A4D3EE"), 5);
        p.setPen(pen);

        int x1, x2, y1, y2;
        int interval = 50;
        int top = 120, left = 25;

        for (int i = 0; i < 2; i ++)
        {
            x1 = left + (i + x) * interval;
            x2 = x1;
            y1 = top;
            y2 = top + interval * 9;
            p.drawLine(x1, y1, x2, y2);
        }
        for (int i = 0; i < 2; i ++)
        {
            x1 = left;
            x2 = left + interval * 9;
            y1 = top + (i + y) * interval;
            y2 = y1;
            p.drawLine(x1, y1, x2, y2);
        }

    }
}

void mainWindow::_paint_sign()
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::red);

    int interval = 50;
    int top = 125, left = 30;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if (m_model._get_if_sign(i, j))
            {
                QPolygon polygon;
                QPoint p1(left + interval * i, top + interval * j);
                QPoint p2(left + interval * i - 8, top + interval * j + 10);
                QPoint p3(left + interval * i + 3, top + interval * j + 10);
                polygon.append(p1);
                polygon.append(p2);
                polygon.append(p3);

                p.drawPolygon(polygon);
            }
        }
}

void mainWindow::mousePressEvent(QMouseEvent *ev)
{
    int interval = 50;
    int top = 120, left = 25;

    int x = ev->pos().rx();
    int y = ev->pos().ry();

    int xi = (x - left) / interval;
    int yi = (y - top) / interval;

    if (x < left) xi = -1;
    if (y < top) yi = -1;

  //  std::cout << x << " " << y << std::endl;
    m_model._set_chosen(xi, yi);
    m_model._set_highlight(xi, yi);
    update();
}

void mainWindow::keyPressEvent(QKeyEvent *ev)
{

}

void mainWindow::_set_model_num(int num)
{
    m_model._set_number(num);
    update();
    if (m_model._check_solved())
        _succeed();
}

void mainWindow::_set_model_difficulty(int diff)
{
    m_model._set_difficulty(diff);
}

void mainWindow::_start()
{
    m_model._initialize();
    m_timer->_start();
    update();
}

void mainWindow::_pause()
{
    m_timer->_pause();
    QMessageBox box(this);

    int ret = box.information(this, "pause", "\n\n\n   whether to continue?    \n\n\n", QMessageBox::Yes, QMessageBox::Yes);
    update();
    if (ret == QMessageBox::Yes)
    {
        _continue();
    }
}

void mainWindow::_continue()
{
    m_timer->_continue();
}

void mainWindow::_back()
{
    m_model._back();
    update();
}

void mainWindow::_forward()
{
    m_model._forward();
    update();
}

void mainWindow::_sign()
{
    m_model._sign();
    update();
}

void mainWindow::_solve()
{
    m_model._solve();
    update();
}

void mainWindow::_succeed()
{
    m_timer->_pause();
    QSound::play("../../../img/victory.wav");
    QMessageBox box(this);
    int ret = box.information(this, "result", "Congratulations!!!", QMessageBox::Yes, QMessageBox::Yes);
}

void mainWindow::_clean()
{
    m_model._clean();
    m_timer->_initialize();
    m_timer->_pause();
    update();
}
