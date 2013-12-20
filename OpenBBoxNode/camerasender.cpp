#include "camerasender.h"

CameraSender::CameraSender(QString device, QString ip, uint16_t port, uint8_t formatType, uint16_t width, uint16_t height) :
    QThread()
{
    this->dev_name = device;
    this->ipaddress = ip;
    this->udpport = port;
    this->format = formatType;
    this->width = width;
    this->height = height;
    this->lenghtBuffer = LENGTH_BASE;
    this->io = IO_METHOD_MMAP;
    this->fd = -1;
    this->stop = false;
}

void errno_exit(const char *s)
{
        qCritical("%s error %d, %s", s, errno, strerror(errno));
        exit(EXIT_FAILURE);
}

int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}

void CameraSender::process_image(const void *p, int size)
{
    uchar * bufImage = (uchar*) malloc (size * sizeof(uchar));

    if (bufImage == NULL) {
        qCritical("ERROR: Not able to alloc");
        exit(EXIT_FAILURE);
    }

    memcpy(bufImage, p, size * sizeof(uchar));


    struct timeval tv;
    gettimeofday(&tv, NULL);

    emit processFramePacket(bufImage, size, (long)time(NULL), tv.tv_usec, this->width, this->height, this->format);
}

int CameraSender::read_frame(void)
{
        struct v4l2_buffer buf;
        unsigned int i;

        switch (io) {
        case IO_METHOD_READ:
                if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                                errno_exit("read");
                        }
                }

                process_image(buffers[0].start, buffers[0].length);
                break;

        case IO_METHOD_MMAP:
                CLEAR(buf);

                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;

                if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                                errno_exit("VIDIOC_DQBUF");
                        }
                }

                assert(buf.index < n_buffers);

                process_image(buffers[buf.index].start, buf.bytesused);

                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                        errno_exit("VIDIOC_QBUF");
                break;

        case IO_METHOD_USERPTR:
                CLEAR(buf);

                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_USERPTR;

                if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                                errno_exit("VIDIOC_DQBUF");
                        }
                }

                for (i = 0; i < n_buffers; ++i)
                        if (buf.m.userptr == (unsigned long)buffers[i].start
                            && buf.length == buffers[i].length)
                                break;

                assert(i < n_buffers);

                process_image((void *)buf.m.userptr, buf.bytesused);

                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                        errno_exit("VIDIOC_QBUF");
                break;
        }

        return 1;
}

void CameraSender::stop_capturing(void)
{
        enum v4l2_buf_type type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
                        errno_exit("VIDIOC_STREAMOFF");
                break;
        }
}

void CameraSender::start_capturing(void)
{
        unsigned int i;
        enum v4l2_buf_type type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
                for (i = 0; i < n_buffers; ++i) {
                        struct v4l2_buffer buf;

                        CLEAR(buf);
                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        buf.memory = V4L2_MEMORY_MMAP;
                        buf.index = i;

                        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                                errno_exit("VIDIOC_QBUF");
                }
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
                        errno_exit("VIDIOC_STREAMON");
                break;

        case IO_METHOD_USERPTR:
                for (i = 0; i < n_buffers; ++i) {
                        struct v4l2_buffer buf;

                        CLEAR(buf);
                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        buf.memory = V4L2_MEMORY_USERPTR;
                        buf.index = i;
                        buf.m.userptr = (unsigned long)buffers[i].start;
                        buf.length = buffers[i].length;

                        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                                errno_exit("VIDIOC_QBUF");
                }
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
                        errno_exit("VIDIOC_STREAMON");
                break;
        }
}

void CameraSender::uninit_device(void)
{
        unsigned int i;

        switch (io) {
        case IO_METHOD_READ:
                free(buffers[0].start);
                break;

        case IO_METHOD_MMAP:
                for (i = 0; i < n_buffers; ++i)
                        if (-1 == munmap(buffers[i].start, buffers[i].length))
                                errno_exit("munmap");
                break;

        case IO_METHOD_USERPTR:
                for (i = 0; i < n_buffers; ++i)
                        free(buffers[i].start);
                break;
        }

        free(buffers);
}

void CameraSender::init_read(unsigned int buffer_size)
{
        buffers = (struct buffer*)calloc(1, sizeof(struct buffer));

        if (!buffers) {
                qCritical("Out of memory");
                exit(EXIT_FAILURE);
        }

        buffers[0].length = buffer_size;
        buffers[0].start = malloc(buffer_size);

        if (!buffers[0].start) {
                qCritical("Out of memory");
                exit(EXIT_FAILURE);
        }
}

void CameraSender::init_mmap(void)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        qCritical() << dev_name << " does not support memory mapping";

                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2) {
                qCritical() << "Insufficient buffer memory on " + dev_name;
                exit(EXIT_FAILURE);
        }

        buffers = (struct buffer*)calloc(req.count, sizeof(struct buffer));

        if (!buffers) {
                qCritical("Out of memory");
                exit(EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                struct v4l2_buffer buf;

                CLEAR(buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

                if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
                        errno_exit("VIDIOC_QUERYBUF");

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start =
                        mmap(NULL /* start anywhere */,
                              buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd, buf.m.offset);

                if (MAP_FAILED == buffers[n_buffers].start)
                        errno_exit("mmap");
        }
}

void CameraSender::init_userp(unsigned int buffer_size)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count  = 4;
        req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        qCritical() << dev_name << " does not support user pointer i/o";
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_REQBUFS");
                }
        }

        buffers = (struct buffer*)calloc(4, sizeof(struct buffer));

        if (!buffers) {
            qCritical("Out of memory");
            exit(EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
                buffers[n_buffers].length = buffer_size;
                buffers[n_buffers].start = malloc(buffer_size);

                if (!buffers[n_buffers].start) {
                        qCritical("Out of memory");
                        exit(EXIT_FAILURE);
                }
        }
}

void CameraSender::init_device(void)
{
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;
        unsigned int min;

        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
                if (EINVAL == errno) {
                        qCritical() << dev_name << " is no V4L2 device";
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_QUERYCAP");
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                        qCritical() << dev_name << " is no video capture device";
                        exit(EXIT_FAILURE);
        }

        switch (io) {
        case IO_METHOD_READ:
                if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
                        qCritical() << dev_name << " does not support read i/o";
                        exit(EXIT_FAILURE);
                }
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                        qCritical() << dev_name << " does not support streaming i/o";
                        exit(EXIT_FAILURE);
                }
                break;
        }


        /* Select video input, video standard and tune here. */

        CLEAR(cropcap);

        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
                crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                crop.c = cropcap.defrect; /* reset to default */

                if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
                        switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                                break;
                        default:
                                /* Errors ignored. */
                                break;
                        }
                }
        } else {
                /* Errors ignored. */
        }


        CLEAR(fmt);

        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        // qCritical("Force Format %d", force_format);
        //if (force_format) {

       // if(force_format == 1){
            fmt.fmt.pix.width	= width;
            fmt.fmt.pix.height	= height;
            fmt.fmt.pix.pixelformat = supported_formats_int[format];
            fmt.fmt.pix.field	= V4L2_FIELD_INTERLACED;
        //}

                if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
                        errno_exit("VIDIOC_S_FMT");

                /* Note VIDIOC_S_FMT may change width and height. */
        //} else {
               /* Preserve original settings as set by v4l2-ctl for example */
       //         if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
       //                errno_exit("VIDIOC_G_FMT");
       // }

        /* Buggy driver paranoia. */
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;

        switch (io) {
        case IO_METHOD_READ:
                init_read(fmt.fmt.pix.sizeimage);
                break;

        case IO_METHOD_MMAP:
                init_mmap();
                break;

        case IO_METHOD_USERPTR:
                init_userp(fmt.fmt.pix.sizeimage);
                break;
        }
}

void CameraSender::close_device(void)
{
        if (-1 == close(fd))
                errno_exit("close");

        fd = -1;
}

bool CameraSender::open_device(void)
{
        struct stat st;

        if (-1 == stat(dev_name.toStdString().data(), &st)) {
                qCritical() << QString("Cannot identify '%1': %2, %3").arg(dev_name).arg(errno).arg(strerror(errno));
                return false;
        }

        if (!S_ISCHR(st.st_mode)) {
                qCritical() << dev_name << " is no device";
                return false;
        }

        fd = open(dev_name.toStdString().data(), O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd) {
                qCritical() << QString("Cannot open %1': %2, %3").arg(dev_name).arg(errno).arg(strerror(errno));
                return false;
        }
        return true;
}

void CameraSender::startSender() {
    this->stop = false;
    start(NormalPriority);
}

void CameraSender::stopSender() {
    this->stop = true;
}

void CameraSender::run() {

    unsigned int count;

    open_device();
    init_device();
    start_capturing();

    qDebug("Starting capture: Width: %d Height: %d Format: %s", this->width, this->height, supported_formats_str[format]);

    count = 0;

    UDPSender * udpSender = new UDPSender(this->ipaddress, this->udpport, this->format);
    QObject::connect(this, SIGNAL(processFramePacket(uchar*,uint,uint,uint,uint,uint,uint)),
                              udpSender, SLOT(sendFrame(uchar*,uint,uint,uint,uint,uint,uint)));

    udpSender->startSender();

    while (!stop) {
                for (;;) {
                            fd_set fds;
                            struct timeval tv;
                            int r;

                            FD_ZERO(&fds);
                            FD_SET(fd, &fds);

                            /* Timeout. */
                            tv.tv_sec = 2;
                            tv.tv_usec = 0;

                            r = select(fd + 1, &fds, NULL, NULL, &tv);

                            if (-1 == r) {
                                if (EINTR == errno)
                                        continue;
                                errno_exit("select");
                            }

                            if (0 == r) {
                                qCritical("select timeout");
                                exit(EXIT_FAILURE);
                            }

                            if (read_frame())
                                    break;
                            /* EAGAIN - continue select loop. */
                }
    }

    udpSender->stopSender();

    //free(udpSender);
    //delete udpSender;

    stop_capturing();
    uninit_device();
    close_device();

    qDebug() << QString("Video stream at %1 stopped").arg(this->dev_name);
}
