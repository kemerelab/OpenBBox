/*!
*   \file   BehaviorTabledao.cpp
*   \class  BehaviorTableDAO
*/

#include "behaviortabledao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
BehaviorTableDAO::BehaviorTableDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(BehaviorTableDAO * obj)
/*!
*   \brief insert a new obj into the table BehaviorTable
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int BehaviorTableDAO::insert(BehaviorTableObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into BehaviorTable values ( NULL, %1, %2, %3, %4, '%5', %6, %7)")
                              .arg(obj->getIDTask()).arg(obj->getPort()).arg(obj->getIDPacket()).arg(obj->getTimeServer()).arg(obj->getEvent()).arg(obj->getMotortime()).arg(obj->getRewardtime()));
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
*   \brief remove a obj from the table BehaviorTable based in the ID
*   \param id a BehaviorTable`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorTableDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from BehaviorTable where id = %1")
                          .arg(id));
    if(!ret) {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(BehaviorTableObject * obj)
/*!
*   \brief update a obj from the table BehaviorTable based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorTableDAO::update(BehaviorTableObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update BehaviorTable set idtask = %1, port = %2, idpacket = %3, timeserver = %4, event = %5, motortime = %6,  rewardtime = %7")
                              .arg(obj->getIDTask()).arg(obj->getPort()).arg(obj->getIDPacket()).arg(obj->getTimeServer()).arg(obj->getEvent()).arg(obj->getMotortime()).arg(obj->getRewardtime()));
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
*   \brief get a list of obj from the table BehaviorTable based in the ID.
*   \param id a BehaviorTable`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<BehaviorTableObject *> BehaviorTableDAO::get(int id) {
    QList<BehaviorTableObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from BehaviorTable where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorTableObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toString(),
                                                        query.value(6).toFloat(),
                                                        query.value(7).toFloat()));
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
*   \brief get a list of obj from the table BehaviorTable based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<BehaviorTableObject *> BehaviorTableDAO::get(QString column, QString value) {
    QList<BehaviorTableObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from BehaviorTable where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorTableObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toString(),
                                                        query.value(6).toFloat(),
                                                        query.value(7).toFloat()));
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
*   \brief get a list of obj from the table BehaviorTable based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<BehaviorTableObject *> BehaviorTableDAO::getAll() {
    QList<BehaviorTableObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from BehaviorTable")))
    {
        while(query.next())
        {
            list.push_back( new BehaviorTableObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toString(),
                                                        query.value(6).toFloat(),
                                                        query.value(7).toFloat()));
        }
    } else {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return list;
}
