#include <QtWidgets>

#include "pinconfigdialog.h"

PinconfigDialog::PinconfigDialog(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget *page = new QWidget;
    signalMapperin = new QSignalMapper(this);
    signalMapperout = new QSignalMapper(this);
    QGridLayout *layout = new QGridLayout(page);
    layout->setColumnStretch(0, 1);
    layout->setColumnMinimumWidth(0, 150);
    layout->setColumnMinimumWidth(2, 150);

    int frameStyle = QFrame::Sunken | QFrame::Panel;

    for(int i = 0; i < NUM_OUTPUTS; i++){
        QLabel * output = new QLabel;
        output->setFrameStyle(frameStyle);
        QPushButton * outputbutton = new QPushButton(QString("OUTPUT %1").arg(QString::number(i+1)));
        connect(outputbutton, SIGNAL(clicked()), signalMapperout, SLOT(map()));
        signalMapperout->setMapping(outputbutton, i+1);
        outputPins.insert(i+1, output);
        layout->addWidget(outputbutton, i, 1);
        layout->addWidget(output, i, 0);
    }
    connect(signalMapperout, SIGNAL(mapped(int)), this, SLOT(setOutpin(int)));

    for(int i = 0; i < NUM_INPUTS; i++){
        QLabel * input = new QLabel;
        input->setFrameStyle(frameStyle);
        QPushButton * inputbutton = new QPushButton(QString("INPUT %1").arg(QString::number(i+1)));
        connect(inputbutton, SIGNAL(clicked()), signalMapperin, SLOT(map()));
        signalMapperin->setMapping(inputbutton, i+1);
        inputPins.insert(i+1, input);
        layout->addWidget(inputbutton, i, 3);
        layout->addWidget(input, i, 2);
    }
    connect(signalMapperin, SIGNAL(mapped(int)), this, SLOT(setInpin(int)));


    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *defaultButton = new QPushButton(tr("default"));

    connect(okButton, SIGNAL(clicked()), this, SLOT(sendSignal()));
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(setDefault()));

    layout->addWidget(okButton, 7, 3);
    layout->addWidget(defaultButton, 6, 3);

    mainLayout->addWidget(page);

    setWindowTitle(tr("Pin Configuration"));

}

char * PinconfigDialog::getPinconfig(){
    return this->pinconfig;
}

void PinconfigDialog::setOutpin(int button)
{
    bool ok;
    QString item = QInputDialog::getItem(this, QString("Output %1").arg(button),
                                         tr("Output:"), outpinname, 0, false, &ok);

    if (ok && !item.isEmpty())
    {
        outputPins.value(button)->setText(item);
        strcpy(pinconfig+(button-1)*20, item.toLatin1().data());

    }else {
        ;
    }
}

void PinconfigDialog::setInpin(int button)
{
    bool ok;
    QString item = QInputDialog::getItem(this, QString("Input %1").arg(button),
                                         tr("Input:"), inpinname, 0, false, &ok);

    if (ok && !item.isEmpty())
    {
        inputPins.value(button)->setText(item);
        strcpy(pinconfig+(button-1)*20+20*NUM_OUTPUTS, item.toLatin1().data());

    }else {

    }
}

void PinconfigDialog::sendSignal(){
    bool ok = true;

    for(int i = 0; i < NUM_OUTPUTS + NUM_INPUTS; i++ ){
        if(QString(pinconfig+i*20).size()==0){
            ok = false;
        }
    }
    if(ok){
        //emit processPinconfig();
        close();
    }
}

void PinconfigDialog::setDefault(){

    for(int i = 0; i < NUM_OUTPUTS; i++){
        outputPins.value(i+1)->setText(outpinname.at(i));
        strcpy(pinconfig+i*20, outpinname.at(i).toLatin1().data());
    }

    for(int i = 0; i < NUM_INPUTS; i++){
        inputPins.value(i+1)->setText(inpinname.at(i));
        strcpy(pinconfig+i*20+NUM_OUTPUTS*20, inpinname.at(i).toLatin1().data());
    }

}

QString PinconfigDialog::pinName(int pin){
    return inpinname.at(pin);
}
