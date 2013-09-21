#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QThread>
#include <QHash>
#include <QMutex>
#include "event.h"

class StateMachine : public QObject
{
    Q_OBJECT
private:
    bool stop;
    QString stateMachineName;
    QHash<QString, Event*> eventsMap;
    QString nextState;

public:
    explicit StateMachine(QString stateMachineName, QHash<QString, Event*> eventsMap);
    void startStateMachine();
    void stopStateMachine();

    void initStateMachine(Context * context);
    bool updateStateMachine(Context * context);

    QList<Event*> getEventsList();

signals:
    void abortStop();

public slots:

};

#endif // STATEMACHINE_H
