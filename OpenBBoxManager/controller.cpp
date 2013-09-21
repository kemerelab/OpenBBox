#include "controller.h"
#include "obbnode.h"

Controller::Controller() :
    QThread()
{
    this->portOffset = 0;
    timer = new QTimer(0);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkConnection()));
}


bool Controller::startOBBNodeStreams(OBBNode * node) {

    //receiverBehavior = new ReceiverBehavior(this->portBehavior);

    PktCommand pktCommand;
    pktCommand.delimiter = COMMAND_PKT_DELIMITER;
    int commandType;
    int commandTypeANS;
    //######### Start stream of all cameras ############
    commandType     = COMMAND_START_VIDEO_STREAM;
    commandTypeANS  = COMMAND_START_VIDEO_STREAM_ANS;

    pktCommand.type = commandType;

    pktCommand.pktCommands.pktCommandStartVideoStream.videoStreamIndex = 0xFF; //Start all

    if(sendCommand(node->getPortController(), &pktCommand)) {
        if(pktCommand.type == commandTypeANS) {
            if(pktCommand.pktCommands.pktCommandStartVideoStreamANS.ack){
                fprintf(stdout, "Video streams started OK\n");
                fflush(stdout);
            }else{
                fprintf(stdout, "Video streams started FAIL\n");
                fflush(stdout);
                return false;
            }
        }else{
            fprintf(stderr, "Error command answer invalid: expected %d received %d", commandTypeANS, commandType);
            fflush(stderr);
            return false;
        }
    }else{
        fprintf(stderr, "Error sending command: %d", commandType);
        fflush(stderr);
        return false;
    }
    //###################################################
    //######### Start Behavior stream ############
    commandType     = COMMAND_START_BEHAVIOR_STREAM;
    commandTypeANS  = COMMAND_START_BEHAVIOR_STREAM_ANS;

    pktCommand.type = commandType;

    //no arguments
    if(sendCommand(node->getPortController(), &pktCommand)) {
        if(pktCommand.type == commandTypeANS) {
            if(pktCommand.pktCommands.pktCommandStartBehaviorStreamANS.ack){
                fprintf(stdout, "Behavior stream started OK\n");
                fflush(stdout);
            }else{
                fprintf(stdout, "Behavior stream started FAIL\n");
                fflush(stdout);
                return false;
            }
        }else{
            fprintf(stderr, "Error command answer invalid: expected %d received %d\n", commandTypeANS, commandType);
            fflush(stderr);
            return false;
        }
    }else{
        fprintf(stderr, "Error sending command: %d\n", commandType);
        fflush(stderr);
        return false;
    }
    //###################################################

    return true;
}

bool Controller::startOBBNodeTask(OBBNode * node) {

    for(int i = 0; i < node->getNumberOfVideoStream(); i++){
        node->getVideoStream(i)->startRecording(node->getCurrentTask());
    }

    PktCommand pktCommand;
    pktCommand.delimiter = COMMAND_PKT_DELIMITER;
    int commandType;
    int commandTypeANS;
    //###################################################
    //######### Send Behavior Task ############

    //###################################################
    //######### Start Behavior stream ############
    node->getBehaviorStream()->startServer(node->getCurrentTask());

    commandType     = COMMAND_START_BEHAVIOR_STREAM;
    commandTypeANS  = COMMAND_START_BEHAVIOR_STREAM_ANS;

    pktCommand.type = commandType;

    //no arguments
    if(sendCommand(node->getPortController(), &pktCommand)) {
        if(pktCommand.type == commandTypeANS) {
            if(pktCommand.pktCommands.pktCommandStartBehaviorStreamANS.ack){
                fprintf(stdout, "Behavior stream started OK\n");
                fflush(stdout);
            }else{
                fprintf(stdout, "Behavior stream started FAIL\n");
                fflush(stdout);
                return false;
            }
        }else{
            fprintf(stderr, "Error command answer invalid: expected %d received %d\n", commandTypeANS, commandType);
            fflush(stderr);
            return false;
        }
    }else{
        fprintf(stderr, "Error sending command: %d\n", commandType);
        fflush(stderr);
        return false;
    }
    //###################################################

    return true;
}

int Controller::allocPort(int portController) {
    if(portList.size() == 0){
        portList.insert(BASELINE_PORT_NUMBER, portController);
        return BASELINE_PORT_NUMBER;
    }else{
        //find available and return the closest one
        int i = 0;
        for(i = BASELINE_PORT_NUMBER; i < 0xFFFF; i++){
            if(!portList.contains(i)){
                portList.insert(i, portController);
                return i;
            }
        }
    }
    return 0;
}


void Controller::freePort(int portController){
       portList.remove(portController);
}

void Controller::addNewNode(uint * portVideo, uint portBehavior, char * label , uchar * mac, uint ipAddress, uint portConsole) {
    obbnodeList.push_back(new OBBNode((uint16_t*) portVideo, (uint16_t)portBehavior, label, (uint8_t*)mac, (uint32_t) ipAddress, (uint16_t)portConsole));

    char ipStr[sizeof("255.255.255.255")];
    sprintf(ipStr, "%d.%d.%d.%d", ((ipAddress) & 0xFF), ((ipAddress >> 8) & 0xFF), ((ipAddress >> 16) & 0xFF), ((ipAddress >> 24) & 0xFF));
    char macStr[sizeof("00:00:00:00:00:00")];
    sprintf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    printf("\nAdded new node... ip %s mac: %s label: %s \n", ipStr, macStr, label);
    fflush(stdout);

    emit processAddNodeList(obbnodeList.at(obbnodeList.size()-1));

    startOBBNodeStreams(obbnodeList.at(obbnodeList.size()-1));
}

//TODO find better solution
void parseCommandToBuffer(PktCommand * pktCommand, uint8_t * bufferRet){
    memcpy(bufferRet, (uint8_t*)pktCommand, SIZE_COMMAND_HEADER);
    memcpy(bufferRet + SIZE_COMMAND_HEADER, (uint8_t*)&pktCommand->pktCommands, sizeCommands[pktCommand->type]);
}

void parseBufferToCommand(uint8_t * buffer, PktCommand * pktCommandRet){
    memcpy(pktCommandRet, buffer, SIZE_COMMAND_HEADER);
    memcpy(&pktCommandRet->pktCommands, buffer + SIZE_COMMAND_HEADER, sizeCommands[pktCommandRet->type]);
}

bool Controller::sendCommand(int socket, PktCommand * pktCommand) {
    uint8_t revbuf[sizeof(PktCommand)]; // Receiver buffer

    int typeCommand = pktCommand->type;

    uint8_t * buffer = (uint8_t *) malloc(sizeCommands[typeCommand] * sizeof(uint8_t));
    parseCommandToBuffer(pktCommand, buffer);

    if(send(socket, buffer, sizeCommands[typeCommand], 0) < 0)
    {
       fprintf(stderr, "ERROR: Error sending command. (errno = %d)\n", errno);
       fflush(stderr);
       free(buffer);
       return false;
    }

    free(buffer);

    bzero(revbuf, sizeCommands[typeCommand+1]);
    int fr_block_sz = 0;
    if((fr_block_sz = recv(socket, revbuf, sizeCommands[typeCommand+1], 0)) > 0)
    {
        if(fr_block_sz == sizeCommands[typeCommand+1]){
            parseBufferToCommand(revbuf, pktCommand);
            return true;
        }
    }else{
        fprintf(stderr, "ERROR: Error sending command. (errno = %d)\n", errno);
        fflush(stderr);
    }

    return false;
}

Controller::~Controller(){
    stop = true;
}

void Controller::startService() {
    timer->start(CHECK_CONN_TIME);
    start(LowPriority);
}

int calcNumberOfPorts(PktCommand * pktCommand){
    int ret = 0;
    if(pktCommand->type == COMMAND_REQUEST_INFO_ANS){
        ret+= pktCommand->pktCommands.pktCommandRequestInfoANS.numCameras;
        ret+= NUMBER_OF_BEHAVIOR_SENDERS;
    }
    return ret;
}

bool Controller::processNewNode( struct sockaddr_in addr_remote, int nsockfd) {
    PktCommand pktCommand;
    PktCommandRequestInfoANS nodeInfo;

    memset(&pktCommand, 0, sizeof(pktCommand));

    pktCommand.delimiter = COMMAND_PKT_DELIMITER;

    int commandType     = COMMAND_REQUEST_INFO;
    int commandTypeANS  = COMMAND_REQUEST_INFO + 1;

    pktCommand.type = commandType;
    //No arguments

    printf("[Server] Sending command %d...\n", commandType);
    fflush(stdout);
    if(sendCommand(nsockfd, &pktCommand)) {
        if(pktCommand.type == commandTypeANS){
            fprintf(stdout, "Received OK\n");
            fflush(stdout);
            nodeInfo = pktCommand.pktCommands.pktCommandRequestInfoANS;
        }else{
            fprintf(stderr, "Error command answer invalid: expected %d received %d\n", commandTypeANS, commandType);
            fflush(stderr);
            return false;
        }
    }else{
        fprintf(stderr, "Error sending command: %d\n", commandType);
        fflush(stderr);
        return false;
    }

        int i = 0;

        commandType  = COMMAND_SET_PORTS;
        commandTypeANS  = COMMAND_SET_PORTS_ANS;

        pktCommand.type = commandType;
        //arguments
        uint16_t portCameras[MAX_CAMERAS] = {0, 0};
        memset(&pktCommand.pktCommands.pktCommandSetPorts, 0, sizeof(PktCommandSetPorts));

        for(i = 0; i < nodeInfo.numCameras; i++) {
            pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[i] = allocPort(nsockfd);
            portCameras[i] = pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[i];
            printf("Alocating port %d to Video %d for %s@%s\n", pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[i], i, nodeInfo.label, inet_ntoa(addr_remote.sin_addr));
            portOffset++;
        }

        pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext = allocPort(nsockfd);
        uint16_t portBehavior = pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext;
        printf("Alocating port %d to Behavior for %s@%s\n", pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext, nodeInfo.label, inet_ntoa(addr_remote.sin_addr));
        portOffset++;

        printf("[Server] Sending command %d...\n", commandType);
        fflush(stdout);
        if(sendCommand(nsockfd, &pktCommand)) {
            if(pktCommand.type == commandTypeANS){
                fprintf(stdout, "Received OK\n");
                fflush(stdout);
                if(pktCommand.pktCommands.pktCommandSetPortsANS.ack){
                    //Add new node to list
                                addNewNode(   (uint *)portCameras,
                                              (uint)portBehavior,
                                              (char *)nodeInfo.label,
                                              (uchar *)nodeInfo.macAddress,
                                              (uint)addr_remote.sin_addr.s_addr,
                                              nsockfd);
                }else{
                    fprintf(stderr, "Error command answer: Ack is false\n");
                    fflush(stderr);
                    return false;
                }
            }else{
                fprintf(stderr, "Error command answer invalid: expected %d received %d\n", commandTypeANS, commandType);
                fflush(stderr);
                return false;
            }
        } else {
            fprintf(stderr, "Error sending command: %d\n", commandType);
            fflush(stderr);
            return false;
        }

        return true;
}


void Controller::checkConnection(){
    int i = 0;
    int j = 0;
    PktCommand pktCommand;
    memset(&pktCommand, 0, sizeof(pktCommand));

    pktCommand.delimiter = COMMAND_PKT_DELIMITER;

    int commandType     = COMMAND_STATUS_CONN;
    int commandTypeANS  = COMMAND_STATUS_CONN_ANS;

    for(i = 0; i < obbnodeList.size(); i++) {
        pktCommand.type = commandType;

        bool status = false;
        mutex.lock();
        if(sendCommand(obbnodeList.at(i)->getPortController(), &pktCommand)) {
            if(pktCommand.type == commandTypeANS){
                if(pktCommand.pktCommands.pktCommandStatusConnANS.ack){
                    status = true;
                }
            }
        }else{

            fprintf(stderr, "Something happen\n");
            fflush(stderr);

        }
        mutex.unlock();
        if(!status) {
            struct sockaddr_in addr_remote;
            addr_remote.sin_addr.s_addr = (in_addr_t)obbnodeList.at(i)->getIPAdress();
            fprintf(stderr, "\nNode disconnected at %s\n", inet_ntoa(addr_remote.sin_addr));
            fflush(stderr);

            //free video stream ports
            for(j = 0; j < obbnodeList.at(i)->getNumberOfVideoStream(); j++)
                freePort(obbnodeList.at(i)->getVideoStream(j)->getPort());

            //free Behavior stream port
            freePort(obbnodeList.at(i)->getBehaviorPort());

            //remove from lists
            emit processRemoveNodeList(obbnodeList.at(i));
            obbnodeList.removeAt(i);
            i--;
        }
     //   sleep(1);
    }
}

void Controller::run()
{
    /* Defining Variables */
    int sockfd;
    int nsockfd;
    socklen_t sin_size;
    struct sockaddr_in addr_local; /* client addr */
    struct sockaddr_in addr_remote; /* server addr */

    /* Get the Socket file descriptor */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
        exit(1);
    }
    else
        printf("[Server] Obtaining socket descriptor successfully.\n");

    fflush(stdout);
    fflush(stderr);

    /* Fill the client socket address struct */
    addr_local.sin_family = AF_INET; // Protocol Family
    addr_local.sin_port = htons(PORT); // Port number
    addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
    bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct
    int opt = true;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
                  (char *)&opt,sizeof(opt));
    /* Bind a special Port */
    if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
    {
        fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
        exit(1);
    }
    else
        printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

    /* Listen remote connect/calling */
    if(listen(sockfd, BACKLOG) == -1)
    {
        fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
        this->exit();
    }
    else
        printf ("[Server] Listening the port %d successfully.\n", PORT);

    stop = false;
    while(!stop)
    {
        sin_size = sizeof(struct sockaddr_in);

         //Wait a connection, and obtain a new socket file despriptor for single connection */
        if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1)
        {
            fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
            fflush(stderr);
        }
        else{
            printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

            int flags = fcntl(nsockfd, F_GETFD);
            if ((flags & O_NONBLOCK) == O_NONBLOCK) {
              fprintf(stderr, "Yup, it's nonblocking");
            }
            else {
              fprintf(stderr, "Nope, it's blocking.");
            }
            fflush(stdout);
        }
        //Allocate new Open BBox Node
        //REQUEST  Minumum Information: #IPADDRESS #MAC_ADDRESS #CAMERAS #LABEL
        if(processNewNode(addr_remote, nsockfd)){
            //TODO !!!!
        }else{
            fprintf(stderr, "New node fail to be processed at %s\n", inet_ntoa(addr_remote.sin_addr));
            fflush(stderr);
        }
    }

    close(sockfd);
    this->exit();
}

bool Controller::sendTask(OBBNode * node, QByteArray * file){

}
