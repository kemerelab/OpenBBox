#include "serializablepacket.h"
#include <QObject>
#include <QDataStream>
#include <QMetaProperty>
#include <QVariant>

SerializablePacket::SerializablePacket()
{

}

QDataStream &operator<<(QDataStream &ds, const SerializablePacket &obj) {
    for(int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if(obj.metaObject()->property(i).isStored(&obj))
        {
            ds << obj.metaObject()->property(i).read(&obj);
        }
    }
    return ds;
}

QDataStream &operator>>(QDataStream &ds, SerializablePacket &obj) {
    QVariant var;
    for(int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if(obj.metaObject()->property(i).isStored(&obj))
        {
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }
    return ds;
}
