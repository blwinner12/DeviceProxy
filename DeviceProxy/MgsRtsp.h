#pragma once
#include <string>
#include <sstream>
#include "MgsDefines.h"

#define BASE64_SIZE(x)  (((x)+2) / 3 * 4 + 1)
#define RN "\r\n"
#define SPACE_CHARS " \t\r\n"
#define DESCRIBE "DESCRIBE"
#define SETUP "SETUP"
#define OPTION "OPTION"
#define PLAY "PLAY"
#define TEARDOWN "TEARDOWN"
#define NOT_NEED_NOW
#define GET_SECTION_INT_VALUE(parse_str, offset_start, offset_header) strtol((parse_str).c_str() + offset_start + (offset_header), nullptr, 10)
#define HAS_HEADER(parse_str, section, posStart, posEnd) (parse_str).find_first_of((section), posStart, posEnd)==0

using namespace std;

class VideoParameters
{
public:
	int crtp_port;
	int crtcp_port;
	int srtp_port;
	int srtcp_port;
	string uri;
};
class AudioParameters
{
public:
	int crtp_port;
	int crtcp_port;
	int srtp_port;
	int srtcp_port;
	string uri;
};

class MgsRtspState
{
public:
	int cseq;
	string sessionID;
	string uri;
	string content;
	string user_agent;
	int content_length;
	int range_start;
	int range_end;
	RTSPTransport transport;
	RTSPLowerTransport sub_transport;
	VideoParameters video;
	AudioParameters audio;
};

class MgsRtspReplyState
{
public:
	int result;
	string desc;
};


class MgsRtsp
{
public:
	MgsRtsp();
	~MgsRtsp();

	string create_option(const MgsRtspState& rs);
	string create_describe(const MgsRtspState& rs);
	string create_setup(const MgsRtspState& rs, bool isVideo);
	string create_play(const MgsRtspState& rs);
	string create_get_parameter();
	string create_set_parameter();
	string create_pause();
	string create_teardown(const MgsRtspState& rs);
	int parse_rtsp_package(const string& package, MgsRtspState &rs);
	int parse_rtsp_headline(const string& package, MgsRtspState &rs);
	int parse_rtsp_headers(const string& strRtsp, MgsRtspState &rs);

private:
	int parse_header_transport(const string& vTransport, MgsRtspState &rs);
	string create_commond_headers(const MgsRtspState& rs, const string& method);
};