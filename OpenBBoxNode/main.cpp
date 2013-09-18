#include <QCoreApplication>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    printf("OpenBBox node started...\n");
    fflush(stdout);

    Controller controller;
    controller.startNode();

    return a.exec();
}
