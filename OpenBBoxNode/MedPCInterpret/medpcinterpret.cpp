#include "medpcinterpret.h"

void MedPCInterpret::abortStopRequest(){
    stop = true;
    emit abortStop();
}

void MedPCInterpret::outputRequest(int pin, int value) {
    qDebug("Output: pin %d value %d", pin, value);
    emit outputPin(pin, value);
}

int MedPCInterpret::parseStateToEvents(QHash<QString, Event*> * eventMaps, QString state, QList<QString> lines){

    QList<QString> sublines;

    //For normal Statement
    for(int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        if(line.contains(":") && !line.contains("@")) {
            QString substate;
            if(i>0){
                substate = "*";
                substate.append(QString::number(i));
            }
            sublines.push_back(line);
            eventMaps->insert(state.remove('\t').append(substate), new Event(state, sublines));
            sublines.clear();
        }else{
            sublines.push_back(line);
        }
    }

    //For IF statement
    if(sublines.size() > 0) {
        eventMaps->insert(state.remove('\t'), new Event(state, sublines));
        return 1;
    }
    return lines.size();
}

MedPCInterpret::MedPCInterpret(BehaviorTaskPacket packet,  const uint * gpioInputs, const uint * gpioOutputs) :
    QThread()
{

    QString keyStateMachine;
    QString keyState;
    QList<QString> lines;
    bool startedStatesMachines = false;
    this->gpios = gpios;
    this->context = new Context(gpioInputs, gpioOutputs);
    QString line;

    int i = 0, j = 0;
    for(i = 0; i < packet.lines; i++){

          line = QString(packet.file+j);
          j = j + line.size()+1;

          line = line.replace(" ", "");
          line = line.replace("\t", "");
          line = line.replace("", "");
          line = line.replace("\r", "");
          line = line.toUpper(); // remove spaces, tabs and put to upper

          if(line.size() > 0){ // be sure
              if(line.at(0) != '\\') { //remove easy comments
                line = line.section('\\', 0, 0);
                        if(line.at(0) == (CONST_DELIMITER)){
                            context->getConstants()->insert(line.section('=',0,0), line.section('=',1,1).toInt());
                            qDebug("%s: %d", qPrintable(line.section('=',0,0)), line.section('=',1,1).toInt());
                        }

                        if(line.contains(DISKVARS_DELIMITER)){
                           line = line.replace(DISKVARS_DELIMITER,"");
                           line = line.replace("=","");
                           QStringList list=  line.split(",");
                           for (int i = 0; i < list.size(); ++i)
                               if(list.at(i) != "" && !list.at(i).isEmpty()){
                                   context->getVariables()->insert(list.at(i), 0);
                                   qDebug("%s: %d", qPrintable(list.at(i)), 0);
                               }
                        }

                        if(line.contains(ARRAY_DELIMITER)){
                            line.replace(ARRAY_DELIMITER,"");
                            context->getArrays()->insert(line.section('=',0,0), new QVector<float>(line.section('=',1,1).toInt()));
                            context->getArrays()->value(line.section('=',0,0))->fill(0);
                            qDebug("%s: %d", qPrintable(line.section('=',0,0)), line.section('=',1,1).toInt());
                        }

                        if(line.contains(LIST_DELIMITER)){
                            line.replace(LIST_DELIMITER,"");
                            QString key = line.section('=',0,0);
                            context->getArrays()->insert(key, new QVector<float>());
                            line = line.section('=',1,1);
                            int i=0;
                            for(i = 0; i < line.count(',') + 1; i++){
                                context->getArrays()->value(key)->push_back(line.section(',',i,i).toFloat());
                                qDebug("%s: %f", qPrintable(key), line.section(',',i,i).toFloat());
                            }
                        }

                        if(line.contains(ZPULSE_DELIMITER)){
                            line = line.section(":",0,0);
                            line = line.section(ZPULSE_DELIMITER, 1, 1);
                            if(!context->getZPulses()->contains(line.toInt())){
                                context->getZPulses()->insert(line.toInt(), false);
                                qDebug("ZPulse: %s", qPrintable(line));
                            }
                        }
                   }
            }
        }


    startedStatesMachines = false;
    i = 0, j = 0;

    for(i = 0; i < packet.lines; i++)
        {
            line = QString(packet.file+j);
            j = j + line.size()+1;

             line = line.replace(" ", "");
             line = line.replace("\t", "");
             line = line.replace("", "");
             line = line.replace("\r", "");
             line = line.toUpper();
              if(line.size() > 0){ // be sure
                  if(line.at(0) != '\\') { //remove easy comments
                    line = line.section('\\', 0, 0);
                    //printf(line.toAscii());
                    //printf("");

                    if(!startedStatesMachines){ // if state machines declaration not started
                        if(line.contains("S.S.")){
                            startedStatesMachines = true;
                            keyStateMachine = line.replace(",","");
                        }
                    }else{// if state machines declaration started
                          if(line.contains("S.S.")){
                              if(lines.size() > 0)
                                  eventsTree.insert(keyState, parseStateToEvents( &eventsMap, keyState, lines));
                              lines.clear();

                              stateMachineMap.insert(keyStateMachine, new StateMachine(keyStateMachine, eventsMap, eventsTree));
                              eventsMap.clear();
                              eventsTree.clear();
                              keyStateMachine = line.replace(",","");
                          }else if(line.at(0) == 'S' && (line.at(2) == ',' || line.at(3) == ',')){
                              if(lines.size() > 0) {
                                  eventsTree.insert(keyState, parseStateToEvents( &eventsMap, keyState, lines));
                                  lines.clear();
                              }
                              keyState = line.replace(",","");
                          }else{
                              lines.push_back(line);
                          }
                    }
                }
            }
        }

        if(lines.size() > 0) {
              eventsTree.insert(keyState, parseStateToEvents( &eventsMap, keyState, lines));
              stateMachineMap.insert(keyStateMachine, new StateMachine(keyStateMachine, eventsMap, eventsTree));
              eventsMap.clear();
              eventsTree.clear();
              lines.clear();
        }

}



void MedPCInterpret::startInterpret(){
    stop = false;
    start(NormalPriority);
}

void MedPCInterpret::stopInterpret(){
    stop = true;
}

void MedPCInterpret::addNewEvent(int pin) {
    qDebug("Input: pin %d", pin);

    mutex.lock();
      this->context->addEventPort(pin);
    mutex.unlock();
}

#define SYSTEM_TICK_MS 10

void MedPCInterpret::run() {

    //update time
    this->context->setSystemTime(this->context->getCurrentTimeSystem());
    qDebug("Started at %lld", context->getSystemTime());

    QList<StateMachine*> stateMachineList = this->stateMachineMap.values();
    //init state machines
    for(int i=0; i < stateMachineList.size(); i++) {
        stateMachineList.at(i)->initStateMachine(this->context);
    }

    while(!this->stop) {

        mutex.lock();
        //update time
        this->context->setSystemTime(this->context->getCurrentTimeSystem());
        //update State Machine
        QList<StateMachine*> stateMachineList = this->stateMachineMap.values();
        for(int i=0; i < stateMachineList.size(); i++){
            if(!stateMachineList.at(i)->updateStateMachine(this->context)){
                //stopabort, stopkill or stopabortflush requested
                this->stop = true;
                //TODO send something to server
                qDebug("Context: %s", qPrintable(this->context->toString()));
                qDebug("Ended at %lld", context->getSystemTime());
                break;
            }
        }
        //Clear events
        this->context->clearEvents();
        //if ZPulse was requested update everyone
        if(this->context->isZPulseRequested()) { //if ZPulse was requested update everone
            QList<StateMachine*> stateMachineList = this->stateMachineMap.values();
            for(int i=0; i < stateMachineList.size(); i++){
                if(!stateMachineList.at(i)->updateStateMachine(this->context)){
                    //stopabort, stopkill or stopabortflush requested
                    this->stop = true;
                    qDebug("Context: %s", qPrintable(this->context->toString()));
                    qDebug("Ended at %lld", context->getSystemTime());
                    //TODO send something to server
                    break;
                }
            }

            //clear ZPulse Requests
            this->context->clearZPulseRequests();
            //Clear events
            this->context->clearEvents();
        }

        mutex.unlock();
        //sleep to not low the load in the processor
        //sleep(5); // 10ms
        struct timespec ts = { SYSTEM_TICK_MS / 1000, (SYSTEM_TICK_MS % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
    }

    this->exit();
}
