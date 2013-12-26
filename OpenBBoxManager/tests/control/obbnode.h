
#ifndef OBBNODE_H
#define OBBNODE_H

#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <stdio.h>
#include <control/receivervideoudp.h>

//DECODER
#include "QVideoDecoder.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QQueue>
#include <control/consoleserverpacket.h>
#include <control/receiverbehaviortcp.h>
#include <config/config.h>

class OBBNode : public QThread
{
    Q_OBJECT
private:
    char macStr[sizeof("00:00:00:00:00:00")];
    int                  idtask;
    int                  iddatabase;
    u_int8_t             sizeCamerasPort;
    u_int8_t             mac[MAC_ADDRESS_SIZE];
    u_int16_t            portConsole;
    u_int16_t            portVideo[MAX_CAMERAS];
    u_int16_t            portBehavior;
    u_int32_t            ipAddress;
    char                 label[MAX_LABEL_SIZE];

    QList<ReceiverVideoUDP*> receiverListCameras;
    ReceiverBehaviorTCP * receiverBehavior;
    QString generateFileName(int port);

public:
    OBBNode(u_int16_t * portVideo, u_int16_t portBehavior, char * label , u_int8_t * mac, uint32_t ipAddress, u_int16_t portConsole);
    ReceiverVideoUDP * getVideoStream(int index);
    ReceiverBehaviorTCP * getBehaviorStream();

    char * getLabel();
    int getNumberOfVideoStream();
    int getPortController();

    void addNewReceiverVideo(u_int16_t port, int delayFrame);
    char * getMacAddress();
    u_int32_t getIPAdress();
    u_int16_t getBehaviorPort();

    void setCurrentTask(int idtask);
    int getCurrentTask();

    void setIDDatabase(int id);
    int getIDDatabase();
signals:
    //Signal to output frame to be displayed
    void processedImage(const QImage &image, QString info);
    void processedImageBuffer(uchar * buffer, uint size, QString info);

};

#endif // OBBNODE_H
