#ifndef PinconfigDialog_H
#define PinconfigDialog_H

#include <QWidget>
#include <QSignalMapper>
#include "obbnode.h"
#include "controller.h"
#include "config.h"
#include "sendertasktcp.h"


QT_BEGIN_NAMESPACE

class QCheckBox;
class QLabel;

QT_END_NAMESPACE

class PinconfigDialog : public QWidget
{
    Q_OBJECT

public:
    PinconfigDialog(QWidget *parent = 0);
    void setPanel(SenderTaskTCP * senderTasktcp);
    void setKeystream(QString key);
    void addOutputpins(QString pinName, int pin);
    void addInputpins(QString pinName, int pin);
    void closeEvent(QCloseEvent * x);


private slots:
    void setInpin(int button);
    void setOutpin(int button);
    void stopSender();

private:
    QString keystream;
    SenderTaskTCP * sender;
    QHash<int, QLabel *> outputPins;
    QHash<int, QLabel *> inputPins;
    QSignalMapper * signalMapperout;
    QSignalMapper * signalMapperin;
    QSignalMapper * signalMapperclose;

signals:
    void processOutpin(int output);
};

#endif
