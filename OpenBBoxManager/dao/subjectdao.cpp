/*!
*   \file   subjectdao.cpp
*   \class  SubjectDAO
*/

#include "subjectdao.h"

//! Constructor
/*!
*   \brief Constructor that receive an instance of the database that is be used
*   \param db a pointer for the database
*/
SubjectDAO::SubjectDAO(SQLDatabase * db)
{
    this->db = db;
}

//! insert(SubjectObject * obj)
/*!
*   \brief insert a new subject into the table subjects
*   \param subject a pointer to the object with of the new subject
*   \return an integer id of new row
*   \retval -1 if fail.
*/
int SubjectDAO::insert(SubjectObject * obj) {
    if(!db->isOpen())
        db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("insert into subject values ( NULL, '%1', '%2', '%3', '%4', %5, %6, %7, %8)")
                              .arg(obj->getTag()).arg(obj->getType()).arg(obj->getLabel()).arg(obj->getProtocol()).arg(obj->getTimeServer()).arg(obj->getBirthDate()).arg(obj->getDeathDate()).arg(obj->getArrivalDate()));
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
*   \brief remove a subject from the table subjects based in the ID
*   \param id a Subject`s id integer
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool SubjectDAO::remove(int id) {
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    bool ret = query.exec(QString("delete from subject where id = %1")
                          .arg(id));
    if(!ret) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return ret;
}

//! update(SubjectObject * obj)
/*!
*   \brief update a subject from the table subjects based in the ID.
*   \note  Object must contains the ID that will be used to update the row.
*   \param subject a pointer to the object with of the subject that contians the new information
*   \return an bool with the result of the operation
*   \retval true if sucess
*   \retval false if fail
*/
bool SubjectDAO::update(SubjectObject * obj) {
        if(!db->isOpen())
            db->open();

        QSqlQuery query;
        bool ret = query.exec(QString("update subject set tag = '%1', type = '%2', label = '%3', protocol = '%4', timeserver = %5, birthdate = %6, deathdate = %7, arrivaldate = %8 where id = %9")
                              .arg(obj->getTag()).arg(obj->getType()).arg(obj->getLabel()).arg(obj->getProtocol()).arg(obj->getTimeServer()).arg(obj->getBirthDate()).arg(obj->getDeathDate()).arg(obj->getArrivalDate()).arg(obj->getID()));
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
*   \brief get a list of subject from the table subjects based in the ID.
*   \param id a Subject`s id integer
*   \return list of all subjects with that match the id
*   \retval empty if fail or not found
*/
QList<SubjectObject *> SubjectDAO::get(int id) {
    QList<SubjectObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from subject where id = '%1'")
                  .arg(id)))
    {
        while(query.next())
        {
            list.push_back( new SubjectObject(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString(), query.value(5).toLongLong(), query.value(6).toLongLong(), query.value(7).toLongLong(), query.value(8).toLongLong()));
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
*   \brief get a list of subject from the table subjects based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all subjects with that match the "column = value"
*   \retval empty if fail or not found
*/
QList<SubjectObject *> SubjectDAO::get(QString column, QString value) {
    QList<SubjectObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from subject where %1 = '%2'")
                  .arg(column).arg(value)))
    {
        while(query.next())
        {
            list.push_back( new SubjectObject(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString(), query.value(5).toLongLong(), query.value(6).toLongLong(), query.value(7).toLongLong(), query.value(8).toLongLong()));
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
*   \brief get a list of subject from the table subjects based in the ID.
*   \param column String with the column to be search
*   \param value  String with the value to be search
*   \return list of all subjects
*   \retval empty if fail
*/
QList<SubjectObject *> SubjectDAO::getAll() {
    QList<SubjectObject *> list;
    if(!db->isOpen())
        db->open();

    QSqlQuery query;
    if(query.exec(QString("select * from subject")))
    {
        while(query.next())
        {
            list.push_back( new SubjectObject(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString(), query.value(5).toLongLong(), query.value(6).toLongLong(), query.value(7).toLongLong(), query.value(8).toLongLong()));
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        qDebug() << db->lastError();
    }
    db->close();
    return list;
}
