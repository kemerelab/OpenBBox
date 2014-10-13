#ifndef PinconfigDialog_H
#define PinconfigDialog_H
#include <QtWidgets>
#include <QWidget>
#include <QSignalMapper>
#include "obbnode.h"
#include "controller.h"
#include "config.h"
#include "sendertasktcp.h"
#include "receiverbehaviortcp.h"


QT_BEGIN_NAMESPACE

class QCheckBox;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class PinconfigDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PinconfigDialog();
    void setPanel(SenderTaskTCP * senderoutputtcp, ReceiverBehaviorTCP * receiverinputtcp);
    void setKeystream(QString key);
    void addOutputpins(QString pinName, int pin);
    void addInputpins(QString pinName, int pin);
    void closeEvent(QCloseEvent * x);

private slots:
    void InputHappenned(int pin);
    void setOutpin(int button);
    void stopSender();

private:
    QString keystream;
    SenderTaskTCP * sender;
    ReceiverBehaviorTCP * receiver;
    QHash<int, QLabel *> outputPins;
    QHash<int, QLabel *> inputPins;
    QHash<int, QPushButton *> outputButtons;
    QHash<int, QLabel *> inputs;
    QSignalMapper * signalMapperout;
    QPushButton *closeButton;
    QGridLayout *layout;

signals:
    void processOutpin(int output);
    void processStopNodeTestSender(QString keystream);
};

#endif
