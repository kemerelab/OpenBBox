/*!
*   \file   taskfiledao.cpp
*   \class  TaskFileDAO
*/

#include "taskfiledao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
TaskFileDAO::TaskFileDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(TaskFileDAO * obj)
/*!
*   \brief insert a new obj into the table taskfile
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int TaskFileDAO::insert(TaskFileObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        query.prepare(QString("insert into taskfile values ( NULL, '%1', %2, '%3', '%4', ?, '%5')")
                      .arg(obj->getFilename()).arg(obj->getTimeServer()).arg(obj->getTaskname()).arg(obj->getType()).arg(obj->getHash()));
        query.addBindValue(obj->getFile());
        bool ret = query.exec();
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
*   \brief remove a obj from the table taskfile based in the ID
*   \param id a taskfile`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool TaskFileDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from taskfile where id = %1")
                          .arg(id));
    if(!ret) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(TaskFileObject * obj)
/*!
*   \brief update a obj from the table taskfile based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool TaskFileDAO::update(TaskFileObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        query.prepare(QString("update taskfile set filename = '%1', timeserver = %2, taskname = '%3',  type = '%4',  file = ?,  hash = '%5' WHERE id = %6")
                      .arg(obj->getFilename()).arg(obj->getTimeServer()).arg(obj->getTaskname()).arg(obj->getType()).arg(obj->getHash()).arg(obj->getID()));
        query.addBindValue(obj->getFile());
        bool ret = query.exec();
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
*   \brief get a list of obj from the table taskfile based in the ID.
*   \param id a taskfile`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<TaskFileObject *> TaskFileDAO::get(int id) {
    QList<TaskFileObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from taskfile where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new TaskFileObject(query.value(0).toInt(),
                                                query.value(1).toString(),
                                                query.value(2).toLongLong(),
                                                query.value(3).toString(),
                                                query.value(4).toString(),
                                                query.value(5).toByteArray(),
                                                query.value(6).toString()));
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
*   \brief get a list of obj from the table taskfile based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<TaskFileObject *> TaskFileDAO::get(QString column, QString value) {
    QList<TaskFileObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from taskfile where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new TaskFileObject(query.value(0).toInt(),
                                                query.value(1).toString(),
                                                query.value(2).toLongLong(),
                                                query.value(3).toString(),
                                                query.value(4).toString(),
                                                query.value(5).toByteArray(),
                                                query.value(6).toString()));
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
*   \brief get a list of obj from the table taskfile based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<TaskFileObject *> TaskFileDAO::getAll() {
    QList<TaskFileObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from taskfile")))
    {
        while(query.next())
        {
            list.push_back( new TaskFileObject(query.value(0).toInt(),
                                                query.value(1).toString(),
                                                query.value(2).toLongLong(),
                                                query.value(3).toString(),
                                                query.value(4).toString(),
                                                query.value(5).toByteArray(),
                                                query.value(6).toString()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return list;
}


//! fileExists(QFile * file)
/*!
*   \brief see if file exists in the database
*   \param file a pointer to the file to be seek
*   \return an bool with the result of the operation
*   \retval true if is in the database
*   \retval false if not found in the database
*/
bool TaskFileDAO::fileExists(QFile * file) {
    if(!db->isOpen())
        db->open();

    file->reset();
    QSqlQuery query;
    bool ret = query.exec(QString("select hash from taskfile where hash = '%1'")
                          .arg(qHash(file->readAll())));
    if(ret) {
        if(query.next())
            return true;
        else
            return false;
    }else{
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return ret;
}
