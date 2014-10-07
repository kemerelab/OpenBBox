#ifndef BEHAVIORCONTEXTSENDER_H
#define BEHAVIORCONTEXTSENDER_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/time.h>
#include "tcpsender.h"
#include "tcpreceiver.h"
#include "gpio.h"
#include "../OpenBBoxManager/config.h"

#include "MedPCInterpret/medpcinterpret.h"


const uint gpioInputs[NUM_INPUTS] = INPUTS;
const uint gpioOutputs[NUM_OUTPUTS] = OUTPUTS;

class BehaviorContextSender : public QThread
{
    Q_OBJECT
private:
    bool stop;
    bool test;
    char * ip;
    u_int16_t port;
    TCPReceiver * tcpRecevier;

public:
    BehaviorContextSender(char * ip, u_int16_t port1, u_int16_t port2);

    void startSender(bool test);
    void stopSender();
    TCPReceiver * getTaskReceiver();
    void resetTaskReceiver();
    bool getstop();

protected:
    void run();

signals:
    void processSendBehaviorContextPacket(BehaviorEventPacket packet);
    void processAddNewEvent(int pin);
public slots:
    void outputResquest(int pin, int value);
};

#endif // BEHAVIORCONTEXTSENDER_H
