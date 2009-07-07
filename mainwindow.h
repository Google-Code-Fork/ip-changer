#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include <QDir>
#include "history.h"
#include "pList/pList.h"
#include "client.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;
    std::vector<pList::Proc> processes;
    History *history;

    void createActions();
    QAction *actionMinimize;
    QAction *actionRestore;
    QAction *actionExit;

    void loadVersions(QString);
    void readHistory(QString);

    QSystemTrayIcon *trayIcon;
    void createTrayIcon();

    void closeEvent(QCloseEvent*);

private slots:
    void updateList();
    void on_pushButton_clicked();
    void iconActivated(QSystemTrayIcon::ActivationReason);
};

#endif // MAINWINDOW_H
