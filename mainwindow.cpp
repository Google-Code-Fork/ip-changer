#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(updateList()));
    connect(ui->pushButton_2, SIGNAL(clicked()), qApp, SLOT(quit()));

    QDir versionsDir(qApp->applicationDirPath()+QString("/versions"));
    QStringList versions=versionsDir.entryList(QDir::Files);
    if(!versions.size()){
        QMessageBox::critical(this, "Error", "Directory \"versions\" is empty or does not exist.\nPlease reinstall the program.");
    }

    ui->comboBox_2->addItems(versions);

    updateList();



    std::ifstream history(QString(qApp->applicationDirPath()+"/history.txt").toStdString().c_str());
    if(history.is_open()){
        while(history.good()){
            char buff[512];
            history.getline(buff, 512);
            ui->comboBox->insertItem(0, buff);
        }
        ui->comboBox->removeItem(0);
        ui->comboBox->setCurrentIndex(0);
        ui->comboBox->setDuplicatesEnabled(false);
        history.close();
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

    QString version=qApp->applicationDirPath()+"/versions/"+ui->comboBox_2->currentText();
    Client client(pid, version.toStdString());


    if(!client.changeIP(ui->comboBox->currentText().toStdString(), ui->spinBox->value())){
        QMessageBox::critical(this, "Error", "IP could not be changed.");
    }

    std::fstream history(QString(qApp->applicationDirPath()+"/history.txt").toStdString().c_str(),
                        std::fstream::out|std::fstream::in|std::fstream::app);
    if(!history.is_open()){
        QMessageBox::warning(this, "Error", "Could not edit \"history.txt\".");
    }
    else{
        history<<ui->comboBox->currentText().toStdString()<<std::endl;
        history.close();
    }
}
