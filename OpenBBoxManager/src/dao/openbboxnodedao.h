/*!
*   \file   openbboxnodedao.h
*   \class  OpenBBoxNodeDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef openbboxnodedao_H
#define openbboxnodedao_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "openbboxnodeobject.h"

class OpenBBoxNodeDAO : public QObject
{
    Q_OBJECT
public:
    OpenBBoxNodeDAO(SQLDatabase * db);           //!< Constructor
    int insert(OpenBBoxNodeObject * obj);           //!< insert new row
    bool update(OpenBBoxNodeObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<OpenBBoxNodeObject *> get(int id);        //!< get rows based in the id
    QList<OpenBBoxNodeObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<OpenBBoxNodeObject*>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // SUBJECTDAO_H
