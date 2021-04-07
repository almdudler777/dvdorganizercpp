#ifndef MOVIE_H
#define MOVIE_H

#include <QString>
#include <QList>
#include "db.h"
#include <QObject>
#include <QVariant>
#include "actor.h"
#include "category.h"
#include "type.h"
#include "roles.h"


class Movie
{
private:
    QString title;
    int length;
    int id;
    int mediums;
    int rated;
    double cost;
    QList<int> myActorIds;
    QList<int> myCategorieIds;
    Type myType;

public:
    Movie();
    Movie(
            int const id,
            QString const title,
            int const length,
            int const mediums,
            int const rated,
            double const cost
            );

    void setTitle(QString const title);
    void setLength(int const length);
    void setMediums(int const mediums);
    void setRated(int const rated);
    void setCost(double const cost);
    void setActors(QList<int> actorIds);
    void setCategories(QList<int> categoryIds);
    void setType(Type type);
    void save();

    QString getTitle() const;
    int getId() const;
    int getLength() const;
    int getMediums() const;
    int getRated() const;
    double getCost() const;
    Type getType() const;
    QList<Actor> getActors() const;
    QString getActorsAsCommaSeparatedString() const;
    QList<Category> getCategories() const;
    QString getCategoriesAsCommaSeparatedString() const;



    /* Global Factories */

    static QList<Movie> getAllMovies();
    static QList<Movie> getAllMovies(int start, int max = 100);
    static int getMovieCount();
    static Movie getMovieById(int id);
    static QList<Movie> getMoviesByType(Type const pType);
};

#endif // MOVIE_H
