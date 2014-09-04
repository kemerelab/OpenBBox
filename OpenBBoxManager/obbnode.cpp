#include "obbnode.h"

OBBNode::OBBNode( u_int16_t * portVideo, u_int16_t portBehavior, u_int16_t portBTask, char * label, u_int8_t * mac, u_int32_t ipAddress, u_int16_t portConsole) :
    QThread()
{
    this->idtask = 0;
    this->portConsole = portConsole;
    memcpy(this->portVideo, portVideo, MAX_CAMERAS);
    this->portBehavior = portBehavior;
    this->portTask = portBTask;
    memcpy(this->label, label, MAX_LABEL_SIZE);
    memcpy(this->mac, mac, MAC_ADDRESS_SIZE);
    this->ipAddress = ipAddress;
    sprintf(this->macStr, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    this->subject.status = false;


    //start cameras receiver threads
    int i = 0;
    for(i = 0; i < MAX_CAMERAS; i++){
         if(portVideo[i] != 0) {
              addNewReceiverVideo(portVideo[i], i);
         } 
     }
    //start behavior receiver threads
    receiverBehavior = new ReceiverBehaviorTCP(portBehavior);
}

void OBBNode::addNewReceiverVideo(u_int16_t port, int delayFrame) {
    receiverListCameras.push_back(new ReceiverVideoUDP(port, delayFrame, generateFileName(port)));
    receiverListCameras.at(receiverListCameras.size()-1)->startServer();
}

ReceiverVideoUDP * OBBNode::getVideoStream(int index){
    return receiverListCameras[index];
}

ReceiverBehaviorTCP * OBBNode::getBehaviorStream(){
    return receiverBehavior;
}

char * OBBNode::getLabel(){
    return label;
}

int OBBNode::getNumberOfVideoStream(){
    return receiverListCameras.size();
}

int OBBNode::getPortController(){
    return portConsole;
}

char * OBBNode::getMacAddress(){
    return macStr;
}

u_int32_t OBBNode::getIPAdress(){
    return ipAddress;
}

u_int16_t OBBNode::getBehaviorPort(){
    return portBehavior;
}

u_int16_t OBBNode::getTaskPort(){
    return portTask;
}
void OBBNode::setCurrentTask(int idtask){
    this->idtask = idtask;
}

int OBBNode::getCurrentTask(){
    return idtask;
}

void OBBNode::setIDDatabase(int id) {
    this->iddatabase = id;
}

int OBBNode::getIDDatabase() {
    return iddatabase;
}

void OBBNode::setSubject(SubInfo sub){
    this->subject = sub;
}

SubInfo OBBNode::getSubject(){
    return this->subject;
}

void OBBNode::setLastEvent(BehaviorEvent lastEvent){
    this->lastEvent = lastEvent;
}

BehaviorEvent OBBNode::getLastevent(){
    return lastEvent;
}

void OBBNode::setTask(QString task){
    this->task = task;
}

QString OBBNode::getTask(){
    return task;
}


QString OBBNode::generateFileName(int port){
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    qint64 time = te.tv_sec * 1000000LL + te.tv_usec; // calculate milliseconds
    QString ret = QString::number(time) + "_" + QString::number(port);
    return ret;
}
