/*!
*   \file   controlcommanddao.h
*   \class  ControlCommandDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef controlcommanddao_H
#define controlcommanddao_H_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "controlcommandobject.h"

class ControlCommandDAO : public QObject
{
    Q_OBJECT
public:
    ControlCommandDAO(SQLDatabase * db);           //!< Constructor
    int insert(ControlCommandObject * obj);           //!< insert new row
    bool update(ControlCommandObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<ControlCommandObject *> get(int id);        //!< get rows based in the id
    QList<ControlCommandObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<ControlCommandObject *>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // controlcommanddao_H
