#include "sendertasktcp.h"

SenderTaskTCP::SenderTaskTCP(u_int32_t ip, u_int16_t port) :
    QThread()
{
    this->ip = ip;
    this->port = port;
    this->stop = true;
}

void SenderTaskTCP::startServer(uint idtask) {
    this->idtask = idtask;
    this->stop = false;
    start(LowPriority);
}

void SenderTaskTCP::stopServer() {
    this->stop = true;
}

void SenderTaskTCP::setTaskPacket(BehaviorTaskPacket packet){
    this->taskPacket = packet;
}

void SenderTaskTCP::run(){
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

    remote_addr.sin_addr.s_addr = (in_addr_t)ip;
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


    qDebug("Task package size: %d",sizeof(BehaviorTaskPacket));
    if(send(sockfd, (u_int8_t*) &taskPacket, sizeof(BehaviorTaskPacket), 0) < 0)
    {
        qFatal("ERROR: Sending command. (errno = %d)", errno);
        if(errno == 104)
            stop = true;
    }else{
            qDebug("Task packet sent");
    }
}

