/******************************************************************************\
|* Copyright (c) 2020 by VeriSilicon Holdings Co., Ltd. ("VeriSilicon")       *|
|* All Rights Reserved.                                                       *|
|*                                                                            *|
|* The material in this file is confidential and contains trade secrets of    *|
|* of VeriSilicon.  This is proprietary information owned or licensed by      *|
|* VeriSilicon.  No part of this work may be disclosed, reproduced, copied,   *|
|* transmitted, or used in any way for any purpose, without the express       *|
|* written permission of VeriSilicon.                                         *|
|*                                                                            *|
\******************************************************************************/

#include <linux/videodev2.h>
#include <linux/media.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <memory.h>

#include "device.h"
#include "V4l2File.h"
#include "log.h"
#include "viv_video_kevent.h"

#define LOGTAG "V4l2File"

V4l2File* V4l2File::mInst = NULL;

V4l2File::~V4l2File() {
    closeVideoDevice(&fd);
}

int V4l2File::open() {
    if (fd < 0) {
        fd = openVideoDevice(0);
        if (fd >= 0) {
            int streamid = -1;
            ioctl(fd, VIV_VIDIOC_S_STREAMID, &streamid);
            return fd;
        }
        fd = openVideoDevice(1);
        if (fd >= 0) {
            int streamid = -1;
            ioctl(fd, VIV_VIDIOC_S_STREAMID, &streamid);
            return fd;
        }
    }
    ALOGE("Failed to open both video devices!\n");
    return fd;
}

uint64_t V4l2File::alloc(uint64_t size) {
    struct ext_buf_info buf = {0, size};
    ioctl(fd, VIV_VIDIOC_BUFFER_ALLOC, &buf);
    return buf.addr;
}

void V4l2File::free(uint64_t addr) {
    struct ext_buf_info buf = {addr, 0};
    ioctl(fd, VIV_VIDIOC_BUFFER_FREE, &buf);
}

void* V4l2File::mmap(uint64_t addr, uint64_t size) {
    return ::mmap(NULL, (int)size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);
}

void V4l2File::munmap(unsigned char* addr, uint64_t size) {
    ::munmap( addr, size);
    return;
}

V4l2File* V4l2File::inst() {
    if (mInst == NULL) {
        mInst = new V4l2File();
    }
    return mInst;
}
