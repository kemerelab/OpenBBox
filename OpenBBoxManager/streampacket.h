#ifndef STREAMPACKET_H
#define STREAMPACKET_H

#define CONTROL_PKT_DELIMITER	'#'

#define PACKET_START_TYPE	1
#define PACKET_CLOSE_TYPE	2

#define SIZE_CONTROL_HEADER     3
#define SIZE_START_PACKET       SIZE_CONTROL_HEADER + (sizeof(ControlPacketContext)/sizeof(u_int8_t))
#define SIZE_CLOSE_PACKET       SIZE_CONTROL_HEADER

#define VERSION 1

#define MAX_MPCTASKFILE 4500
#define NUM_PINS 11
#define PIN_CONFIGURATION 20 * NUM_PINS
#define MAX_STRING_CHAR 100

//#################### VIDEO STREAM PACKET ##########################
typedef struct Image_struct {
    uint id;
    uint size;
    u_int8_t * bufptr;
    u_int32_t  time;
    u_int32_t  time_usec;
    u_int16_t  width;
    u_int16_t  height;
    u_int8_t   format;
} Image;

typedef struct ControlPacketContext_struct {
    u_int32_t  id;
    u_int32_t  time;
    u_int32_t  time_usec;
    u_int32_t  size;
    u_int16_t  width;
    u_int16_t  height;
    u_int8_t   format;
    u_int16_t   checksumFrame;
    u_int16_t   checksum;
} ControlPacketContext ;

typedef struct ControlPacket_struct {
    u_int8_t   delimiter;
    u_int8_t   type;
    u_int8_t   version;
    ControlPacketContext pktControlContext;
} ControlPacket;

typedef union CastPktControl{
    ControlPacket * pktcontrol;
    u_int8_t * pktcontrolptr;
} PktControl;

//######################################################################
//#################### BEHAVIOR STREAM PACKET ##########################
#define PACKET_BEHAVIOR_EVENT   1
#define PACKET_BEHAVIOR_SHOW    2

typedef struct BehaviorEventContext_struct {
    u_int32_t  id;
    u_int32_t  time;
    u_int32_t  time_usec;
    u_int32_t  pinsContext;
    u_int32_t  typeEvent;
    u_int8_t   pin;
    u_int8_t   checksum;
} BehaviorEventContext ;

//Show Command
typedef struct BehaviorEventShow_struct {
    u_int8_t   string[MAX_STRING_CHAR];
} BehaviorEventShow;

//Pin Event
typedef struct BehaviorEventPacket_struct {
    u_int8_t   delimiter;
    u_int8_t   type;
    u_int8_t   version;
   // union{
        BehaviorEventContext pktBehaviorContext;
      //  BehaviorEventShow    pktBehaviorShow;
    //} pkt;
} BehaviorEventPacket;

//######################################################################
//#################### Task STREAM PACKET ##########################


typedef struct BehaviorTaskPacket_struct {
    u_int8_t   delimiter;
    u_int8_t   type;
    u_int8_t   version;
    u_int16_t  lines;
    char       file[MAX_MPCTASKFILE];

} BehaviorTaskPacket;


//######################################################################

#endif // STREAMPACKET_H
