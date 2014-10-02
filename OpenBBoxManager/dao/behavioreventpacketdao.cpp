/*!
*   \file   behavioreventpacketdao.cpp
*   \class  BehaviorEventPacketDAO
*/

#include "behavioreventpacketdao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
BehaviorEventPacketDAO::BehaviorEventPacketDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(BehaviorEventPacketDAO * obj)
/*!
*   \brief insert a new obj into the table behavioreventpacket
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int BehaviorEventPacketDAO::insert(BehaviorEventPacketObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into behavioreventpacket values ( NULL, %1, %2, %3, datetime('%4', 'unixepoch', 'localtime'), %5, %6, %7, %8, %9, '%10')")
                              .arg(obj->getIDTask())
                              .arg(obj->getPort())
                              .arg(obj->getIDPacket())
                              .arg(obj->getTimeServer())
                              .arg(obj->getTimeSec())
                              .arg(obj->getTimeUSec())
                              .arg(obj->getPin())
                              .arg(obj->getTypeEvent())
                              .arg(obj->getTypePin())
                              .arg(obj->getPinContext()));
        int newId = -1;
        // Get database given autoincrement value
        if (ret)
        {
            newId = query.lastInsertId().toInt();
        }else{
            qCritical() << query.lastQuery();
            qCritical() << query.lastError();
            qCritical() << db->lastError();
        }

    db->close();
    return newId;
}

//! remove(int id)
/*!
*   \brief remove a obj from the table behavioreventpacket based in the ID
*   \param id a behavioreventpacket`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorEventPacketDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from behavioreventpacket where id = %1")
                          .arg(id));
    if(!ret) {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(BehaviorEventPacketObject * obj)
/*!
*   \brief update a obj from the table behavioreventpacket based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorEventPacketDAO::update(BehaviorEventPacketObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update behavioreventpacket set idtask = %1, port = %2, idpacket = %3, timeserver = %4, timesec = %5, timeusec = %6,  pinscontext = %7,  pinevent = %8,  pineventlabel = '%9' WHERE id = %10")
                              .arg(obj->getIDTask())
                              .arg(obj->getPort())
                              .arg(obj->getIDPacket())
                              .arg(obj->getTimeServer())
                              .arg(obj->getTimeSec())
                              .arg(obj->getTimeUSec())
                              .arg(obj->getPin())
                              .arg(obj->getTypeEvent())
                              .arg(obj->getTypePin())
                              .arg(obj->getPinContext())
                              .arg(obj->getID()));
        if (!ret)
        {
            qCritical() << query.lastQuery();
            qCritical() << query.lastError();
            qCritical() << db->lastError();
        }

    db->close();
    return ret;
}

//! get(int id)
/*!
*   \brief get a list of obj from the table behavioreventpacket based in the ID.
*   \param id a behavioreventpacket`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<BehaviorEventPacketObject *> BehaviorEventPacketDAO::get(int id) {
    QList<BehaviorEventPacketObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behavioreventpacket where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorEventPacketObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toInt(),
                                                        query.value(8).toInt(),
                                                        query.value(9).toInt(),
                                                        query.value(10).toString()));
        }
    } else {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();

    }
    db->close();
    return list;
}

//! get(QString column, QString value)
/*!
*   \brief get a list of obj from the table behavioreventpacket based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<BehaviorEventPacketObject *> BehaviorEventPacketDAO::get(QString column, QString value) {
    QList<BehaviorEventPacketObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behavioreventpacket where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorEventPacketObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toInt(),
                                                        query.value(8).toInt(),
                                                        query.value(9).toInt(),
                                                        query.value(10).toString()));
        }
    } else {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();

    }
    db->close();
    return list;
}

//! getAll()
/*!
*   \brief get a list of obj from the table behavioreventpacket based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<BehaviorEventPacketObject *> BehaviorEventPacketDAO::getAll() {
    QList<BehaviorEventPacketObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behavioreventpacket")))
    {
        while(query.next())
        {
            list.push_back( new BehaviorEventPacketObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toInt(),
                                                        query.value(8).toInt(),
                                                        query.value(9).toString()));
        }
    } else {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return list;
}
