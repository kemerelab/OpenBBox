#include "receiverbehaviortcp.h"

ReceiverBehaviorTCP::ReceiverBehaviorTCP(u_int16_t port) :
    QThread()
{
    this->port = port;
    this->stop = true;
}

void ReceiverBehaviorTCP::startServer(uint idtask, bool test){
    this->idtask = idtask;
    this->stop = false;
    this->test = test;
    start(LowPriority);
}

void ReceiverBehaviorTCP::stopServer(){
    this->stop = true;
}

void ReceiverBehaviorTCP::setKeySteam(QString key){
        keyStream = key;
}

QString ReceiverBehaviorTCP::getKeyStream(){
        return keyStream;
}

bool ReceiverBehaviorTCP::getstop(){
    return this->stop;
}

void ReceiverBehaviorTCP::run(){

    socklen_t sin_size;
    struct sockaddr_in addr_local; /* server addr */
    struct sockaddr_in addr_remote; /* server addr */
    BehaviorEventPacket packet;

    /* Get the Socket file descriptor */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        qFatal("ERROR: Failed to obtain Socket Descriptor. (errno = %d)", errno);
        this->exit();
    }
    else
         qDebug() << "Obtaining socket descriptor successfully";

    sin_size = sizeof(struct sockaddr_in);

    /* Fill the client socket address struct */
    addr_local.sin_family = AF_INET; // Protocol Family
    addr_local.sin_port = htons(port); // Port number
    addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
    bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct
    int opt = true;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                  (char *)&opt,sizeof(opt));
    /* Bind a special Port */
    if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
    {
        qCritical("ERROR: Failed to bind Port. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Binded tcp port %d in addr %s sucessfully.",port, "sss");

    /* Listen remote connect/calling */
    if(listen(sockfd, BACKLOG) == -1)
    {
        qCritical("ERROR: Failed to listen Port. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Listening the port %d successfully.", port);

    /* Wait a connection, and obtain a new socket file despriptor for single connection */
    if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1)
    {
        qFatal("ERROR: Obtaining new Socket Despcritor. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Server has got connected from %s.", inet_ntoa(addr_remote.sin_addr));

    while(!stop) {
        int fr_block_sz = 0;
        if((fr_block_sz = recv(nsockfd, (void *)&packet, sizeof(BehaviorEventPacket), 0)) > 0)
        {
            if(fr_block_sz == sizeof(BehaviorEventPacket)) {
                if(!test){
                    qCritical("Behavior packet received %d. Event at pin: %d", packet.pktBehaviorContext.id, packet.pktBehaviorContext.pin);
                    emit processAddNewEvent(this->getKeyStream(), packet);
                    emit processAddPacketDB(this->getKeyStream(), idtask, packet, port, QDateTime::currentDateTime().toTime_t());
                    qDebug("behaviortask id: %d", idtask);
                    if(packet.pktBehaviorContext.pin == 0 && packet.pktBehaviorContext.typeEvent == 1){
                        stop = true;
                    }

                }else{
                    qCritical("Input test received. Event at pin: %d", packet.pktBehaviorContext.pin);
                    emit processInputEvent(packet.pktBehaviorContext.pin);
                }
            }
        }else{
            qCritical("ERROR: Error receiving command. (errno = %d)", errno);
            stop = true;
        }
    }
    qDebug("Behavior Receiver ended");
    close(nsockfd);
    close(sockfd);
}
