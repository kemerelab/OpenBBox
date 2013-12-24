#ifndef MODELELEMENTTEST_H
#define MODELELEMENTTEST_H

#include <QObject>


class ReceiverBehaviorTCPTest : public QObject
{
    Q_OBJECT
public:
    explicit ReceiverBehaviorTCPTest(u_int16_t port);
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testConstructors();
};

#endif // MODELELEMENTTEST_H
