#include <QCoreApplication>
#include "controller.h"
#include <util/logger.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    char IP[20] = DEFAULT_IPADDRESS;
    Logger logger;
    logger.install();
    qDebug("Initializing OpenBBox Node...");

    if(argc==1){
        qDebug("Develop Mode...");
    }else if(argc == 2){
        strcpy(IP, argv[1]);
        qDebug("Sever IP Address: %s", IP);
    }else if(argc >2){
        return a.exec();
    }

    Controller controller(IP);
    controller.startNode();

    return a.exec();
}
