/*!
*   \file  videostreampacketobject.h
*   \class VideoStreamPacketObject
*   \brief Class that represents a row in the table OpenBBoxNode on the database
*/

#ifndef videostreampacketobject_H
#define videostreampacketobject_H
#include <QString>

class VideoStreamPacketObject{

public:

    //!< Constructor
VideoStreamPacketObject(int id,
                            int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            long timesec,
                            long timeusec,
                            int  sizeexpected,
                            int  size,
                            int  width,
                            int  height,
                            int  format,
                            QString filename,
                            int headerframe,
                            int checksum)
    {
        this->id = id;
        this->idtask = idtask;
        this->port = port;
        this->idpacket  = idpacket;
        this->timeserver = timeserver;
        this->timesec = timesec;
        this->timeusec = timeusec;
        this->sizeexpected = sizeexpected;
        this->size = size;
        this->width = width;
        this->height = height;
        this->format = format;
        this->filename = filename;
        this->headerframe = headerframe;
        this->checksum = checksum;
    }

    //!< Constructor
    VideoStreamPacketObject(int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            long timesec,
                            long timeusec,
                            int  sizeexpected,
                            int  size,
                            int  width,
                            int  height,
                            int  format,
                            QString filename,
                            int headerframe,
                            int checksum)
    {
        this->idtask = idtask;
        this->port = port;
        this->idpacket = idpacket;
        this->timeserver = timeserver;
        this->timesec = timesec;
        this->timeusec = timeusec;
        this->sizeexpected = sizeexpected;
        this->size = size;
        this->width = width;
        this->height = height;
        this->format = format;
        this->filename = filename;
        this->headerframe = headerframe;
        this->checksum = checksum;
    }

    //SET
    //!< setID(int id)
    /*!
    *   \brief Set a new value
    *   \param id integer new value
    */
    void setID(int id){ this->id = id;}

    //!< setIDTask(int idtask)
    /*!
    *   \brief Set a new value
    *   \param idtask integer new value
    */
    void setIDTask(int idtask){ this->idtask = idtask;}

    //!< setPort(int port)
    /*!
    *   \brief Set a new value
    *   \param port integer new value
    */
    void setPort(int port){ this->port = port;}

    //!< setIDPacket(int idpacket)
    /*!
    *   \brief Set a new value
    *   \param idpacket integer new value
    */
    void setIDPacket(int idpacket){ this->idpacket = idpacket;}

    //!< setTimeServer(long timeserver)
    /*!
    *   \brief Set a new value
    *   \param timeserver long new value
    */
    void setTimeServer(long timeserver){ this->timeserver = timeserver;}

    //!< setTimeSec(long timesec)
    /*!
    *   \brief Set a new value
    *   \param timesec long new value
    */
    void setTimeSec(long timesec){ this->timesec = timesec;}

    //!< setTimeUSec(long timeusec)
    /*!
    *   \brief Set a new value
    *   \param timeusec long new value
    */
    void setTimeUSec(long timeusec){ this->timeusec = timeusec;}

    //!< setSizeExpected(int sizeexpected)
    /*!
    *   \brief Set a new value
    *   \param sizeexpected integer new value
    */
    void setSizeExpected(int sizeexpected){ this->sizeexpected = sizeexpected;}

    //!< setSize(int size)
    /*!
    *   \brief Set a new value
    *   \param size integer new value
    */
    void setSize(int size){ this->size = size;}

    //!< setWidth(int width)
    /*!
    *   \brief Set a new value
    *   \param width integer new value
    */
    void setWidth(int width){ this->width = width;}

    //!< setHeight(int height)
    /*!
    *   \brief Set a new value
    *   \param height integer new value
    */
    void setHeight(int height){ this->height = height;}

    //!< setFormat(int format)
    /*!
    *   \brief Set a new value
    *   \param format integer new value
    */
    void setFormat(int format){ this->format = format;}

    //!< setHeaderFrame(int headerframe)
    /*!
    *   \brief Set a new value
    *   \param headerframe integer new value
    */
    void setHeaderFrame(int headerframe){ this->headerframe = headerframe;}

    //!< setChecksum(int checksum)
    /*!
    *   \brief Set a new value
    *   \param checksum integer new value
    */
    void setChecksum(int checksum){ this->checksum = checksum;}

    //!< setIDTask(int idtask)

    //!< setFilename(QString filename)
    /*!
    *   \brief Set a new value
    *   \param filename String new value
    */
    void setFilename(QString filename){ this->filename = filename;}

    //GET
    //!< getID()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getID(){ return this->id; }

    //!< getIDTask()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getIDTask(){ return this->idtask; }

    //!< getIDPacket()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getIDPacket(){ return this->idpacket; }

    //!< getTimeServer()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeServer(){ return this->timeserver; }

    //!< getTimeSec()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeSec(){ return this->timesec; }

    //!< getTimeUSec()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeUSec(){ return this->timeusec; }

    //!< getPort()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getPort(){ return this->port; }

    //!< getSizeExpected()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getSizeExpected(){ return this->sizeexpected; }

    //!< getSize()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getSize(){ return this->size; }

    //!< getFormat()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getFormat(){ return this->format; }

    //!< getWidth()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getWidth(){ return this->width; }

    //!< getHeight()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getHeight(){ return this->height ; }

    //!< getHeaderFrame()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getHeaderFrame(){ return this->headerframe; }

    //!< getChecksum()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getChecksum(){ return this->checksum; }

    //!< getFilename()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getFilename(){ return this->filename; }


private:
    int id;             //!< OpenBBoxNode`s instance ID
    int idtask;
    int port;
    int idpacket;
    long timeserver;
    long timesec;     //!< OpenBBoxNode`s start time
    long timeusec;       //!< OpenBBoxNode`s end time
    int  sizeexpected;       //!< OpenBBoxNode`s end time
    int  size;       //!< OpenBBoxNode`s end time
    int  width;       //!< OpenBBoxNode`s end time
    int  height;       //!< OpenBBoxNode`s end time
    int  format;       //!< OpenBBoxNode`s end time
    QString filename;      //!< OpenBBoxNode`s label
    int headerframe;
    int checksum;
};

#endif //videostreampacketobject_H
