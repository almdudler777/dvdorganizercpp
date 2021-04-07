#include "actor.h"
#include <QMessageBox>

Actor::Actor(QString const name, QString const prename, int const id ){
    this->id = id;
    this->setName(name);
    this->setPreName(prename);
}

/* SETTER PROCEDURES */

void Actor::setName(QString name){
    this->name = name;
}

void Actor::setPreName(QString prename){
    this->prename = prename;
}

/* GETTER FUNCTIONS */

QString Actor::getName() const {
    return this->name;
}

QString Actor::getPreName() const {
    return this->prename;
}

int Actor::getId() const {
    return this->id;
}

/* Factory Functions */

QList<Actor> Actor::getAllActors(){
    QList<Actor> allActors;

    QSqlQuery* qry = DB::getInstance()->getQuery();

    if(qry->exec("SELECT name, prename, id FROM actors ORDER BY name ASC")){
        while(qry->next()){
            allActors.append(Actor(qry->value(0).toString(), qry->value(1).toString(), qry->value(2).toInt()));
        }
    }

    qry->clear();
    delete qry;
    return allActors;
}

QList<Actor> Actor::getActorsByMovieId(int const movieId){
    QList<Actor> actors;
    QSqlQuery* qry = DB::getInstance()->getQuery();

    qry->prepare("SELECT name, prename, id FROM actors WHERE id IN (SELECT actor_id FROM cast WHERE movie_id = ?)");
    qry->addBindValue(movieId);
    if(qry->exec()){
        while(qry->next()){
            actors.append(Actor(qry->value(0).toString(), qry->value(1).toString(), qry->value(2).toInt()));
        }
    }else{
        QMessageBox::warning(0,"Fehler in getActorsByMovieId", qry->lastError().text(), QMessageBox::Ok);
    }

    qry->clear();
    delete qry;
    return actors;
}

int Actor::getActorCount(){
    QSqlQuery* qry = DB::getInstance()->getQuery();
    int ret;

    if(qry->exec("SELECT count(*) FROM actors") && qry->next()){
        ret = qry->value(0).toInt();
    }else{
        ret = -1;
    }

    delete qry;
    return ret;
}

Actor Actor::getActorById(int id)
{
    QSqlQuery *qry=DB::getInstance()->getQuery();

    if(qry->exec(QString("SELECT id, name, prename FROM actors WHERE id = %1").arg(id))
            && qry->next()){
        Actor ret = Actor(qry->value(1).toString(),
                     qry->value(2).toString(),
                     qry->value(0).toInt());

        delete qry;
        return ret;
    }

    delete qry;
    return Actor("false", "false",0);
}

int Actor::getMovieCount() const {
    QSqlQuery *qry = DB::getInstance()->getQuery();
    int ret = 0;

    qry->prepare("SELECT count(*) FROM cast WHERE actor_id = ?");
    qry->addBindValue(this->getId());
    if(qry->exec() && qry->next())
        ret = qry->value(0).toInt();

    delete qry;
    return ret;
}
