#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <QList>
#include "db.h"
#include <QObject>
#include <QVariant>
#include "roles.h"

class Category
{
private:
    int id;
    QString name;

public:
    Category(int const id, QString const name);

    void setName(QString const name);

    int getId() const;
    QString getName() const;
    int getMovieCount() const;

    static QList<Category> getCategoriesByMovieId(int movieId);
    static int getCategoryCount();
    static QList<Category> getAllCategories();
    static Category getCategoryById(int id);
};

#endif // CATEGORY_H
