/*!
*   \file   behaviortaskdao.cpp
*   \class  BehaviorTaskDAO
*/

#include "behaviortaskdao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
BehaviorTaskDAO::BehaviorTaskDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(BehaviorTaskDAO * obj)
/*!
*   \brief insert a new obj into the table behaviortask
*   \param obj a pointer to the object with of the new obj
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int BehaviorTaskDAO::insert(BehaviorTaskObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into behaviortask values ( NULL, %1, %2, %3, %4, %5, '%6')")
                              .arg(obj->getIDConn())
                              .arg(obj->getIDSubject())
                              .arg(obj->getIDTask())
                              .arg(obj->getTimeStart())
                              .arg(0)
                              .arg(obj->getLabel()));
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
*   \brief remove a obj from the table behaviortask based in the ID
*   \param id a obj`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorTaskDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from behaviortask where id = %1")
                          .arg(id));
    if(!ret) {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(BehaviorTaskObject * obj)
/*!
*   \brief update a obj from the table behaviortask based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param obj a pointer to the object with of the obj that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool BehaviorTaskDAO::update(BehaviorTaskObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        QString command = QString("update behaviortask set idconn = %1, idsubject = %2, idtaskfile = %3, timestart = %4, timeend = %5, label = '%6' where id = %7")
                .arg(obj->getIDConn())
                .arg(obj->getIDSubject())
                .arg(obj->getIDTask())
                .arg(obj->getTimeStart())
                .arg(obj->getTimeEnd())
                .arg(obj->getLabel())
                .arg(obj->getID());
        bool ret = query.exec(command);

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
*   \brief get a list of obj from the table behaviortask based in the ID.
*   \param id a obj`s id integer
*   \return list of all objs with that match the id
*   \retval empty if fail or not found
*/
QList<BehaviorTaskObject *> BehaviorTaskDAO::get(int id) {
    QList<BehaviorTaskObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select idconn, idsubject, idtaskfile, timestart, timeend, label from behaviortask where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorTaskObject(query.value(0).toInt(),
                                                   query.value(1).toInt(),
                                                   query.value(2).toInt(),
                                                   query.value(3).toLongLong(),
                                                   query.value(4).toLongLong(),
                                                   query.value(5).toString()));
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
*   \brief get a list of obj from the table behaviortask based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<BehaviorTaskObject *> BehaviorTaskDAO::get(QString column, QString value) {
    QList<BehaviorTaskObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behaviortask where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new BehaviorTaskObject(query.value(0).toInt(),
                                                   query.value(1).toInt(),
                                                   query.value(2).toInt(),
                                                   query.value(3).toInt(),
                                                   query.value(4).toLongLong(),
                                                   query.value(5).toLongLong(),
                                                   query.value(6).toString()));
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
*   \brief get a list of obj from the table behaviortask based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all objs
*   \retval empty if fail
*/
QList<BehaviorTaskObject *> BehaviorTaskDAO::getAll() {
    QList<BehaviorTaskObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from behaviortask")))
    {
        while(query.next())
        {
            list.push_back( new BehaviorTaskObject(query.value(0).toInt(),
                                                   query.value(1).toInt(),
                                                   query.value(2).toInt(),
                                                   query.value(3).toInt(),
                                                   query.value(4).toLongLong(),
                                                   query.value(5).toLongLong(),
                                                   query.value(6).toString()));
        }
    } else {
        qCritical() << query.lastQuery();
        qCritical() << query.lastError();
        qCritical() << db->lastError();
    }
    db->close();
    return list;
}
