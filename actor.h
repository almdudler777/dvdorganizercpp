#ifndef ACTOR_H
#define ACTOR_H

#include <QString>
#include <QList>
#include "db.h"
#include <QObject>
#include <QVariant>
#include "roles.h"

class Actor
{

private:
    QString prename;
    QString name;
    int id;

public:
    Actor(QString const name, QString const prename, int const id );
    /* SETTER */
    void setPreName(QString);
    void setName(QString);

    /* GETTER */
    QString getName() const;
    QString getPreName() const;
    int getId() const;
    int getMovieCount() const;

    /* Factory Functions */

    static QList<Actor> getAllActors();
    static QList<Actor> getActorsByMovieId(int const movieId);
    static int getActorCount();
    static Actor getActorById(int id);
};

#endif // ACTOR_H
