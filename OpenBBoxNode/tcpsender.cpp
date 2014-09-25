#include "tcpsender.h"

TCPSender::TCPSender(QString ip, u_int16_t port) :
    QThread()
{
    this->ip = ip;
    this->port = port;
    this->stop = true;
}

void TCPSender::startSender() {
    this->stop = false;
    start(LowPriority);
}

void TCPSender::stopSender() {
    this->stop = true;
}

QSemaphore * TCPSender::getQsemaphore(){
    return &qsem;
}


void TCPSender::sendBehaviorPacket(BehaviorEventPacket packet){
    this->behaviorPackets.enqueue(packet);
    qsem.release();
}

void TCPSender::run(){
    int sockfd;
    struct sockaddr_in remote_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        qFatal("Behavior socket failed to obtain Socket Descriptor! (errno = %d)",errno);
        exit(1);
    }

    /* Fill the socket address struct */
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
    inet_pton(AF_INET, qPrintable(ip), &remote_addr.sin_addr);
    bzero(&(remote_addr.sin_zero), 8);
    int opt = true;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
                  (char *)&opt,sizeof(opt));

    qDebug("Waiting new connection with server...");
    /* Try to connect the remote */
    if (::connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        qFatal("Behavior socket failed to connect to the host (errno = %d)",errno);
        exit(1);//TODO
    }
    else {
        qDebug("Connected to server at port %d...ok!", port);
    }

    while(!stop) {
        qsem.acquire();
        if(!behaviorPackets.isEmpty()) {
            BehaviorEventPacket packet = behaviorPackets.dequeue();\
            if(send(sockfd, (u_int8_t*) &packet, sizeof(BehaviorEventPacket), 0) < 0)
            {
                qFatal("ERROR: Sending command. (errno = %d)", errno);
                if(errno == 104)
                    stop = true;
            }else{
                qDebug("Behavior packet %d sent", packet.pktBehaviorContext.id);
            }
        }

    }
    qDebug("TCP sender ended");
}
