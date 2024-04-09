#ifndef DP_H
#define DP_H

#include <QMainWindow>
#include <QProcess>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QShortcut>
#include <QDebug>
#include <create.h>

QT_BEGIN_NAMESPACE
namespace Ui { class DP; }
QT_END_NAMESPACE

class DP : public QMainWindow {
    Q_OBJECT

public:
    DP(QWidget *parent = nullptr);
    ~DP();
    int service_variable = 1;

private slots:
    void on_btn_create_clicked();
    void on_btn_open_clicked();
    void on_btn_help_triggered();
    void on_btn_exit_triggered();

private:
    Ui::DP *ui;
    QShortcut *key_info;
    Create *form_create;
};
#endif // DP_H
