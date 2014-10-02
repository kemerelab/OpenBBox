/*!
*   \file   openbboxnodedao.cpp
*   \class  OpenBBoxNodeDAO
*/

#include "openbboxnodedao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
OpenBBoxNodeDAO::OpenBBoxNodeDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(OpenBBoxNodeObject * obj)
/*!
*   \brief insert a new obj into the table openbboxnode
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int OpenBBoxNodeDAO::insert(OpenBBoxNodeObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into openbboxnode values ( NULL, %1, datetime('%2', 'unixepoch', 'localtime'), %3, '%4', '%5', '%6', '%7', %8)")
                              .arg(obj->getIDManager()).arg(obj->getTimeStart()).arg(obj->getTimeEnd()).arg(obj->getLabel()).arg(obj->getMAC()).arg(obj->getIP()).arg(obj->getPort()).arg(obj->getNumCameras()));
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
*   \brief remove a obj from the table openbboxnode based in the ID
*   \param id a obj`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool OpenBBoxNodeDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from openbboxnode where id = %1")
                          .arg(id));
    if(!ret) {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(OpenBBoxNodeObject * obj)
/*!
*   \brief update a obj from the table openbboxnode based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool OpenBBoxNodeDAO::update(OpenBBoxNodeObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update openbboxnode set idmanager = %1, timestart = %2, timeend = %3, label = '%4', mac = '%5', ip = '%6', port = '%7', numcameras = %8 where id = %9")
                              .arg(obj->getIDManager()).arg(obj->getTimeStart()).arg(obj->getTimeEnd()).arg(obj->getLabel()).arg(obj->getMAC()).arg(obj->getIP()).arg(obj->getPort()).arg(obj->getNumCameras()).arg(obj->getID()));
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
*   \brief get a list of obj from the table openbboxnode based in the ID.
*   \param id a obj`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<OpenBBoxNodeObject *> OpenBBoxNodeDAO::get(int id) {
    QList<OpenBBoxNodeObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from openbboxnode where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new OpenBBoxNodeObject(query.value(0).toInt(),
                                                   query.value(1).toInt(),
                                                   query.value(2).toLongLong(),
                                                   query.value(3).toLongLong(),
                                                   query.value(4).toString(),
                                                   query.value(5).toString(),
                                                   query.value(6).toString(),
                                                   query.value(7).toInt(),
                                                   query.value(8).toInt()));
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
*   \brief get a list of obj from the table openbboxnode based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<OpenBBoxNodeObject *> OpenBBoxNodeDAO::get(QString column, QString value) {
    QList<OpenBBoxNodeObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from openbboxnode where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new OpenBBoxNodeObject(query.value(0).toInt(),
                                                   query.value(1).toInt(),
                                                   query.value(2).toLongLong(),
                                                   query.value(3).toLongLong(),
                                                   query.value(4).toString(),
                                                   query.value(5).toString(),
                                                   query.value(6).toString(),
                                                   query.value(7).toInt(),
                                                   query.value(8).toInt()));
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
*   \brief get a list of obj from the table openbboxnode based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<OpenBBoxNodeObject *> OpenBBoxNodeDAO::getAll() {
    QList<OpenBBoxNodeObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from openbboxnode")))
    {
        while(query.next())
        {
            list.push_back( new OpenBBoxNodeObject(query.value(0).toInt(),
                                                   query.value(1).toInt(),
                                                   query.value(2).toLongLong(),
                                                   query.value(3).toLongLong(),
                                                   query.value(4).toString(),
                                                   query.value(5).toString(),
                                                   query.value(6).toString(),
                                                   query.value(7).toInt(),
                                                   query.value(8).toInt()));
        }
    } else {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return list;
}
