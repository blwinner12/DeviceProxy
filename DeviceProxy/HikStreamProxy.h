#pragma once

#include <string>
#include <sstream>
#include "..\base64\base64.h"

using namespace std;

class HikStreamInfo
{
public:
	string username;
	string password;	
	string ip;
	int port;
	int channelID;
	short streamType; //1:main 2:sub
};

class HikStreamProxy
{
public:
	HikStreamProxy();
	~HikStreamProxy();
private:
	//example:
	//hikUri1="rtsp://192.168.0.186:554/ISAPI/streaming/channels/101/" --main:1 channelID 01 main stream
	//hikUri1="rtsp://192.168.0.186:554/ISAPI/streaming/channels/102/" --sub:1 channelID 02 sub stream
	//hikUri2="rtsp://192.168.0.186:554/mpeg4/ch1/main/av_stream" -- 1 channelID
	//hikUri2="rtsp://192.168.0.186:554/mpeg4/ch1/sub/av_stream" -- 1 channelID
	const static string hikUri1;//"rtsp://%s:%d/ISAPI/streaming/channels/%d%02d/" -- %s:ip %d:port %d:channelID %02d:sub channelID
	const static string hikUri2;//"rtsp://%s:%d/mpeg4/ch%d/%s/av_stream" -- %s:ip %d:port %d:channelID %s:"main" or "sub"
	
private:
	string create_auth_in_uri(const string& username, const string& password);
public:
	string create_hikUri(const HikStreamInfo& streamInfo);
};

