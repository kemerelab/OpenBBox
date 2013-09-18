/*!
*   \file   openbboxmanagerdao.h
*   \class  OpenBBoxManagerDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxmanager in the database.
*/

#ifndef openbboxmanagerdao_H
#define openbboxmanagerdao_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "openbboxmanagerobject.h"

class OpenBBoxManagerDAO : public QObject
{
    Q_OBJECT
public:
    OpenBBoxManagerDAO(SQLDatabase * db);           //!< Constructor
    int insert(OpenBBoxManagerObject * obj);    //!< insert new row
    bool update(OpenBBoxManagerObject * obj);   //!< update row
    bool remove(int id);                    //!< remove row based in the id
    QList<OpenBBoxManagerObject *> get(int id);     //!< get rows based in the id
    QList<OpenBBoxManagerObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<OpenBBoxManagerObject*>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // SUBJECTDAO_H
