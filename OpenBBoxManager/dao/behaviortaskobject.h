/*!
*   \file  behaviortaskobject.h
*   \class BehaviorTaskObject
*   \brief Class that represents a row in the table OpenBBoxNode on the database
*/

#ifndef behaviortaskobject_H
#define behaviortaskobject_H
#include <QString>

class BehaviorTaskObject{

public:

    //!< Constructor
    BehaviorTaskObject(int id,
                        int idconn,
                        int idsubject,
                        int idtask,
                        long timestart,
                        long timeend,
                        QString label)
    {
        this->id = id;
        this->idconn = idconn;
        this->idsubject = idsubject;
        this->idtask = idtask;
        this->timestart = timestart;
        this->timeend = timeend;
        this->label = label;
    }

    //!< Constructor
    BehaviorTaskObject(int idconn,
                       int idsubject,
                       int idtask,
                       long timestart,
                       long timeend,
                       QString label)
    {
        this->idconn = idconn;
        this->idsubject = idsubject;
        this->idtask = idtask;
        this->timestart = timestart;
        this->timeend = timeend;
        this->label = label;
    }

    //SET
    //!< setID(int id)
    /*!
    *   \brief Set a new value
    *   \param id integer new value
    */
    void setID(int id){ this->id = id;}

    //!< setIDConn(int idconn)
    /*!
    *   \brief Set a new value
    *   \param idconn integer new value
    */
    void setIDConn(int idconn){ this->idconn = idconn;}

    //!< setIDSubject(int idsubject)
    /*!
    *   \brief Set a new value
    *   \param idsubject integer new value
    */
    void setIDSubject(int idsubject){ this->idsubject = idsubject;}

    //!< setIDTask(int idtask)
    /*!
    *   \brief Set a new value
    *   \param idtask integer new value
    */
    void setIDTask(int idtask){ this->idtask = idtask;}

    //!< setTimeStart(long timestart)
    /*!
    *   \brief Set a new value
    *   \param timestart long new value
    */
    void setTimeStart(long timestart){ this->timestart = timestart;}

    //!< setTimeEnd(long timeend)
    /*!
    *   \brief Set a new value
    *   \param timeend long new value
    */
    void setTimeEnd(long timeend){ this->timeend = timeend;}

    //!< setLabel(QString label)
    /*!
    *   \brief Set a new value
    *   \param label String new value
    */
    void setLabel(QString label){ this->label = label;}

    //GET
    //!< getID()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getID(){ return this->id; }

    //!< getIDConn()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getIDConn(){ return this->idconn; }

    //!< getIDSubject()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getIDSubject(){ return this->idsubject; }

    //!< getIDTask()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getIDTask(){ return this->idtask; }

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


private:
    int id;             //!< OpenBBoxNode`s instance ID
    int idconn;
    int idsubject;
    int idtask;
    long timestart;     //!< OpenBBoxNode`s start time
    long timeend;       //!< OpenBBoxNode`s end time
    QString label;      //!< OpenBBoxNode`s label
};


#endif //behaviortaskobject_H
