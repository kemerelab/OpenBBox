#include <QCoreApplication>
#include "controller.h"
#include <util/logger.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Logger logger;
    logger.install();
    qDebug("Initializing OpenBBox Node...");

    Controller controller;
    controller.startNode();

    return a.exec();
}
