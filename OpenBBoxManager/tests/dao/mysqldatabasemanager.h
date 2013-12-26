#ifndef MYSQLDATABASEMANAGER_H
#define MYSQLDATABASEMANAGER_H

#include <QObject>
#include "sqldatabase.h"
class MySQLDatabaseManager : public QObject, public SQLDatabase
{
    Q_OBJECT
    Q_INTERFACES(SQLDatabase)
public:
    explicit MySQLDatabaseManager(QObject *parent = 0);
    
public:
    bool open();
    bool create();
    bool remove();
    bool exists();
};

#endif // MYSQLDATABASEMANAGER_H
