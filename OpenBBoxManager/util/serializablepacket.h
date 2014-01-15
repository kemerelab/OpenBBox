#ifndef SERIALIZABLEPACKET_H
#define SERIALIZABLEPACKET_H

#include "serializableclass.h"

class SerializablePacket :  public  SerializableClass
{
    Q_OBJECT

    Q_PROPERTY(Type type READ getType WRITE setType)

public:
    typedef enum {
        CMD_REQUEST_INFO        =   0   ,
        CMD_REQUEST_INFO_ANS            ,
        CMD_SET_PORTS                   ,
        CMD_SET_PORTS_ANS               ,
        CMD_START_VIDEO_STREAM          ,
        CMD_START_VIDEO_STREAM_ANS      ,
        CMD_STOP_VIDEO_STREAM           ,
        CMD_STOP_VIDEO_STREAM_ANS       ,
        CMD_START_BEHAVIOR_STREAM       ,
        CMD_START_BEHAVIOR_STREAM_ANS   ,
        CMD_STOP_BEHAVIOR_STREAM        ,
        CMD_STOP_BEHAVIOR_STREAM_ANS    ,
        CMD_STATUS_CONN                 ,
        CMD_STATUS_CONN_ANS
    } Type;

    SerializablePacket();
    Type getType(){return type;}
    void setType(Type type){ this->type = type;}

protected:
    Type type;
};

#endif // SERIALIZABLEPACKET_H
