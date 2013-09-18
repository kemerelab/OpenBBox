#include "medpcinterpret.h"

void MedPCInterpret::abortStopRequest(){
    stop = true;
    emit abortStop();
}

void MedPCInterpret::outputRequest(int pin, int value) {
    printf("Output: pin %d value %d\n", pin, value);
    fflush(stdout);
    emit outputPin(pin, value);
}

void MedPCInterpret::parseStateToEvents(QHash<QString, Event*> * eventMaps, QString state, QList<QString> lines){

    QList<QString> sublines;
    for(int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        if(line.contains(":") && !line.contains("@")) {
            sublines.push_back(line);
            eventMaps->insert(state.remove('\t'), new Event(state, sublines));
            sublines.clear();
        }else{
            sublines.push_back(line);
        }
    }

    if(sublines.size() > 0) { 
        eventMaps->insert(state.remove('\t'), new Event(state, sublines));
    }
}

MedPCInterpret::MedPCInterpret(QString filename,  const uint * gpioInputs, const uint * gpioOutputs) :
    QThread()
{
    this->gpios = gpios;

    bool startedStatesMachines = false;

    QFile inputFile(filename);

    QString keyStateMachine;
    QString keyState;
    QList<QString> lines;

    this->context = new Context(gpioInputs, gpioOutputs);

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )
        {
          QString line = in.readLine();
            //printf("%s\n", line.toAscii().data());
            //printf("\n");
            //exclude spaces
          line = line.replace(" ", "");
          line = line.replace("\t", "");
          line = line.replace("\n", "");
          line = line.replace("\r", "");
          line = line.toUpper(); // remove spaces, tabs and put to upper

          if(line.size() > 0){ // be sure
              if(line.at(0) != '\\') { //remove easy comments
                line = line.section('\\', 0, 0);
                        if(line.at(0) == (CONST_DELIMITER)){
                            context->getConstants()->insert(line.section('=',0,0), line.section('=',1,1).toInt());
                            printf("%s: %d\n", line.section('=',0,0).toAscii().data(), line.section('=',1,1).toInt());
                        }

                        if(line.contains(DISKVARS_DELIMITER)){
                           line = line.replace(DISKVARS_DELIMITER,"");
                           line = line.replace("=","");
                           QStringList list=  line.split(",");
                           for (int i = 0; i < list.size(); ++i)
                               if(list.at(i) != "" && !list.at(i).isEmpty()){
                                   context->getVariables()->insert(list.at(i), 0);
                                   printf("%s: %d\n", list.at(i).toAscii().data(), 0);
                               }
                        }

                        if(line.contains(ARRAY_DELIMITER)){
                            line.replace(ARRAY_DELIMITER,"");
                            context->getArrays()->insert(line.section('=',0,0), new QVector<float>(line.section('=',1,1).toInt()));
                            context->getArrays()->value(line.section('=',0,0))->fill(0);
                            printf("%s: %d\n", line.section('=',0,0).toAscii().data(), line.section('=',1,1).toInt());
                        }

                        if(line.contains(LIST_DELIMITER)){
                            line.replace(LIST_DELIMITER,"");
                            QString key = line.section('=',0,0);
                            context->getArrays()->insert(key, new QVector<float>());
                            line = line.section('=',1,1);
                            int i=0;
                            for(i = 0; i < line.count(',') + 1; i++){
                                context->getArrays()->value(key)->push_back(line.section(',',i,i).toFloat());
                                printf("%s: %f\n", key.toAscii().data(),line.section(',',i,i).toFloat());
                            }
                        }

                        if(line.contains(ZPULSE_DELIMITER)){
                            line = line.section(":",0,0);
                            line = line.section(ZPULSE_DELIMITER, 1, 1);
                            if(!context->getZPulses()->contains(line.toInt())){
                                context->getZPulses()->insert(line.toInt(), false);
                                printf("ZPulse: %s\n", line.toAscii().data());
                            }
                        }
                   }
            }
        }

        in.seek(0);
        startedStatesMachines = false;

        while ( !in.atEnd() )
        {
         QString line = in.readLine();
              //exclude spaces
              //line.simplified();
             line = line.replace(" ", "");
             line = line.replace("\t", "");
             line = line.replace("\n", "");
             line = line.replace("\r", "");
             line = line.toUpper();
              if(line.size() > 0){ // be sure
                  if(line.at(0) != '\\') { //remove easy comments
                    line = line.section('\\', 0, 0);
                    //printf(line.toAscii());
                    //printf("\n");

                    if(!startedStatesMachines){ // if state machines declaration not started
                        if(line.contains("S.S.")){
                            startedStatesMachines = true;
                            keyStateMachine = line.replace(",","");
                        }
                    }else{// if state machines declaration started
                          if(line.contains("S.S.")){
                              if(lines.size() > 0)
                                  parseStateToEvents( &eventsMap, keyState, lines);
                              lines.clear();

                              stateMachineMap.insert(keyStateMachine, new StateMachine(keyStateMachine, eventsMap));
                              eventsMap.clear();
                              keyStateMachine = line.replace(",","");
                          }else if(line.at(0) == 'S' && (line.at(2) == ',' || line.at(3) == ',')){
                              if(lines.size() > 0) {
                                  parseStateToEvents( &eventsMap, keyState, lines);
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
              parseStateToEvents( &eventsMap, keyState, lines);
              stateMachineMap.insert(keyStateMachine, new StateMachine(keyStateMachine, eventsMap));
              eventsMap.clear();
              lines.clear();
        }
    }

    inputFile.close();
    this->stop = true;

    /*printf("%f\n",context->getValue("1"));
    printf("%f\n",context->getValue("1+2+3+5"));
    printf("%f\n",context->getValue("1+3"));
    printf("%f\n",context->getValue("4-2"));
    printf("%f\n",context->getValue("2*2"));
    printf("%f\n",context->getValue("6/2"));
    printf("%f\n",context->getValue("F(0)"));
    printf("%f\n",context->getValue("D(^l)"));
    printf("%f\n",context->getValue("F(^l+1)*2"));
    printf("%f\n",context->getValue("F(^l+1)*2*2"));
    printf("%f\n",context->getValue("D(F(^l+1)/2)"));
    printf("\n");

    context->executeCommand("ADD X");
    context->executeCommand("ADD X,Y");
    context->executeCommand("ADD U(Y+1)");
    context->executeCommand("SET U(0) = 1, U(1) = 2, U(2) = 3");
    context->executeCommand("RANDI P = F");
    context->executeCommand("LIST B = U(Y+1)");
    context->executeCommand("ARITHMETICMEAN C = U, 0, 2");

    printf("C: %f\n",context->getValue("C"));
    printf("X: %f\n",context->getValue("X"));
    printf("Y: %f\n",context->getValue("Y"));
    printf("P: %f\n",context->getValue("P"));
    printf("%f\n",context->getValue("B"));
    printf("%f\n",context->getValue("U(2)"));*/
    fflush(stdout);
}

void MedPCInterpret::startInterpret(){
    stop = false;
    start(NormalPriority);
}

void MedPCInterpret::stopInterpret(){
    stop = true;
}

void MedPCInterpret::addNewEvent(int pin) {
    printf("Input: pin %d\n", pin);
    fflush(stdout);
    mutex.lock();
      this->context->addEventPort(pin);
    mutex.unlock();
}

#define SYSTEM_TICK_MS 10

void MedPCInterpret::run() {

    //update time
    this->context->setSystemTime(this->context->getCurrentTimeSystem());
    printf("Started at %ld\n", context->getSystemTime());

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
                printf("\n\nContext: %s\n\n", this->context->toString().toAscii().data());
                printf("Ended at %ld\n", context->getSystemTime());
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
                    printf("\n\nContext: %s\n\n", this->context->toString().toAscii().data());
                    printf("Ended at %ld\n", context->getSystemTime());
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
        fflush(stderr);
        fflush(stdout);

        //sleep(5); // 10ms
        struct timespec ts = { SYSTEM_TICK_MS / 1000, (SYSTEM_TICK_MS % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
    }

    this->exit();
}
