#ifndef TYPE_H
#define TYPE_H

#include <QList>
#include <QString>
#include "db.h"
#include "roles.h"

class Movie;
class Type
{
private:
    QString name;
    int id;

public:
    Type();
    Type(int const id, QString const name);

    void setName(QString const name);

    int getId() const;
    QString getName() const;

    // GLOBAL FACTORIES
    static QList<Type> getAllTypes();
    static int getTypeCount();
    static Type getTypeById(int id);

    // INSTANCE BASED FACTORIES
    QList<Movie> getMovies();
    int getMovieCount() const;
};

#endif // TYPE_H
