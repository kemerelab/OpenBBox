#include "controller.h"

Controller::Controller() :
    QThread()
{
    this->portOffset = 0;
    timer = new QTimer(0);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkConnection()));

}

bool Controller::startOBBNodeStreams(OBBNode * node) {

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
                qDebug("Video streams started OK");
            }else{
                qDebug("Video streams started FAIL");
                return false;
            }
        }else{
            qCritical("Error command answer invalid: expected %d received %d", commandTypeANS, commandType);
            return false;
        }
    }else{
        qCritical("Error sending command: %d", commandType);
        return false;
    }
    //###################################################

    return true;
}

int Controller::startOBBNodeTask(OBBNode * node, bool test) {
    BehaviorTaskPacket * packet = node->getBehaviorTask();
    if(node->getCurrentTask()==0)
        return 1;
    if(!node->getSubject().status&&!test)
        return 2;

    //###################################################
    //######### Send Behavior Task ############
    node->getBehaviorStream()->startServer(node->getBehaviorTaskID(), test);
    if(!test){
        node->getSenderTaskStream()->setTaskPacket(packet);
        for(int i = 0; i < node->getNumberOfVideoStream(); i++){
            node->getVideoStream(i)->startRecording(node->getBehaviorTaskID());
        }
    }

    //###################################################
    //######### Start Behavior stream ############
    PktCommand pktCommand;
    int commandType;
    int commandTypeANS;
    commandType     = COMMAND_START_BEHAVIOR_STREAM;
    commandTypeANS  = COMMAND_START_BEHAVIOR_STREAM_ANS;
    pktCommand.type = commandType;
    pktCommand.delimiter = COMMAND_PKT_DELIMITER;
    pktCommand.pktCommands.pktCommandStartBehaviorStream.test = test;
    if(sendCommand(node->getPortController(), &pktCommand)) {
        if(pktCommand.type == commandTypeANS) {
            if(pktCommand.pktCommands.pktCommandStartBehaviorStreamANS.ack){
                qDebug("Behavior stream from node started OK");
            }else{
                qDebug("Behavior stream from node started FAIL");
                return false;
            }
        }else{
            qCritical("Error command answer invalid: expected %d received %d", commandTypeANS, commandType);
            return false;
        }
    }else{
        qCritical("Error sending command: %d", commandType);
        return false;
    }

    node->getSenderTaskStream()->startServer(node->getCurrentTask(), test);

    return 0;
}

bool Controller::stopOBBNodeTask(OBBNode * node, int stopType){
    node->getBehaviorStream()->stopServer();
    qDebug(qPrintable(node->getBehaviorStream()->getKeyStream()));
    if(stopType == STOPTYPE_BUTTON){
        PktCommand pktCommand;
        pktCommand.delimiter = COMMAND_PKT_DELIMITER;
        int commandType;
        int commandTypeANS;

        commandType     = COMMAND_STOP_BEHAVIOR_STREAM;
        commandTypeANS  = COMMAND_STOP_BEHAVIOR_STREAM_ANS;
        pktCommand.type = commandType;

        if(sendCommand(node->getPortController(), &pktCommand)) {
            if(pktCommand.type == commandTypeANS) {
                if(pktCommand.pktCommands.pktCommandStartBehaviorStreamANS.ack){

                }else{
                    qDebug("Behavior stream from node stopped FAIL");
                    return false;
                }
            }else{
                qCritical("Error command answer invalid: expected %d received %d", commandTypeANS, commandType);
                return false;
            }
        }else{
            qCritical("Error sending command: %d", commandType);
            return false;
        }
    }
    qDebug("Behavior stream from node stopped OK");
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

void Controller::addNewNode(uint * portVideo, uint portBehavior, uint portBTask, char * label , uchar * mac, uint ipAddress, uint portConsole) {
    obbnodeList.push_back(new OBBNode((uint16_t*) portVideo, (uint16_t)portBehavior, (uint16_t)portBTask, label, (uint8_t*)mac, (uint32_t) ipAddress, (uint16_t)portConsole));

    char ipStr[sizeof("255.255.255.255")];
    sprintf(ipStr, "%d.%d.%d.%d", ((ipAddress) & 0xFF), ((ipAddress >> 8) & 0xFF), ((ipAddress >> 16) & 0xFF), ((ipAddress >> 24) & 0xFF));
    char macStr[sizeof("00:00:00:00:00:00")];
    sprintf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    qDebug("Added new node... ip %s mac: %s label: %s ", ipStr, macStr, label);

    emit processAddNodeList(obbnodeList.at(obbnodeList.size()-1));

    if(obbnodeList.at(obbnodeList.size()-1)->getNumberOfVideoStream()>0)
        startOBBNodeStreams(obbnodeList.at(obbnodeList.size()-1));
}

//TODO [ParseCommands] Find better solution
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
       qCritical("ERROR: Error sending command. (errno = %d)", errno);

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
        qCritical("ERROR: Error sending command. (errno = %d)", errno);
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

    qDebug("Sending command %d...", commandType);

    if(sendCommand(nsockfd, &pktCommand)) {
        if(pktCommand.type == commandTypeANS){
            qDebug("Received OK");
            nodeInfo = pktCommand.pktCommands.pktCommandRequestInfoANS;
        }else{
            qCritical("Error command answer invalid: expected %d received %d", commandTypeANS, commandType);
            return false;
        }
    }else{
        qCritical("Error sending command: %d", commandType);
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
            qDebug("Alocating port %d to Video %d for %s@%s", pktCommand.pktCommands.pktCommandSetPorts.portVideoStream[i], i, nodeInfo.label, inet_ntoa(addr_remote.sin_addr));
            portOffset++;
        }

        pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext = allocPort(nsockfd);
        uint16_t portBehavior = pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext;
        qDebug("Alocating port %d to Behavior for %s@%s", pktCommand.pktCommands.pktCommandSetPorts.portBehaviorContext, nodeInfo.label, inet_ntoa(addr_remote.sin_addr));

        portOffset++;

        pktCommand.pktCommands.pktCommandSetPorts.portTask = allocPort(nsockfd);
        uint16_t portBTask = pktCommand.pktCommands.pktCommandSetPorts.portTask;
        qDebug("Alocating port %d to Task for %s@%s", pktCommand.pktCommands.pktCommandSetPorts.portTask, nodeInfo.label, inet_ntoa(addr_remote.sin_addr));

        portOffset++;

        qDebug("Sending command %d...", commandType);

        if(sendCommand(nsockfd, &pktCommand)) {
            if(pktCommand.type == commandTypeANS){
                qDebug("Received OK");

                if(pktCommand.pktCommands.pktCommandSetPortsANS.ack){
                    //Add new node to list
                                addNewNode(   (uint *)portCameras,
                                              (uint)portBehavior,
                                              (uint)portBTask,
                                              (char *)nodeInfo.label,
                                              (uchar *)nodeInfo.macAddress,
                                              (uint)addr_remote.sin_addr.s_addr,
                                              nsockfd);
                }else{
                    qCritical("Error command answer: Ack is false");
                    return false;
                }
            }else{
                qCritical("Error command answer invalid: expected %d received %d", commandTypeANS, commandType);
                return false;
            }
        } else {
            qCritical("Error sending command: %d", commandType);
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
            qCritical("Unable to commmunicate with %d", obbnodeList.at(i)->getPortController());
        }

        mutex.unlock();
        if(!status) {
            struct sockaddr_in addr_remote;
            addr_remote.sin_addr.s_addr = (in_addr_t)obbnodeList.at(i)->getIPAdress();
            qCritical("Node disconnected at %s", inet_ntoa(addr_remote.sin_addr));
            //free video stream ports
            for(j = 0; j < obbnodeList.at(i)->getNumberOfVideoStream(); j++)
                freePort(obbnodeList.at(i)->getVideoStream(j)->getPort());

            //free Behavior stream port
            freePort(obbnodeList.at(i)->getBehaviorPort());

            //free Behavior Task stream port
            freePort(obbnodeList.at(i)->getTaskPort());

            //remove from lists
            emit processRemoveNodeList(obbnodeList.at(i));
            obbnodeList.removeAt(i);
            i--;
        }
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
        qFatal("ERROR: Failed to obtain Socket Descriptor. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Obtaining socket descriptor successfully");

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
        qFatal("ERROR: Failed to bind Port. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug("Binded tcp port %d in addr 127.0.0.1 sucessfully.",PORT);

    /* Listen remote connect/calling */
    if(listen(sockfd, BACKLOG) == -1)
    {
        qFatal("ERROR: Failed to listen Port. (errno = %d)", errno);
        this->exit();
    }
    else
        qDebug ("Listening the port %d successfully.", PORT);

    stop = false;
    while(!stop)
    {
        sin_size = sizeof(struct sockaddr_in);

         //Wait a connection, and obtain a new socket file despriptor for single connection */
        if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1)
        {
            qCritical("ERROR: Obtaining new Socket Despcritor. (errno = %d)", errno);
        }
        else{
            qDebug("Server has got connected from %s.", inet_ntoa(addr_remote.sin_addr));

            int flags = fcntl(nsockfd, F_GETFD);
            if ((flags & O_NONBLOCK) == O_NONBLOCK) {
              qDebug("Yup, it's nonblocking");
            }
            else {
              qDebug("Nope, it's blocking.");
            }
        }
        //Allocate new Open BBox Node
        //REQUEST  Minumum Information: #IPADDRESS #MAC_ADDRESS #CAMERAS #LABEL
        if(processNewNode(addr_remote, nsockfd)){
            qDebug("New node processed at %s.", inet_ntoa(addr_remote.sin_addr));
        }else{
            qCritical("New node fail to be processed at %s", inet_ntoa(addr_remote.sin_addr));
        }
    }

    close(sockfd);
    this->exit();
}
