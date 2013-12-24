/*!
*   \file   behaviorinfodao.h
*   \class  BehaviorInfoDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef behaviorinfodao_H
#define behaviorinfodao_H_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "behaviorinfoobject.h"

class BehaviorInfoDAO : public QObject
{
    Q_OBJECT
public:
    BehaviorInfoDAO(SQLDatabase * db);           //!< Constructor
    int insert(BehaviorInfoObject * obj);           //!< insert new row
    bool update(BehaviorInfoObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<BehaviorInfoObject *> get(int id);        //!< get rows based in the id
    QList<BehaviorInfoObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<BehaviorInfoObject *>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // behaviorinfodao_H
