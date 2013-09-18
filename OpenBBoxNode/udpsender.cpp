#include "udpsender.h"
#include <QByteArray>
UDPSender::UDPSender(char* ipaddress, uint16_t udpport, uint8_t format) :
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

    printf("Starting UDP Sender thread!\nSending to %d@%s\n", this->udpport, this->ipaddress);
    fflush(stdout);
    int sizeToSend = lenghtBuffer;
    int fs_block_sz;
    int offset;
    int rc;

    sockfd = socket(AF_INET,SOCK_DGRAM, 0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(this->ipaddress);
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
                     fprintf(stderr, "ERROR: Failed to send file. (errno = %d)\n %d", errno, sizeToSend);
                 }else{
     #if DEBUG
                     fprintf(stderr, ".");
     #endif
                 }
         //##################SENDING FRAMES PACKETS####################
                 fs_block_sz = frame->size;
                 offset = 0;
                 while(fs_block_sz > 0)
                 {
                     if(fs_block_sz < sizeToSend)
                     sizeToSend = fs_block_sz;

                     if((rc = sendto(sockfd,  frame->bufptr + offset, sizeToSend, 0, (struct sockaddr *)&servaddr,sizeof(servaddr))) < 0)
                     {
                        fprintf(stderr, "\nERROR: Failed to send file. (errno = %d)\n", errno);
                     }else{
         #if DEBUG
                         if(rc != sizeToSend){
                             fprintf(stderr, "!");
                         }
                             fprintf(stderr, ".");
         #endif
                     }
                     fs_block_sz -= sizeToSend;
                     offset += sizeToSend;
                 }

                 sizeToSend = lenghtBuffer;

                 free(frame->bufptr);
                 bytesSent += frame->size;
                 free(frame);    
     #if DEBUG
                 fprintf(stderr,"_");
                 fflush(stderr);
     #endif

    }

    //##################SENDING ClOSE CONTROL PACKET####################
    controlpkt.type = PACKET_CLOSE_TYPE;
    if((sendto(sockfd, sendPktCtl.pktcontrolptr, SIZE_CLOSE_PACKET, 0, (struct sockaddr *)&servaddr,sizeof(servaddr))) < 0)
    {
        fprintf(stderr, "ERROR: Failed to send file. (errno = %d)\n", errno);
    }else{
        fprintf(stderr, "$$$\n");
        fprintf(stderr,"Bytes sent: %d\n", bytesSent);
        fflush(stderr);
        //TODO free stuff!!!!!!!!
        bytesSent = 0;
        cnt = 0;
        //close(sockfd);
    }
}
