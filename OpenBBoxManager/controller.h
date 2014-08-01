#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <QObject>
#include <QHash>
#include <QMutex>
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
#include "consoleserverpacket.h"
#include "obbnode.h"
#include "config.h"
#include "sendertasktcp.h"

#define CHECK_CONN_TIME 5*1000

class Controller : public QThread
{
    Q_OBJECT

signals:
     void processAddNodeList(OBBNode * node);
     void processRemoveNodeList(OBBNode * node);
private:
     void addNewNode(uint * portVideo, uint portBehavior, uint portBTask, char * label , uchar * mac, uint ipAddress, uint portConsole);
     bool stop;
     int  portOffset;
     bool startOBBNodeStreams(OBBNode * node);

     QList<OBBNode *>      obbnodeList;
     QHash<int, int>       portList;

     int allocPort(int portController);
     void freePort(int portController);

     QTimer *timer;
     QMutex mutex;
public:
    explicit Controller();
    ~Controller();
    bool sendCommand(int socket, PktCommand * pktCommand);
    void startService();
    bool processNewNode( struct sockaddr_in addr_remote, int nsockfd);
    bool startOBBNodeTask(OBBNode * node, BehaviorTaskPacket packet);
    bool sendTask(OBBNode * node, QByteArray * file);

protected:
    void run();

public slots:
    void checkConnection();
    
};

#endif // CONTROLLER_H
