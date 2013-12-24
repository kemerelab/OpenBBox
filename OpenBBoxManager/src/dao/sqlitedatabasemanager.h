#ifndef SQLITEDATABASEMANAGER_H
#define SQLITEDATABASEMANAGER_H

#include <QObject>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "config.h"
#include <QDebug>
#include <QStringList>
#include "sqldatabase.h"
class SQLiteDatabaseManager : public QObject, public SQLDatabase
{   
    Q_OBJECT

    Q_INTERFACES(SQLDatabase)

public:
    explicit SQLiteDatabaseManager(QObject *parent = 0);

public:
    bool open();
    bool create();
    bool exists();
    bool remove();
};

#endif // SQLITEDATABASEMANAGER_H
