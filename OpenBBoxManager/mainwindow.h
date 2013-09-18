#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <stdio.h>
#include "QVideoDecoder.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QHash>
#include <QStandardItemModel>
#include "streampacket.h"
#include "controller.h"
#include "obbnode.h"
#include "config.h"
#include "controlwindow.h"
#include <dao/sqldatabase.h>
#include <dao/sqlitedatabasemanager.h>
#include <dao/mysqldatabasemanager.h>
#include <dao/taskfiledao.h>
#include <dao/openbboxmanagerdao.h>
#include <dao/openbboxnodedao.h>
#include <dao/behaviortaskdao.h>

#define RESOURCE_IMAGE_LOAD     ":/resource/Images/load.png"
#define RESOURCE_IMAGE_START    ":/resource/Images/play.png"
#define RESOURCE_IMAGE_STOP     ":/resource/Images/stop.png"

#define MAX_ROWS_TABLE_EVENTS       500
#define MAX_COLUMNS_TABLE_EVENTS    sizeof(columns_name)/sizeof(QString)

#define COLUMNS_NAME {QString("ID"), QString("Time"), QString("Time uS"), QString("Type"), QString("Pin"), QString("Context")}

const QString columns_name[] = COLUMNS_NAME;

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
    void on_actionControl_triggered();

    void on_listUIServers_doubleClicked(const QModelIndex &index);

    void on_startStopButton_clicked();

    void on_listUIServers_clicked(const QModelIndex &index);

    void addNewEvent(QList<QString> keys, BehaviorEventPacket packet);

    void on_loadBtn_clicked();

    void on_actionMySQL_triggered();

    void on_actionSQLite_triggered();

private:
    int idmanager;

    Ui::MainWindow *ui;
    SQLDatabase * sqldb;
    Controller * controller;
    int numberOfStream;
    int lastIndexLiveStream;

    QHash<QString, OBBNode *> mapNode;
    QHash<QString, ReceiverVideoUDP *> mapReceiver;
    QHash<QString, QStandardItemModel *> mapEventsStream;

    ReceiverVideoUDP * receiverLiveStream;

    void updateRecordingButtonByList(QString key);
     // Decoder demo
    void image2Pixmap(QImage &img,QPixmap &pixmap);

    ControlWindow controlWindow;

    QAction * lastQAction;
};
#endif // MAINWINDOW_H

