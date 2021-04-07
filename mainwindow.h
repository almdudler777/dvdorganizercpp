#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QStringList>
#include <QSqlResult>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QList>
#include "roles.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_Movie_triggered();
    void on_actionSchlie_en_triggered();
    void on_actionAbout_Qt_triggered();
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_pageSelector_activated(int index);
    void on_pageBack_clicked();
    void on_pageForth_clicked();
    void treeheader_customMenuRequested(QPoint pos);
    void on_perPage_activated(int arg);

    void on_actionNeuer_Schauspieler_triggered();

private:
    Ui::MainWindow *ui;
    unsigned int currentPage;
    void handlePages();
    void assembleFilterView();
    void fillMovieList();
    int getPerPage();
};

#endif // MAINWINDOW_H
