#include "selectionwindow.h"
#include "ui_selectionwindow.h"
#include <QPushButton>
#include <QList>
#include <QTreeWidgetItem>
#include "category.h"
#include "actor.h"
#include "roles.h"

selectionWindow::selectionWindow(const int modus, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectionWindow)
{
    ui->setupUi(this);

    //set icons for the buttonbox, couldnt find that in the designer?!
    ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon("res/add.png"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setIconSize(QSize(16,16));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon("res/cancel.png"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setIconSize(QSize(16,16));

    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);

    QTreeWidgetItem *item;

    switch(modus){
        case WindowSelectionRoles::Actor: {//schauspieler suchen
            ui->treeWidget->headerItem()->setText(0, "Schauspieler");
            this->setWindowTitle("Schauspieler hinzufügen");

            QList<Actor> allActors =  Actor::getAllActors();
            for(int i=0; i < allActors.count(); i++){
                item = new QTreeWidgetItem(ui->treeWidget);
                item->setText(0,QString("%2, %1")
                              .arg(allActors.at(i).getPreName())
                              .arg(allActors.at(i).getName())
                              );
                item->setData(0, UserRoles::ActorId, allActors.at(i).getId());
                ui->treeWidget->addTopLevelItem(item);
            }
            break;
        }
        case WindowSelectionRoles::Category: { //kategorie suchen
            ui->treeWidget->headerItem()->setText(0, "Kategorie");
            this->setWindowTitle("Kategorie hinzufügen");

            QList<Category> allCategories = Category::getAllCategories();
            for(int i =0; i< allCategories.count(); i++){
                item = new QTreeWidgetItem(ui->treeWidget);
                item->setText(0,allCategories.at(i).getName());
                item->setData(0, UserRoles::CategoryId, allCategories.at(i).getId());
                ui->treeWidget->addTopLevelItem(item);
            }
            break;
        }
        case WindowSelectionRoles::Movie: {
            break;
        }
    }
}

selectionWindow::~selectionWindow()
{
    delete ui;
}

QList<int> selectionWindow::getSelectedIds(int UserRole)
{
    QList<int> ret;

    for(int i = 0; i < ui->treeWidget->selectedItems().count(); i++){
        if(!ui->treeWidget->selectedItems().at(i)->data(0,UserRole).isNull())
            ret.append(ui->treeWidget->selectedItems().at(i)->data(0, UserRole).toInt());
    }

    return ret;
}
