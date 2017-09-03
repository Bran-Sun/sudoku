#include "sudotimer.h"

SudoTimer::SudoTimer(QWidget *parent) : QWidget(parent)
{
    lcd = new QLCDNumber(this);
    timer = new QTimer(this);
    //设置定时器每个多少毫秒发送一个timeout()信号
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(_show_time()));

    lcd->setGeometry(0, 0, 90, 50);

    //设置晶体管控件QLCDNumber能显示的位数
    lcd->setDigitCount(6);
    //设置显示的模式为十进制
    lcd->setMode(QLCDNumber::Dec);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    lcd->setStyleSheet("border: 0px solid white; color: black;");
    lcd->display("00:00");

    m_second = 0;
    m_minute = 0;
}

void SudoTimer::_initialize()
{
    m_second = 0;
    m_minute = 0;
    lcd->display("00:00");
}

void SudoTimer::_start()
{
    _initialize();
    timer->start();
}

void SudoTimer::_show_time()
{
    m_second++;
    m_minute += m_second / 60;
    m_second = m_second % 60;
    char s[10];
    sprintf(s, "%02d:%02d", m_minute, m_second);
    lcd->display(s);
}

void SudoTimer::_pause()
{
    timer->stop();
}

void SudoTimer::_continue()
{
    timer->start();
}
