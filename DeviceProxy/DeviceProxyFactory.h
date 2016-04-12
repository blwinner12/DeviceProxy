#pragma once
#include <string>
#include "DeviceProxy.h"
using namespace std;
class DeviceProxyFactory
{
public:
	DeviceProxyFactory();
	~DeviceProxyFactory();
	// factory funcs
	static DeviceProxy* getInstance(string deviceType);
	static void releaseInstance(DeviceProxy *deviceProxy);
};

