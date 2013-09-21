/*!
*   \file   behaviorinfodao.cpp
*   \class  BehaviorInfoDAO
*/

#include "behaviorinfodao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
BehaviorInfoDAO::BehaviorInfoDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(BehaviorInfoDAO * obj)
/*!
*   \brief insert a new obj into the table behaviorinfo
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int BehaviorInfoDAO::insert(BehaviorInfoObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into behaviorinfo values ( NULL, %1, %2, %3, %4, %5, %6, '%7')")
                              .arg(obj->getIDTask()).arg(obj->getPort()).arg(obj->getIDPacket()).arg(obj->getTimeServer()).arg(obj->getTimeSec()).arg(obj->getTimeUSec()).arg(obj->getInfo()));
        int newId = -1;
        // Get database given autoincrement value
        if (ret)
        {
            newId = query.lastInsertId().toInt();
        }else{
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();
            qDebug() << db->lastError();
        }

    db->close();
    return newId;
}

//! remove(int id)
/*!
*   \brief remove a obj from the table behaviorinfo based in the ID
*   \param id a behaviorinfo`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorInfoDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from behaviorinfo where id = %1")
                          .arg(id));
    if(!ret) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(BehaviorInfoObject * obj)
/*!
*   \brief update a obj from the table behaviorinfo based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorInfoDAO::update(BehaviorInfoObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update behaviorinfo set idtask = %1, port = %2, idpacket = %3, timeserver = %4, timesec = %5, timeusec = %6, info = '%7' WHERE id = %8")
                              .arg(obj->getIDTask()).arg(obj->getPort()).arg(obj->getIDPacket()).arg(obj->getTimeServer()).arg(obj->getTimeSec()).arg(obj->getTimeUSec()).arg(obj->getInfo()).arg(obj->getID()));
        if (!ret)
        {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();
            qDebug() << db->lastError();
        }

    db->close();
    return ret;
}

//! get(int id)
/*!
*   \brief get a list of obj from the table behaviorinfo based in the ID.
*   \param id a behaviorinfo`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<BehaviorInfoObject *> BehaviorInfoDAO::get(int id) {
    QList<BehaviorInfoObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behaviorinfo where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorInfoObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toString()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();

    }
    db->close();
    return list;
}

//! get(QString column, QString value)
/*!
*   \brief get a list of obj from the table behaviorinfo based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<BehaviorInfoObject *> BehaviorInfoDAO::get(QString column, QString value) {
    QList<BehaviorInfoObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behaviorinfo where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorInfoObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toString()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();

    }
    db->close();
    return list;
}

//! getAll()
/*!
*   \brief get a list of obj from the table behaviorinfo based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<BehaviorInfoObject *> BehaviorInfoDAO::getAll() {
    QList<BehaviorInfoObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behaviorinfo")))
    {
        while(query.next())
        {
            list.push_back( new BehaviorInfoObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toString()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return list;
}
