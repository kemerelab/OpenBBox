#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include <QObject>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <config/config.h>

class SQLDatabase
{

public:
    virtual bool open()= 0;
    virtual void close(){db.close();}
    virtual bool remove()= 0;
    virtual bool create()= 0;
    virtual bool isOpen(){return db.open();}
    virtual bool exists() = 0;
    virtual QSqlDatabase * getDatabase(){ return &db;}
    virtual QSqlError lastError(){return db.lastError();}
    virtual void setType(QString type){this->type = type;}
    virtual QString getType(){return type;}
protected:
    QSqlDatabase db;
    QString type;
};

Q_DECLARE_INTERFACE(SQLDatabase, "SQLDatabase/1.0")

#endif // SQLDATABASE_H
