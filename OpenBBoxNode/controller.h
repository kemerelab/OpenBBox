#ifndef CONTROLLER_H
#define CONTROLLER_H

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
#include <QThread>
#include <QTcpSocket>
#include <../OpenBBoxManager/dao/openbboxnodeobject.h>
#include "../OpenBBoxManager/consoleserverpacket.h"
#include <QNetworkInterface>
#include "camerasender.h"
#include "behaviorcontextsender.h"

#define PORT 20000
#define LENGTH 512

#define STATE_WAITING_CONNETION		0
#define STATE_WAITING_REQUESTINFO	1
#define STATE_WAITING_SET_PORTS		2
#define STATE_WAITING_COMMANDS		3

#define SERVER_IPADDRESS "192.168.7.1"

class Controller : public QThread
{
    Q_OBJECT

private:
    int state;
    bool stop;
    int numCameras;
    bool processCommand(int socket, PktCommand * pktCommand);
    uint16_t portsCamera[MAX_CAMERAS];
    CameraSender * cameras[MAX_CAMERAS];
    BehaviorContextSender * behaviorContextSender;
    bool stopAllStreams();

public:
    explicit Controller();
    void startNode();
    void stopNode();

protected:
    void run();

signals:

public slots:

};

#endif // CONTROLLER_H
