#include "statemachine.h"
#include <QStringList>
StateMachine::StateMachine(QString stateMachineName, QHash<QString, Event*> eventsMap, QHash<QString, int> eventsTree) :
    QObject()
{
    this->eventsMap = eventsMap;
    this->stateMachineName = stateMachineName;
    this->stop = true;
    this->eventsTree = eventsTree;

    qDebug("State Machine: %s", qPrintable(stateMachineName));
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
                qDebug("State machine %s change to: %s", qPrintable(this->stateMachineName), qPrintable(nextState));
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
                     qDebug("Key not found: %s", qPrintable(nextState));
                     QList<QString> keys = eventsMap.keys();
                     for(int i = 0; i < keys.size(); i++) {
                         qDebug("Possible key: %s", qPrintable(keys.at(i)));
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
