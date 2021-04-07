#ifndef DB_H
#define DB_H

#include <QMutex>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include "roles.h"

// According to: http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern

class DB
{
public:
    static DB* getInstance()
    {
        static QMutex mutex;
        if (!m_Instance)
        {
            mutex.lock();

            if (!m_Instance)
                m_Instance = new DB;

            mutex.unlock();
        }
        return m_Instance;
    }

    QSqlQuery* getQuery(){
        return new QSqlQuery(this->db);
    }

    QSqlError getLastError(){
        return this->db.lastError();
    }

    QSqlDatabase getDatabase() {
        return this->db;
    }

    ~DB();

private:
    DB() {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setDatabaseName("data.sqlite");

        if(!this->db.open()){
            qDebug("No Connection to Database");
            exit(0);
        }
    }

    DB(const DB &);
    DB& operator=(const DB &);
    static DB* m_Instance;
    QSqlDatabase db;
};

#endif // DB_H



