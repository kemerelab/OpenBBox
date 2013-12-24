/*!
*   \file   taskfiledao.h
*   \class  TaskFileDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef taskfiledao_H
#define taskfiledao_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "taskfileobject.h"

class TaskFileDAO : public QObject
{
    Q_OBJECT
public:
    TaskFileDAO(SQLDatabase * db);           //!< Constructor
    int insert(TaskFileObject * obj);           //!< insert new row
    bool update(TaskFileObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<TaskFileObject *> get(int id);        //!< get rows based in the id
    QList<TaskFileObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<TaskFileObject*>  getAll();        //!< get all rows from table
    bool fileExists(QFile * file);

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // taskfiledao_H
