#include "controller.h"
#include <QFileInfo>
Controller::Controller() :
    QThread()
{
    this->stop = false;
    this->state = STATE_WAITING_CONNETION;
    this->numCameras = 0;

    for(int i = 0; i < MAX_CAMERAS; i++) {
        QString video = "/dev/video"+QString::number(i);
        QFileInfo file(video);
        if(file.exists()){
            this->numCameras++;
        }
    }

    printf("Found %d cameras\n", this->numCameras);

    //this->behaviorContextSender = NULL;
}

void parseCommandToBuffer(PktCommand * pktCommand, uint8_t * bufferRet){

    memcpy(bufferRet, (uint8_t*)pktCommand, SIZE_COMMAND_HEADER);
    memcpy(bufferRet + SIZE_COMMAND_HEADER, (uint8_t*)&pktCommand->pktCommands, sizeCommands[pktCommand->type]);
}

void parseBufferToCommand(uint8_t * buffer, PktCommand * pktCommandRet){

    memcpy(pktCommandRet, buffer, SIZE_COMMAND_HEADER);
    memcpy(&pktCommandRet->pktCommands, buffer + SIZE_COMMAND_HEADER, sizeCommands[pktCommandRet->type]);
}

bool waitingForCommand(int socket, PktCommand * pktCommand) {
  uint8_t revbuf[sizeof(PktCommand)]; // Receiver buffer

  int typeCommand = pktCommand->type;
    bzero(revbuf, sizeCommands[typeCommand]);
    int fr_block_sz = 0;

    if((fr_block_sz = recv(socket, revbuf, sizeCommands[typeCommand], 0)) > 0)
    {
        if(fr_block_sz == sizeCommands[typeCommand]){
            parseBufferToCommand(revbuf, pktCommand);
            return true;
        }else{
        fprintf(stderr, "Erro != received %d ... expected: %d", fr_block_sz, sizeCommands[typeCommand]);
        }
    }

    return false;
}

bool waitingForAnyCommand(int socket, PktCommand * pktCommand) {

    uint8_t revbuf[sizeof(PktCommand)]; // Receiver buffer
    int fr_block_sz = 0;

    if((fr_block_sz = recv(socket, revbuf, sizeof(PktCommand), 0)) > 0)
    {
        parseBufferToCommand(revbuf, pktCommand);
        return true;
    }
    return false;
}

bool sendCommandANS(int socket, PktCommand * pktCommand) {

    int typeCommand = pktCommand->type;
    uint8_t buffer[sizeCommands[typeCommand]];

    parseCommandToBuffer(pktCommand, buffer);

    if(send(socket, (uint8_t *)buffer, sizeCommands[typeCommand], 0) < 0)
    {
        fprintf(stderr, "ERROR: Sending command. (errno = %d)\n", errno);
    }else{
        fprintf(stdout, "Sent command: %d\n", typeCommand);
        return true;
    }

    return false;
}


void Controller::startNode() {
    this->stop = false;
    start(InheritPriority);
}

void Controller::stopNode() {
    this->stop =  true;
}

bool Controller::processCommand(int socket, PktCommand * pktCommand){

    switch (pktCommand->type) {
        case COMMAND_REQUEST_INFO:
        break;
        case COMMAND_SET_PORTS:
        break;
        case COMMAND_START_VIDEO_STREAM:
            if(pktCommand->pktCommands.pktCommandStartVideoStream.videoStreamIndex == 0xFF){
                    //Start all
                int i = 0;

                for(i = 0; i < numCameras; i++){
                    QString video = "/dev/video"+QString::number(i);
                    cameras[i] = new CameraSender(video, SERVER_IPADDRESS, portsCamera[i], DEFAULT_FORMAT, DEFAULT_WIDTH, DEFAULT_HEIGHT);
                    cameras[i]->startSender();
                }
            }

            pktCommand->type++; //answer
            pktCommand->pktCommands.pktCommandSetPortsANS.ack = 1;
            if(sendCommandANS(socket, pktCommand)) {
                fprintf(stdout, "Sending ack: %d\n", pktCommand->pktCommands.pktCommandSetPortsANS.ack);
                fflush(stdout);
                return true;
            }
        break;
        case COMMAND_STOP_VIDEO_STREAM:
                if(pktCommand->pktCommands.pktCommandStopVideoStream.videoStreamIndex == 0xFF){
                        //Stop all
                    int i = 0;
                    for(i = 0; i < numCameras; i++) {
                          cameras[i]->stopSender();
                          //while(!cameras[i]->isRunning());
                    }
                }

                pktCommand->type++; //answer
                pktCommand->pktCommands.pktCommandSetPortsANS.ack = 1;
                if(sendCommandANS(socket, pktCommand)) {
                    fprintf(stdout, "Sending ack: %d\n", pktCommand->pktCommands.pktCommandSetPortsANS.ack);
                    fflush(stdout);
                    return true;
                }
        break;
        case COMMAND_START_BEHAVIOR_STREAM:
            behaviorContextSender->startSender();
            pktCommand->type++; //answer
            pktCommand->pktCommands.pktCommandStartBehaviorStreamANS.ack = 1;
            if(sendCommandANS(socket, pktCommand)) {
                fprintf(stdout, "Sending ack: %d\n", pktCommand->pktCommands.pktCommandSetPortsANS.ack);
                fflush(stdout);
                return true;
            }
        break;
        case COMMAND_STOP_BEHAVIOR_STREAM:
            behaviorContextSender->stopSender();
            pktCommand->type++; //answer
            pktCommand->pktCommands.pktCommandStopBehaviorStreamANS.ack = 1;
            if(sendCommandANS(socket, pktCommand)) {
                fprintf(stdout, "Sending ack: %d\n", pktCommand->pktCommands.pktCommandSetPortsANS.ack);
                fflush(stdout);
                return true;
            }
        break;
        case COMMAND_STATUS_CONN:
            pktCommand->type++; //answer
            pktCommand->pktCommands.pktCommandSetPortsANS.ack = 1;
            if(sendCommandANS(socket, pktCommand)) {
                fprintf(stdout, "Sending ack: %d\n", pktCommand->pktCommands.pktCommandSetPortsANS.ack);
                fflush(stdout);
                return true;
            }
        break;
        default:
            fprintf(stderr, "Command not supported: %d\n", pktCommand->type);
            fflush(stderr);
        break;
    }
    return false;
}

bool Controller::stopAllStreams() {
    int i = 0;
    for(i = 0; i < numCameras; i++) {
          cameras[i]->stopSender();
          //while(!cameras[i]->isRunning());
    }

    this->behaviorContextSender->stopSender();
    return true;
}

void Controller::run() {

    /* Variable Definition */
    int sockfd;
    struct sockaddr_in remote_addr;
    bool connected = false;
    int commandType  = 0;

    uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strcpy(s.ifr_name, "eth0"); //get gateway
    if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
        memcpy(mac, s.ifr_addr.sa_data, 6);
        int i;
        for (i = 0; i < 6; ++i)
            printf(" %02x", (unsigned char) s.ifr_addr.sa_data[i]);
            puts("\n");
    }

    PktCommand pktCommand;
    memset(&pktCommand, 0, sizeof(pktCommand));
    pktCommand.delimiter = COMMAND_PKT_DELIMITER;

    while(!stop) {
        switch(state) {
                    case STATE_WAITING_CONNETION:
                        if(connected){
                            state++;
                        }else{
                                /* Get the Socket file descriptor */
                            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
                            {
                                fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
                                exit(1);
                            }

                            /* Fill the socket address struct */
                            remote_addr.sin_family = AF_INET;
                            remote_addr.sin_port = htons(PORT);
                            inet_pton(AF_INET, SERVER_IPADDRESS, &remote_addr.sin_addr);
                            bzero(&(remote_addr.sin_zero), 8);
                            int opt = true;
                            setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
                                          (char *)&opt,sizeof(opt));
                            fprintf(stdout, "[Client] Waiting new connection with server ...\n");
                            /* Try to connect the remote */
                            if (::connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
                            {
                                //fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
                                sleep(5);
                            }
                            else {
                                printf("[Client] Connected to server at port %d...ok!\n", PORT);
                                connected = true;
                            }
                            fflush(stdout);
                        }
                    break;
                    case STATE_WAITING_REQUESTINFO:
                        fprintf(stdout, "[Client] Waiting request info...\n");
                        commandType  = COMMAND_REQUEST_INFO;
                        pktCommand.type = commandType;

                        if(waitingForCommand(sockfd, &pktCommand)){
                            pktCommand.type = COMMAND_REQUEST_INFO_ANS;
                            //arguments
                            pktCommand.pktCommands.pktCommandRequestInfoANS.numCameras = this->numCameras;
                            strcpy(pktCommand.pktCommands.pktCommandRequestInfoANS.label, "Cage1");
                            memcpy(pktCommand.pktCommands.pktCommandRequestInfoANS.macAddress, mac, 6);
                            if(sendCommandANS(sockfd, &pktCommand)) {
                                fprintf(stdout, "Request info sent\n");
                                fflush(stdout);
                                state++;
                            }
                        }else{
                            fprintf(stderr, "ERROR: Failed on waiting command: %d! (errno = %d)\n", commandType, errno);
                            state = STATE_WAITING_CONNETION;
                            connected = 0;
                            close(sockfd);
                            stopAllStreams();
                        }
                    break;
                    case STATE_WAITING_SET_PORTS:
                        fprintf(stdout, "[Client] Waiting set ports...\n");
                        commandType  = COMMAND_SET_PORTS;
                        pktCommand.type = commandType;
                        if(waitingForCommand(sockfd, &pktCommand)){
                            pktCommand.type = COMMAND_SET_PORTS_ANS;
                            //arguments
                            int i = 0;
                            int ok = 1;
                            for(i = 0; i < MAX_CAMERAS; i++) {
                                if(pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[i] < 50000 &&  pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[i] != 0)
                                ok = 0;
                            }

                            if( pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext < 50000 && pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext != 0)
                                ok = 0;

                            behaviorContextSender = new BehaviorContextSender(SERVER_IPADDRESS, pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext);

                            if(ok){
                                for(int i =0 ;i< MAX_CAMERAS; i++){
                                    portsCamera[i] = pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[i];
                                }
                                pktCommand.pktCommands.pktCommandSetPortsANS.ack = 1;
                                state++;
                            }else{
                                pktCommand.pktCommands.pktCommandSetPortsANS.ack = 0;
                            }


                            fprintf(stdout, "Ports %d %d %d\n", pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[0],  pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[1], pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext);
                            fflush(stdout);
                            if(sendCommandANS(sockfd, &pktCommand)) {
                                //TODO
                            }
                        }else{
                            fprintf(stderr, "ERROR: Failed on waiting command: %d! (errno = %d)\n", commandType, errno);
                            state = STATE_WAITING_CONNETION;
                            connected = 0;
                            close(sockfd);
                            stopAllStreams();
                        }
                    break;

                    case STATE_WAITING_COMMANDS:
                            fprintf(stdout, "[Client] Waiting for new command...\n");
                            fflush(stdout);
                            if(waitingForAnyCommand(sockfd, &pktCommand)){
                                fprintf(stdout, "Received command: %d\n", pktCommand.type);
                                fflush(stdout);
                                processCommand(sockfd, &pktCommand);
                            }else{
                                //connection broken
                                state = STATE_WAITING_CONNETION;
                                connected = 0;
                                close(sockfd);
                                stopAllStreams();
                            }
                    break;

                    default :
                        fprintf(stderr, "ERROR: State not defined %d)\n", state);
                        sleep(1);
                }
    }
    this->exit();
}
