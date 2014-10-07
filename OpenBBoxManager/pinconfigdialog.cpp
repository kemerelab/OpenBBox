#include "pinconfigdialog.h"

PinconfigDialog::PinconfigDialog()
{
    signalMapperout = new QSignalMapper(this);
    layout = new QGridLayout(this);
    layout->setColumnMinimumWidth(0, 70);
    layout->setColumnMinimumWidth(2, 70);

    for(int i = 1; i <= NUM_OUTPUTS; i++){
        QPushButton * outputbutton = new QPushButton(QString("OUT %1").arg(QString::number(i)));
        outputButtons.insert(i, outputbutton);
        connect(outputbutton, SIGNAL(clicked()), signalMapperout, SLOT(map()));
        signalMapperout->setMapping(outputbutton, i);
        layout->addWidget(outputbutton, i-1, 1);
        QLabel * output = new QLabel();
        outputPins.insert(i, output);
        layout->addWidget(outputPins.value(i), i-1, 0);
    }
    connect(signalMapperout, SIGNAL(mapped(int)), this, SLOT(setOutpin(int)));

    for(int i = 1; i <= NUM_INPUTS; i++){
        QPushButton * inputbutton = new QPushButton(QString("IN %1").arg(QString::number(i)));
        inputButtons.insert(i, inputbutton);
        QLabel * input = new QLabel();
        inputPins.insert(i, input);
        layout->addWidget(inputPins.value(i), i-1, 2);
    }

    closeButton = new QPushButton(tr("Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    layout->addWidget(closeButton, NUM_OUTPUTS-1, 3);
    setWindowTitle(tr("Pin Test: %1").arg(keystream));
}

void PinconfigDialog::setPanel(SenderTaskTCP *senderoutputtcp, ReceiverBehaviorTCP *receiverinputtcp){

    this->sender = senderoutputtcp;
    this->receiver = receiverinputtcp;
    connect(receiver, SIGNAL(processInputEvent(int)), this, SLOT(InputHappenned(int)));

}

void PinconfigDialog::setKeystream(QString key){
    keystream = key;
}

void PinconfigDialog::InputHappenned(int pin){
    if(inputPins.contains(pin)){
        if(!inputPins.value(pin)->text().isEmpty()){
            qDebug(qPrintable(inputPins.value(pin)->styleSheet()));
            if(inputPins.value(pin)->styleSheet().contains("red")){
                inputPins.value(pin)->setStyleSheet("QLabel { color : black; }");
            }else if(inputPins.value(pin)->styleSheet().contains("black")){
                inputPins.value(pin)->setStyleSheet("QLabel { color : red; }");
            }
        }else{
            qDebug("Unassigned Input event at %d", pin);
        }
    }else{
        qDebug("What??");
    }
}

void PinconfigDialog::setOutpin(int button){
    sender->sendTestPinPacket(button);
}

void PinconfigDialog::addOutputpins(QString pinName, int pin){
    outputPins.value(pin)->setText(pinName);
    qDebug("pin %d name %s size %d", pin, qPrintable(pinName), outputPins.size());
    int frameStyle = QFrame::Sunken | QFrame::Panel;
    outputPins.value(pin)->setFrameStyle(frameStyle);

}

void PinconfigDialog::addInputpins(QString pinName, int pin){
    inputPins.value(pin)->setText(pinName);
    qDebug("pin %d name %s size %d", pin, qPrintable(pinName), inputPins.size());
    int frameStyle = QFrame::Sunken | QFrame::Panel;
    inputPins.value(pin)->setFrameStyle(frameStyle);
    inputPins.value(pin)->setStyleSheet("QLabel { color : black; }");
}

void PinconfigDialog::closeEvent(QCloseEvent *event)
{
    stopSender();
    emit processStopNodeTestSender(keystream);
    close();
}

void PinconfigDialog::stopSender(){
    setOutpin(0);
}

