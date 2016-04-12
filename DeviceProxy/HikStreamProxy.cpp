
#include <vector>

#include "HikStreamProxy.h"

const string HikStreamProxy::hikUri1 = "rtsp://%s:%d/ISAPI/streaming/channels/%d%02d/";
const string HikStreamProxy::hikUri2 = "rtsp://%s:%d/mpeg4/ch%02d/%s/av_stream/";

HikStreamProxy::HikStreamProxy()
{
}


HikStreamProxy::~HikStreamProxy()
{
}

std::string HikStreamProxy::create_auth_in_uri(const string& username, const string& password)
{
	string auth("?auth=");
	string srcData = username + ":" + password;
//	vector<char> dstData(AV_BASE64_DECODE_SIZE(srcData.length()));
//	if (av_base64_encode(dstData.data(), sizeof(dstData), (uint8_t*)srcData.c_str(), srcData.length()))
// 	{
// 		auth.append(dstData.data());
// 		return auth;
// 	}
// 	else
// 	{
		return "";
//	}
}

std::string HikStreamProxy::create_hikUri(const HikStreamInfo& streamInfo)
{
	string uri(hikUri2);
	uri.replace(uri.find_first_of("%s"), 2, streamInfo.ip);
	uri.replace(uri.find_first_of("%d"), 2, to_string(streamInfo.port));
	uri.replace(uri.find_first_of("%d"), 2, to_string(streamInfo.channelID));
	uri.replace(uri.find_first_of("%s"), 2, streamInfo.streamType == 1 ? "main" : "sub");
	uri.append(create_auth_in_uri(streamInfo.username, streamInfo.password));

	return uri;	
}
