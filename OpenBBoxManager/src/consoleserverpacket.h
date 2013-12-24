#ifndef CONSOLESERVERPACKET_H
#define CONSOLESERVERPACKET_H

#include "config.h"

#define COMMAND_PKT_DELIMITER	'#'

#define COMMAND_REQUEST_INFO                0
#define COMMAND_REQUEST_INFO_ANS            1
#define COMMAND_SET_PORTS                   2
#define COMMAND_SET_PORTS_ANS               3
#define COMMAND_START_VIDEO_STREAM          4
#define COMMAND_START_VIDEO_STREAM_ANS      5
#define COMMAND_STOP_VIDEO_STREAM           6
#define COMMAND_STOP_VIDEO_STREAM_ANS       7
#define COMMAND_START_BEHAVIOR_STREAM       8
#define COMMAND_START_BEHAVIOR_STREAM_ANS   9
#define COMMAND_STOP_BEHAVIOR_STREAM        10
#define COMMAND_STOP_BEHAVIOR_STREAM_ANS    11

#define COMMAND_STATUS_CONN                 12
#define COMMAND_STATUS_CONN_ANS             13


#define ACK_ERROR   0
#define ACK_OK      1

#define SIZE_COMMAND_HEADER    3

#define MAX_LABEL_SIZE      20
#define IP_ADDRESS_SIZE     4
#define MAC_ADDRESS_SIZE    6

#define NUMBER_OF_BEHAVIOR_SENDERS 1

#define BASELINE_PORT_NUMBER    50000

//PktCommandRequestInfo
typedef struct PktCommandRequestInfo_struct {
    uint8_t dummy;
} PktCommandRequestInfo;

typedef struct PktCommandRequestInfoANS_struct {
    uint8_t macAddress[MAC_ADDRESS_SIZE];
    uint8_t numCameras;
    char label[MAX_LABEL_SIZE];
} PktCommandRequestInfoANS;

//PktCommandSetPortsANS
typedef struct PktCommandSetPorts_struct {
    uint8_t    dummy;
    uint16_t   portBehaviorContext;
    uint16_t   portVideoStream[MAX_CAMERAS];
} PktCommandSetPorts;

typedef struct PktCommandSetPortsANS_struct {
    uint8_t ack;
} PktCommandSetPortsANS;

//PktCommandStartVideoStream
typedef struct PktCommandStartVideoStream_struct {
    uint8_t   videoStreamIndex;
} PktCommandStartVideoStream;

typedef struct PktCommandStartVideoStreamANS_struct {
    uint8_t ack;
} PktCommandStartVideoStreamANS;

//PktCommandStopVideoStream
typedef struct PktCommandStopVideoStream_struct {
    uint8_t   videoStreamIndex;
} PktCommandStopVideoStream;

typedef struct PktCommandStopVideoStreamANS_struct {
    uint8_t ack;
} PktCommandStopVideoStreamANS;

//PktCommandStartBehaviorStream
typedef struct PktCommandStartBehaviorStream_struct {
     uint8_t dummy;
} PktCommandStartBehaviorStream;

typedef struct PktCommandStartBehaviorStreamANS_struct {
    uint8_t ack;
} PktCommandStartBehaviorStreamANS;

//PktCommandStopBehaviorStream
typedef struct PktCommandStopBehaviorStream_struct {
     uint8_t dummy;
} PktCommandStopBehaviorStream;

typedef struct PktCommandStopBehaviorStreamANS_struct {
    uint8_t ack;
} PktCommandStopBehaviorStreamANS;

//PktCommandStopBehaviorStream
typedef struct PktCommandStatusConn_struct {
     uint8_t dummy;
} PktCommandStatusConn;

typedef struct PktCommandStatusConnANS_struct {
    uint8_t ack;
} PktCommandStatusConnANS;

typedef union CastPktCommands {
    PktCommandRequestInfo               pktCommandRequestInfo;
    PktCommandRequestInfoANS            pktCommandRequestInfoANS;
    PktCommandSetPorts                  pktCommandSetPorts;
    PktCommandSetPortsANS               pktCommandSetPortsANS;
    PktCommandStartVideoStream          pktCommandStartVideoStream;
    PktCommandStartVideoStreamANS       pktCommandStartVideoStreamANS;
    PktCommandStopVideoStream           pktCommandStopVideoStream;
    PktCommandStopVideoStreamANS        pktCommandStopVideoStreamANS;
    PktCommandStartBehaviorStream       pktCommandStartBehaviorStream;
    PktCommandStartBehaviorStreamANS    pktCommandStartBehaviorStreamANS;
    PktCommandStopBehaviorStream        pktCommandStopBehaviorStream;
    PktCommandStopBehaviorStreamANS     pktCommandStopBehaviorStreamANS;
    PktCommandStatusConn                pktCommandStatusConn;
    PktCommandStatusConnANS             pktCommandStatusConnANS;
    uint8_t * ptr;
} PktCommands;

typedef struct PktCommand_struct {
    uint8_t     delimiter;
    uint8_t     type;
    uint8_t     version;
    PktCommands pktCommands;
} PktCommand;


const int sizeCommands[20] = {
                                SIZE_COMMAND_HEADER + sizeof(PktCommandRequestInfo),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandRequestInfoANS),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandSetPorts),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandSetPortsANS),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStartVideoStream),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStartVideoStreamANS),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStopVideoStream),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStopVideoStreamANS),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStartBehaviorStream),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStartBehaviorStreamANS),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStopBehaviorStream),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStopBehaviorStreamANS),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStatusConn),
                                SIZE_COMMAND_HEADER + sizeof(PktCommandStatusConnANS)
                               };

#endif // CONSOLESERVERPACKET_H
