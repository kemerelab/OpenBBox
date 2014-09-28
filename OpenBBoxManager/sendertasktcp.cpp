#include "sendertasktcp.h"
#include "mainwindow.h"
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

void SenderTaskTCP::setTaskPacket(BehaviorTaskPacket * packet){
    this->taskPacket = packet;
}

void SenderTaskTCP::sendTestPinPacket(int pin){
    BehaviorEventPacket packet;
    packet.delimiter = CONTROL_PKT_DELIMITER;
    packet.type = 0;
    packet.version = VERSION;
    packet.pktBehaviorContext.id = 0x00;
    packet.pktBehaviorContext.time = 0x00;
    packet.pktBehaviorContext.time_usec = 0x00;
    if(pin != 0){
        packet.pktBehaviorContext.pin = Output[pin-1];
    }else{
        packet.pktBehaviorContext.pin = 0;
    }
    packet.pktBehaviorContext.pinsContext = 0x00;
    packet.pktBehaviorContext.typeEvent = 0x00;
    this->behaviorPackets.enqueue(packet);
    qsem.release();
}

void SenderTaskTCP::setTestModel(bool test){
    this->test = test;
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

    if(!test){
        if(send(sockfd, (u_int8_t*) taskPacket, sizeof(BehaviorTaskPacket), 0) < 0)
        {
            qFatal("ERROR: Sending command. (errno = %d)", errno);
            if(errno == 104)
                stop = true;
        }else{
            qDebug("Task packet sent");
        }
    }else{
        qDebug("Test Mode Started");
        while(!stop){
            qsem.acquire();
            if(!behaviorPackets.isEmpty()){
                BehaviorEventPacket packet = behaviorPackets.dequeue();\
                if(send(sockfd, (u_int8_t*) &packet, sizeof(BehaviorEventPacket), 0) < 0)
                {
                    qFatal("ERROR: Sending command. (errno = %d)", errno);
                    if(errno == 104)
                        stop = true;
                }else{
                    qDebug("Test Behavior packet sent");
                }
                if(packet.pktBehaviorContext.pin == 0){
                    stop = true;
                    qDebug("Test Mode Ended");
                }
            }
        }
    }
}
