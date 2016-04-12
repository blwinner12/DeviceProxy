#pragma once
#include <functional>

#define RUN_FUN(fun, ...) if((fun)) return (fun)(##__VA_ARGS__); else return -1;

using namespace std;

class DeviceBaseInfo
{
public:
	string username;
	string password;
	string IP;
	int    port;
};

class DeviceProxy
{
public:
	DeviceProxy();
	~DeviceProxy();
protected:
	DeviceBaseInfo m_deviceBaseInfo;
protected:
	function<int(const DeviceBaseInfo&)> m_fnLogin = nullptr;
	function<int()> m_fnLogout = nullptr;

public:
	int login(const DeviceBaseInfo& dbi){
		m_deviceBaseInfo = dbi;
		RUN_FUN(m_fnLogin, dbi);
	};
	int logout(){
		RUN_FUN(m_fnLogout);
	}
};

