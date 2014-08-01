#include "receivervideoudp.h"

ReceiverVideoUDP::ReceiverVideoUDP(u_int16_t port, int delayFrame, QString filename) :
    QThread()
{
    this->filenameout = filename;
    this->port = port;
    this->delayFrame = delayFrame;

    this->supported_formats_livestream_mult[0] = 1;     //YUV
    this->supported_formats_livestream_mult[1] = 1;     //MJPEG
    this->supported_formats_livestream_mult[2] = 1000;  //H264 //300

    this->stop = false;
    this->totalBytesReceived = 0;
    this->totalBytesReceivedFrame = 0;
    this->cnt = 0;
    this->totalBytesReceivedFrame = 0;

    this->liveStream = false;
    this->openedDecoder = DEFAULT;
    this->recording = false;
}

ReceiverVideoUDP::~ReceiverVideoUDP(){

}

void ReceiverVideoUDP::run()
{
    PktControl pktControl;
    PktControl pktControlAux;
    u_int16_t size_buffer = sizeof(u_int8_t) * LENGTH_BASE;
    u_int16_t checksum = 0;
    buffer = (u_int8_t *)malloc(size_buffer);
    this->formatType = -1;
    bool receivedEND = true;
    bool startedHeaderH264 = false;
    this->openedDecoder = DEFAULT;
    len = sizeof(cliaddr);

    u_int32_t lastSizeExpected = 0;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    int opt = true;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
                  (char *)&opt,sizeof(opt));

    QByteArray * bufferBytes = new QByteArray();

    if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1 )
    {
        qCritical( "ERROR: Failed to bind Port. (errno = %d)", errno);

        exit(1);
    }

     /*Receive File from Client */
     fr = fopen(qPrintable(filenameout), "w");
     if(fr == NULL)
         qDebug("File %s Cannot be opened file on server.", qPrintable(filenameout));

    qDebug("Started listening on port %d", port);

    while(!stop) {

                n = recvfrom(sockfd, buffer, size_buffer, 0, (struct sockaddr *)&cliaddr, &len);

                if(SIZE_START_PACKET == n && buffer[0] == CONTROL_PKT_DELIMITER && buffer[1] == PACKET_START_TYPE){ //TODO Change name
                    pktControl.pktcontrolptr = buffer;

                    //First alloc buffer to the right format. If change over time change the buffer too
                    if(formatType != pktControl.pktcontrol->pktControlContext.format) {
                        formatType = pktControl.pktcontrol->pktControlContext.format;
                        //fprintf(stdout,"Allocating new buffer for format %s...", supported_formats_str[formatType]);

                        free(buffer);
                        if((buffer = (u_int8_t *)malloc(supported_formats_bufferSize[formatType])) == NULL){
                             qCritical("Error allocating buffer");
                             break;
                        } else {//Alloc OK
                             size_buffer = supported_formats_bufferSize[formatType];
                        }

                        bufferBytes->clear();
                    }

                    if(!receivedEND) {

                        /*if(checksum != qChecksum(bufferBytes->data(), bufferBytes->size())){
                            qCritical("Error: Checksum of frame not match: %d != %d",checksum, qChecksum(bufferBytes->data(), bufferBytes->size()));

                        }*/


                        if(formatType == FORMAT_H264) { //Save by format
                             /*//checking for header PPS
                             if(0x67 == getNalType((u_int8_t*)bufferBytes->data(), bufferBytes->size())) {
                                 //printf("Nal Header type found...", bufferBytes->size());

                                 startedHeaderH264 = true;
                                  ////record the first h264 header to fast the live stream
                              }

                            //make sure that the first frame in the file is a header PPS
                             if(startedHeaderH264) {
                                if(recording) {
                                    if(writeOnFile(fr, (u_int8_t*)bufferBytes->data(), bufferBytes->size())) {
                                        //printf("Frame Stopped. Received %d bytes", bufferBytes->size());

                                        cnt++;
                                        totalBytesExpected += lastSizeExpected;
                                        totalBytesReceived += bufferBytes->size();
                                        fflush(fr);

                                        emit processAddPacketDB(idtask, filenameout, pktControlAux, port, 0x67 == getNalType((u_int8_t*)bufferBytes->data(), bufferBytes->size()) ? 1 : 0, bufferBytes->size(), QDateTime::currentDateTime().toTime_t());
                                     }else{
                                         //qCritical("Error on write in file %s", filenameout));

                                         lastSizeExpected = 0;
                                     }
                                }
                             }

                             //make sure that the first frame in the file is a header PPS
                             if(startedHeaderH264 && cnt > (MIN_TO_STREAM + delayFrame) && openedDecoder == DEFAULT) {
                                 decoder.openFile(qPrintable(filenameout));
                                 if(decoder.isOk() == false)
                                 {
                                     //qCritical("Error on open decoder of file %s", filenameout));

                                     //qCritical("%s not supported yet!!", supported_formats_str[formatType]);

                                     openedDecoder = NOT_SUPPORTED; //error
                                 }else{
                                    openedDecoder = SUCCESS;
                                    //emit processFrames(&decoder, cnt-2, TYPE_SKIP_DEC);
                                    mutex.lock();
                                    for(uint i=0; i< cnt-3; i++)
                                         decoder.skipFrame();//decoder.seekNextFrame();
                                    mutex.unlock();

                                 }
                             }*/
                        } else {
                            if(recording) {
                                if(writeOnFile(fr, (u_int8_t*)bufferBytes->data(), bufferBytes->size())) {
                                   //printf("Frame Stopped. Received %d bytes", bufferBytes->size());
                                   cnt++;
                                   totalBytesExpected += lastSizeExpected;
                                   totalBytesReceived += bufferBytes->size();
                                   emit processAddPacketDB( idtask, filenameout, pktControlAux, port, 0, bufferBytes->size(), QDateTime::currentDateTime().toTime_t());
                                }else{
                                    qCritical("Error on write in file %s", qPrintable(filenameout));
                                }
                            }
                            openedDecoder = NOT_SUPPORTED; //error
                        }

                        if(isLiveStream()) {
                            switch(formatType) { //TODO possible problem if type of stream change
                                case FORMAT_YUV: //Just need one frame to decode
                                case FORMAT_MJPEG:
                                  uchar * image;
                                  if((image = (u_int8_t *) malloc(bufferBytes->size())) == NULL) {
                                        qCritical("Error allocating buffer for LastImage");
                                  }else{//Alloc OK
                                        memcpy(image, (u_int8_t*)bufferBytes->data(), bufferBytes->size());
                                        emit processDisplayFrames(image, bufferBytes->size(), formatType, width, height);
                                  }

                                 break;
                                case FORMAT_H264:
                                         if(openedDecoder == SUCCESS)
                                            this->semphServer.release();
                                break;
                            }
                        }else{
                            switch(formatType) { //TODO possible problem if type of stream change
                                case FORMAT_YUV: //Just need one frame to decode
                                case FORMAT_MJPEG:
                                break;
                                case FORMAT_H264:/*
                                    if(openedDecoder == SUCCESS){
                                        decoder.seekNextFrame();
                                    }*/
                                break;
                            }
                        }
                    }

                    //printf("Port %d: Frame %d (id: %d version: %d time: %d time_usec: %d size: %d width: %d height: %d format: %s checksum: %d) Started...", port, cnt+1, pktControl.pktcontrol->pktControlContext.id, pktControl.pktcontrol->version, pktControl.pktcontrol->pktControlContext.time, pktControl.pktcontrol->pktControlContext.time_usec, pktControl.pktcontrol->pktControlContext.size, pktControl.pktcontrol->pktControlContext.width, pktControl.pktcontrol->pktControlContext.height, supported_formats_str[pktControl.pktcontrol->pktControlContext.format], pktControl.pktcontrol->pktControlContext.checksum);


                    lastSizeExpected = pktControl.pktcontrol->pktControlContext.size;

                    this->width = pktControl.pktcontrol->pktControlContext.width;
                    this->height = pktControl.pktcontrol->pktControlContext.height;
                    checksum =  pktControl.pktcontrol->pktControlContext.checksumFrame;

                    //if(pktControl.pktcontrol->pktControlContext.checksum != qChecksum((char*)&pktControl.pktcontrol->pktControlContext, (sizeof(ControlPacketContext)/sizeof(u_int8_t)) - 2)){
                    //    qCritical("Error: Checksum of start not match: %d != %d", pktControl.pktcontrol->pktControlContext.checksum, qChecksum((char*)pktControl.pktcontrolptr, SIZE_START_PACKET - 2));
                    //
                   // }
                    pktControlAux = pktControl;
                    bufferBytes->clear();
                    receivedEND = false;

                }else if(SIZE_CLOSE_PACKET == n && buffer[0] == CONTROL_PKT_DELIMITER && buffer[1] == PACKET_CLOSE_TYPE){
                    if(writeOnFile(fr, (u_int8_t*)bufferBytes->data(), bufferBytes->size())) {
                        totalBytesExpected += lastSizeExpected;
                        totalBytesReceived += bufferBytes->size();
                        //printf("Frame Stopped. Received %d bytes",  bufferBytes->size());

                        this->semphServer.release();
                    }else{
                        qCritical("Error on write in file %s", qPrintable(filenameout));
                    }

                    //MSG On close
                    //printf("Total frames received: %d frames", cnt+1);
                    //printf("Total bytes received: %d bytes", totalBytesReceived);
                    //printf("Total bytes expected: %d bytes", totalBytesExpected);
                    //printf("Total bytes lost: %d bytes", totalBytesExpected - totalBytesReceived);

                    stop = true;
                } else {
                    bufferBytes->append((char*)buffer, n);
                }
     }

     close(sockfd);
     this->exit();
}

void ReceiverVideoUDP::startServer()
{
    this->stop = false;
    start(LowestPriority);
}


void ReceiverVideoUDP::stopServer()
{
    this->stop = true;
    free(buffer);
    buffer = NULL;
    close(sockfd);
}

QSemaphore * ReceiverVideoUDP::getSemaphore(){
    return &this->semphServer;
}

char * ReceiverVideoUDP::getFilename(){
    return this->filenameout.toLatin1().data();
}

bool ReceiverVideoUDP::writeOnFile(FILE* fd, u_int8_t * data, u_int32_t size)
{
    uint write_sz = fwrite(data, sizeof(u_int8_t), size, fd);
    fflush(fd);
    if(write_sz < size)
    {
        qCritical("File write failed on server.");
        return false;
    }

    return true;
}

void ReceiverVideoUDP::setLiveStream(bool liveStream){
    this->liveStream = liveStream;
}

bool ReceiverVideoUDP::isLiveStream() {
    return this->liveStream;
}

int ReceiverVideoUDP::isDecoderOpen() {
    return this->openedDecoder;
}

int ReceiverVideoUDP::getNalType( void *p, int len )
{
    if ( !p || 5 >= len )
        return -1;

    unsigned char *b = (unsigned char*)p;

    // Verify NAL marker
    if ( b[ 0 ] || b[ 1 ] || 0x01 != b[ 2 ] )
    {   b++;
        if ( b[ 0 ] || b[ 1 ] || 0x01 != b[ 2 ] )
            return -1;
    } // end if

    b += 3;

    return *b;
}

/*QVideoDecoder * ReceiverVideoUDP::getDecoder(){
    return &this->decoder;
}*/

u_int8_t ReceiverVideoUDP::getFormatType(){
    return formatType;
}

u_int16_t ReceiverVideoUDP::getPort(){
    return port;
}

void ReceiverVideoUDP::setKeyStream(QString key){
    this->keyStream = key;
}

QString ReceiverVideoUDP::getKeyStream(){
    return this->keyStream;
}

void ReceiverVideoUDP::startRecording(uint idtask){
    this->idtask = idtask;
    this->recording = true;
}

void ReceiverVideoUDP::stopRecording(){
    this->recording = false;
}

bool ReceiverVideoUDP::getRecording(){
    return this->recording;
}
