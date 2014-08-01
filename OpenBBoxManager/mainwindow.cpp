#include "mainwindow.h"
#include "ui_mainwindow.h"

void msg(QString msg){
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

MainWindow::MainWindow() :
    QMainWindow(),
    ui(new Ui::MainWindow)
{
    this->controller = new Controller();

    QObject::connect(controller, SIGNAL(processAddNodeList(OBBNode*)),
                              this, SLOT(addNodeList(OBBNode*)));

    QObject::connect(controller, SIGNAL(processRemoveNodeList(OBBNode*)),
                              this, SLOT(removeNodeList(OBBNode*)));

    QObject::connect(&dialog, SIGNAL(processPassSubinfo(SubInfo)),
                              this, SLOT(passSubinfo(SubInfo)));

    ui->setupUi(this);
    this->receiverLiveStream = NULL;
    this->numberOfStream = 0;
    this->lastIndexLiveStream = -1;

    QStandardItemModel *model = new QStandardItemModel(0, MAX_COLUMNS_TABLE_EVENTS, this); //2 Rows and 3 Columns
    uint j = 0;
    for(j = 0; j < MAX_COLUMNS_TABLE_EVENTS; j++){
        model->setHorizontalHeaderItem(j, new QStandardItem(columns_name[j]));
    }

    ui->tableEvents->setModel(model);

    this->controller->startService();

    qRegisterMetaType<BehaviorEventPacket>("BehaviorEventPacket");
    qRegisterMetaType<QList<QString> >("QList<QString>");

    sqldb = new SQLiteDatabaseManager(); //default
    if(!sqldb->exists())
        sqldb->create();

    OpenBBoxManagerDAO dao(sqldb);

    idmanager = dao.insert(new OpenBBoxManagerObject(QDateTime::currentDateTime().toTime_t()));

    if(idmanager < 0){
        msg("OpenBBox Manager: Error inserting new Manager`s instance.\n " + sqldb->lastError().text() + "\n\n Closing aplication");
        exit(EXIT_FAILURE);
    }

    ui->actionSQLite->setChecked(true);
    lastQAction = ui->actionSQLite;
}

void MainWindow::addNewEvent(QList<QString> keys, BehaviorEventPacket packet){
    int i = 0;
    for(i = 0; i < keys.size(); i++){
        QString key = keys.at(i);
        if(mapEventsStream.contains(key)) {
            if(mapEventsStream.value(key)->rowCount() >= MAX_ROWS_TABLE_EVENTS) {
                mapEventsStream.value(key)->takeRow(0);
            }

            QList<QStandardItem *> *info = new  QList<QStandardItem *>();
            info->push_back(new QStandardItem(QString::number(packet.pktBehaviorContext.id)));
            info->push_back(new QStandardItem(QString::number(packet.pktBehaviorContext.time)));
            info->push_back(new QStandardItem(QString::number(packet.pktBehaviorContext.time_usec)));
            info->push_back(new QStandardItem(QString::number(packet.pktBehaviorContext.typeEvent)));
            info->push_back(new QStandardItem(QString::number(packet.pktBehaviorContext.pin)));
            info->push_back(new QStandardItem(QString::number(packet.pktBehaviorContext.pinsContext)));

            if(info->size() == MAX_COLUMNS_TABLE_EVENTS){
                mapEventsStream.value(key)->appendRow(*info);
                ui->tableEvents->scrollToBottom();
            }else{
                qCritical("Error: Size of column events dont match");
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNodeList(OBBNode * node)
{
    char labelStr[sizeof("Stream xxxxxxx")];
    char tooltipStr[sizeof("Label: xxxxxxxxxxxxxxxxxxxxx IP: xxxxxxxxxxxxxxx Port: xxxxxx MAC: xx:xx:xx:xx:xx:xx Cam: xx")];
    struct sockaddr_in addr_remote;
    addr_remote.sin_addr.s_addr = (in_addr_t)node->getIPAdress();
    QString ip = inet_ntoa(addr_remote.sin_addr);

    for(int i = 0; i < node->getNumberOfVideoStream(); i++){
        sprintf(labelStr,"Stream %d", numberOfStream);
        QString key(labelStr);

        sprintf(tooltipStr,"Label: %s IP: %s Port:%d MAC: %s Cam: %d", node->getLabel(), qPrintable(ip), node->getVideoStream(i)->getPort(), node->getMacAddress(), i+1);
        QString tooltip(tooltipStr);
        numberOfStream++;
        QListWidgetItem * item = new QListWidgetItem(key);
        item->setToolTip(tooltip);
        item->setIcon(QIcon(RESOURCE_IMAGE_STOP));
        ui->listUIServers->addItem(item);
        mapReceiver.insert(labelStr, node->getVideoStream(i));
        mapNode.insert(labelStr, node);
        node->getVideoStream(i)->setKeyStream(labelStr);

        //Creating map of behavior events
        QStandardItemModel *model = new QStandardItemModel(0, MAX_COLUMNS_TABLE_EVENTS, this); //2 Rows and 3 Columns
        uint j = 0;
        for(j = 0; j < MAX_COLUMNS_TABLE_EVENTS; j++){
            model->setHorizontalHeaderItem(j, new QStandardItem(columns_name[j]));
        }
        node->getBehaviorStream()->addKeySteam(labelStr);
        mapEventsStream.insert(labelStr, model);
    }

    QObject::connect(node->getBehaviorStream(), SIGNAL(processAddNewEvent(QList<QString>,BehaviorEventPacket)),
                              this, SLOT(addNewEvent(QList<QString>,BehaviorEventPacket)));

    OpenBBoxNodeDAO dao(sqldb);
    int id = dao.insert(new OpenBBoxNodeObject(idmanager, QDateTime::currentDateTime().toTime_t(), 0, node->getLabel(), node->getMacAddress(), ip, node->getPortController(), node->getNumberOfVideoStream()));
    if(id != -1) {
        node->setIDDatabase(id);
    }else{
        msg("OpenBBox Manager: Error inserting new OpenBBoxNode`s instance.\n " + sqldb->lastError().text() + "\n\n Closing aplication");
    }
}

void MainWindow::removeNodeList(OBBNode * node)
{
    int i = 0;
    int j = 0;
    for(i = 0; i < node->getNumberOfVideoStream(); i++){

        mapReceiver.remove(node->getVideoStream(i)->getKeyStream());
        mapNode.remove(node->getVideoStream(i)->getKeyStream());

        mapEventsStream.remove(node->getVideoStream(i)->getKeyStream());
        for(j = 0; j < ui->listUIServers->count(); j++){
            QListWidgetItem* item = ui->listUIServers->item(j);
            if(item->text().compare(node->getVideoStream(i)->getKeyStream()) == 0){
                QListWidgetItem* itemToRemove = ui->listUIServers->takeItem(j);
                delete itemToRemove;
                break;
            }
        }

        if(receiverLiveStream ==  node->getVideoStream(i)){
            QObject::disconnect(receiverLiveStream, SIGNAL(processDisplayFrames(uchar*, uint, uint, uint, uint)),
                                  this, SLOT(updatePlayerUIBuffer(uchar*, uint, uint, uint, uint)));
            receiverLiveStream->setLiveStream(false);
            lastIndexLiveStream = -1;
        }

        node->getVideoStream(i)->stopServer();
        //delete node->getVideoStream(i);
    }

    node->getBehaviorStream()->stopServer();
   //delete node;
}

void MainWindow::updatePlayerUI(QImage img, QString info)
{
    (void)info;
    if (!img.isNull())
    {
        // Convert the QImage to a QPixmap for display
        QPixmap p;
        image2Pixmap(img,p);
        ui->label->setPixmap(p);
    }
}
//TODO [DecodeYUV] Improve method
QImage decodeYUV_impl(uint8_t * rawStreamingFrame, int width, int height, int size)
{
    //int sz = width * height;
    QImage img(width, height, QImage::Format_RGB888);
      img.fill(QColor(Qt::white).rgb());

    QImage rgbImage;

    /*  for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
          img.setPixel(x, y, qRgb(0, 0, 0));
        }
      }
    int i, j;
    int Y, Cr = 0, Cb = 0;
    for (j = 0; j < height; j++) {
        int pixPtr = j * width;
        int jDiv2 = j >> 1;
                    for (i = 0; i < width; i++) {
                        Y = fg[pixPtr];
                        if (Y < 0)
                            Y += 255;
                        if ((i & 0x1) != 1) {
                            int cOff = sz + jDiv2 * width + (i >> 1) * 2;
                            Cb = fg[cOff];
                            if (Cb < 0)
                                Cb += 127;
                            else
                                Cb -= 128;
                            Cr = fg[cOff + 1];
                            if (Cr < 0)
                                Cr += 127;
                            else
                                Cr -= 128;
                        }
                        int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
                        if (R < 0)
                            R = 0;
                        else if (R > 255)
                            R = 255;
                        int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1)
                                + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
                        if (G < 0)
                            G = 0;
                        else if (G > 255)
                            G = 255;
                        int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
                        if (B < 0)
                            B = 0;
                        else if (B > 255)
                            B = 255;
                        //rgb[pixPtr++] = (0xff000000 + (B << 16) + (G << 8) + R);
                        img.setPixel(i, j, qRgb(R, G, B));
                    }
    }*/

          // Print data size
          //qDebug() << "Print the data size from read file: " << size << " bytes...";
          // Get the raw data in the bytearray

          const char * binaryData = (char*)rawStreamingFrame;

          // Define the H and W of our output image ( we know it's 640 x 480 )
          // Create a QByteArray to store the RGB Data

          int redContainer[height*width];
          int greenContainer[height*width];
          int blueContainer[height*width];

          // To reconstruct pixels in pairs, 4 bytes of data are required. The bytes are arranged as u, y1, v, y2. The total number of bytes in the image stream is 2 x width x height.

          //qDebug() << "Looping through... " << size-4;

          // Loop through 4 bytes at a time

          int cnt = -1;

          for ( int i = 0 ; i <= size-4 ; i += 4 ) {

             // Extract yuv components

             int u  = (int)binaryData[i];
             int y1 = (int)binaryData[i+1];
             int v  = (int)binaryData[i+2];
             int y2 = (int)binaryData[i+3];

             // Define the RGB

             int r1 = 0 , g1 = 0 , b1 = 0;
             int r2 = 0 , g2 = 0 , b2 = 0;

             // u and v are +-0.5
             u -= 128;
             v -= 128;
             // Conversion

             r1 = y1 + 1.403*v;
             g1 = y1 - 0.344*u - 0.714*v;
             b1 = y1 + 1.770*u;
             r2 = y2 + 1.403*v;
             g2 = y2 - 0.344*u - 0.714*v;
             b2 = y2 + 1.770*u;

             // Increment by one so we can insert

             cnt+=1;

             // Append to the array holding our RGB Values

             redContainer[cnt] = r1;
             greenContainer[cnt] = g1;
             blueContainer[cnt] = b1;

             // Increment again since we have 2 pixels per uv value
             cnt+=1;
             // Store the second pixel

             redContainer[cnt] = r2;
             greenContainer[cnt] = g2;
             blueContainer[cnt] = b2;

          }

          // Define a QImage for our RGB Data
          rgbImage = QImage(width, height, QImage::Format_RGB888);
          // Print constructing RGB image
          //qDebug() << "Now constructing RGB Image... ";
          // Now construct our RGB image

          int pixelCounter = -1;
          for ( int i = 0; i < height; ++i ) {
              for ( int j = 0; j < width; ++j ) {
                  pixelCounter+=1;
                  rgbImage.setPixel( j, i, qRgb( redContainer[pixelCounter] ,  greenContainer[pixelCounter] , blueContainer[pixelCounter] ) );
              }
          }

          // Print pixel counter
          //qDebug() << "Counted number of pixels: " << pixelCounter;

    return rgbImage;
}

const u_int8_t huffman_table[] = {
            0xFF,0xC4,0x01,0xA2,0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,
            0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,
            0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x01,0x00,0x03,
            0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
            0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
            0x0A,0x0B,0x10,0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,
            0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,0x02,0x03,0x00,0x04,
            0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,
            0x71,0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,
            0x52,0xD1,0xF0,0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,
            0x18,0x19,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,0x34,0x35,0x36,
            0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,
            0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,
            0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,
            0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,
            0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,
            0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,
            0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,
            0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
            0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,
            0xFA,0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,
            0x04,0x04,0x00,0x01,0x02,0x77,0x00,0x01,0x02,0x03,0x11,0x04,
            0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,
            0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,
            0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,
            0xF1,0x17,0x18,0x19,0x1A,0x26,0x27,0x28,0x29,0x2A,0x35,0x36,
            0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,
            0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,
            0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,
            0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,
            0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
            0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,
            0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,
            0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
            0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA};

QByteArray fixJpegImage(QByteArray * array){
    QByteArray ret;

    if((0xFF & array->at(0)) != 0xFF && (0xFF & array->at(1)) != 0xD8)
        return NULL;

    ret.append(array->mid(0,2));
    array->remove(0,2);

    bool has_dht = false;
    QByteArray aux;
    while(!has_dht){
        aux = array->mid(0,4);
        array->remove(0,4);
        if ((0xFF & aux.at(0)) != 0xff)
            return NULL;

        if((0xFF & aux.at(1)) == 0xc4)
            has_dht = true;
        else if ((0xFF & aux.at(1)) == 0xda)
            break;

        //Skip to the next marker.
        int size = (((0xFF &aux.at(2))) << 8) + (0xFF & aux.at(3));
        ret.append(aux);
        ret.append(array->mid(0, size-2));
                array->remove(0, size-2);
    }

    if(!has_dht){
        ret.append((char*)huffman_table, sizeof(huffman_table)/sizeof(u_int8_t));
        ret.append(aux);
    }

    ret.append(*array);

    return ret;
}

void MainWindow::updatePlayerUIBuffer(uchar* buffer, uint size, uint type, uint width, uint height)
{
    QPixmap pix;
    QImage img;
    QByteArray aux;
    switch(type){
        case FORMAT_YUV:
                img = decodeYUV_impl(buffer, width, height, size);
                if(!img.isNull())
                {
                  // Convert the QImage to a QPixmap for display
                  image2Pixmap(img, pix);
                  ui->label->setPixmap(pix);
                }
        break;
        case FORMAT_MJPEG:
                aux.append((char*)buffer, size);
                pix.loadFromData(fixJpegImage(&aux), "JPEG");
                if (!pix.isNull())
                {
                    // Convert the QImage to a QPixmap for display
                    ui->label->setPixmap(pix);
                }else{
                    qCritical("Null image. Fail to converte Jpeg to Qimage");
                }
        break;
        case FORMAT_H264:
        default:
            qCritical("Error: Format not supported to display");
        break;
    }

    free(buffer);
}

void MainWindow::image2Pixmap(QImage &img, QPixmap &pixmap)
{
        // Convert the QImage to a QPixmap for display
        pixmap = QPixmap(img.size());
        QPainter painter;
        painter.begin(&pixmap);
        painter.drawImage(0,0,img);
        painter.end();
}

void MainWindow::on_actionControl_triggered()
{
    controlWindow.show();
}

void MainWindow::on_listUIServers_doubleClicked(const QModelIndex &index)
{
    if(ui->listUIServers->selectedItems().count() == 1) {
        const QString& key = ui->listUIServers->currentItem()->text();
        if (!key.isEmpty()) {
                    if(receiverLiveStream != NULL) {
                       if(lastIndexLiveStream > -1) {
                           ui->listUIServers->item(lastIndexLiveStream)->setTextColor(QColor("Black"));
                       }

                       QObject::disconnect(receiverLiveStream, SIGNAL(processDisplayFrames(uchar*, uint, uint, uint, uint)),
                                              this, SLOT(updatePlayerUIBuffer(uchar*, uint, uint, uint, uint)));
                       receiverLiveStream->setLiveStream(false);
                    }

                    receiverLiveStream = mapReceiver.value(key);
                    QObject::connect(receiverLiveStream, SIGNAL(processDisplayFrames(uchar*, uint, uint, uint, uint)),
                                              this, SLOT(updatePlayerUIBuffer(uchar*, uint, uint, uint, uint)));
                    receiverLiveStream->setLiveStream(true);

                    ui->listUIServers->item(index.row())->setTextColor(QColor("Blue"));
                    lastIndexLiveStream = index.row();

                    ui->tableEvents->setModel(mapEventsStream.value(key));
            }else{
                QMessageBox msgBox;
                msgBox.setText("Unable to find in list");
                msgBox.exec();
            }
    }else{
            QMessageBox msgBox;
            msgBox.setText("OpenBBox Node not selected");
            msgBox.exec();
    }
}

void MainWindow::updateRecordingButtonByList(QString key){
    if(mapReceiver.value(key)->getRecording()){
        ui->startStopButton->setIcon(QIcon(RESOURCE_IMAGE_STOP));
        ui->startStopButton->setIconSize(QSize(30,30));
    }else{
        ui->startStopButton->setIcon(QIcon(RESOURCE_IMAGE_START));
        ui->startStopButton->setIconSize(QSize(30,30));
    }
}

void MainWindow::on_startStopButton_clicked()
{
     if(ui->listUIServers->selectedItems().count() > 0) {
        QList<QListWidgetItem* > list = ui->listUIServers->selectedItems();
        int i = 0;
        for(i = 0; i < list.size(); i++){
           QString key = list.at(i)->text();
            if (!key.isEmpty()) {
                if(mapReceiver.value(key)->getRecording()){
                    mapReceiver.value(key)->stopRecording();
                    list.at(i)->setIcon(QIcon(RESOURCE_IMAGE_STOP));
                }else{
                    if(controller->startOBBNodeTask(mapNode.value(key),packet)) {
                        BehaviorTaskDAO dao(sqldb);
                        dao.insert(new BehaviorTaskObject(mapNode.value(key)->getIDDatabase(), 0, mapNode.value(key)->getCurrentTask(), QDateTime::currentDateTime().toTime_t(), 0, ""));
                        list.at(i)->setIcon(QIcon(RESOURCE_IMAGE_START));
                    }
                    else{
                        msg("Missing Task or Subject Info!!");
                    }
                }
                updateRecordingButtonByList(key);
            }
        }
    }else{
         msg("OpenBBox Node not selected");
    }
}

void MainWindow::on_listUIServers_clicked(const QModelIndex &index)
{
     updateRecordingButtonByList(ui->listUIServers->item(index.row())->text());
}

void MainWindow::on_loadBtn_clicked()
{
   if(ui->listUIServers->selectedItems().count() > 0) {

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("Files (*.MPC)"));
        if(fileName != ""){
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Can't open file";
                return;
            }
            QByteArray all = file.readAll();

            QByteArray hash = QCryptographicHash::hash(all,QCryptographicHash::Md5);
            qDebug() << "hash" << hash.toHex();

            int idtaskfile = -1;
            TaskFileDAO tf(sqldb);
            OBBNode * node;


            if(!tf.fileExists(hash)) {
                idtaskfile = tf.insert(new TaskFileObject(file.fileName().section("/",
                                                          file.fileName().count("/"),
                                                          file.fileName().count("/")),
                                                          QDateTime::currentDateTime().toTime_t(),
                                                          "test",
                                                          file.fileName().section(".", 1, 1),
                                                          all, hash.toHex()));
                if( idtaskfile > -1) {
                     msg("Added new task file to database");
                }
            } else {
                idtaskfile = tf.get("hash", hash.toHex()).at(0)->getID();
                msg("Task file already exists in database");
            }

            QList<QListWidgetItem* > list = ui->listUIServers->selectedItems();
            for(int i = 0; i < list.size(); i++){
                node = this->mapNode.value(list.at(i)->text());
                node->setCurrentTask(idtaskfile);

                packet.delimiter = CONTROL_PKT_DELIMITER;
                packet.type = 0;
                packet.version = VERSION;

                QByteArray fileStream = tf.get(idtaskfile).at(0)->getFile();
                QTextStream in(&fileStream,QIODevice::ReadOnly);
                int j = 0;
                int k = 0;
                int l;
                const char *c_str;
                while ( !in.atEnd())
                {
                  QString line = in.readLine();
                  l = line.size();
                  QByteArray qba = line.toLatin1();
                  c_str = qba.data();
                  strcpy(packet.file+j, c_str);
                  j = j+l+1;
                  k++;
                }
                qDebug("Task size: %d",j);
                packet.lines = k;
            }
        }else
           msg("MPC file not selected");
   }else{
        msg("OpenBBox Node not selected");
   }
}

void MainWindow::on_actionSQLite_triggered()
{
    sqldb = new SQLiteDatabaseManager(); //default
    lastQAction->setChecked(false);
    lastQAction = ui->actionSQLite;
    lastQAction->setChecked(true);

    if(!sqldb->exists())
        sqldb->create();

    OpenBBoxManagerDAO dao(sqldb);

    idmanager = dao.insert(new OpenBBoxManagerObject(QDateTime::currentDateTime().toTime_t()));

    if(idmanager < 0){
        msg("OpenBBox Manager: Error inserting new Manager`s instance.\n " + sqldb->lastError().text() + "\n\nClosing application....");
        exit(EXIT_FAILURE);
    }
}

void MainWindow::on_actionMySQL_triggered()
{
    sqldb = new MySQLDatabaseManager(); //default
    lastQAction->setChecked(false);
    lastQAction = ui->actionMySQL;
    lastQAction->setChecked(true);

    if(!sqldb->exists())
        sqldb->create();

    OpenBBoxManagerDAO dao(sqldb);

    idmanager = dao.insert(new OpenBBoxManagerObject(QDateTime::currentDateTime().toTime_t()));

    if(idmanager < 0){
        msg("OpenBBox Manager: Error inserting new Manager`s instance.\n " + sqldb->lastError().text() + "\n\nClosing application....");
        exit(EXIT_FAILURE);
    }
}

void MainWindow::on_subinfobutton_clicked()
{
    if(ui->listUIServers->selectedItems().count() > 0) {
        dialog.show();
    }else{
         msg("OpenBBox Node not selected");
    }
}

void MainWindow::on_listUIServers_itemSelectionChanged()
{
    if(ui->listUIServers->selectedItems().count() == 1) {

        QString nnode = ui->listUIServers->selectedItems().at(0)->text();
        OBBNode * node = mapNode.value(nnode);
        if(node->getSubject().status){
            ui->subinfobutton->setText("Rat is Ready!");
        }else{
            ui->subinfobutton->setText("Not Ready...");
        }
    }else{
        ui->subinfobutton->setText("subject info");
    }
}

void MainWindow::passSubinfo(SubInfo sub){

    if(ui->listUIServers->selectedItems().count() == 1) {

        QString nnode = ui->listUIServers->selectedItems().at(0)->text();
        mapNode.value(nnode)->setSubject(sub);
        qDebug("subject tag: %s",qPrintable(mapNode.value(nnode)->getSubject().name));
        int idsub;
        SubjectDAO subDAO(sqldb);
        idsub = subDAO.insert(new SubjectObject(sub.name,"","","",QDateTime::currentDateTime().toTime_t(),0,0,0));
        qDebug("idsub: %d", idsub);

    }

}
