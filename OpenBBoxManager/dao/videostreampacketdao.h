/*!
*   \file   videostreampacketdao.h
*   \class  VideoStreamPacketDAO
*   \brief  Class that contains methods to select, insert, update and delete on table openbboxnode in the database.
*/

#ifndef videostreampacketdao_H
#define videostreampacketdao_H
#include <QObject>
#include <QList>
#include "sqldatabase.h"
#include "videostreampacketobject.h"

class VideoStreamPacketDAO : public QObject
{
    Q_OBJECT
public:
    VideoStreamPacketDAO(SQLDatabase * db);           //!< Constructor
    int insert(VideoStreamPacketObject * obj);           //!< insert new row
    bool update(VideoStreamPacketObject * obj);          //!< update row
    bool remove(int id);                            //!< remove row based in the id
    QList<VideoStreamPacketObject *> get(int id);        //!< get rows based in the id
    QList<VideoStreamPacketObject *> get(QString column, QString value); //!< get rows based in a generic column
    QList<VideoStreamPacketObject*>  getAll();        //!< get all rows from table

private:
    SQLDatabase * db; //!< Database object pointer for operations of select, insert, update and delete
};

#endif // videostreampacketdao_H
