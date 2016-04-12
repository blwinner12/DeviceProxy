#include "DeviceProxyFactory.h"
#include "..\Hik\HikProxy.h"

DeviceProxyFactory::DeviceProxyFactory()
{
}


DeviceProxyFactory::~DeviceProxyFactory()
{
}


// factory func
DeviceProxy* DeviceProxyFactory::getInstance(string deviceType)
{
	if (deviceType.compare("hikvision") == 0 || deviceType.compare("Hikvision") == 0)
		return new HikProxy();
	return NULL;
}

void DeviceProxyFactory::releaseInstance(DeviceProxy *deviceProxy)
{
	if (deviceProxy)
		delete deviceProxy;
}
