#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "movie.h"
#include "type.h"
#include "movieWindow.h"
#include "actorwindow.h"
#include <cmath>
#include <QDebug>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set up the treewidget, and predefine the resize modes for each column
    ui->treeWidget->header()->setMinimumSectionSize(20);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->hideSection(1);
    ui->treeWidget->header()->hideSection(2);
    ui->treeWidget->header()->hideSection(3);
    ui->treeWidget->header()->setStretchLastSection(false);

    assembleFilterView(); //build the Filter Naviation on the left

    //initialize the page handling
    this->currentPage = 0;
    handlePages();

    ui->treeWidget->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget->header(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(treeheader_customMenuRequested(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::treeheader_customMenuRequested(QPoint pos){
    QMenu *menu=new QMenu(this);
    QAction *action;

    //Medien
    action = new QAction(this);
    action->setText("Medien");
    action->setCheckable(true);
    action->setIcon(QIcon("res/eye.png"));
    menu->addAction(action);

    //USK

    //Preis

    menu->popup(ui->treeWidget->header()->viewport()->mapToGlobal(pos));
}

void MainWindow::assembleFilterView(){
    QTreeWidgetItem *itemRoot, *itemChild, *itemChild2; //define a couple of pointers for our new items

    //Root Node
    itemRoot = new QTreeWidgetItem(ui->dbtreewidget);
    itemRoot->setText(0, DB::getInstance()->getDatabase().databaseName());
    itemRoot->setExpanded(true);
    ui->dbtreewidget->insertTopLevelItem(0,itemRoot);

    //Filter: All Movies
    itemChild = new QTreeWidgetItem(itemRoot);
    itemChild->setText(0,QString("Alle Filme (%1)").arg(Movie::getMovieCount()));
    itemRoot->addChild(itemChild);

    //Filter: by Category
    itemChild = new QTreeWidgetItem(itemRoot);
    itemChild->setText(0,QString("Nach Kategorie (%1)").arg(Category::getCategoryCount()));

    QList<Category> allCategories = Category::getAllCategories();
    for(int i = 0; i < allCategories.count(); i++){
        itemChild2 = new QTreeWidgetItem(itemChild);
        itemChild2->setText(0, QString("%1 (%2)")
                            .arg(allCategories.at(i).getName())
                            .arg(QString::number(allCategories.at(i).getMovieCount()))
                            );
        itemChild->addChild(itemChild2);
    }

    itemRoot->addChild(itemChild);

    //Filte: by Actor
    itemChild = new QTreeWidgetItem(itemRoot);
    itemChild->setText(0,QString("Nach Schauspielern (%1)").arg(Actor::getActorCount()));

    QList<Actor> allActors = Actor::getAllActors();
    for(int i = 0; i < allActors.count(); i++){
        itemChild2 = new QTreeWidgetItem(itemChild);
        itemChild2->setText(0, QString("%2, %1 (%3)")
                            .arg(allActors.at(i).getPreName())
                            .arg(allActors.at(i).getName())
                            .arg(allActors.at(i).getMovieCount())
                            );
        itemChild->addChild(itemChild2);
    }
    itemRoot->addChild(itemChild);

    //Filter: by Type
    itemChild = new QTreeWidgetItem(itemRoot);
    itemChild->setText(0, QString("Nach Typ (%1)").arg(Type::getTypeCount()));

    QList<Type> allTypes = Type::getAllTypes();
    for(int i = 0; i < allTypes.count(); i++){
        itemChild2 = new QTreeWidgetItem(itemChild);
        itemChild2->setText(0, QString("%1 (%2)")
                            .arg(allTypes.at(i).getName())
                            .arg(allTypes.at(i).getMovieCount())
                            );
        itemChild->addChild(itemChild2);
    }
    itemRoot->addChild(itemChild);
}

void MainWindow::on_actionNew_Movie_triggered()
{
    movieWindow w(this); //open a new movie window (keep movieId empty)
    w.exec();
}

void MainWindow::on_actionSchlie_en_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    column++; //i hate unused parameters...

    //open a new movie window and preload the selected movie for editing
    movieWindow w(this, item->data(0,UserRoles::MovieId).toInt());

    w.exec();
}

void MainWindow::on_pageSelector_activated(int index)
{
    this->currentPage = index;
    handlePages();
}

void MainWindow::on_pageBack_clicked()
{
    if (currentPage > 0)
        --currentPage;
    handlePages();
}

void MainWindow::on_pageForth_clicked()
{
    ++currentPage;
    handlePages();
}

void MainWindow::fillMovieList(){
    QList<Movie> allMovies;

    allMovies = Movie::getAllMovies(currentPage * getPerPage(), getPerPage());

    QTreeWidgetItem* item;
    ui->treeWidget->clear();
    for (int i = 0;  i < allMovies.count(); i++) {
        item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, allMovies.at(i).getTitle());
        item->setText(1, QString::number(allMovies.at(i).getLength()));
        item->setText(2, QString::number(allMovies.at(i).getMediums()));
        item->setText(3, QString::number(allMovies.at(i).getRated()));
        item->setText(4, allMovies.at(i).getCategoriesAsCommaSeparatedString());
        item->setText(5, allMovies.at(i).getActorsAsCommaSeparatedString());
        item->setText(6, QString("%1 €").arg(QString::number(allMovies.at(i).getCost(),'f', 2)));
        item->setData(0, UserRoles::MovieId, allMovies.at(i).getId());
        ui->treeWidget->addTopLevelItem(item);

        for(int z=0; z<item->columnCount() - 1;z++){
            item->setTextAlignment(z, ui->treeWidget->headerItem()->textAlignment(z));
        }

        item->setTextAlignment(item->columnCount() -1, Qt::AlignRight);
    }
    allMovies.clear();
}

void MainWindow::on_perPage_activated(int arg)
{
    handlePages();
    ++arg;
}

int MainWindow::getPerPage(){
    int ret;
    if(ui->perPage->currentIndex() > 0){
        ret = ui->perPage->currentText().toInt();
    }else{
        /* if Paging should be disabled we will return -1
         * Reason:
         *  SQLite will only allow 0+ (unsigned) values for
         *  SELECT ... LIMIT x,y
         *  Returning -1 will result in:
         *  SELECT ... LIMIT (currentPage * -1 =) -x, (getPerPage() => -1 =) -y
         *  SQLite will graciously ignore that Limit Filter in the Select Statement
         */
        ret = -1;
    }

    return ret;
}

void MainWindow::handlePages(){
    unsigned int numberOfPages = 0;
    if(getPerPage() != -1){
        numberOfPages = trunc(Movie::getMovieCount() / getPerPage());

        if(currentPage > numberOfPages)
            currentPage = numberOfPages;

        //disable back button if we're on the first page
        ui->pageBack->setEnabled((this->currentPage == 0) ? false : true);
        //disable forward button if we're on the last page
        ui->pageForth->setEnabled(((this->currentPage) == numberOfPages) ? false : true);
        ui->pageSelector->setEnabled(true);
    }else{
        ui->pageBack->setEnabled(false);
        ui->pageForth->setEnabled(false);
        ui->pageSelector->setEnabled(false);
    }

    ui->pageSelector->clear();
    for (unsigned int i = 1; i<=numberOfPages + 1; i++){
        ui->pageSelector->addItem(QString::number(i));
    }
    ui->pageSelector->setCurrentIndex(currentPage);

    fillMovieList();
}

void MainWindow::on_actionNeuer_Schauspieler_triggered()
{
    ActorWindow w(this);
    w.exec();
}
