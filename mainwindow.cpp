#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(updateProcList()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(hide()));

    loadDirs();

    loadVersions();

    updateProcList();

    readHistory("history.txt");

    createActions();
    connect(ui->pushButton_2, SIGNAL(clicked()), actionExit, SLOT(trigger()));

    createTrayIcon();
}

void MainWindow::loadDirs(){
    versionsDir=new QDir(qApp->applicationDirPath()+"/versions");
    if(!versionsDir->exists() || versionsDir->entryList(QDir::Files).isEmpty())
        QMessageBox::critical(this, "Error", "Versions directory is empty or doesn't exist.\nPlease reinstall the program.");

    confDir=new QDir(QString(getenv("HOME"))+"/.ip-changer");
    if(!confDir->exists())
        confDir->mkpath(confDir->path());
}

void MainWindow::loadVersions(){
    QStringList versions=versionsDir->entryList(QDir::Files);

    ui->comboBox_2->addItems(versions);
}

void MainWindow::updateProcList(){
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
    history=new History((confDir->path()+QDir::separator()+filename).toStdString());

    std::list<std::string> items;
    history->read(&items);

    for(std::list<std::string>::const_iterator item=items.begin();item!=items.end();item++)
        ui->comboBox->addItem((*item).c_str());
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

void MainWindow::on_pushButton_clicked()
{
    pid_t pid=processes[ui->comboBox_3->currentIndex()].pid;

    QString version=versionsDir->path()+'/'+ui->comboBox_2->currentText();
    Client client(pid, version.toStdString());


    if(!client.changeIP(ui->comboBox->currentText().toStdString(), ui->spinBox->value())){
        QMessageBox::critical(this, "Error", "IP could not be changed.");
    }

    if(!history->write(ui->comboBox->currentText().toStdString())){
        QMessageBox::warning(this, "Warning", "Could not edit history file.");
    }
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
    delete versionsDir;
    delete confDir;
    delete history;
    delete ui;
}
