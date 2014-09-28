#include "tcpreceiver.h"
#include <iostream>
#include <unistd.h>
#include "MedPCInterpret/medpcinterpret.h"
#include "behaviorcontextsender.h"

TCPReceiver::TCPReceiver(u_int16_t port) :
    QThread()
{
    this->port = port;
    for(int i = 0; i < NUM_OUTPUTS; i++){
        this->dir.insert(gpioOutputs[i], true);
    }
}

void TCPReceiver::startReceiver(bool test) {
    this->stop = false;
    start(LowPriority);
    this->test = test;
}

void TCPReceiver::stopReceiver() {
    this->stop = true;
    this->test = NULL;
}

BehaviorTaskPacket TCPReceiver::getTaskPacket(){ //during train
    qsem_Task.acquire();
    return taskpacket;
}
void TCPReceiver::waitConnectAck(){
    qsem_ack.acquire();
}

void TCPReceiver::run(){

    /* Defining Variables */

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
        qDebug("Obtaining socket descriptor successfully");

    sin_size = sizeof(struct sockaddr_in);

    /* Fill the client socket address struct */
    addr_local.sin_family = AF_INET; // Protocol Family
    addr_local.sin_port = htons(port); // Port number
    addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
    bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct
    int opt = true;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    /* Bind a special Port */
    if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
    {
        qCritical("ERROR: Failed to bind Port. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Binded tcp port %d in addr 127.0.0.1 sucessfully.",port);

    /* Listen remote connect/calling */
    if(listen(sockfd, BACKLOG) == -1)
    {
        qCritical("ERROR: Failed to listen Port. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Listening the port %d successfully.", port);

    qsem_ack.release();

    /* Wait a connection, and obtain a new socket file despriptor for single connection */
    if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1)
    {
        qFatal("ERROR: Obtaining new Socket Despcritor. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Server has got connected from %s.", inet_ntoa(addr_remote.sin_addr));

    if(!test){
        int fr_block_sz = 0;
        if((fr_block_sz = recv(nsockfd, (void *)&taskpacket, sizeof(BehaviorTaskPacket), 0)) > 0)
        {
            if(fr_block_sz == sizeof(BehaviorTaskPacket)) {
                qCritical("Task MPC file received: size %d",fr_block_sz);
                qsem_Task.release();
            }
        }else{
            qCritical("ERROR: Error receiving command. (errno = %d)", errno);
        }
    }else{
        qDebug("Test Mode started");
        for(int i = 0; i < NUM_OUTPUTS; i++) {
            GPIO::gpio_export(gpioOutputs[i]);
            GPIO::gpio_set_dir(gpioOutputs[i], 1);
            GPIO::gpio_set_value(gpioOutputs[i], 0);
        }
        for(int i = 0; i < NUM_INPUTS; i++) {
            GPIO::gpio_export(gpioInputs[i]);
            GPIO::gpio_set_dir(gpioInputs[i], 0);
            GPIO::gpio_set_edge(gpioInputs[i], "rising");
        }
        while(!stop) {
            int fr_block_sz = 0;
            if((fr_block_sz = recv(nsockfd, (void *)&packet, sizeof(BehaviorEventPacket), 0)) > 0)
            {
                if(fr_block_sz == sizeof(BehaviorEventPacket)) {
                    if(packet.pktBehaviorContext.pin != 0){
                        qCritical("Test packet received %d. Event at pin: %d", packet.pktBehaviorContext.id, packet.pktBehaviorContext.pin);
                        if(dir.value(packet.pktBehaviorContext.pin)){
                            GPIO::gpio_set_value(packet.pktBehaviorContext.pin, 1);
                        }else{
                            GPIO::gpio_set_value(packet.pktBehaviorContext.pin, 0);
                        }
                        dir.insert(packet.pktBehaviorContext.pin,!dir.value(packet.pktBehaviorContext.pin));
                    }else{
                        stop = true;
                        qDebug("Test Mode ended");
                    }
                }
            }else{
                qCritical("ERROR: Error receiving command. (errno = %d)", errno);
                stop = true;
            }
        }
    }
    qDebug("TCP receiver ended");
    close(nsockfd);
    close(sockfd);
}
