#ifndef PinconfigDialog_H
#define PinconfigDialog_H

#include <QWidget>
#include <QSignalMapper>
#include "obbnode.h"
#include "config.h"

//^House 		= 1
//^Reward 	= 2
//^RewardLight 	= 3
//^Click 		= 4
//^LLever 	= 5
//^LLeverLight 	= 6
//^RLever 	= 7
//^RLeverLight 	= 8



#define PIN_NAME { QString("Click"), QString("Reward pump"), QString("Reward"), QString("Lever 1"), QString("Lever 2"), QString("Lever light 1"), QString("Lever light 2")}

QT_BEGIN_NAMESPACE

class QCheckBox;
class QLabel;

QT_END_NAMESPACE

const uint Outputs[NUM_OUTPUTS] = OUTPUTS;
const uint Inputs[NUM_INPUTS] = INPUTS;
class PinconfigDialog : public QWidget
{
    Q_OBJECT

public:
    PinconfigDialog(QWidget *parent = 0);
    char * getPinconfig();

private slots:

    void setInpin(int button);
    void setOutpin(int button);
    void sendSignal();
    void setDefault();

private:

    QHash<int, QLabel *> outputPins;
    QHash<int, QLabel *> inputPins;
    QSignalMapper * signalMapperout;
    QSignalMapper * signalMapperin;
    //INQSignalMapper * signalMapper;
    QStringList outpinname = outputs_name.split(", ");
    QStringList inpinname = inputs_name.split(", ");

    char       pinconfig[330];


signals:
    void processPinconfig();

};

#endif
