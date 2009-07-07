#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(updateList()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(hide()));

    loadVersions("versions");

    updateList();

    readHistory("history.txt");

    createActions();
    connect(ui->pushButton_2, SIGNAL(clicked()), actionExit, SLOT(trigger()));

    createTrayIcon();
}

void MainWindow::loadVersions(QString dir){
    QDir versionsDir(qApp->applicationDirPath()+"/"+dir);
    QStringList versions=versionsDir.entryList(QDir::Files);
    if(!versions.size()){
        QMessageBox::critical(this, "Error", "Directory \"versions\" is empty or does not exist.\nPlease reinstall the program.");
    }

    ui->comboBox_2->addItems(versions);
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

void MainWindow::readHistory(QString filename){
    std::string file=QString(qApp->applicationDirPath()+"/"+filename).toStdString();
    std::list<std::string> items;
    history=new History(file);
    history->read(items);

    for(std::list<std::string>::const_iterator item=items.begin();item!=items.end();item++)
        ui->comboBox->addItem((*item).c_str());
}

void MainWindow::on_pushButton_clicked()
{
    pid_t pid=processes[ui->comboBox_3->currentIndex()].pid;

    QString version=qApp->applicationDirPath()+"/versions/"+ui->comboBox_2->currentText();
    Client client(pid, version.toStdString());


    if(!client.changeIP(ui->comboBox->currentText().toStdString(), ui->spinBox->value())){
        QMessageBox::critical(this, "Error", "IP could not be changed.");
    }

    if(!history->write(ui->comboBox->currentText().toStdString())){
        QMessageBox::warning(this, "Warning", "Could not edit history file.");
    }
}

void MainWindow::createActions(){
    actionMinimize=new QAction(QIcon(":/icons/icons/bottom.png"), "&minimize", this);
    actionRestore=new QAction(QIcon(":/icons/icons/top.png"), "&restore", this);
    actionExit=new QAction(QIcon(":/icons/icons/quit.png"), "&Quit", this);
    connect(actionMinimize, SIGNAL(triggered()), this, SLOT(hide()));
    connect(actionRestore, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon(){
    QMenu *trayMenu=new QMenu(this);
    trayMenu->addAction(actionRestore);
    trayMenu->addAction(actionMinimize);
    trayMenu->addAction(actionExit);

    trayIcon=new QSystemTrayIcon(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/icons/icons/tools.png"));
    trayIcon->setToolTip("IP Changer");

    trayIcon->setVisible(true);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch(reason){
        case QSystemTrayIcon::Context:
        break;
        default:
            showNormal();
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *e){
    hide();
    e->ignore();
}

MainWindow::~MainWindow(){
    delete ui;
}
