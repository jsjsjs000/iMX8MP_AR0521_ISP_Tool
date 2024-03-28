#include <stdio.h>
#include <stdlib.h>
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
#include <math.h>
#include <inttypes.h>

#include "json_helper.h"
#include "cam_device_api.hpp"
#include "viv_video_kevent.h"
#include "ioctl_cmds.h"

#define DEBUG

int fd;
int streamid = 0;

int open_video()
{
	int videoid = 0;
	char szFile[256] = {0};
	sprintf(szFile, "/dev/video%d", videoid);
	fd = ::open(szFile, O_RDWR | O_NONBLOCK);
	if (fd < 0)
	{
		printf("can't open video file %s", szFile);
		return 1;
	}
	v4l2_capability caps;
	int result = ::ioctl(fd, VIDIOC_QUERYCAP, &caps);
	if (result  < 0)
	{
		printf("failed to get device caps for %s (%d = %s)", szFile, errno, strerror(errno));
		return 1;
	}

#ifdef DEBUG
	printf("Open: %s \n", szFile);
	printf("Open: (fd=%d)\n", fd);
	printf("Open Device: %s (fd=%d)\n", szFile, fd);
	printf("  Driver: %s\n", caps.driver);
#endif

	if (strcmp((const char*)caps.driver, "viv_v4l2_device") == 0)
	{
		// printf("found viv video dev %s\n", szFile);
		int streamid = -1;
		::ioctl(fd, VIV_VIDIOC_S_STREAMID, &streamid);
	}
	else
	{
		printf("Open wrong type of viv video dev\n");
		return 1;
	}

	return 0;
}

#define VIV_CUSTOM_CID_BASE (V4L2_CID_USER_BASE | 0xf000)
#define V4L2_CID_VIV_EXTCTRL (VIV_CUSTOM_CID_BASE + 1)

bool viv_private_ioctl(const char *cmd, Json::Value& jsonRequest, Json::Value& jsonResponse)
{
	if (!cmd)
	{
		printf("cmd should not be null!");
		return false;
	}
	jsonRequest["id"] = cmd;
	jsonRequest["streamid"] = streamid;

	struct v4l2_ext_controls ecs;
	struct v4l2_ext_control ec;
	memset(&ecs, 0, sizeof(ecs));
	memset(&ec, 0, sizeof(ec));
	ec.string = new char[VIV_JSON_BUFFER_SIZE];
	ec.id = V4L2_CID_VIV_EXTCTRL;
	ec.size = 0;
	ecs.controls = &ec;
	ecs.count = 1;

	::ioctl(fd, VIDIOC_G_EXT_CTRLS, &ecs);

	strcpy(ec.string, jsonRequest.toStyledString().c_str());
#ifdef DEBUG
	printf("DEBUG out: %s\n", ec.string);
	// std::string inputString;
	// std::getline(std::cin, inputString);
#endif
	int ret = ::ioctl(fd, VIDIOC_S_EXT_CTRLS, &ecs);
	if (ret != 0)
	{
		printf("failed to set ext ctrl\n");
		goto end;
	}
	else
	{
		::ioctl(fd, VIDIOC_G_EXT_CTRLS, &ecs);
#ifdef DEBUG
		printf("DEBUG in: %s\n", ec.string);
#endif
		Json::Reader reader;
		reader.parse(ec.string, jsonResponse, true);
		delete[] ec.string;
		ec.string = NULL;
		return jsonResponse["result"].asInt() == 0;
		// return jsonResponse["MC_RET"].asInt(); // $$ - oryginalnie, ale nie działa
	}

end:
	delete ec.string;
	ec.string = NULL;
	return false;
}

bool set_cproc_brightness(int brightness)
{
	if (brightness > -128 && brightness < 128)
	{
		Json::Value jRequest, jResponse;
		if (!viv_private_ioctl(IF_CPROC_G_CFG, jRequest, jResponse))
			return false;

		jRequest = jResponse;
		jRequest[CPROC_BRIGHTNESS_PARAMS] = brightness;
		return viv_private_ioctl(IF_CPROC_S_CFG, jRequest, jResponse);
	}

	return false;
}

void get_cproc()
{
	Json::Value jRequest, jResponse;
	viv_private_ioctl(IF_CPROC_G_CFG, jRequest, jResponse);
printf("brightness = %d\n", jResponse[CPROC_BRIGHTNESS_PARAMS].asInt());
}

int main(int argc, char* argv[])
{
	printf("-------- iMX8MP_AR0521_ISP_Tool --------\n\n");

	if (open_video())
		return 1;

	if (argc >= 2)
	{
		int brightness = atoi(argv[1]);
		printf("new brightness=%d\n", brightness);
		set_cproc_brightness(brightness);
	}
	else
	{
		get_cproc();
	}

	close(fd);
	return 0;
}
