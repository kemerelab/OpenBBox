/*!
*   \file  behavioreventpacketobject.h
*   \class BehaviorEventPacketObject
*   \brief Class that represents a row in the table OpenBBoxNode on the database
*/

#ifndef behavioreventpacketobject_H
#define behavioreventpacketobject_H
#include <QString>

class BehaviorEventPacketObject {

public:

    //!< Constructor
BehaviorEventPacketObject(int id,
                            int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            long timesec,
                            long timeusec,
                            int  pinscontext,
                            int  pinevent,
                            QString pineventlabel)
    {
        this->id = id;
        this->idtask = idtask;
        this->port = port;
        this->idpacket  = idpacket;
        this->timeserver = timeserver;
        this->timesec = timesec;
        this->timeusec = timeusec;
        this->pinscontext = pinscontext;
        this->pinevent = pinevent;
        this->pineventlabel = pineventlabel;
    }

    //!< Constructor
    BehaviorEventPacketObject(int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            long timesec,
                            long timeusec,
                            int  pinscontext,
                            int  pinevent,
                            QString pineventlabel)
    {
        this->idtask = idtask;
        this->port = port;
        this->idpacket = idpacket;
        this->timeserver = timeserver;
        this->timesec = timesec;
        this->timeusec = timeusec;
        this->pinscontext = pinscontext;
        this->pinevent = pinevent;
        this->pineventlabel = pineventlabel;
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

    //!< setPinsContext(int pinscontext)
    /*!
    *   \brief Set a new value
    *   \param pinscontext integer new value
    */
    void setPinsContext(int pinscontext){ this->pinscontext = pinscontext;}

    //!< setPinEvent(int pinevent)
    /*!
    *   \brief Set a new value
    *   \param pinevent integer new value
    */
    void setPinEvent(int pinevent){ this->pinevent = pinevent;}

    //!< setPinEventLabel(int pineventlabel)
    /*!
    *   \brief Set a new value
    *   \param pineventlabel String new value
    */
    void setPinEventLabel(QString pineventlabel){ this->pineventlabel = pineventlabel;}

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

    //!< getPinsContext()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getPinsContext(){ return this->pinscontext; }

    //!< getPinEvent()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getPinEvent(){ return this->pinevent; }

    //!< getPinEventLabel()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getPinEventLabel(){ return this->pineventlabel; }

private:
    int id;             //!< OpenBBoxNode`s instance ID
    int idtask;
    int port;
    int idpacket;
    long timeserver;
    long timesec;     //!< OpenBBoxNode`s start time
    long timeusec;       //!< OpenBBoxNode`s end time
    int  pinscontext;       //!< OpenBBoxNode`s end time
    int  pinevent;       //!< OpenBBoxNode`s end time
    QString pineventlabel;      //!< OpenBBoxNode`s label
 };

#endif //behavioreventpacketobject_H
