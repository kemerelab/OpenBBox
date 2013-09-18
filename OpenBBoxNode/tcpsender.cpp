#include "tcpsender.h"

TCPSender::TCPSender(char * ip, u_int16_t port) :
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

void TCPSender::sendBehaviorPacket(BehaviorEventPacket packet){
    this->behaviorPackets.enqueue(packet);
    qsem.release();
}

void TCPSender::run(){
    int sockfd;
    struct sockaddr_in remote_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
        exit(1);
    }

    /* Fill the socket address struct */
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &remote_addr.sin_addr);
    bzero(&(remote_addr.sin_zero), 8);
    int opt = true;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
                  (char *)&opt,sizeof(opt));
    fprintf(stdout, "[Client] Waiting new connection with server ...\n");
    /* Try to connect the remote */
    if (::connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
        fflush(stderr);
        exit(1);//TODO
    }
    else {
        printf("[Client] Connected to server at port %d...ok!\n", port);
    }

    while(!stop) {
        qsem.acquire();
            if(!behaviorPackets.isEmpty()) {
                BehaviorEventPacket packet = behaviorPackets.dequeue();\
                if(send(sockfd, (u_int8_t*) &packet, sizeof(BehaviorEventPacket), 0) < 0)
                {
                    fprintf(stderr, "ERROR: Sending command. (errno = %d)\n", errno);
                    if(errno == 104)
                        stop = true;
                }else{
                    fprintf(stdout, "#");
                }
                fflush(stderr);
                fflush(stdout);
            }
    }
}
