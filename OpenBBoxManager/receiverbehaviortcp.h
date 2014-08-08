#ifndef RECEIVERBEHAVIORTCP_H
#define RECEIVERBEHAVIORTCP_H

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
#include "streampacket.h"
#include "config.h"

class ReceiverBehaviorTCP : public QThread
{
    Q_OBJECT

private:
    uint idtask;
    int sockfd;
    int nsockfd;
    u_int16_t port;
    bool stop;
    QString keyStream;

public:
    explicit ReceiverBehaviorTCP(u_int16_t port);
    void startServer(uint idtask);
    void stopServer();
    void setKeySteam(QString key);
    QString getKeyStream();
    bool getstop();

protected:
    void run();

signals:
    void processAddNewEvent(QString key, BehaviorEventPacket packet);
    void processAddPacketDB(uint idtask, BehaviorEventPacket packet, uint port, long time);
public slots:
    
};

#endif // RECEIVERBEHAVIORTCP_H
