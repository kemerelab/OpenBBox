#include "mysqldatabasemanager.h"

MySQLDatabaseManager::MySQLDatabaseManager(QObject *parent) :
    QObject(parent)
{
    setType("QMYSQL");
}

bool MySQLDatabaseManager::open()
{
    if(db.isOpen())
        return true;
    // Find QSLite driver
    db = QSqlDatabase::addDatabase(getType());

    #ifdef Q_OS_LINUX
        // NOTE: We have to store database file into user home folder in Linuxs
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("root");
        db.setDatabaseName("openbboxdb");
    #else
        // NOTE: File exists in the application private folder, in Symbian Qt implementation
        db.setDatabaseName(DATABASE_SQLITE_PATH);
    #endif

    // Open databasee
    return db.open();
}

bool MySQLDatabaseManager::create(){

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

        // NOTE: We have to store database file into user home folder in Linuxs
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("root");
        db.setDatabaseName("mysql");

     if(db.open()){
        QSqlQuery query;
        query.exec("DROP DATABASE openbboxdb");
        query.exec("CREATE DATABASE openbboxdb");
    }

    if(open())
    {
        // Create table "person"
        QFile filesql(MYSQLDB_RESOUCE_FILE);
        if(!filesql.open(QIODevice::ReadOnly))
        {
             qDebug() << "Error opening file";
             return false;
        }

        QTextStream sql(&filesql);
        QString string = sql.readAll();

        QStringList listsql = string.split(";");
        listsql.removeDuplicates();
        listsql.removeAll("");
        qDebug() << listsql.size();
        for(int i = 0; i< listsql.size(); i++){
            QSqlQuery query;
            if(listsql.at(i).contains("CREATE")){
                qDebug() << listsql.at(i);
                if(!query.exec(listsql.at(i)))
                    return false;
            }
        }
    }
    return true;
}

bool MySQLDatabaseManager::remove(){
    return true;
}

bool MySQLDatabaseManager::exists(){
    return open();
}
