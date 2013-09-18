#ifndef OBJECTDAO_H
#define OBJECTDAO_H
#include <QObject>
#include <sqldatabase.h>
class ObjectDAO
{

public:
    virtual bool open()= 0;
   // virtual close
protected:
    //SqlDatabase db;
    QString type;
};

Q_DECLARE_INTERFACE(SQLDatabase, "SQLDatabase/1.0")
#endif // OBJECTDAO_H
