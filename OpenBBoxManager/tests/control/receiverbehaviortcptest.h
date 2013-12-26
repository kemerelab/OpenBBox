#ifndef RECEIVERBEHAVIORTCPTEST_H
#define RECEIVERBEHAVIORTCPTEST_H

#include <QObject>
#include <control/receiverbehaviortcp.h>
#include <QTcpSocket>
#include <QHostAddress>

class ReceiverBehaviorTCPTest : public QObject
{
    Q_OBJECT
public:
    explicit ReceiverBehaviorTCPTest();
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testConstructors();
    void testThread();
    void testKey();
};

#endif // RECEIVERBEHAVIORTCPTEST_H
