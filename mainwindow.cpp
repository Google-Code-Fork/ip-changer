#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(updateList()));
    connect(ui->pushButton_2, SIGNAL(clicked()), qApp, SLOT(quit()));

    QDir versionsDir("./versions");
    QStringList versions=versionsDir.entryList(QDir::Files);
    if(!versions.size()){
        QMessageBox::critical(this, "Error", "Directory \"versions\" is empty or does not exist.\nPlease reinstall the program.");
    }

    ui->comboBox_2->addItems(versions);

    updateList();



    std::ifstream config("./history.txt");
    if(config.is_open()){
        while(config.good()){
            char buff[512];
            config.getline(buff, 512);
            ui->comboBox->insertItem(0, buff);
        }
        ui->comboBox->removeItem(0);
        ui->comboBox->setCurrentIndex(0);
        config.close();
    }
}

void MainWindow::updateList(){
    ui->comboBox_3->clear();
    if(!pList::getList(processes, false)){
        QMessageBox::critical(this, "Error", "Could not get list of running processes.");
    }

    for(unsigned i=0;i<processes.size();i++){
        QString str=processes[i].name.c_str();
        str.append(" (");
        str.append(QString::number(processes[i].pid));
        str.append(")");
        ui->comboBox_3->addItem(str);
        if(str.contains("tibia", Qt::CaseInsensitive))
            ui->comboBox_3->setCurrentIndex(i);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    pid_t pid=processes[ui->comboBox_3->currentIndex()].pid;
    Client client(pid, ui->comboBox_2->currentText().toStdString());
    if(!client.changeIP(ui->comboBox->currentText().toStdString(), ui->spinBox->value())){
        QMessageBox::critical(this, "Error", "IP could not be changed.");
    }

    std::ofstream config("./history.txt", std::ofstream::out|std::ofstream::app);
    if(!config.is_open()){
        QMessageBox::warning(this, "Error", "Could not edit config file.");
    }

    config<<ui->comboBox->currentText().toStdString()<<std::endl;
    config.close();
}
