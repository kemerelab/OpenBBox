#ifndef SENDERTASKTCP_H
#define SENDERTASKTCP_H

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
#include "streampacket.h"
#include "config.h"

class SenderTaskTCP : public QThread
{
    Q_OBJECT

private:
    bool stop;
    uint idtask;
    u_int32_t ip;
    u_int16_t port;

    BehaviorTaskPacket * taskPacket;
    QSemaphore qsem;

public:
    explicit SenderTaskTCP(u_int32_t ip, u_int16_t port);
    void startServer(uint idtask);
    void stopServer();
    void setTaskPacket(BehaviorTaskPacket *taskPacket);

protected:
    void run();

signals:

public slots:
    //void sendTaskPacket(BehaviorTaskPacket packet);
};

#endif // SENDERTASKTCP_H
