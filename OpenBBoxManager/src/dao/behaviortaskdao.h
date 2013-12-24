/*!
*   \file   behaviortaskdao.h
*   \class  BehaviorTaskDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef behaviortaskdao_H
#define behaviortaskdao_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "behaviortaskobject.h"

class BehaviorTaskDAO : public QObject
{
    Q_OBJECT
public:
    BehaviorTaskDAO(SQLDatabase * db);           //!< Constructor
    int insert(BehaviorTaskObject * obj);           //!< insert new row
    bool update(BehaviorTaskObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<BehaviorTaskObject *> get(int id);        //!< get rows based in the id
    QList<BehaviorTaskObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<BehaviorTaskObject*>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // SUBJECTDAO_H
