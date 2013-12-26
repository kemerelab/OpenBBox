#ifndef SERIALIZABLECLASS_H
#define SERIALIZABLECLASS_H

#include <QObject>
#include <QDataStream>
class SerializableClass : public QObject
{
    Q_OBJECT
public:
    explicit SerializableClass(QObject *parent = 0);

};
QDataStream &operator<<(QDataStream &ds, const SerializableClass &obj);
QDataStream &operator>>(QDataStream &ds, SerializableClass &obj) ;

#endif // SERIALIZABLECLASS_H
