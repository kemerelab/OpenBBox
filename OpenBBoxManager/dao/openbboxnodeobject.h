/*!
*   \file  openbboxnodeobject.h
*   \class OpenBBoxNodeObject
*   \brief Class that represents a row in the table OpenBBoxNode on the database
*/

#ifndef openbboxnodeobject_H
#define openbboxnodeobject_H
#include <QString>

class OpenBBoxNodeObject{

public:

    //!< Constructor
    OpenBBoxNodeObject(int id,
                        int idmanager,
                        long timestart,
                        long timeend,
                        QString label,
                        QString mac,
                        QString ip,
                        int port,
                        int numcameras)
    {
        this->id = id;
        this->idmanager = idmanager;
        this->timestart = timestart;
        this->timeend = timeend;
        this->label = label;
        this->mac = mac;
        this->port = port;
        this->ip = ip;
        this->port = port;
        this->numcameras = numcameras;
    }

    //!< Constructor
    OpenBBoxNodeObject(int idmanager,
                       long timestart,
                       long timeend,
                       QString label,
                       QString mac,
                       QString ip,
                       int port,
                       int numcameras)
    {
        this->idmanager = idmanager;
        this->timestart = timestart;
        this->timeend = timeend;
        this->label = label;
        this->mac = mac;
        this->port = port;
        this->ip = ip;
        this->port = port;
        this->numcameras = numcameras;
    }

    //SET
    //!< setID(int id)
    /*!
    *   \brief Set a new value
    *   \param id integer new value
    */
    void setID(int id){ this->id = id;}

    //!< setTimeStart(long timestart)
    /*!
    *   \brief Set a new value
    *   \param timeserver long new value
    */
    void setTimeStart(long timestart){ this->timestart = timestart;}


    //GET
    //!< getID()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getID(){ return this->id; }

    //!< getIDManager()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getIDManager(){ return this->idmanager; }

    //!< getTimeStart()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeStart(){ return this->timestart; }

    //!< getTimeEnd()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeEnd(){ return this->timeend; }

    //!< getLabel()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getLabel(){ return this->label; }

    //!< getMAC()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getMAC(){ return this->mac; }

    //!< getIP()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getIP(){ return this->ip; }

    //!< getPort()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getPort(){ return this->port; }

    //!< getNumCameras()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getNumCameras(){ return this->numcameras; }

private:
    int id;             //!< OpenBBoxNode`s instance ID
    int idmanager;
    long timestart;     //!< OpenBBoxNode`s start time
    long timeend;       //!< OpenBBoxNode`s end time
    QString label;      //!< OpenBBoxNode`s label
    QString mac;        //!< OpenBBoxNode`s start time
    QString ip;         //!< OpenBBoxNode`s start time
    int port;
    int numcameras;
};
#endif //openbboxnodeobject_H
