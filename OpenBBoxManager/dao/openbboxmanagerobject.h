/*!
*   \file  openbboxmanagerobject.h
*   \class OpenBBoxManagerObject
*   \brief Class that represents a row in the table OpenBBoxManager on the database
*/

#ifndef openbboxmanagerobject_H
#define openbboxmanagerobject_H
#include <QString>

class OpenBBoxManagerObject{

public:

    //!< Constructor
    OpenBBoxManagerObject(int id,
                  long timestart)
    {
        this->id = id;
        this->timestart = timestart;
    }

    //!< Constructor
    OpenBBoxManagerObject(long timestart)
    {
        this->timestart = timestart;
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

    //!< getTimeStart()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeStart(){ return this->timestart; }
private:
    int id;             //!< OpenBBoxManager`s instance ID
    long timestart;     //!< OpenBBoxManager`s start time
};
#endif //openbboxmanagerobject_H
