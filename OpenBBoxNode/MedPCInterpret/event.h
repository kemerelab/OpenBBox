#ifndef EVENT_H
#define EVENT_H

#include <QThread>
#include <QList>
#include <QString>
#include <stdio.h>
#include <QMutex>
#include "context.h"
#include <QStringList>

#define TYPE_EVENT_NORMAL   0
#define TYPE_EVENT_WITH_IF  1

#define TYPE_INPUT_START        0
#define TYPE_INPUT_TIME_SEC     1
#define TYPE_INPUT_TIME_MIN     2
#define TYPE_INPUT_RESPONSE     3
#define TYPE_INPUT_ZPULSE       4

//NOTE: Put one more in the end in case of not found
const QString inputs_keys[] = { "#START", "\"","'","#R", "#Z", ""};

const QString commands[] = {"ON","OFF", "ADD", "SET", "SHOW", "RANDI",""};
const QString type_state_key[] = {"", "IF", "#Z"};
const QString type_ops_if_key[] = {">=", "<=", "!=", "==", ">", "<", ""};

class Event : public QObject
{
    Q_OBJECT
private:
    bool stop;
    bool executeCommands;
    uint type;

    int cntAux;

    int typeInput;
    QString p1;
    QString p2;

    QString stateName;
    QList<QString> lines;
    QList<QString> outputs;

    QString input;
    QString nextState;

    QString arg1,arg2;
    int typeIF;
    QHash<bool, QList<QString>*> outputsIF;
    QHash<bool, QString> nextStateIF;
    QHash<QString, QString> IFStatements;

    qint64 initTime;

public:
    explicit Event(QString stateName, QList<QString> lines);
    void executeCommand(QString command, Context * context);
    bool executeIFOperation( QString arg1, QString arg2, int op, Context * context);
    bool executeIF(QString condition, Context * context);
    bool executeIFLogic( bool arg1, bool arg2, int op);
    bool initEvent(Context * context);
    bool updateEvent(Context * context);
    QString getState();
    QString getNextState();

signals:
    void abortStop();
    void outputPin(int pin, int value);

public slots:
    
};

#endif // EVENT_H
