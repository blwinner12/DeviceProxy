#include "HikProxy.h"

atomic<bool> HikProxy::m_bInit = false;

int HikProxy::initSDK()
{
	if (!m_bInit)
	{
		int res = NET_DVR_Init();
		if (res)
		{
			m_bInit = false;
		}
		else
		{
			m_bInit = true;
		}
		return res;
	}
	return 0;
}

void CALLBACK HikProxy::s_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	//NEED TEST:pUser is either all in one or not.
	HikProxy *pDevice = nullptr;
	if (pUser)
	{//get device pointer
		pDevice = static_cast<HikProxy*>(pUser);
	}
	else
	{
		return;
	}
	if (dwType == EXCEPTION_EXCHANGE)
	{//hik doc:sdk's heartbeat timeout, timespan is 2 minutes
		pDevice->m_bIsConnect = false;
	}
	else if (dwType == RESUME_EXCHANGE)
	{//hik doc:heartbeat resume
		pDevice->m_bIsConnect = true;
	}
}

void CALLBACK HikProxy::s_REALDATACALLBACK(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser)
{
	//send data
}

int HikProxy::login(const DeviceBaseInfo& dbi)
{
	int res = initSDK();
	if (res != 0)
	{
		return res;
	}
	res = NET_DVR_SetExceptionCallBack_V30(NULL, NULL, HikProxy::s_ExceptionCallBack, this);

	NET_DVR_DEVICEINFO_V30 di = { 0 };
	m_userID = NET_DVR_Login_V30((char*)m_deviceBaseInfo.IP.c_str(), m_deviceBaseInfo.port, (char*)m_deviceBaseInfo.username.c_str(),
		(char*)m_deviceBaseInfo.password.c_str(), &di);
	if (m_userID < 0)
	{
		return NET_DVR_GetLastError();
	}

	m_vPlayHandle.resize(di.byChanNum);

	m_bIsConnect = true;
	return res;
}

int HikProxy::logout()
{
	if (NET_DVR_Logout(m_userID))
	{
		m_userID = -1;
	}
	else
	{
		return NET_DVR_GetLastError();
	}
	return 0;
}

int HikProxy::realplayStart(int chID, int streamType)
{
	NET_DVR_PREVIEWINFO pvi = { chID, streamType};
	
	LONG playHandle = NET_DVR_RealPlay_V40(m_userID, &pvi, HikProxy::s_REALDATACALLBACK, this);
	if (playHandle < 0)
	{
		return NET_DVR_GetLastError();
	}

	if (streamType)
	{
		NET_DVR_MakeKeyFrameSub(m_userID, playHandle);
	}
	else 
	{
		NET_DVR_MakeKeyFrame(m_userID, playHandle);
	}

	return m_vPlayHandle[chID - 1] = playHandle;
}

int HikProxy::realplayStop(int chID)
{
	if (NET_DVR_StopRealPlay(m_vPlayHandle[chID - 1]))
	{
		m_vPlayHandle[chID - 1] = -1;
		return 0;
	}
	return NET_DVR_GetLastError();
}

