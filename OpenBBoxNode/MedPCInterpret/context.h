#ifndef CONTEXT_H
#define CONTEXT_H

#include <QHash>
#include <QList>
#include <QQueue>
#include <QString>
#include "stdio.h"
#include <QTime>
#include <sys/time.h>
#include <limits>
#include <QVector>
#include "../OpenBBoxManager/config.h"
#include "../OpenBBoxNode/gpio.h"

#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>

#define CONST_DELIMITER     '^'
#define ARRAY_DELIMITER     "DIM"
#define LIST_DELIMITER      "LIST"
#define ZPULSE_DELIMITER    "#Z"
#define DISKVARS_DELIMITER  "DISKVARS"

#define ARRAY_LIST_DELIMITER_BEGIN  '('
#define ARRAY_LIST_DELIMITER_END    ')'

class Context
{
private:
    qint64 systemTime;
    QHash<QString, int> pinNames;
    QHash<QString, int> constsMap;
    QHash<QString, QVector<float>* >  arraysMap;
    QHash<int, bool>  zpulseMap;
    QHash<QString, float> variablesMap;
    QList<int> inputsEvents;
    int lastOutput;
    struct timeval tv;
    const uint * gpioInputs;
    const uint * gpioOutputs;


public:

    Context(const uint * gpioInputs, const uint * gpioOutputs, QHash<QString, int> pinNames);

    QHash<QString, int>                 *getConstants();
    QHash<QString, QVector<float>* >    *getArrays();
    QHash<int, bool>                    *getZPulses();
    QHash<QString, float>               *getVariables();

    float getValue(QString var);
    bool setValue(QString var, float value);
    void executeCommand(QString command);
    bool executeIF(QString var);

    qint64 getSystemTime();
    void setSystemTime(qint64 time);
    QList<int> *getInputsEvents();
    int getlastOutput();
    void resetlastOutput();
    struct timeval getlastOutputtv();

    bool isZPulseRequested();
    bool isZPulseRequested(int index);
    void clearZPulseRequests();

    void addEventPort(int pin);
    bool isEventHappened(int pin);
    void clearEvents();

    bool isTimePassed(qint64 timeBase, uint time);
    bool addVarIfNotExists(QString var);
    qint64 getCurrentTimeSystem();
    QString toString();

};

#endif // CONTEXT_H
