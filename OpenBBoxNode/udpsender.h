#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QThread>
#include <QQueue>
#include <QSemaphore>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "camerasender.h"
#include "../OpenBBoxManager/streampacket.h"

class UDPSender : public QThread
{
    Q_OBJECT
private:

    QQueue<Image *> packetsFrameQueue;
    QSemaphore qsem;

    bool stop;
    char* ipaddress;
    uint8_t type;
    //socket
    int n;
    struct sockaddr_in servaddr,cliaddr;
    int sockfd;
    int cnt;
    uint16_t lenghtBuffer;
    uint16_t udpport;
    uint32_t bytesSent;

public:
    explicit UDPSender(char* ipaddress, uint16_t port, uint8_t format);
    void startSender();
    void stopSender();

protected:
    void run();

signals:
    
public slots:
    void sendFrame(uchar * buffer, uint size, uint timeSec, uint timeUSec, uint  width, uint  height, uint format);
};

#endif // UDPSENDER_H
