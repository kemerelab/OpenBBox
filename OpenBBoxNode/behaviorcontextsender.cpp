#include "behaviorcontextsender.h"

BehaviorContextSender::BehaviorContextSender(char * ip, u_int16_t port1, u_int16_t port2):
    QThread()
{
    this->ip = ip;
    this->port = port1;
    this->stop = true;
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
            GPIO::gpio_set_value(gpioOutputs[i], 0);
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

        BehaviorTaskPacket taskpacket = tcpRecevier->getTaskPacket();

        MedPCInterpret * interpret = new MedPCInterpret(taskpacket, gpioInputs, gpioOutputs);

        QObject::connect(this, SIGNAL(processAddNewEvent(int)), interpret, SLOT(addNewEvent(int)));

        QObject::connect(interpret, SIGNAL(outputPin(int,int)), this, SLOT(outputResquest(int,int)));

        interpret->startInterpret();

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

        //send task start event
        BehaviorEventPacket packet;
        packet.delimiter = CONTROL_PKT_DELIMITER;
        packet.type = 0;
        packet.version = VERSION;
        packet.pktBehaviorContext.id = cnt;

        packet.pktBehaviorContext.pin = 0;
        packet.pktBehaviorContext.pinsContext = 0x00;
        qDebug("Start Event");
        packet.pktBehaviorContext.typeEvent = 0;
        interpret->waitStartTime();
        packet.pktBehaviorContext.time =(long)interpret->getTime().tv_sec;
        packet.pktBehaviorContext.time_usec =(long)interpret->getTime().tv_usec;
        emit processSendBehaviorContextPacket(packet);
        cnt++;

        while (!stop&&!interpret->getstop()) {
            rc = poll(fdset, nfds, 0);
            if (rc < 0) {
                qCritical("poll() failed!");
            }
            //send output events
            int output = interpret->getCurrentContext()->getlastOutput();
            if(interpret->getCurrentContext()->getPinLever()->contains(output)
                    ||interpret->getCurrentContext()->getPinReward()->contains(output))
            {
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

                    if (interpret->getCurrentContext()->getConstants()->values("Reward").contains(output)){
                        packet.pktBehaviorContext.typeEvent = 1;
                    }else{
                        packet.pktBehaviorContext.typeEvent = 0;
                    }

                    if (timeStamp > dT){
                        emit processSendBehaviorContextPacket(packet); //tell server
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

        //send task end event
        packet.delimiter = CONTROL_PKT_DELIMITER;
        packet.type = 0;
        packet.version = VERSION;
        packet.pktBehaviorContext.id = cnt;
        packet.pktBehaviorContext.pin = 0;
        packet.pktBehaviorContext.pinsContext = 0x00;
        if(interpret->getstop()){
            packet.pktBehaviorContext.typeEvent = 1;
        }else{
            packet.pktBehaviorContext.typeEvent = 2;
            interpret->stopInterpret();
        }
        packet.pktBehaviorContext.time =
                (long)interpret->getTime().tv_sec;
        packet.pktBehaviorContext.time_usec =
                (long)interpret->getTime().tv_usec;
        emit processSendBehaviorContextPacket(packet);

        tcpsender->stopSender();
        tcpsender->getQsemaphore()->release();

        for(i = 0; i < NUM_OUTPUTS; i++) {
            GPIO::gpio_export(gpioOutputs[i]);
            GPIO::gpio_set_dir(gpioOutputs[i], 1);
            GPIO::gpio_set_value(gpioOutputs[i], 0);
        }
        qDebug("behavior sender ended");
        this->exit(); // connect with main app
}
