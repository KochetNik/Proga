#include "create.h"
#include "ui_create.h"

Create::Create(QWidget *parent): QWidget(parent), ui(new Ui::Create) {
    ui->setupUi(this);

    this->setFixedSize(475, 342);
    ish();

    timer_1 = new QTimer(this);
    timer_2 = new QTimer(this);
    connect(timer_1, SIGNAL(timeout()), this, SLOT(TimerSlot_1()));
    connect(timer_2, SIGNAL(timeout()), this, SLOT(TimerSlot_2()));

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_2->setValidator(ipValidator);
    ui->lineEdit_3->setValidator(ipValidator);
    ui->lineEdit_4->setValidator(ipValidator);
}

Create::~Create() {
    delete ui;
}

void Create::ish() {
    ui->stackedWidget->setCurrentIndex(0);
    ui->edit_path->clear();
    ui->btn_fileDialog->setEnabled(true);
    ui->btn_load->setEnabled(false);
    value = 0;
    ui->progressBar_1->setValue(0);
    ui->btn_page_1->setEnabled(false);

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();

    ui->edit_nameVM->clear();
    ui->sBox_1->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->sBox_3->setEnabled(true);
    ui->btn_page_3->setEnabled(true);
    ui->progressBar_2->setValue(0);
    ui->edit_nameVM->setEnabled(true);
}

void Create::TimerSlot_1() {
    ui->progressBar_1->setValue(int(value * running_line_1));
    value++;
    if(ui->progressBar_1->value() > 90) {
        ui->progressBar_1->setValue(100);
        ui->btn_page_1->setEnabled(true);
        value = 0;
        timer_1->stop();
    }
}

void Create::TimerSlot_2() {
    ui->progressBar_2->setValue(int(value * running_line_2));
    value++;
    if(ui->progressBar_2->value() > 90) {
        ui->progressBar_2->setValue(100);
        value = 0;
        timer_2->stop();
        QMessageBox::information(0, "Создание VM", "VM успешно создана!");
        this->close();
        ish();

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

void Create::on_btn_page_1_clicked() {
    ui->stackedWidget->setCurrentIndex(1);

}

void Create::on_btn_page_2_clicked() {
    QString command = "oneimage create --datastore default --name HD-" + QString::number(ID_count) +
                      " --type DATABLOCK --driver qcow2 --prefix vd --size " + ui->cBox_1->currentText();
    qDebug() << command;
    QProcess::startDetached("bash", QStringList() << "-c" << command);

    ui->stackedWidget->setCurrentIndex(2);
}

void Create::on_radioButton_2_clicked() {
    if(ui->radioButton_2->isChecked()) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
        ui->spinBox->setEnabled(true);
    }
}

void Create::on_radioButton_clicked() {
    if(ui->radioButton->isChecked()) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(false);
        ui->lineEdit_3->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
        ui->spinBox->setEnabled(false);
    }
}

void Create::on_btn_page_7_clicked() {
    if(ui->radioButton_2->isChecked()) {
        int ID_2 = 0;
        QFile ID_new(QApplication::applicationDirPath() + "/files/path/ID_2.txt");
        if(ID_new.open(QFile::ReadOnly)) {
            ID_2 = QString(ID_new.readAll()).toInt();
            ID_new.close();
        }

        if(ui->lineEdit->text().isEmpty())
            QMessageBox::warning(0, "Уведомление", "Заполните поле «Имя сети»!");
        else if(ui->lineEdit_2->text().isEmpty())
            QMessageBox::warning(0, "Уведомление", "Заполните поле «Первый IP-адрес»!");
        else if(ui->lineEdit_3->text().isEmpty())
            QMessageBox::warning(0, "Уведомление", "Заполните поле «DNS-сервер»!");
        else if(ui->lineEdit_4->text().isEmpty())
            QMessageBox::warning(0, "Уведомление", "Заполните поле «Адрес сети»!");
        else {
            QFile file(QApplication::applicationDirPath() + "/priv.net");
            if(file.open(QFile::WriteOnly)) {
                QTextStream stream(&file);
                stream << "NAME = \"" + ui->lineEdit->text() + "\"" << endl;
                stream << "VN_MAD = \"bridge\"" << endl;
                stream << "AR = [TYPE = \"IP4\", IP = \"" + ui->lineEdit_2->text() + "\", SIZE = \"" + ui->spinBox->text() + "\"]" << endl;
                stream << "DNS = \"" + ui->lineEdit_3->text() + "\"" << endl;
                stream << "NETWORK_ADDRESS = \"" + ui->lineEdit_4->text() + "\"" << endl;
                stream << "NETWORK_MASK = \"" + ui->spinBox_2->text() + "\"" << endl;
                stream << "BRIDGE = \"bridge" + QString::number(ID_2) + "\"";
                file.close();
            }



            QString ip_address = ui->lineEdit_2->text();
            QStringList IP_ADDRESS_PARSER = ip_address.split(".");
            int four_ok = IP_ADDRESS_PARSER[3].toInt();
            four_ok--;
            ip_address = IP_ADDRESS_PARSER[0] + "." + IP_ADDRESS_PARSER[1] + "." + IP_ADDRESS_PARSER[2] + "." + QString::number(four_ok);
            qDebug() << ip_address;

            QString str_1 = "nmcli connection add ifname bridge" + QString::number(ID_2) + " type bridge ipv4.address " + ip_address +"/"
                            + ui->spinBox_2->text() + " ipv4.dns " + ui->lineEdit_3->text() + " ipv4.method manual && onevnet create priv.net";
            QProcess::startDetached("bash",QStringList() << "-c" << str_1);
            qDebug() << str_1;

            if(ID_new.open(QFile::WriteOnly)) {
                QTextStream stream(&ID_new);
                ID_2++;
                qDebug() << ID_2;
                stream << QString::number(ID_2);
                ID_new.close();
            }

            ui->stackedWidget->setCurrentIndex(3);
        }
    }
    else {
        if(ui->lineEdit->text().isEmpty())
            QMessageBox::warning(0, "Уведомление", "Заполните поле «Имя сети»!");
        else
            ui->stackedWidget->setCurrentIndex(3);
    }
}

void Create::on_btn_page_3_clicked() {
    if(!ui->edit_nameVM->text().isEmpty()) {
        double cpu = ui->sBox_1->text().toDouble();
        cpu = cpu / 100.0;
    if(ui->spinBox_3->text().toInt()==1){
        QString command = "onetemplate create --name " + ui->edit_nameVM->text() +
                          " --memory " + ui->comboBox->currentText() + " --cpu " + QString::number(cpu) +
                          " --vcpu " + ui->sBox_3->text() + " --disk ISO-" + QString::number(ID_count) +
                          ",HD-" + QString::number(ID_count) + " --nic " + ui->lineEdit->text() +
                          " && ansible all -m shell -a \"sudo systemctl restart opennebula.service\"" +
                          " && onetemplate instantiate " + QString::number(ID_count) +
                          " && ansible all -m shell -a \"sudo systemctl restart opennebula.service\"";
        qDebug() << command;
        QProcess::startDetached("bash", QStringList() << "-c" << command);}
    else {
            QString command = "onetemplate create --name " + ui->edit_nameVM->text() +
                          " --memory " + ui->comboBox->currentText() + " --cpu " + QString::number(cpu) +
                          " --vcpu " + ui->sBox_3->text() + " --disk ISO-" + QString::number(ID_count) +
                          ",HD-" + QString::number(ID_count) + " --nic " + ui->lineEdit->text() +
                          " && ansible all -m shell -a \"sudo systemctl restart opennebula.service\"" +
                          " && onetemplate instantiate " + QString::number(ID_count) + " --multiple "+ ui->spinBox_3->text()+
                          " && ansible all -m shell -a \"sudo systemctl restart opennebula.service\"";
        qDebug() << command;
        QProcess::startDetached("bash", QStringList() << "-c" << command);
          }
        //command = "onetemplate instantiate " + QString::number(ID_count);
        //qDebug() << command;
        //QProcess::startDetached("bash", QStringList() << "-c" << command);

        timer_2->start(1000);
        ui->sBox_1->setEnabled(false);
        ui->comboBox->setEnabled(false);
        ui->sBox_3->setEnabled(false);
        ui->btn_page_3->setEnabled(false);
        ui->edit_nameVM->setEnabled(false);
    }
    else
        QMessageBox::warning(0, "Уведомление", "Для начала введите имя VM!");
}

void Create::on_btn_fileDialog_clicked() {
    QString models_path = "/var/tmp/";
    QString file = QFileDialog::getOpenFileName(this, "Выберите ISO-образ", models_path);
    if(!file.isEmpty()) {
        ui->edit_path->setText(file);
        ui->btn_load->setEnabled(true);
    }
}

void Create::on_btn_load_clicked() {
    //
    int pBar_1 = 4, pBar_2 = 2;
    QString str_1 = "4", str_2 = "2";
    QFile time(QCoreApplication::applicationDirPath() + "/files/pBar.kns");
    if(!time.exists())
        pBar_1++;
    else {
        int pp = 0;
        if(time.open(QFile::ReadOnly))
            while(!time.atEnd()) {
                if(pp == 0)
                    str_1 = QString::fromUtf8(time.readLine());
                if(pp == 1)
                    str_2 = QString::fromUtf8(time.readLine());
                pp++;
            }
        time.close();
        pp = 0;
    }
    //str_1 = str_1.remove(1, 4);
    //str_2 = str_2.remove(1, 4);

    pBar_1 = str_1.toInt();
    pBar_2 = str_2.toInt();
    qDebug() << pBar_1 << " " << pBar_2;
    running_line_1 = 100.0 / (pBar_1 * 60);
    running_line_2 = 100.0 / (pBar_2 * 60);
    timer_1->start(1000);
    ui->btn_fileDialog->setEnabled(false);
    ui->btn_load->setEnabled(false);

    QFile ID(QCoreApplication::applicationDirPath() + "/files/path/ID.txt");
    QString ID_str;
    if(ID.open(QFile::ReadOnly))
        ID_str = QString::fromUtf8(ID.readLine());
    ID.close();
    //ID_str = ID_str.remove(ID_str.size() - 1, 1);
    ID_count = ID_str.toInt();
    qDebug() << ID_count;
    ID_count++;
    if(ID.open(QFile::WriteOnly)) {
        QTextStream stream(&ID);
        stream << ID_count;
    }
    ID.close();

    QString command = "oneimage create --datastore default --name ISO-" + QString::number(ID_count) +
                      " --type CDROM --path " + ui->edit_path->text() + " --driver raw";
    qDebug() << command;

    QProcess::startDetached("bash", QStringList() << "-c" << command);

    //ui->btn_page_1->setEnabled(true);
}
