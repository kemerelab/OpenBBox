#include "behaviorcontextsender.h"

BehaviorContextSender::BehaviorContextSender(char * ip, u_int16_t port):
    QThread()
{
    this->ip = ip;
    this->port = port;
    this->stop = true;
}

void BehaviorContextSender::startSender() {
    this->stop = false;
    start(InheritPriority);
}

void BehaviorContextSender::stopSender() {
    this->stop = true;
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
        //Phase3_LeverPress_50Reward_20min2
        MedPCInterpret * interpret = new MedPCInterpret("Phase3_LeverPress_50Reward_20min2.MPC", gpioInputs, gpioOutputs);
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

         while (!stop) {
            rc = poll(fdset, nfds, timeout);

            if (rc < 0) {
                qCritical("poll() failed!");
            }

            if (rc == 0) {
                qDebug(".");
            }

            struct timeval tv;
            gettimeofday(&tv, NULL);
            long timeStamp = (long)time(NULL);
            for(i = 0; i < NUM_INPUTS; i++) {
                if (fdset[i].revents & POLLPRI) {
                    len = read(fdset[i].fd, buf, MAX_BUF);
                    //TODO Change that
                    BehaviorEventPacket packet;
                    packet.delimiter = CONTROL_PKT_DELIMITER;
                    packet.type = 0;
                    packet.version = VERSION;
                    packet.pktBehaviorContext.id = cnt;
                    packet.pktBehaviorContext.time = timeStamp;
                    packet.pktBehaviorContext.time_usec = (long)tv.tv_usec;
                    packet.pktBehaviorContext.typeEvent = 0;
                    packet.pktBehaviorContext.pin = gpioInputs[i];
                    packet.pktBehaviorContext.pinsContext = 0x00;

                    emit processSendBehaviorContextPacket(packet);
                    emit processAddNewEvent(i+1);

                    qDebug("New event %d. Pin: %d",cnt, gpioInputs[i]);
                    cnt++;
                }
            }
        }

        interpret->stopInterpret();
        this->exit(); // connect with main app
}
