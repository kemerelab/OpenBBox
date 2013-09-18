#ifndef GPIO_H
#define GPIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
/****************************************************************
* Constants
****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (60 * 1000) /* 3 seconds */
#define MAX_BUF 64

class GPIO
{
public:
    GPIO();

    static int gpio_fd_close(int fd);
    static int gpio_fd_open(unsigned int gpio);
    static int gpio_set_edge(unsigned int gpio, char *edge);
    static int gpio_get_value(unsigned int gpio, unsigned int *value);
    static int gpio_set_value(unsigned int gpio, unsigned int value);
    static int gpio_set_dir(unsigned int gpio, unsigned int out_flag);
    static int gpio_unexport(unsigned int gpio);
    static int gpio_export(unsigned int gpio);
};

#endif // GPIO_H
