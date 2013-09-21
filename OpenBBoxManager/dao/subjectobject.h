/*!
*   \file  subjectobject.h
*   \class SubjectObject
*   \brief Class that represents a row in the table subject on the database
*/

#ifndef SUBJECTOBJECT_H
#define SUBJECTOBJECT_H
#include <QString>

class SubjectObject{

public:

    //!< Constructor
    SubjectObject(int id,
                  QString tag,
                  QString type,
                  QString label,
                  QString protocol,
                  long timeserver,
                  long birthdate,
                  long deathdate,
                  long arrivaldate)
    {
        this->id = id;
        this->tag = tag;
        this->type = type;
        this->label = label;
        this->protocol = protocol;
        this->timeserver = timeserver;
        this->birthdate = birthdate;
        this->deathdate = deathdate;
        this->arrivaldate = arrivaldate;
    }

    //!< Constructor
    SubjectObject(QString tag,
                  QString type,
                  QString label,
                  QString protocol,
                  long timeserver,
                  long birthdate,
                  long deathdate,
                  long arrivaldate)
    {
        this->tag = tag;
        this->type = type;
        this->label = label;
        this->protocol = protocol;
        this->timeserver = timeserver;
        this->birthdate = birthdate;
        this->deathdate = deathdate;
        this->arrivaldate = arrivaldate;
    }

    //SET
    //!< setID(int id)
    /*!
    *   \brief Set a new value
    *   \param id integer new value
    */
    void setID(int id){ this->id = id;}

    //!< setTag(QString tag)
    /*!
    *   \brief Set a new value
    *   \param tag String new value
    */
    void setTag(QString tag){ this->tag = tag;}

    //!< setType(QString type)
    /*!
    *   \brief Set a new value
    *   \param type String new value
    */
    void setType(QString type){ this->type = type;}

    //!< setLabel(QString label)
    /*!
    *   \brief Set a new value
    *   \param label String new value
    */
    void setLabel(QString label){ this->label = label;}

    //!< setProtocol(QString protocol)
    /*!
    *   \brief Set a new value
    *   \param protocol String new value
    */
    void setProtocol(QString protocol){ this->protocol = protocol;}

    //!< setTimeServer(long timeserver)
    /*!
    *   \brief Set a new value
    *   \param timeserver long new value
    */
    void setTimeServer(long timeserver){ this->timeserver = timeserver;}

    //!< setBirthDate(long birthdate)
    /*!
    *   \brief Set a new value
    *   \param birthdate long new value
    */
    void setBirthDate(long birthdate){  this->birthdate = birthdate; }

    //!< setDeathDate(long deathdate)
    /*!
    *   \brief Set a new value
    *   \param deathdate long new value
    */
    void setDeathDate(long deathdate){  this->deathdate = deathdate; }

    //!< setArrivalDate(long arrivaldate)
    /*!
    *   \brief Set a new value
    *   \param arrivaldate long new value
    */
    void setArrivalDate(long arrivaldate){ this->arrivaldate = arrivaldate; }

    //GET
    //!< getID()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getID(){ return this->id; }

    //!< getTag()
    /*!
    *   \brief Get the value
    *   \return String
    */
    QString getTag(){ return this->tag; }

    //!< getType()
    /*!
    *   \brief Get the value
    *   \return String
    */
    QString getType(){ return this->type; }

    //!< getLabel()
    /*!
    *   \brief Get the value
    *   \return String
    */
    QString getLabel(){ return this->label; }

    //!< getProtocol()
    /*!
    *   \brief Get the value
    *   \return String
    */
    QString getProtocol(){ return this->protocol; }

    //!< getTimeServer()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeServer(){ return this->timeserver; }

    //!< getBirthDate()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getBirthDate(){ return this->birthdate; }

    //!< getDeathDate()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getDeathDate(){ return this->deathdate; }

    //!< getArrivalDate()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getArrivalDate(){ return this->arrivaldate; }
private:
    int id;             //!< Suject`s ID
    QString tag;        //!< Suject`s Tag
    QString type;       //!< Suject`s Type
    QString label;      //!< Suject`s Label
    QString protocol;   //!< Suject`s Protocol
    long timeserver;    //!< Suject`s time of register on server
    long birthdate;     //!< Suject`s Date of birth
    long deathdate;     //!< Suject`s Date of death
    long arrivaldate;   //!< Suject`s Date of Arrival
};
#endif // SUBJECTOBJECT_H
