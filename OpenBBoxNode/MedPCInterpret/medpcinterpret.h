#ifndef MEDPCINTERPRET_H
#define MEDPCINTERPRET_H
#include <QObject>
#include <QThread>
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QList>
#include <QMutex>
#include <QDateTime>
#include "event.h"
#include "statemachine.h"
#include "context.h"
#include "behaviorcontextsender.h"
#include "gpio.h"
#include <QVector>
class MedPCInterpret : public QThread
{
    Q_OBJECT

private:
    bool stop;
    QString filename;
    QHash<QString, StateMachine* > stateMachineMap;
    QHash<QString, Event* > eventsMap;
    QHash<QString, int > eventsTree;
    Context * context;
    QMutex mutex;
    int parseStateToEvents(QHash<QString, Event*> * eventMaps, QString state, QList<QString> lines);
    int * gpios;

public:
    MedPCInterpret(BehaviorTaskPacket packet, const uint * gpioInputs, const uint * gpioOutputs);
    Context *getCurrentContext();
    void startInterpret();
    void stopInterpret();
    bool getstop();

protected:
    void run();

signals:
    void abortStop();
    void outputPin(int pin, int value);

public slots:
    void addNewEvent(int pin);
    void abortStopRequest();
    void outputRequest(int pin, int value);
};

#endif // MEDPCINTERPRET_H
