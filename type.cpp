#include "type.h"
#include "movie.h"

Type::Type(){}

Type::Type(int const id, QString const name)
{
    this->id = id;
    this->setName(name);
}

void Type::setName(QString const name){
    this->name = name;
}

int Type::getId() const {
    return this->id;
}

QString Type::getName() const {
    return this->name;
}

QList<Type> Type::getAllTypes(){
    QSqlQuery *qry = DB::getInstance()->getQuery();

    QList<Type> ret;
    if(qry->exec("SELECT id, name FROM type")){
        while(qry->next()){
            ret.append(Type(qry->value(0).toInt(), qry->value(1).toString()));
        }
    }

    delete qry;
    return ret;
}

int Type::getMovieCount() const {
    QSqlQuery *qry = DB::getInstance()->getQuery();
    int ret = 0;

    if((qry->exec(QString("SELECT count(*) FROM movies WHERE type_id = %1")
                 .arg(this->getId())) && qry->next()))
            ret = qry->value(0).toInt();

    delete qry;
    return ret;
}

int Type::getTypeCount(){
    QSqlQuery *qry = DB::getInstance()->getQuery();
    int ret = 0;

    if(qry->exec("SELECT count(*) FROM type") && qry->next())
        ret = qry->value(0).toInt();

    delete qry;
    return ret;
}

QList<Movie> Type::getMovies(){
    return Movie::getMoviesByType(*this);
}

Type Type::getTypeById(int id){
    QSqlQuery *qry = DB::getInstance()->getQuery();

    if(qry->exec(QString("SELECT id, name FROM type WHERE id = %1").arg(id)) && qry->next()){
        Type ret(qry->value(0).toInt(), qry->value(1).toString());
        delete qry;
        return ret;
    }

    delete qry;
    return Type(0, "false");
}
