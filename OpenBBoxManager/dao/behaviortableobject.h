/*!
*   \file  BehaviorTableobject.h
*   \class BehaviorTableObject
*   \brief Class that represents a row in the table OpenBBoxNode on the database
*/

#ifndef BehaviorTableobject_H
#define BehaviorTableobject_H
#include <QString>

class BehaviorTableObject{

public:

    //!< Constructor
BehaviorTableObject(int id,
                            int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            QString event,
                            float motortime,
                            float rewardtime)
    {
        this->id = id;
        this->idtask = idtask;
        this->port = port;
        this->idpacket  = idpacket;
        this->timeserver = timeserver;
        this->event = event;
        this->motortime = motortime;
        this->rewardtime = rewardtime;

    }

    //!< Constructor
    BehaviorTableObject(int idtask,
                            int port,
                            int idpacket,
                            long timeserver,
                            QString event,
                            float motortime,
                            float rewardtime)
    {
        this->idtask = idtask;
        this->port = port;
        this->idpacket = idpacket;
        this->timeserver = timeserver;
        this->event = event;
        this->motortime = motortime;
        this->rewardtime = rewardtime;
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

    void setEvent(QString event){this->event = event;}

    void setMotortime(float motortime){this->motortime = motortime;}

    void setRewardtime(float rewardtime){this->rewardtime = rewardtime;}

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

    int getPort(){ return this->port;}

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

    QString getEvent(){return this->event;}

    float getMotortime(){return this->motortime;}

    float getRewardtime(){return this->rewardtime;}

private:
    int id;             //!< OpenBBoxNode`s instance ID
    int idtask;
    int port;
    int idpacket;
    long timeserver;
    QString event;
    float motortime;
    float rewardtime;

 };

#endif //BehaviorTableobject_H
