/*!
*   \file main.cpp
*   \brief Main Method
*   Execute MainWindow and Save the startup time in the database
*
*   \mainpage Commercial solutions for automated control of operant conditioning chambers or other interactive rodent behavioral environments are fairly expensive ($5-10K/box), and sometimes lack adequate temporal resolution for experiments involving sensory stimuli. Consequently, custom behavioral control solutions are common. For high throughput purposes, neither commercial solutions nor common custom applications, e.g., using LabView or MatLab, scale well in cost or usability. We will develop a distributed control system with open source software and hardware designs that lowers costs to < $300/box. We envision a modern low-cost single board computer (SBC) as the primary controller for an individual behavioral environment ("node"), with behavioral data archived in a cloud-based server (Figure 8). We will develop open source software to record and instruct behavior with < 0.1 ms timing accuracy, sufficient for most rodent behavioral tasks. We will also enable synchronization to an input clock for cases in which an external device, such as an electrophysiological data acquisition system, needs to share a common time-stamping with behavioral events. Nodes will run behavioral programs written in language compatible with existing commercial systems, e.g., MedPC (Med Associates, St. Albans, VT). Nodes will also capture 1-3 video streams and process them in real time, for example performing compression and/or behavioral analysis (see below). Video frames will be synchronized with behavior, and transmitted to the cloud storage server(s). Finally, nodes will communicate wirelessly with our dynamic DBS devices (RO2), enabling automatic animal identification and experiment parameter selection and real-time modification. In addition, we will develop the cloud server software to upload behavioral programs
*/

#include <QApplication>
#include "mainwindow.h"
#include <QDateTime>
#include <dao/sqlitedatabasemanager.h>
#include <dao/taskfiledao.h>
#include <dao/taskfileobject.h>
#include <QFile>
#include <util/logger.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    Logger logger;
    logger.install();
    qDebug("Initializing OpenBBox Manager...");

    MainWindow w;
    w.show();

    return a.exec();
}
