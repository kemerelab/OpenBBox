/*!
*   \file  taskfileobject.h
*   \class TaskFileObject
*   \brief Class that represents a row in the table OpenBBoxNode on the database
*/

#ifndef taskfileobject_H
#define taskfileobject_H
#include <QString>

class TaskFileObject{

public:

    //!< Constructor
    TaskFileObject( int id,
                    QString filename,
                    long timeserver,
                    QString taskname,
                    QString type,
                    QByteArray file,
                    QString hash)
    {
        this->id = id;
        this->timeserver = timeserver;
        this->filename = filename;
        this->taskname = taskname;
        this->timeserver = timeserver;
        this->type = type;
        this->file = file;
        this->hash = hash;
    }

    //!< Constructor
    TaskFileObject( QString filename,
                    long timeserver,
                    QString taskname,
                    QString type,
                    QByteArray file,
                    QString hash)
    {
        this->timeserver = timeserver;
        this->filename = filename;
        this->taskname = taskname;
        this->timeserver = timeserver;
        this->type = type;
        this->file = file;
        this->hash = hash;
     }

    //SET
    //!< setID(int id)
    /*!
    *   \brief Set a new value
    *   \param id integer new value
    */
    void setID(int id){ this->id = id;}

    //!< setTimeServer(long timeserver)
    /*!
    *   \brief Set a new value
    *   \param timeserver long new value
    */
    void setTimeServer(long timeserver){ this->timeserver = timeserver;}

    //!< setFilename(QString filename)
    /*!
    *   \brief Set a new value
    *   \param filename String new value
    */
    void setFilename(QString filename){ this->filename = filename;}

    //!< setTaskname(QString taskname)
    /*!
    *   \brief Set a new value
    *   \param taskname String new value
    */
    void setTaskname(QString taskname){ this->taskname = taskname;}

    //!< setType(QString type)
    /*!
    *   \brief Set a new value
    *   \param type String new value
    */
    void setType(QString type){ this->type = type;}

    //!< setHash(QString hash)
    /*!
    *   \brief Set a new value
    *   \param hash String new value
    */
    void setHash(QString hash){ this->hash = hash;}

    //!< setFile(QByteArray file)
    /*!
    *   \brief Set a new value
    *   \param file String new value
    */
    void setFile(QByteArray file){ this->file = file;}

    //GET
    //!< getID()
    /*!
    *   \brief Get the value
    *   \return int
    */
    int getID(){ return this->id; }

    //!< getTimeServer()
    /*!
    *   \brief Get the value
    *   \return long
    */
    long getTimeServer(){ return this->timeserver; }

    //!< getFilename()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getFilename(){ return this->filename; }

    //!< getTaskname()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getTaskname(){ return this->taskname; }

    //!< getType()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getType(){ return this->type; }

    //!< getType()
    /*!
    *   \brief Get the value
    *   \return QByteArray
    */
    QByteArray getFile(){ return this->file; }

    //!< getHash()
    /*!
    *   \brief Get the value
    *   \return QString
    */
    QString getHash(){ return this->hash; }
private:
    int id;             //!< OpenBBoxNode`s instance ID
    long timeserver;
    QString filename;      //!< OpenBBoxNode`s label
    QString taskname;      //!< OpenBBoxNode`s label
    QString type;      //!< OpenBBoxNode`s label
    QByteArray file;
    QString hash;
 };

#endif //taskfileobject_H
