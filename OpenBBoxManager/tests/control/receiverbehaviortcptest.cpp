#include <QString>
#include <QtTest>
#include "receiverbehaviortcptest.h"

ReceiverBehaviorTCPTest::ReceiverBehaviorTCPTest()
{
}

void ReceiverBehaviorTCPTest::initTestCase()
{
    ReceiverBehaviorTCPTest test;
    Q_UNUSED(test)
}

void ReceiverBehaviorTCPTest::cleanupTestCase()
{
}

void ReceiverBehaviorTCPTest::testConstructors()
{
    ReceiverBehaviorTCP *element = new ReceiverBehaviorTCP(3000);
    QVERIFY(element);
    QVERIFY(element->getPort() == 3000);
    delete element;
}

void ReceiverBehaviorTCPTest::testThread()
{
    u_int16_t port = 3000;
    BehaviorEventPacket packet;
    ReceiverBehaviorTCP *element = new ReceiverBehaviorTCP(port);
    QVERIFY(!element->isRunning());
    element->startServer(100);
    QVERIFY(element->isRunning());
    QTcpSocket socket;
    socket.connectToHost(QHostAddress::LocalHost, port);

    if (socket.waitForConnected( 2000 )) {
            qDebug("Client connected on address %s:%04d", "Localhost", socket.localPort() );
            socket.write((const char *)&packet, sizeof(BehaviorEventPacket));
            socket.flush();

    }

    element->stopServer();
    QTest::qWait(1000);
    QVERIFY(!element->isRunning());
    delete element;
}

void ReceiverBehaviorTCPTest::testKey()
{
    ReceiverBehaviorTCP *element = new ReceiverBehaviorTCP(3000);
    element->addKeySteam("key");
    QVERIFY(element->getKeyString().contains("key"));
    delete element;
}
