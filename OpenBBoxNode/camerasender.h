#ifndef CAMERASENDER_H
#define CAMERASENDER_H

#include <QThread>
/*
 *  V4L2 video capture example, modified by Derek Molloy for the Logitech C920 camera
 *  Modifications, added the -F mode for H264 capture and associated help detail
 *
 *
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 *
 *      This program is provided with the V4L2 API
 * see http://linuxtv.org/docs.php for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include <semaphore.h> /* Semaphore */
#include <stdlib.h>
#include <sys/time.h>
#include "udpsender.h"
#include "../OpenBBoxManager/config.h"
#include <linux/videodev2.h>

#define DEBUG	1

#define LENGTH_BASE 512

#define MAX_BUF 64
#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define DEFAULT_DEVICE 		"/dev/video0"
#define DEFAULT_IPADDRESS 	"192.168.7.1"
#define DEFAULT_PORT 		50000
#define DEFAULT_WIDTH 		640
#define DEFAULT_HEIGHT 		480
//default format is MJPEG because is the the best supported
#define DEFAULT_FORMAT 		1

#define MAX_IMAGES_TO_SEND 	100

enum io_method {
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR
};

struct buffer {
        void   *start;
        size_t  length;
};

class CameraSender : public QThread
{
    Q_OBJECT
private:

    bool            stop;

    QString          ipaddress 	;
    int              udpport  	;
    QString          dev_name 	;
    uint16_t         width 		;
    uint16_t         height 	;
    uint8_t          format		;
    uint16_t         lenghtBuffer;
    enum io_method   io;
    int              fd;
    struct buffer    *buffers;
    unsigned int     n_buffers;
    int              out_buf;

    char start_packet[100];

    void *sendFrame(void *threadid);
    void process_image(const void *p, int size);
    int read_frame(void);
    void stop_capturing(void);
    void start_capturing(void);
    void uninit_device(void);
    void init_read(unsigned int buffer_size);
    void init_mmap(void);
    void init_userp(unsigned int buffer_size);
    void init_device(void);
    void close_device(void);
    bool open_device(void);

public:
    explicit CameraSender(QString device, QString ip, uint16_t port, uint8_t formatType, uint16_t width, uint16_t height);
    void startSender();
    void stopSender();

protected:
    void run();

signals:
    void processFramePacket(uchar * buffer, uint size, uint timeSec, uint timeUSec, uint  width, uint height, uint format);
public slots:

};

#endif // CAMERASENDER_H
