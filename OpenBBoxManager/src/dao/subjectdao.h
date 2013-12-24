/*!
*   \file   subjectdao.h
*   \class  SubjectDAO
*   \brief  Class that contains methods to select, insert, update and delete on table subject in the database.
*/

#ifndef SUBJECTDAO_H
#define SUBJECTDAO_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "subjectobject.h"

class SubjectDAO : public QObject
{
    Q_OBJECT
public:
    SubjectDAO(SQLDatabase * db);           //!< Constructor
    int insert(SubjectObject * obj);        //!< insert new row
    bool update(SubjectObject * obj);       //!< update row
    bool remove(int id);                    //!< remove row based in the id
    QList<SubjectObject *> get(int id);     //!< get rows based in the id
    QList<SubjectObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<SubjectObject*>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // SUBJECTDAO_H
