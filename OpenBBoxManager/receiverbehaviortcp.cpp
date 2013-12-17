#include "receiverbehaviortcp.h"

ReceiverBehaviorTCP::ReceiverBehaviorTCP(u_int16_t port) :
    QThread()
{
    this->port = port;
    this->stop = true;
}

void ReceiverBehaviorTCP::startServer(uint idtask){
    this->idtask = idtask;
    this->stop = false;
    start(LowPriority);
}

void ReceiverBehaviorTCP::stopServer(){
    this->stop = true;
    close(nsockfd);
    close(sockfd);
}

void ReceiverBehaviorTCP::addKeySteam(QString key){
        keyStream.push_back(key);
}

QList<QString> ReceiverBehaviorTCP::getKeyString(){
        return keyStream;
}

void ReceiverBehaviorTCP::run(){
    /* Defining Variables */

    socklen_t sin_size;
    struct sockaddr_in addr_local; /* server addr */
    struct sockaddr_in addr_remote; /* server addr */
    uint8_t revbuf[sizeof(BehaviorEventPacket)]; // Receiver buffer
    BehaviorEventPacket packet;

    /* Get the Socket file descriptor */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        qFatal("ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
        this->exit();
    }
    else
         qDebug() << "Obtaining socket descriptor successfully";

    sin_size = sizeof(struct sockaddr_in);

    /* Get the Socket file descriptor */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        qFatal("ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
        this->exit();
    }
    else
        qDebug("Obtaining socket descriptor successfully.\n");

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
        qCritical("ERROR: Failed to bind Port. (errno = %d)\n", errno);
        this->exit();
    }
    else
        qDebug("Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",port);

    /* Listen remote connect/calling */
    if(listen(sockfd, BACKLOG) == -1)
    {
        qCritical("ERROR: Failed to listen Port. (errno = %d)\n", errno);
        this->exit();
    }
    else
        qDebug("Listening the port %d successfully.\n", port);

    /* Wait a connection, and obtain a new socket file despriptor for single connection */
    if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1)
    {
        qFatal("ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
        this->exit();
    }
    else
        qDebug("Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

    while(!stop) {

           bzero(revbuf, sizeof(BehaviorEventPacket));
           int fr_block_sz = 0;
           if((fr_block_sz = recv(nsockfd, (void *)&packet, sizeof(BehaviorEventPacket), 0)) > 0)
           {
               if(fr_block_sz == sizeof(BehaviorEventPacket)) {
                   qCritical("Behavior packet received %d. Event at pin: %d\n", packet.pktBehaviorContext.id, packet.pktBehaviorContext.pin);
                   //TODO Save in some place
                   emit processAddNewEvent(getKeyString(), packet);
                   emit processAddPacketDB( idtask, packet, port, QDateTime::currentDateTime().toTime_t());
               }
           }else{
               qCritical("ERROR: Error receiving command. (errno = %d)\n", errno);
               stop = true;
               //Close connection
           }
    }

    close(nsockfd);
    close(sockfd);
}
