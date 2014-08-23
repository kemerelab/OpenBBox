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
        MedPCInterpret * interpret = new MedPCInterpret(packet, gpioInputs, gpioOutputs);

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
         while (!stop) {
            rc = poll(fdset, nfds, timeout);

            if (rc < 0) {
                qCritical("poll() failed!");
            }

            if (rc == 0) {
                qDebug(".");
            }

            qDebug("poll: %d",rc);
            int output = interpret->getCurrentContext()->getlastOutput();
            if(output == 31 || output == 48){

                BehaviorEventPacket packet;
                packet.delimiter = CONTROL_PKT_DELIMITER;
                packet.type = 0;
                packet.version = VERSION;
                packet.pktBehaviorContext.id = cnt;
                packet.pktBehaviorContext.time = (long)interpret->getCurrentContext()->getlastOutputtv().tv_sec;
                packet.pktBehaviorContext.time_usec = (long)interpret->getCurrentContext()->getlastOutputtv().tv_usec;
                packet.pktBehaviorContext.typeEvent = 1;
                packet.pktBehaviorContext.pin = output;
                packet.pktBehaviorContext.pinsContext = 0x00;

                emit processSendBehaviorContextPacket(packet);
                qDebug("New Output %d. Pin: %d",cnt, output);
                cnt++;
                interpret->getCurrentContext()->resetlastOutput();
            }
            gettimeofday(&tv, NULL);
            for(i = 0; i < NUM_INPUTS-5; i++) {
                if (fdset[i].revents & POLLPRI) {
                    len = read(fdset[i].fd, buf, MAX_BUF);
                    BehaviorEventPacket packet;
                    packet.delimiter = CONTROL_PKT_DELIMITER;
                    packet.type = 0;
                    packet.version = VERSION;
                    packet.pktBehaviorContext.id = cnt;
                    packet.pktBehaviorContext.time = (long)tv.tv_sec;
                    packet.pktBehaviorContext.time_usec = (long)tv.tv_usec;
                    packet.pktBehaviorContext.typeEvent = 0;
                    packet.pktBehaviorContext.pin = gpioInputs[i];
                    packet.pktBehaviorContext.pinsContext = 0x00;
                    timeStamp_s = packet.pktBehaviorContext.time - timelast_s;
                    timeStamp_us = packet.pktBehaviorContext.time_usec - timelast_us;
                    long timeStamp = timeStamp_s*1000000 + timeStamp_us;

                    if (timeStamp>dT){
                        emit processSendBehaviorContextPacket(packet);
                        emit processAddNewEvent(i+1);
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
        this->exit(); // connect with main app
}
