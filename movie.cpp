#include "movie.h"
#include <QMessageBox>

Movie::Movie(){
    this->id = 0;
}

Movie::Movie(const int id, const QString title, const int length, const int mediums, const int rated, const double cost)
{
    this->id = id;
    this->setTitle(title);
    this->setLength(length);
    this->setMediums(mediums);
    this->setRated(rated);
    this->setCost(cost);
}

void Movie::setCost(const double cost){
    this->cost = cost;
}

void Movie::setActors(QList<int> actorIds){
    if(actorIds.count() > 0){
        this->myActorIds.clear();
        this->myActorIds.append(actorIds);
    }
}

void Movie::setCategories(QList<int> categoryIds){
    if(categoryIds.count() > 0){
        this->myCategorieIds.clear();
        this->myCategorieIds.append(categoryIds);
    }
}

void Movie::setLength(const int length){
    this->length = length;
}

void Movie::setMediums(const int mediums){
    this->mediums = mediums;
}

void Movie::setRated(const int rated){
    this->rated = rated;
}

void Movie::setTitle(const QString title){
    this->title = title;
}

QString Movie::getTitle() const {
    return this->title;
}

int Movie::getId() const {
    return this->id;
}

double Movie::getCost() const {
    return this->cost;
}

int Movie::getLength() const {
    return this->length;
}

int Movie::getMediums() const {
    return this->mediums;
}

int Movie::getRated() const {
    return this->rated;
}

QList<Movie> Movie::getAllMovies(){
    return Movie::getAllMovies(-1);
}

QList<Movie> Movie::getAllMovies(int start, int max){
    QList<Movie> allMovies;
    QSqlQuery* qry = DB::getInstance()->getQuery();

    if(start < 0){
        qry->prepare("SELECT id, titel, laenge, medien, usk, preis FROM movies ORDER BY titel COLLATE NOCASE ASC");
    }else{
        qry->prepare("SELECT id, titel, laenge, medien, usk, preis FROM movies ORDER BY titel COLLATE NOCASE ASC LIMIT ?,?");
        qry->addBindValue(start);
        qry->addBindValue(max);
    }

    if(qry->exec()){
        while(qry->next()){
            allMovies.append(Movie(qry->value(0).toInt(),
                                   qry->value(1).toString(),
                                   qry->value(2).toInt(),
                                   qry->value(3).toInt(),
                                   qry->value(4).toInt(),
                                   qry->value(5).toDouble()
                                   )
                             );
        }
    }else{
        QMessageBox::warning(0,"Fehler in getAllMovies", qry->lastError().text(), QMessageBox::Ok);
    }

    qry->clear();
    delete qry;
    return allMovies;
}

QList<Actor> Movie::getActors() const {
    return Actor::getActorsByMovieId(this->getId());
}

QString Movie::getActorsAsCommaSeparatedString() const {
    QList<Actor> tmpList = this->getActors();
    QString ret;

    for(int i = 0; i < tmpList.count(); i++){
        if(i + 1 <tmpList.count()){
            ret.append(QString("%1 %2, ")
                       .arg(tmpList.at(i).getPreName())
                       .arg(tmpList.at(i).getName()));
        }else{
            ret.append(QString("%1 %2")
                       .arg(tmpList.at(i).getPreName())
                       .arg(tmpList.at(i).getName()));
        }
    }
    return ret;
}

QList<Category> Movie::getCategories() const
{
    return Category::getCategoriesByMovieId(this->getId());
}

QString Movie::getCategoriesAsCommaSeparatedString() const
{
    QList<Category> tmpList = this->getCategories();
    QString ret;
    for(int i = 0; i < tmpList.count(); i++){
        if(i + 1 <tmpList.count()){
            ret.append(QString("%1, ")
                       .arg(tmpList.at(i).getName()));
        }else{
            ret.append(QString("%1")
                       .arg(tmpList.at(i).getName()));
        }
    }
    return ret;
}

int Movie::getMovieCount(){
    int ret;
    QSqlQuery* qry = DB::getInstance()->getQuery();
    if(qry->exec("SELECT count(*) FROM movies") && qry->next()){
        ret = qry->value(0).toInt();
    }else{
        ret = -1;
    }

    delete qry;
    return ret;
}

Movie Movie::getMovieById(int id){
    QSqlQuery* qry = DB::getInstance()->getQuery();

    qry->prepare("SELECT id, titel, laenge, medien, usk, preis FROM movies WHERE id = ?");
    qry->addBindValue(id);
    if(qry->exec() && qry->next()){
        Movie movie(qry->value(0).toInt(),
                    qry->value(1).toString(),
                    qry->value(2).toInt(),
                    qry->value(3).toInt(),
                    qry->value(4).toInt(),
                    qry->value(5).toDouble()
                    );
        delete qry;
        return movie;
    }

    delete qry;
    return Movie(0,"False",0,0,0,0);
}

QList<Movie> Movie::getMoviesByType(const Type pType){
    QSqlQuery *qry = DB::getInstance()->getQuery();
    QList<Movie> ret;

    qry->prepare("SELECT id, titel, laenge, medien, usk, preis FROM movies WHERE type_id = ? ORDER BY titel COLLATE NOCASE ASC");
    qry->addBindValue(pType.getId());
    if(qry->exec()){
        while(qry->next()){
            ret.append(Movie(qry->value(0).toInt(),
                             qry->value(1).toString(),
                             qry->value(2).toInt(),
                             qry->value(3).toInt(),
                             qry->value(4).toInt(),
                             qry->value(5).toDouble()
                             )
                       );
        }
    }

    delete qry;
    return ret;
}

void Movie::save(){
    QSqlQuery *qry = DB::getInstance()->getQuery();

    //if the movie id is 0 we create a new row later update it
    if(this->id == 0){
        if(!qry->exec("INSERT INTO movies (`titel`) VALUES ('New Item')")){
            QMessageBox::warning(0,"Could not create new movie.", qry->lastError().text(), QMessageBox::Ok);
        }else{
            this->id = qry->lastInsertId().toInt();
            qry->clear();
        }
    }

    qry->prepare("UPDATE movies SET titel = ?, laenge = ?, medien = ?, usk = ?, preis = ?, type_id = ? WHERE id = ?");

    qry->addBindValue(this->getTitle());
    qry->addBindValue(this->getLength());
    qry->addBindValue(this->getMediums());
    qry->addBindValue(this->getRated());
    qry->addBindValue(this->getCost());
    qry->addBindValue(this->myType.getId());
    qry->addBindValue(this->getId());

    if(!qry->exec()){
        QMessageBox::warning(0,"Error when updating while saving",qry->lastError().text(),QMessageBox::Ok);
    }

    if(this->myActorIds.count() > 0){
        qry->exec(QString("DELETE FROM cast WHERE movie_id = %1").arg(this->getId()));
        qry->clear();

        qry->prepare("INSERT INTO cast (movie_id, actor_id) VALUES(?,?)");
        foreach (int id, myActorIds) {
            qry->addBindValue(this->getId());
            qry->addBindValue(id);
            qry->exec();
        }
    }

    if(this->myCategorieIds.count() > 0){
        qry->exec(QString("DELETE FROM tags WHERE movie_id = %1").arg(this->getId()));
        qry->clear();

        qry->prepare("INSERT INTO tags (movie_id, category_id) VALUES(?,?)");
        foreach (int id, myCategorieIds) {
            qry->addBindValue(this->getId());
            qry->addBindValue(id);
            qry->exec();
        }
    }

    delete qry;
}

void Movie::setType(Type type){
    this->myType = type;
}

Type Movie::getType() const{
    return this->myType;
}
