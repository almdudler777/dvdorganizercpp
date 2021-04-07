#include "category.h"
#include <QMessageBox>

Category::Category(const int id, const QString name)
{
    this->id = id;
    this->setName(name);
}

void Category::setName(const QString name)
{
    this->name = name;
}

int Category::getId() const {
    return this->id;
}

QString Category::getName() const {
    return this->name;
}

QList<Category> Category::getCategoriesByMovieId(int movieId)
{
    QList<Category> ret;
    QSqlQuery* qry = DB::getInstance()->getQuery();

    qry->prepare("SELECT id, name FROM category WHERE id IN (SELECT category_id FROM tags WHERE movie_id = ?)");
    qry->addBindValue(movieId);
    if(qry->exec()){
        while(qry->next()){
            ret.append(Category(qry->value(0).toInt(), qry->value(1).toString()));
        }
    }else{
        QMessageBox::warning(0,"Fehler in getCategoriesByMovieId", qry->lastError().text(), QMessageBox::Ok);
    }

    qry->clear();
    delete qry;
    return ret;
}

int Category::getCategoryCount(){
    QSqlQuery* qry = DB::getInstance()->getQuery();
    int ret;

    if(qry->exec("SELECT count(*) FROM category") && qry->next()){
        ret = qry->value(0).toInt();
    }else{
        ret = -1;
    }

    delete qry;
    return ret;
}

QList<Category> Category::getAllCategories(){
    QSqlQuery *qry = DB::getInstance()->getQuery();
    QList<Category> ret;

    if(qry->exec("SELECT id, name FROM category ORDER BY name ASC")){
        while(qry->next()){
            ret.append(Category(qry->value(0).toInt(), qry->value(1).toString()));
        }
    }else{
        QMessageBox::warning(0,"Fehler in getAllCategories", qry->lastError().text(), QMessageBox::Ok);
    }

    delete qry;
    return ret;
}

Category Category::getCategoryById(int id)
{
    QSqlQuery *qry = DB::getInstance()->getQuery();

    if(qry->exec(QString("SELECT id, name FROM category WHERE id = %1").arg(id))
                 && qry->next()){
        Category cat = Category(qry->value(0).toInt(), qry->value(1).toString());
        delete qry;
        return cat;
    }
    delete qry;
    return Category(0,"false");
}

int Category::getMovieCount() const{
    QSqlQuery *qry = DB::getInstance()->getQuery();
    int count = 0;

    qry->prepare("SELECT count(*) FROM tags WHERE category_id = ?");
    qry->addBindValue(this->getId());
    if(qry->exec() && qry->next())
        count = qry->value(0).toInt();

    delete qry;
    return count;
}
