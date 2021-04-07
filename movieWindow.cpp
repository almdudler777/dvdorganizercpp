#include "movieWindow.h"
#include "ui_movieWindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QDate>
#include "movie.h"
#include "actor.h"
#include "category.h"
#include "selectionwindow.h"

movieWindow::movieWindow(QWidget *parent, int movieId) :
    QDialog(parent),
    ui(new Ui::movieWindow)
{
    ui->setupUi(this);

    //set icons for the buttonbox, couldnt find that in the designer?!
    ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon("res/film_save.png"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setIconSize(QSize(16,16));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon("res/cancel.png"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setIconSize(QSize(16,16));

    // just to make sure remove buttons are disabled first from the startup,
    // since there is supposedly nothing selected yet
    this->on_kategorieTree_itemSelectionChanged();
    this->on_schauspielerTree_itemSelectionChanged();

    this->movieId = movieId;

    if(movieId > 0){ //window will be used to edit a movie that already exists in the database
        this->setWindowTitle("Film bearbeiten");
        Movie mov = Movie::getMovieById(movieId);
        ui->titel->setText(mov.getTitle());
        ui->laenge->setValue(mov.getLength());
        ui->preis->setValue(mov.getCost());
        ui->medien->setValue(mov.getMediums());
        ui->usk->setCurrentIndex(ui->usk->findText(QString::number(mov.getRated()),Qt::MatchStartsWith));

        QTreeWidgetItem *item;

        QList<Actor> movieActors = mov.getActors();
        for(int i = 0; i < movieActors.count(); i++){
            item = new QTreeWidgetItem(ui->schauspielerTree);
            item->setText(0, QString("%1 %2")
                          .arg(movieActors.at(i).getPreName())
                          .arg(movieActors.at(i).getName())
                          );
            item->setData(0, UserRoles::ActorId, movieActors.at(i).getId());
            ui->schauspielerTree->addTopLevelItem(item);
        }

        QList<Category> movieCategories = mov.getCategories();
        for(int i = 0; i<movieCategories.count(); i++){
            item = new QTreeWidgetItem(ui->kategorieTree);
            item->setText(0,movieCategories.at(i).getName());
            item->setData(0, UserRoles::CategoryId, movieCategories.at(i).getId());
            ui->kategorieTree->addTopLevelItem(item);
        }
    }else{
        //set date fields to a date nearby
        this->setWindowTitle("Neuen Film hinzufügen");
        ui->gekauft->setDate(QDate::currentDate());
        ui->release->setDate(QDate::currentDate());

        //fill typ combobox
        QList<Type> allTypes = Type::getAllTypes();
        for(int i = 0; i < allTypes.count(); i++){
            ui->typ->addItem(allTypes.at(i).getName(), QVariant(allTypes.at(i).getId()));
        }
    }
}

movieWindow::~movieWindow()
{
    delete ui;
}

void movieWindow::on_schauspielerTree_itemSelectionChanged()
{
    if(ui->schauspielerTree->selectedItems().count() >= 1){
        ui->SchauspielerRemove->setEnabled(true);
    }else{
        ui->SchauspielerRemove->setEnabled(false);
    }
}

void movieWindow::on_kategorieTree_itemSelectionChanged()
{
    if(ui->kategorieTree->selectedItems().count() >= 1){
        ui->kategorieRemove->setEnabled(true);
    }else{
        ui->kategorieRemove->setEnabled(false);
    }
}

void movieWindow::on_kategorieRemove_clicked()
{
    for(int i = 0; i < ui->kategorieTree->selectedItems().count(); i++){
        delete ui->kategorieTree->selectedItems().at(i);
        --i; //dont forget to decremnt i here, since we deleted a treewidgetitem that is referenced by the forcycle criteria
    }
}

void movieWindow::on_SchauspielerRemove_clicked()
{
    for(int i = 0; i< ui->schauspielerTree->selectedItems().count(); i++){
        delete ui->schauspielerTree->selectedItems().at(i);
        --i; //dont forget to decremnt i here, since we deleted a treewidgetitem that is referenced by the forcycle criteria
    }
}

void movieWindow::on_schauspielerAdd_clicked()
{
    bool found = false;
    QTreeWidgetItem *item;

    selectionWindow w(WindowSelectionRoles::Actor, this);

    if(w.exec() == QDialog::Accepted){
        foreach (int i, w.getSelectedIds(UserRoles::ActorId)) {

            found = false;

            for(int z = 0; z < ui->schauspielerTree->topLevelItemCount(); z++){
                if(ui->schauspielerTree->topLevelItem(z)->data(0, UserRoles::ActorId) == i){
                    found = true;
                    break;
                }
            }

            if(!found){
                item = new QTreeWidgetItem(ui->schauspielerTree);
                Actor act = Actor::getActorById(i);
                item->setText(0, QString("%1 %2")
                              .arg(act.getPreName())
                              .arg(act.getName())
                              );
                item->setData(0, UserRoles::ActorId, act.getId());
                ui->schauspielerTree->addTopLevelItem(item);
            }
        }
    }
}

void movieWindow::on_kategorieAdd_clicked()
{
    bool found = false;
    QTreeWidgetItem *item;

    selectionWindow w(WindowSelectionRoles::Category, this);
    if(w.exec() == QDialog::Accepted){
        foreach (int i , w.getSelectedIds(UserRoles::CategoryId)){
            found = false;

            for(int z = 0; z < ui->kategorieTree->topLevelItemCount(); z++){
                if(ui->kategorieTree->topLevelItem(z)->data(0, UserRoles::CategoryId) == i){
                    found = true;
                    break;
                }
            }

            if(!found){
                item = new QTreeWidgetItem(ui->kategorieTree);
                Category cat = Category::getCategoryById(i);
                item->setText(0, cat.getName());
                item->setData(0, UserRoles::CategoryId, cat.getId());
                ui->kategorieTree->addTopLevelItem(item);
            }
        }
    }
}

void movieWindow::on_movieWindow_accepted()
{
    Movie mov;

    if(this->movieId != 0){
        mov  = Movie::getMovieById(this->movieId);
    }

    mov.setCost(ui->preis->value());
    mov.setLength(ui->laenge->value());
    mov.setMediums(ui->medien->value());
    mov.setRated(ui->usk->currentText().toInt());
    mov.setTitle(ui->titel->text());
    mov.setType(Type::getTypeById(ui->typ->currentData().toInt()));

    QList<int> actors;
    for(int i = 0; i< ui->schauspielerTree->topLevelItemCount(); i++){
        actors.append(ui->schauspielerTree->topLevelItem(i)->data(0, UserRoles::ActorId).toInt());
    }

    mov.setActors(actors);

    QList<int> categories;
    for (int i=0; i< ui->kategorieTree->topLevelItemCount(); i++){
        categories.append(ui->kategorieTree->topLevelItem(i)->data(0,UserRoles::CategoryId).toInt());
    }

    mov.setCategories(categories);
    mov.save();
}

void movieWindow::done(int r){
    if(QDialog::Accepted == r)
    {
        if(ui->titel->text() == ""){
            QMessageBox::warning(0, "Fehler", "Bitte einen Titel eingeben!", QMessageBox::Ok);
            return;
        }
    }
    QDialog::done(r);
    return;
}
