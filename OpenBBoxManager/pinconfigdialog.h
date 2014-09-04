#ifndef PinconfigDialog_H
#define PinconfigDialog_H

#include <QWidget>
#include <QSignalMapper>
#include "obbnode.h"
#include "config.h"


QT_BEGIN_NAMESPACE

class QCheckBox;
class QLabel;

QT_END_NAMESPACE

class PinconfigDialog : public QWidget
{
    Q_OBJECT

public:
    PinconfigDialog(QWidget *parent = 0);
    char * getPinconfig();
    QString pinName(int pin);

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

    char       pinconfig[PIN_CONFIGURATION];
    //QHash<QString, int> pinsMap;


signals:
    void processPinconfig();

};

#endif
