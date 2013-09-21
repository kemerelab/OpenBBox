/*!
*   \file  behaviorinfoobject.h
*   \class BehaviorInfoObject
*   \brief Class that represents a row in the table OpenBBoxNode on the database
*/

#ifndef behaviorinfoobject_H
#define behaviorinfoobject_H
#include <QString>

class BehaviorInfoObject{

public:

    //!< Constructor
BehaviorInfoObject(int id,
                            int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            long timesec,
                            long timeusec,
                            QString )
    {
        this->id = id;
        this->idtask = idtask;
        this->port = port;
        this->idpacket  = idpacket;
        this->timeserver = timeserver;
        this->timesec = timesec;
        this->timeusec = timeusec;
        this->info = info;
    }

    //!< Constructor
    BehaviorInfoObject(int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            long timesec,
                            long timeusec,
                            QString info)
    {
        this->idtask = idtask;
        this->port = port;
        this->idpacket = idpacket;
        this->timeserver = timeserver;
        this->timesec = timesec;
        this->timeusec = timeusec;
        this->info = info;
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


    //!< setInfo(int info)
    /*!
    *   \brief Set a new value
    *   \param info String new value
    */
    void setInfo(QString info){ this->info = info;}

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

    //!< getInfo()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getInfo(){ return this->info; }

private:
    int id;             //!< OpenBBoxNode`s instance ID
    int idtask;
    int port;
    int idpacket;
    long timeserver;
    long timesec;     //!< OpenBBoxNode`s start time
    long timeusec;       //!< OpenBBoxNode`s end time
    QString info;      //!< OpenBBoxNode`s label
 };

#endif //behaviorinfoobject_H
