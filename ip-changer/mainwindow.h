#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QPushButton>
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

    void loadDirs();
    void loadFiles();
    void loadVersions();
    void readHistory();
    void readLastVersion();
    void writeLastVersion();

    QFile *historyFile, *lastVersionFile;

    QDir *versionsDir, *confLocalDir, *confGlobalDir;

    QSystemTrayIcon *trayIcon;
    void createTrayIcon();

    void closeEvent(QCloseEvent*);

private slots:
    void updateProcList();
    void on_pushButton_clicked();
    void iconActivated(QSystemTrayIcon::ActivationReason);
};

#endif // MAINWINDOW_H
