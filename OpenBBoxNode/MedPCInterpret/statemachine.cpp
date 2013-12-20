#include "statemachine.h"
#include <QStringList>
StateMachine::StateMachine(QString stateMachineName, QHash<QString, Event*> eventsMap) :
    QObject()
{
    this->eventsMap = eventsMap;
    this->stateMachineName = stateMachineName;
    this->stop = true;

    qDebug() << "State Machine: " << stateMachineName;
}

void StateMachine::initStateMachine(Context * context) {
    //Connect everyone with abort signal/slot
    QList<Event *> eventsAll = eventsMap.values();
    //get the first state
    nextState = eventsAll.at(0)->getState();

    //init the first state
    QList<Event *> events = eventsMap.values(nextState);
    for(int i=0; i < events.size(); i++){
        events.at(i)->initEvent(context); //TODO see if something went wrong
    }
}

const QStringList listAbortStates = QStringList() << "STOPABORT" << "STOPKILL" << "STOPABORTFLUSH";
bool StateMachine::updateStateMachine(Context * context) {

        //connect new event
        QList<Event *> events = eventsMap.values(nextState);
        for(int i=0; i < events.size(); i++){
            if(events.at(i)->updateEvent(context)) { // if event complete
                //get the first state
                nextState = events.at(i)->getNextState();
                qDebug() << QString("State machine %1 change to: %2").arg(this->stateMachineName).arg(nextState);
                //init the first state
                if(eventsMap.contains(nextState)){
                    QList<Event *> events = eventsMap.values(nextState);
                    for(int i=0; i < events.size(); i++){
                   //         qDebug("Events changed: %d", i);
                            events.at(i)->initEvent(context); //TODO see if something went wrong
                    }
                }else if(listAbortStates.contains(nextState)){ //Stop requested
                        return false;
                }else{
                     qDebug() << "Key not found: " << nextState;
                     QList<QString> keys = eventsMap.keys();
                     for(int i = 0; i < keys.size(); i++) {
                         qDebug() << "Possible key: " << keys.at(i);
                     }
                }
                break; //TODO Lock that
            }
        }
        return true;
}

QList<Event *> StateMachine::getEventsList(){
    return  eventsMap.values();
}
