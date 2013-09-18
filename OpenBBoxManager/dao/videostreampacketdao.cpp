/*!
*   \file   videostreampacketdao.cpp
*   \class  VideoStreamPacketDAO
*/

#include "videostreampacketdao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
VideoStreamPacketDAO::VideoStreamPacketDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(VideoStreamPacketDAO * obj)
/*!
*   \brief insert a new obj into the table videostreampacket
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int VideoStreamPacketDAO::insert(VideoStreamPacketObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into videostreampacket values ( NULL, %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, '%12',%13, %14)")
                              .arg(obj->getIDTask()).arg(obj->getPort()).arg(obj->getIDPacket()).arg(obj->getTimeServer()).arg(obj->getTimeSec()).arg(obj->getTimeUSec()).arg(obj->getSizeExpected()).arg(obj->getSize()).arg(obj->getWidth()).arg(obj->getHeight()).arg(obj->getFormat()).arg(obj->getFilename()).arg(obj->getHeaderFrame()).arg(obj->getChecksum()));
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
*   \brief remove a obj from the table videostreampacket based in the ID
*   \param id a videostreampacket`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool VideoStreamPacketDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from videostreampacket where id = %1")
                          .arg(id));
    if(!ret) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(VideoStreamPacketObject * obj)
/*!
*   \brief update a obj from the table videostreampacket based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool VideoStreamPacketDAO::update(VideoStreamPacketObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update videostreampacket set idtask = %1, port = %2, idpacket = %3, timeserver = %4, timesec = %5, timeusec = %6,  sizeexpected = %7,  size = %8,  width = %9,  height = %10,  format = %11, filename = '%12', format = %13, format = %14 where id = %15")
                              .arg(obj->getIDTask()).arg(obj->getPort()).arg(obj->getIDPacket()).arg(obj->getTimeServer()).arg(obj->getTimeSec()).arg(obj->getTimeUSec()).arg(obj->getSizeExpected()).arg(obj->getSize()).arg(obj->getWidth()).arg(obj->getHeight()).arg(obj->getFormat()).arg(obj->getFilename()).arg(obj->getHeaderFrame()).arg(obj->getChecksum()).arg(obj->getID()));
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
*   \brief get a list of obj from the table videostreampacket based in the ID.
*   \param id a videostreampacket`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<VideoStreamPacketObject *> VideoStreamPacketDAO::get(int id) {
    QList<VideoStreamPacketObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from videostreampacket where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new VideoStreamPacketObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toInt(),
                                                        query.value(8).toInt(),
                                                        query.value(9).toInt(),
                                                        query.value(10).toInt(),
                                                        query.value(11).toInt(),
                                                        query.value(12).toString(),
                                                        query.value(13).toInt(),
                                                        query.value(14).toInt()));
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
*   \brief get a list of obj from the table videostreampacket based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<VideoStreamPacketObject *> VideoStreamPacketDAO::get(QString column, QString value) {
    QList<VideoStreamPacketObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from videostreampacket where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new VideoStreamPacketObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toInt(),
                                                        query.value(8).toInt(),
                                                        query.value(9).toInt(),
                                                        query.value(10).toInt(),
                                                        query.value(11).toInt(),
                                                        query.value(12).toString(),
                                                        query.value(13).toInt(),
                                                        query.value(14).toInt()));
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
*   \brief get a list of obj from the table videostreampacket based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<VideoStreamPacketObject *> VideoStreamPacketDAO::getAll() {
    QList<VideoStreamPacketObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from videostreampacket")))
    {
        while(query.next())
        {
            list.push_back( new VideoStreamPacketObject(query.value(0).toInt(),
                                                        query.value(1).toInt(),
                                                        query.value(2).toInt(),
                                                        query.value(3).toInt(),
                                                        query.value(4).toLongLong(),
                                                        query.value(5).toLongLong(),
                                                        query.value(6).toLongLong(),
                                                        query.value(7).toInt(),
                                                        query.value(8).toInt(),
                                                        query.value(9).toInt(),
                                                        query.value(10).toInt(),
                                                        query.value(11).toInt(),
                                                        query.value(12).toString(),
                                                        query.value(13).toInt(),
                                                        query.value(14).toInt()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return list;
}
