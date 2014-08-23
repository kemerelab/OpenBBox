#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <stdio.h>

//#include "QVideoDecoder.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QHash>
#include <QStandardItemModel>
#include <QCryptographicHash>
#include "streampacket.h"
#include "controller.h"
#include "obbnode.h"
#include "config.h"
#include "controlwindow.h"
#include "dialog.h"
#include <dao/sqldatabase.h>
#include <dao/sqlitedatabasemanager.h>
#include <dao/mysqldatabasemanager.h>
#include <dao/taskfiledao.h>
#include <dao/subjectdao.h>
#include <dao/openbboxmanagerdao.h>
#include <dao/openbboxnodedao.h>
#include <dao/behaviortaskdao.h>
#include <dao/behavioreventpacketdao.h>
#include <dao/behaviortabledao.h>

#define RESOURCE_IMAGE_LOAD     ":/resource/Images/load.png"
#define RESOURCE_IMAGE_START    ":/resource/Images/play.png"
#define RESOURCE_IMAGE_STOP     ":/resource/Images/stop.png"
#define RESOURCE_IMAGE_CAMERA     ":/resource/Images/camera.png"
#define RESOURCE_IMAGE_CAMERARECORD ":/resource/Images/record.png"

#define MAX_ROWS_TABLE_EVENTS       500
#define MAX_COLUMNS_TABLE_EVENTS    sizeof(columns_name)/sizeof(QString)


//#define COLUMNS_NAME {QString("ID"), QString("Time"), QString("Time uS"), QString("Type"), QString("Pin"), QString("Context")}

#define COLUMNS_NAME {QString("Trial"), QString("Lever"), QString("Push"), QString("Motor Time"), QString("Rewards"), QString("Reward Time")}

const QString columns_name[] = COLUMNS_NAME;
const uint Outputs[NUM_OUTPUTS] = OUTPUTS;
const uint Inputs[NUM_INPUTS] = INPUTS;

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private slots:

    //add Node
    void addNodeList(OBBNode * node);
    //remove Node
    void removeNodeList(OBBNode * node);

    //Display video frame in player UI
    void updatePlayerUI(QImage img, QString info);

    void updatePlayerUIBuffer(uchar* buffer, uint size, uint type, uint width, uint height);

    //Slot for the load video push button.
    void on_listCameras_doubleClicked(const QModelIndex &index);

    void on_listUIServers_doubleClicked(const QModelIndex &index);

    void on_listUIServers_clicked(const QModelIndex &index);

    void on_listUIServers_itemSelectionChanged();

    void addNewEvent(QString key, BehaviorEventPacket packet);

    void addPacketDB(QString key, uint idtask, BehaviorEventPacket packet, uint port, long time);

    void on_startStopButton_clicked();

    void on_loadBtn_clicked();

    void passSubinfo(SubInfo sub);

    void on_actionMySQL_triggered();

    void on_actionSQLite_triggered();

    void on_actionControl_triggered();

    void on_actionAdd_New_Subject_triggered();

private:
    int idmanager;

    Ui::MainWindow *ui;
    SQLDatabase * sqldb;
    Controller * controller;
    BehaviorTaskPacket packet;
    int numberOfBStream;
    int numberOfVStream;
    int lastIndexLiveStream;
    QString lastBStream;
    int id_bt;

    QHash<QString, OBBNode *> mapNode;
    QHash<QString, ReceiverVideoUDP *> mapReceiver;
    QHash<QString, QStandardItemModel *> mapEventsStream;
    QHash<int, QString> tablecontext;

    ReceiverVideoUDP * receiverLiveStream;

    void updateRecordingButtonByList(QString key);
     // Decoder demo
    void image2Pixmap(QImage &img,QPixmap &pixmap);

    Dialog dialog;
    ControlWindow controlWindow;
    QAction * lastQAction;
};
#endif // MAINWINDOW_H
