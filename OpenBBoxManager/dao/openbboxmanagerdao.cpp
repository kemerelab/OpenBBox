/*!
*   \file   openbboxmanagerdao.cpp
*   \class  OpenBBoxManagerDAO
*/

#include "openbboxmanagerdao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
OpenBBoxManagerDAO::OpenBBoxManagerDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(OpenBBoxManagerObject * obj)
/*!
*   \brief insert a new obj into the table objs
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int OpenBBoxManagerDAO::insert(OpenBBoxManagerObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into openbboxmanager values ( NULL, '%1')")
                              .arg(obj->getTimeStart()));
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
*   \brief remove a obj from the table objs based in the ID
*   \param id a obj`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool OpenBBoxManagerDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from openbboxmanager where id = %1")
                          .arg(id));
    if(!ret) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(OpenBBoxManagerObject * obj)
/*!
*   \brief update a obj from the table objs based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool OpenBBoxManagerDAO::update(OpenBBoxManagerObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update openbboxmanager set tag = '%1', type = '%2', label = '%3', protocol = '%4', timeserver = %5, birthdate = %6, deathdate = %7, arrivaldate = %8 where id = %9")
                              .arg(obj->getTimeStart()).arg(obj->getID()));
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
*   \brief get a list of obj from the table objs based in the ID.
*   \param id a obj`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<OpenBBoxManagerObject *> OpenBBoxManagerDAO::get(int id) {
    QList<OpenBBoxManagerObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from openbboxmanager where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new OpenBBoxManagerObject(query.value(0).toInt(), query.value(1).toLongLong()));
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
*   \brief get a list of obj from the table objs based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<OpenBBoxManagerObject *> OpenBBoxManagerDAO::get(QString column, QString value) {
    QList<OpenBBoxManagerObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from openbboxmanager where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new OpenBBoxManagerObject(query.value(0).toInt(), query.value(1).toLongLong()));
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
*   \brief get a list of obj from the table objs based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<OpenBBoxManagerObject *> OpenBBoxManagerDAO::getAll() {
    QList<OpenBBoxManagerObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from openbboxmanager")))
    {
        while(query.next())
        {
            list.push_back( new OpenBBoxManagerObject(query.value(0).toInt(), query.value(1).toLongLong()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return list;
}
