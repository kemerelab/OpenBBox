/*!
*   \file  controlcommandobject.h
*   \class ControlCommandObject
*   \brief Class that represents a row in the table controlcommand on the database
*/

#ifndef controlcommandobject_H
#define controlcommandobject_H
#include <QString>

class ControlCommandObject{

public:

    //!< Constructor
ControlCommandObject(int id,
                     int idconn,
                     long timesent,
                     long timeanswered,
                     int command,
                     int answer)
    {
        this->id = id;
        this->idconn = idconn;
        this->timesent = timesent;
        this->timeanswered = timeanswered;
        this->command = command;
        this->answer = answer;
    }

    //!< Constructor
    ControlCommandObject(int idconn,
                            long timesent,
                            long timeanswered,
                            int command,
                            int answer)
    {
        this->idconn = idconn;
        this->timesent = timesent;
        this->timeanswered = timeanswered;
        this->command = command;
        this->answer = answer;
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

    //!< setTimeSent(long timesent)
    /*!
    *   \brief Set a new value
    *   \param timesent long new value
    */
    void setTimeSent(long timesent){ this->timesent = timesent;}

    //!< setTimeAnswered(long timeanswered)
    /*!
    *   \brief Set a new value
    *   \param timeanswered long new value
    */
    void setTimeUSec(long timeanswered){ this->timeanswered = timeanswered;}

    //!< setCommand(int command)
    /*!
    *   \brief Set a new value
    *   \param command integer new value
    */
    void setCommand(int command){ this->command = command;}

    //!< setAnswer(int answer)
    /*!
    *   \brief Set a new value
    *   \param answer integer new value
    */
    void setAnswer(int answer){ this->answer = answer;}

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

    //!< getTimeSent()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeSent(){ return this->timesent; }

    //!< getTimeAnswered()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeAnswered(){ return this->timeanswered; }

    //!< getCommand()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getCommand(){ return this->command; }

    //!< getAnswer()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getAnswer(){ return this->answer; }



private:
    int id;             //!< OpenBBoxNode`s instance ID
    int idconn;
    long timesent;
    long timeanswered;     //!< OpenBBoxNode`s start time
    int command;       //!< OpenBBoxNode`s end time
    int answer;      //!< OpenBBoxNode`s label
 };

#endif //controlcommandobject_H
