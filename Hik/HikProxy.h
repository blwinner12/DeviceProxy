#pragma once

#include <atomic>
#include "..\DeviceProxy\DeviceProxy.h"
#include "HCNetSDK.h"
#include <vector>

using namespace std;
class HikProxy : public DeviceProxy
{
public:
	HikProxy()
	{
		swap();
	};
	~HikProxy();
private:
	static atomic<bool> m_bInit;
private:
	bool m_bIsConnect = false;
	LONG m_userID = -1;
	vector<LONG> m_vPlayHandle;

private:
	int initSDK();
	void cleanSDK();
	void swap()
	{
		m_fnLogin = bind(&HikProxy::login, this, std::placeholders::_1);
		m_fnLogout = bind(&HikProxy::logout, this);
	}
public:
	static void CALLBACK s_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
	static void CALLBACK s_REALDATACALLBACK(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser);
private:
	int login(const DeviceBaseInfo& dbi);;
	int logout();
	int isConnect();
	//streamType:main-0 sub-1
	int realplayStart(int chID, int streamType);
	int realplayStop(int chID);
	//command:ptz - stop,speed  preset - stop=0,speed is preset index
	int ptzControl(int chID, int command, int stop, int speed);
};

