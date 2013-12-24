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
    bool stop;
    int sockfd;
    int nsockfd;
    uint idtask;
    u_int16_t port;

    QList<QString> keyStream;

public:
    explicit ReceiverBehaviorTCP(u_int16_t port);
    void startServer(uint idtask);
    void stopServer();
    void addKeySteam(QString key);
    QList<QString> getKeyString();

protected:
    void run();

signals:
    void processAddNewEvent(QList<QString> key, BehaviorEventPacket packet);
    void processAddPacketDB(uint idtask, BehaviorEventPacket packet, uint port, long time);

public slots:
    
};

#endif // RECEIVERBEHAVIORTCP_H
