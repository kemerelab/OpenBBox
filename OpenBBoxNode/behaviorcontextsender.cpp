#include "behaviorcontextsender.h"

BehaviorContextSender::BehaviorContextSender(char * ip, u_int16_t port1, u_int16_t port2):
    QThread()
{
    this->ip = ip;
    this->port = port1;
    this->stop = true;
    this->sendstop = false;
    tcpRecevier = new TCPReceiver(port2);
}

void BehaviorContextSender::startSender() {
    this->stop = false;
    start(InheritPriority);
}

void BehaviorContextSender::stopSender() {
    this->stop = true;
}

TCPReceiver * BehaviorContextSender::getTaskReceiver(){
    return tcpRecevier;
}

bool BehaviorContextSender::getstop(){
    return stop;
}

bool BehaviorContextSender::getsendstop(){
    return this->sendstop;
}

void BehaviorContextSender::setsendstop(bool sendstop){
    this->sendstop = sendstop;
}

void BehaviorContextSender::outputResquest(int pin, int value){
    qDebug("Output: pin %d value %d", pin, value);
    GPIO::gpio_set_value(gpioOutputs[pin-1], value);
}

void BehaviorContextSender::run() {
        int cnt = 0;
        struct pollfd fdset[NUM_INPUTS];
        int nfds = NUM_INPUTS;
        int timeout, rc;
        char *buf[MAX_BUF];
        int len;
        timeout = POLL_TIMEOUT;
        memset((void*)fdset, 0, sizeof(fdset));

        int i = 0;
        for(i = 0; i < NUM_OUTPUTS; i++) {
            GPIO::gpio_export(gpioOutputs[i]);
            GPIO::gpio_set_dir(gpioOutputs[i], 1);
        }

        for(i = 0; i < NUM_INPUTS; i++) {
            GPIO::gpio_export(gpioInputs[i]);
            GPIO::gpio_set_dir(gpioInputs[i], 0);
            GPIO::gpio_set_edge(gpioInputs[i], "rising");
            fdset[i].fd = GPIO::gpio_fd_open(gpioInputs[i]);
            fdset[i].events = POLLPRI;
        }

        TCPSender * tcpsender = new TCPSender(ip, port);
        QObject::connect(this, SIGNAL(processSendBehaviorContextPacket(BehaviorEventPacket)),
                                  tcpsender, SLOT(sendBehaviorPacket(BehaviorEventPacket)));

        tcpsender->startSender();

        BehaviorTaskPacket packet = tcpRecevier->getTaskPacket();
        QHash<QString, int> pinname;
        for(i = 0; i < NUM_OUTPUTS; i++){
            pinname.insert(QString(packet.pinconfig+i*20),gpioOutputs[i]);
            qDebug("%s %d", qPrintable( pinname.key(gpioOutputs[i])), gpioOutputs[i]);
        }
        for(i = 0; i < NUM_INPUTS; i++){
            pinname.insert(QString(packet.pinconfig+i*20+8*20),gpioInputs[i]);
            qDebug("%s %d", qPrintable( pinname.key(gpioInputs[i])),gpioInputs[i]);
        }
        MedPCInterpret * interpret = new MedPCInterpret(packet, gpioInputs, gpioOutputs, pinname);

        QObject::connect(this, SIGNAL(processAddNewEvent(int)), interpret, SLOT(addNewEvent(int)));

        QObject::connect(interpret, SIGNAL(outputPin(int,int)), this, SLOT(outputResquest(int,int)));

        interpret->startInterpret();

        //first pool
         rc = poll(fdset, nfds, timeout);
         for(i = 0; i < NUM_INPUTS; i++) {
             if (fdset[i].revents & POLLPRI) {
                 len = read(fdset[i].fd, buf, MAX_BUF);
             }
         }
         struct timeval tv;
         gettimeofday(&tv, NULL);
         long timeStamp_s = 0, timeStamp_us = 0;
         long timelast_s = (long)tv.tv_sec, timelast_us = (long)tv.tv_usec;


         while (!stop && !interpret->getstop()) {
            rc = poll(fdset, nfds, timeout);

            if (rc < 0) {
                qCritical("poll() failed!");
            }

            if (rc == 0) {
                qDebug(".");
            }

            qDebug("poll: %d",rc);
            //send lever push output events
            int output = interpret->getCurrentContext()->getlastOutput();
            qDebug("out %d %d", output, pinname.value("Lever 1"));
            if(output == pinname.value("Lever 1")
                    || output == pinname.value("Lever 2")){

                BehaviorEventPacket packet;
                packet.delimiter = CONTROL_PKT_DELIMITER;
                packet.type = 0;
                packet.version = VERSION;
                packet.pktBehaviorContext.id = cnt;
                packet.pktBehaviorContext.time =
                        (long)interpret->getCurrentContext()->getlastOutputtv().tv_sec;
                packet.pktBehaviorContext.time_usec =
                        (long)interpret->getCurrentContext()->getlastOutputtv().tv_usec;
                packet.pktBehaviorContext.typeEvent = 0;
                packet.pktBehaviorContext.pin = output;
                packet.pktBehaviorContext.pinsContext = 0x00;

                emit processSendBehaviorContextPacket(packet);
                qDebug("New Output %d. Pin: %d",cnt, output);
                cnt++;
                interpret->getCurrentContext()->resetlastOutput();
            }
            //send input events
            gettimeofday(&tv, NULL);
            for(i = 0; i < NUM_INPUTS; i++) {
                if (fdset[i].revents & POLLPRI) {
                    len = read(fdset[i].fd, buf, MAX_BUF);
                    BehaviorEventPacket packet;
                    packet.delimiter = CONTROL_PKT_DELIMITER;
                    packet.type = 0;
                    packet.version = VERSION;
                    packet.pktBehaviorContext.id = cnt;
                    packet.pktBehaviorContext.time = (long)tv.tv_sec;
                    packet.pktBehaviorContext.time_usec = (long)tv.tv_usec;

                    packet.pktBehaviorContext.pin = gpioInputs[i];
                    packet.pktBehaviorContext.pinsContext = 0x00;
                    timeStamp_s = packet.pktBehaviorContext.time - timelast_s;
                    timeStamp_us = packet.pktBehaviorContext.time_usec - timelast_us;
                    long timeStamp = timeStamp_s*1000000 + timeStamp_us;

                    if (output == pinname.value("Reward Pump")){
                        packet.pktBehaviorContext.typeEvent = 1;
                    }else{
                        packet.pktBehaviorContext.typeEvent = 0;
                    }

                    if (timeStamp > dT){
                        if (gpioInputs[i] != pinname.value("Head Detector")
                           || interpret->getCurrentContext()->getlastInput() == pinname.value("Lever 1 press")
                           || interpret->getCurrentContext()->getlastInput() == pinname.value("Lever 2 press"))
                        {
                            emit processSendBehaviorContextPacket(packet); //tell server
                        }
                        emit processAddNewEvent(i+1); //tell client it self
                        qDebug("New event %d. Pin: %d",cnt, gpioInputs[i]);
                        cnt++;
                        timelast_s = packet.pktBehaviorContext.time;
                        timelast_us = packet.pktBehaviorContext.time_usec;
                    }
                    break;
                }

            }

         }

        interpret->stopInterpret();
        tcpsender->stopSender();
        sendstop = true;
        stop = true;

        this->exit(); // connect with main app
}
