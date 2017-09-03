#ifndef SUDOTIMER_H
#define SUDOTIMER_H

#include <QWidget>
#include <QLCDNumber>
#include <QTime>
#include <QTimer>

class SudoTimer : public QWidget
{
    Q_OBJECT
public:
    explicit SudoTimer(QWidget *parent = nullptr);

signals:

public slots:
    void _show_time();
public:
    void _start();
    void _pause();
    void _continue();
    void _initialize();

private:
    QTimer* timer;
    QLCDNumber* lcd;
    int m_second, m_minute;
};

#endif // SUDOTIMER_H
