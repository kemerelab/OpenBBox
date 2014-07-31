#ifndef RECEIVERVIDEOUDP_H
#define RECEIVERVIDEOUDP_H

#include <QSemaphore>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <QTimer>
#include <QMutex>
#include <QByteArray>
//#include "QVideoDecoder.h"
#include "streampacket.h"
#include "config.h"
#include <QDateTime>

class ReceiverVideoUDP : public QThread
{
    Q_OBJECT

signals:
    //void processFrames(QVideoDecoder * decoder, int size, int typeSkip);
    void processDisplayFrames(uchar * buffer, uint size, uint typeFrame, uint width, uint height);
    void processAddPacketDB(uint idtask, QString Filename, PktControl pktControl, uint port, uint headerframe, uint size, long time);

private:
    bool liveStream;
    bool startedHeaderH264Live;
    bool stop;
    bool recording;

    uint idtask;

    uint width;
    uint height;

    QString filenameout;
    u_int8_t * buffer;
    u_int8_t formatType;
    u_int16_t port;
    u_int32_t lastImageSize; //TODO
    u_int32_t cnt;
    u_int32_t totalBytesReceivedFrame, totalBytesReceived, totalBytesExpected;
    u_int32_t supported_formats_livestream_mult[N_SUPPORTED_FORMATS];
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t len;
    FILE *fr;

    QString keyStream;
    QSemaphore semphServer;
    QMutex mutex;

    //DECODER
    //QVideoDecoder decoder;
    int openedDecoder;
    int delayFrame;

    //Methods
    bool  writeOnFile(FILE* fd, u_int8_t * data, u_int32_t size);
    int getNalType( void *p, int len );

protected:
    void run();
    void msleep(int ms);

public:
    explicit ReceiverVideoUDP(u_int16_t port, int delayFrame, QString filename);
    ~ReceiverVideoUDP();

    //Stop the video
    void startServer();
    void stopServer();
    void setLiveStream(bool liveStream);
    void setQntToSkip(int skip);
    char * getFilename();
    char * getFilenameStream();
    bool isLiveStream();
    bool getLastImage(u_int8_t *buffer, u_int32_t * size);
    int getQntToSkip();
    int isDecoderOpen();
    u_int8_t getFormatType();
    u_int16_t getPort();
    QSemaphore *getSemaphore();
    //QVideoDecoder * getDecoder();

    void setKeyStream(QString key);
    QString getKeyStream();
    void startRecording(uint idtask);
    void stopRecording();
    bool getRecording();
public slots:
    
};

#endif // RECEIVERVIDEOUDP_H
