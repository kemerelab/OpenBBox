#include "sqlitedatabasemanager.h"

SQLiteDatabaseManager::SQLiteDatabaseManager(QObject *parent) :
    QObject(parent)
{
    setType("QSQLITE");
}

bool SQLiteDatabaseManager::open()
{
    if(db.isOpen())
        return true;
    // Find QSLite driver
    db = QSqlDatabase::addDatabase(getType());

    #ifdef Q_OS_LINUX
        // NOTE: We have to store database file into user home folder in Linux
        db.setDatabaseName(DATABASE_SQLITE_PATH);
    #else
        // NOTE: File exists in the application private folder, in Symbian Qt implementation
        db.setDatabaseName(DATABASE_SQLITE_PATH);
    #endif

    // Open databasee
    return db.open();
}

bool SQLiteDatabaseManager::create(){

    remove();
    // Create table "person"
    QFile file(DATABASE_SQLITE_PATH);
    file.open(QIODevice::WriteOnly);
    file.close();

    if(open())
    {
        // Create table "person"
        QFile filesql(SQLITEDB_RESOUCE_FILE);
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

bool SQLiteDatabaseManager::remove(){
    if(QFile::remove(DATABASE_SQLITE_PATH))
        return true;
    return false;
}

bool SQLiteDatabaseManager::exists() {
    return QFile::exists(DATABASE_SQLITE_PATH);
}
