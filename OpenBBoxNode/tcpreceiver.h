#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H

#include <QSemaphore>
#include <QThread>
#include <QObject>
#include <QList>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <QDateTime>
#include "../OpenBBoxManager/config.h"
#include "../OpenBBoxManager/streampacket.h"


class TCPReceiver : public QThread
{
    Q_OBJECT
private:
    uint idtask;
    bool test;
    bool stop;
    bool dir;
    int sockfd;
    int nsockfd;
    u_int16_t port;
    QSemaphore qsem_Task;
    QSemaphore qsem_ack;
    QString TaskFile;
    BehaviorTaskPacket taskpacket;
public:
    explicit TCPReceiver(u_int16_t port);
    void startReceiver(bool test);
    void stopReceiver();
    BehaviorTaskPacket getTaskPacket();
    void waitConnectAck();


protected:
    void run();

signals:

public slots:

};
#endif // TCPRECEIVER_H
