#include <QtWidgets>

#include "pinconfigdialog.h"

PinconfigDialog::PinconfigDialog(QWidget *parent)
    : QWidget(parent)
{
    signalMapperin = new QSignalMapper(this);
    signalMapperout = new QSignalMapper(this);
}

void PinconfigDialog::setPanel(SenderTaskTCP *senderTasktcp){
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *page = new QWidget;
    QGridLayout *layout = new QGridLayout(page);

    layout->setColumnMinimumWidth(0, 70);
    layout->setColumnMinimumWidth(2, 70);

    int frameStyle = QFrame::Sunken | QFrame::Panel;

    for(int i = 1; i <= NUM_OUTPUTS; i++){
        QPushButton * outputbutton = new QPushButton(QString("OUT %1").arg(QString::number(i)));
        connect(outputbutton, SIGNAL(clicked()), signalMapperout, SLOT(map()));
        signalMapperout->setMapping(outputbutton, i);
        layout->addWidget(outputbutton, i-1, 1);
        if(outputPins.contains(i)){
            outputPins.value(i)->setFrameStyle(frameStyle);
            layout->addWidget(outputPins.value(i), i-1, 0);
        }
    }
    connect(signalMapperout, SIGNAL(mapped(int)), this, SLOT(setOutpin(int)));

    for(int i = 1; i <= NUM_INPUTS; i++){
        QPushButton * inputbutton = new QPushButton(QString("IN %1").arg(QString::number(i)));
        connect(inputbutton, SIGNAL(clicked()), signalMapperin, SLOT(map()));
        signalMapperin->setMapping(inputbutton, i);
        layout->addWidget(inputbutton, i-1, 3);
        if(inputPins.contains(i)){
            inputPins.value(i)->setFrameStyle(frameStyle);
            layout->addWidget(inputPins.value(i), i-1, 2);
        }
    }

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, SIGNAL(clicked()), signalMapperclose, SLOT(map()));
    signalMapperclose->setMapping(closeButton, this->keystream);
    //connect(signalMapperclose, SIGNAL(mapped(QObject),)
    layout->addWidget(closeButton, NUM_OUTPUTS-1, 3);

    mainLayout->addWidget(page);
    setWindowTitle(tr("Pin Test"));

}

void PinconfigDialog::setKeystream(QString key){
    keystream = key;
}

void PinconfigDialog::setInpin(int button){
    bool ok;
}

void PinconfigDialog::setOutpin(int button){
    bool ok;
    qDebug("dddddddddddd");
}


void PinconfigDialog::addOutputpins(QString pinName, int pin){
    QLabel * output = new QLabel;
    output->setText(pinName);
    outputPins.insert(pin, output);
}

void  PinconfigDialog::addInputpins(QString pinName, int pin){
    QLabel * input = new QLabel;
    input->setText(pinName);
    inputPins.insert(pin, input);
}
