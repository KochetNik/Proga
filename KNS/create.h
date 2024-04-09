#ifndef CREATE_H
#define CREATE_H

#include <QWidget>
#include <QProcess>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>

namespace Ui {
class Create;
}

class Create : public QWidget {
    Q_OBJECT

public:
    explicit Create(QWidget *parent = nullptr);
    ~Create();
    void ish();
    double running_line_1 = 0, running_line_2 = 0;
    int value = 0, ID_count = 0;

private slots:
    void TimerSlot_1();
    void TimerSlot_2();
    void on_btn_page_1_clicked();
    void on_btn_page_2_clicked();
    void on_btn_page_3_clicked();
    void on_btn_fileDialog_clicked();
    void on_btn_load_clicked();

    void on_btn_page_7_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

private:
    Ui::Create *ui;
    QTimer *timer_1, *timer_2;
};

#endif // CREATE_H
