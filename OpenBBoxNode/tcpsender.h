#ifndef TCPSENDER_H
#define TCPSENDER_H

#include <QThread>
#include <QQueue>
#include <QSemaphore>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include "../OpenBBoxManager/streampacket.h"

const int sizeBehaviorPacket[] = {SIZE_CONTROL_HEADER + sizeof(BehaviorEventContext)/sizeof(u_int8_t),
                                  SIZE_CONTROL_HEADER + sizeof(BehaviorEventShow)/sizeof(u_int8_t)};

class TCPSender : public QThread
{
    Q_OBJECT
private:
    bool stop;
    char * ip;
    u_int16_t port;

    QQueue<BehaviorEventPacket> behaviorPackets;
    QSemaphore qsem;

public:
    explicit TCPSender(char * ip, u_int16_t port);
    void startSender();
    void stopSender();

protected:
    void run();

signals:

public slots:
   void sendBehaviorPacket(BehaviorEventPacket packet);
};

#endif // TCPSENDER_H
