#ifndef CONFIG_H
#define CONFIG_H
#include <QDebug>
#include <QCoreApplication>
#include <linux/videodev2.h>

#include <iostream>
#include <unistd.h>

#ifndef V4L2_PIX_FMT_H264
#define V4L2_PIX_FMT_H264 v4l2_fourcc('H', '2', '6', '4') /* H264 with start codes */
#endif


#define VERBOSE_LEVEL (1*100)

#define NOT_SUPPORTED       -1
#define DEFAULT             0
#define WAITING_OPEN        1
#define SUCCESS             2

#define N_MAX_NAME_FORMATS  	6
#define N_SUPPORTED_FORMATS  	3
#define FORMAT_YUV      0
#define FORMAT_MJPEG    1
#define FORMAT_H264     2
#define SUPPORTED_FORMATS  	{"YUV", "MJPEG", "H264"}

#define MIN_TO_STREAM 150

#define TYPE_SKIP_DEC     1
#define TYPE_SKIP_NOT_DEC 2

#define LENGTH_BASE 512

//TODO Must be a dynamic configuration
#define AVOID_OVERFLOW_SIZE         (100*1024)
#define QNT_MAX_FRAMES              300
#define SIZE_MAX_DEFAULT_FRAME      16000
#define SIZE_BUFFER_FRAMES          (QNT_MAX_FRAMES * SIZE_MAX_DEFAULT_FRAME)

//Control pkts defines

#define N_MAX_NAME_FORMATS      6
#define N_SUPPORTED_FORMATS     3
#define SUPPORTED_FORMATS               {"YUV", "MJPEG", "H264"}
#define SUPPORTED_FORMATS_CONST         {V4L2_PIX_FMT_RGB24, V4L2_PIX_FMT_MJPEG, V4L2_PIX_FMT_H264}
#define SUPPORTED_FORMATS_BUFFERSIZE 	{60 * 2 * LENGTH_BASE,  60 * 2 * LENGTH_BASE, 2 * LENGTH_BASE}

#define MAX_NODES_SUPPORTED 100
#define MAX_CAMERAS 3

// Console SERVER
#define PORT 20000
#define BACKLOG 5
#define LENGTH 512

#define NUM_INPUTS      3
#define NUM_OUTPUTS     8
#define NUM_TOTAL_PINS  (NUM_INPUTS + NUM_OUTPUTS)
#define dT 500000 //minimum time difference (us) between two input

//#define OUTPUTS  {30, 60, 31, 50, 48, 51, 5, 4, 3, 2, 49, 15, 20, 116, 7, 114}
//#define INPUTS {66, 67, 69, 68, 45, 44, 23, 26}

//#define OUTPUTS  {30, 31, 48, 5, 3, 49, 115, 20, 66, 67, 69, 68, 45, 44, 23, 26}
//#define INPUTS {60, 50, 51, 4, 2, 15, 14, 7}

#define OUTPUTS  {30, 31, 48, 5, 3, 49, 115, 20}
#define INPUTS {60, 50, 51}

const char supported_formats_str[N_SUPPORTED_FORMATS][N_MAX_NAME_FORMATS] = SUPPORTED_FORMATS;
const int supported_formats_int [N_SUPPORTED_FORMATS] = SUPPORTED_FORMATS_CONST;
const int supported_formats_bufferSize[N_SUPPORTED_FORMATS] = SUPPORTED_FORMATS_BUFFERSIZE;

const QString outputs_name = QString("House, Reward Pump, Reward, Click, Lever 1, Lever 2, Lever Light 1, Lever Light 2");
const QString inputs_name = QString("Lever 1 press, Head Detector, Lever 2 press");

#define DATABASE_SQLITE_PATH "openbboxdb.sqlite"
#define SQLITEDB_RESOUCE_FILE ":/resource/Database/OpenBBoxDB.sql"
#define MYSQLDB_RESOUCE_FILE ":/resource/Database/OpenBBoxDBMySQL.sql"

//":/resource/Database/openbboxdb.sqlite"
//"Database/openbboxdb.sqllite"

#endif // CONFIG_H
