#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QSignalMapper>
#include <QMessageBox>
#include <QLabel>
#include <QComboBox>
#include <QPalette>
#include <QInputDialog>

#include "model.h"
#include "sudotimer.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void _set_model_num(int num);
    void _set_model_difficulty(int diff);
    void _start();
    void _continue();
    void _pause();
    void _back();
    void _forward();
    void _sign();
    void _clean();
    void _solve();

private:
    //void _restart();
    void _paint_number();
    void _paint_chosen();
    void _paint_sign();
    void _paint_highlight();

    void _succeed();

protected:
    void paintEvent(QPaintEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    QPushButton *m_pause, *m_start, *m_clean, *m_forward, *m_back;
    QSignalMapper *m_signal_mapper;
    QPushButton *m_figure[9];
    QPushButton *m_sign;
    QPushButton *m_solve;
    QPushButton *m_read;
    QLabel* m_clock;

    model m_model;

    QLabel* m_diff;
    QComboBox* m_list;

    SudoTimer* m_timer;
};

#endif // MAINWINDOW_H
