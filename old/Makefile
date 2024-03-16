CXXFLAGS = -Wall -Wextra -std=c++11 -Og -g
LDFLAGS = -L/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sysroots/cortexa53-crypto-phytec-linux/usr/lib
#LDLIBS = -li2c
CPLUS_INCLUDE_PATH = -Ilib/json_helper -Ilib/jsoncpp/include -Ilib/cam_device/include -Ilib/cam_device/vvcam/common -Ilib/mediacontrol/include_api
BUILD_PATH=build
PROGRAM=hello

all: main.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $(CPLUS_INCLUDE_PATH) main.cpp -o ${BUILD_PATH}/${PROGRAM} && file ${BUILD_PATH}/${PROGRAM}
clean:
	rm -f ${BUILD_PATH}/${PROGRAM}
