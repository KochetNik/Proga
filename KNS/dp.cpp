#include "dp.h"
#include "ui_dp.h"

DP::DP(QWidget *parent): QMainWindow(parent), ui(new Ui::DP) {
    ui->setupUi(this);

    this->setFixedSize(340, 239);

    ui->btn_help->setIcon(QPixmap(QApplication::applicationDirPath() + "/files/Ico/i.ico"));
    ui->btn_exit->setIcon(QPixmap(QApplication::applicationDirPath() + "/files/Ico/exit.ico"));

    ui->lbl_ico_create->setPixmap(QIcon(QApplication::applicationDirPath() + "/files/Ico/create.ico").pixmap(64, 64));
    ui->lbl_ico_open->setPixmap(QIcon(QApplication::applicationDirPath() + "/files/Ico/csv.ico").pixmap(64, 64));

    key_info = new QShortcut(this);
    key_info->setKey(Qt::Key_F1);
    connect(key_info, SIGNAL(activated()), this, SLOT(on_btn_help_triggered()));

    form_create = new Create;
}

DP::~DP() {
    delete ui;
}

void DP::on_btn_create_clicked() {
    if(service_variable == 0)
        QMessageBox::information(0, "Ошибка сети", "На узле не развернут ПК СВ Брест");
    else {
        form_create->setWindowModality(Qt::ApplicationModal);
        form_create->show();
    }
}

void DP::on_btn_open_clicked() {
    if(service_variable == 0)
        QMessageBox::information(0, "Ошибка сети", "На узле не развернут ПК СВ Брест");
    else {
        QFile path(QCoreApplication::applicationDirPath() + "/files/path/openVM.txt");
        QString kns;
        if(path.open(QFile::ReadOnly))
            kns += path.readAll();
        qDebug() << kns;
        QString command = "bash";
        QStringList args;
        args << "-c" << kns;
        QProcess::startDetached(command, args);
    }
}

void DP::on_btn_help_triggered() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/files/Руководство пользователя.docx"));
}

void DP::on_btn_exit_triggered() {
    this->close();
}
