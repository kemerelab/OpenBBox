/*!
*   \file   BehaviorTabledao.h
*   \class  BehaviorTableDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef behaviortabledao_H
#define behaviortabledao_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "behaviortableobject.h"

class BehaviorTableDAO : public QObject
{
    Q_OBJECT
public:
    BehaviorTableDAO(SQLDatabase * db);           //!< Constructor
    int insert(BehaviorTableObject * obj);           //!< insert new row
    bool update(BehaviorTableObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<BehaviorTableObject *> get(int id);        //!< get rows based in the id
    QList<BehaviorTableObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<BehaviorTableObject*>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // BehaviorTabledao_H
