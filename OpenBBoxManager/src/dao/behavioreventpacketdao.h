/*!
*   \file   behavioreventpacketdao.h
*   \class  BehaviorEventPacketDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef behavioreventpacketdao_H
#define behavioreventpacketdao_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "behavioreventpacketobject.h"

class BehaviorEventPacketDAO : public QObject
{
    Q_OBJECT
public:
    BehaviorEventPacketDAO(SQLDatabase * db);           //!< Constructor
    int insert(BehaviorEventPacketObject * obj);           //!< insert new row
    bool update(BehaviorEventPacketObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<BehaviorEventPacketObject *> get(int id);        //!< get rows based in the id
    QList<BehaviorEventPacketObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<BehaviorEventPacketObject*>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // behavioreventpacketdao_H
