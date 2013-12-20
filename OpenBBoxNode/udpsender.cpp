#include "udpsender.h"
#include <QByteArray>

UDPSender::UDPSender(QString ipaddress, uint16_t udpport, uint8_t format) :
    QThread()
{
    this->ipaddress = ipaddress;
    this->udpport = udpport;
    this->type = type;

    lenghtBuffer = supported_formats_bufferSize[format];

    this->stop = false;
    this->cnt = 0;
}

void UDPSender::startSender(){
    this->stop = false;
    start(LowestPriority);
}

void UDPSender::stopSender(){
    this->stop = true;
}

void UDPSender::sendFrame(uchar * buffer, uint size, uint timeSec, uint timeUSec, uint  width, uint height, uint format) {

    Image * image =  (Image *) malloc(sizeof(Image));
    image->id = cnt;
    image->bufptr = buffer;
    image->size = size;
    image->time = timeSec;
    image->time_usec = timeUSec;
    image->width = width;
    image->height = height;
    image->format = format;

    packetsFrameQueue.enqueue(image);
    qsem.release();

    cnt++;
}

void UDPSender::run(){

    qDebug() << QString("Starting UDP Sender thread! Sending to %1@%2").arg(this->udpport).arg(this->ipaddress);

    int sizeToSend = lenghtBuffer;
    int fs_block_sz;
    int offset;
    int rc;

    sockfd = socket(AF_INET,SOCK_DGRAM, 0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(this->ipaddress.toStdString().data());
    servaddr.sin_port = htons(udpport);
    int opt = true;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
                  (char *)&opt,sizeof(opt));
    ControlPacket controlpkt;
    controlpkt.delimiter = CONTROL_PKT_DELIMITER;
    controlpkt.version = VERSION;
    ControlPacketContext pktStartContext;

    PktControl sendPktCtl;
    sendPktCtl.pktcontrol = &controlpkt;

    while(!stop || qsem.available() != 0){

             qsem.acquire();

             Image * frame = (Image *)packetsFrameQueue.dequeue();

         //##################SENDING START CONTROL PACKET####################
                 pktStartContext.id             = frame->id;
                 pktStartContext.time           = frame->time;
                 pktStartContext.time_usec      = frame->time_usec;
                 pktStartContext.size           = frame->size;
                 pktStartContext.width          = frame->width;
                 pktStartContext.height         = frame->height;
                 pktStartContext.format         = frame->format;
                 pktStartContext.checksumFrame  = qChecksum((char *)frame->bufptr, frame->size);
                 pktStartContext.checksum       = qChecksum((char *)&pktStartContext, (sizeof(ControlPacketContext)/sizeof(u_int8_t)) - 2);
                 controlpkt.type = PACKET_START_TYPE;
                 controlpkt.pktControlContext = pktStartContext;

                 if((rc = sendto(sockfd, sendPktCtl.pktcontrolptr, SIZE_START_PACKET, 0, (struct sockaddr *)&servaddr,sizeof(servaddr))) < 0)
                 {
                     qCritical("Video socket failed to send file. (errno = %d) %d", errno, sizeToSend);
                 }else{
                     if(frame->id % VERBOSE_LEVEL == 0) {
                        qDebug("Start control packet %d sent", frame->id);
                     }
                 }
         //##################SENDING FRAMES PACKETS####################
                 fs_block_sz = frame->size;
                 offset = 0;
                 while(fs_block_sz > 0)
                 {
                     if(fs_block_sz < sizeToSend)
                     sizeToSend = fs_block_sz;

                     if((rc = sendto(sockfd, frame->bufptr + offset, sizeToSend, 0, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
                     {
                        qCritical("Video socket failed to send file. (errno = %d)", errno);
                     }else{
                         if(rc != sizeToSend){
                            qCritical("Error sending udp packet");
                         }
                     }
                     fs_block_sz -= sizeToSend;
                     offset += sizeToSend;
                 }


                 if(frame->id % VERBOSE_LEVEL == 0) {
                    qDebug("Frame packet %d sent", frame->id);
                 }

                 sizeToSend = lenghtBuffer;
                 free(frame->bufptr);
                 bytesSent += frame->size;
                 free(frame);
    }

    //##################SENDING ClOSE CONTROL PACKET####################
    controlpkt.type = PACKET_CLOSE_TYPE;
    if((sendto(sockfd, sendPktCtl.pktcontrolptr, SIZE_CLOSE_PACKET, 0, (struct sockaddr *)&servaddr,sizeof(servaddr))) < 0)
    {
        qCritical("ERROR: Failed to send file. (errno = %d)", errno);
    }else{
        qDebug("Stop control packet sent");
        qDebug("Bytes sent: %d", bytesSent);
        //TODO free stuff!!!!!!!!
        bytesSent = 0;
        cnt = 0;
        //close(sockfd);
    }
}
