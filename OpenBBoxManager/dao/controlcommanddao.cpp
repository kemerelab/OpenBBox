/*!
*   \file   controlcommanddao.cpp
*   \class  ControlCommandDAO
*/

#include "controlcommanddao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
ControlCommandDAO::ControlCommandDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(ControlCommandDAO * obj)
/*!
*   \brief insert a new obj into the table controlcommand
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int ControlCommandDAO::insert(ControlCommandObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into controlcommand values ( NULL, %1, %2, %3, %4, %5)")
                              .arg(obj->getIDConn()).arg(obj->getTimeSent()).arg(obj->getTimeAnswered()).arg(obj->getCommand()).arg(obj->getAnswer()));
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
*   \brief remove a obj from the table controlcommand based in the ID
*   \param id a controlcommand`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool ControlCommandDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from controlcommand where id = %1")
                          .arg(id));
    if(!ret) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(ControlCommandObject * obj)
/*!
*   \brief update a obj from the table controlcommand based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool ControlCommandDAO::update(ControlCommandObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update controlcommand set idconn = %1, timesent = %2, timeanswered = %3, command = %4, answer = %5 WHERE id = %6")
                              .arg(obj->getIDConn()).arg(obj->getTimeSent()).arg(obj->getTimeAnswered()).arg(obj->getCommand()).arg(obj->getAnswer()).arg(obj->getID()));
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
*   \brief get a list of obj from the table controlcommand based in the ID.
*   \param id a controlcommand`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<ControlCommandObject *> ControlCommandDAO::get(int id) {
    QList<ControlCommandObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from controlcommand where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new ControlCommandObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toLongLong(),
                                                        query.value(3).toLongLong(),
                                                        query.value(4).toInt(),
                                                        query.value(5).toInt()));
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
*   \brief get a list of obj from the table controlcommand based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<ControlCommandObject *> ControlCommandDAO::get(QString column, QString value) {
    QList<ControlCommandObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from controlcommand where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new ControlCommandObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toLongLong(),
                                                        query.value(3).toLongLong(),
                                                        query.value(4).toInt(),
                                                        query.value(5).toInt()));
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
*   \brief get a list of obj from the table controlcommand based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<ControlCommandObject *> ControlCommandDAO::getAll() {
    QList<ControlCommandObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from controlcommand")))
    {
        while(query.next())
        {
            list.push_back( new ControlCommandObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toLongLong(),
                                                        query.value(3).toLongLong(),
                                                        query.value(4).toInt(),
                                                        query.value(5).toInt()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return list;
}
