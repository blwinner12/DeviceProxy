#include "MgsRtsp.h"


MgsRtsp::MgsRtsp()
{
}


MgsRtsp::~MgsRtsp()
{
}

std::string MgsRtsp::create_option(const MgsRtspState& rs)
{
	stringstream ss;
	ss << create_commond_headers(rs, OPTION);
	ss << RN;

	return ss.str();
}

std::string MgsRtsp::create_describe(const MgsRtspState& rs)
{
	stringstream ss;
	ss << create_commond_headers(rs, DESCRIBE);
	ss << RN;

	return ss.str();
}

std::string MgsRtsp::create_setup(const MgsRtspState& rs, bool isVideo)
{
	stringstream ss;
	//step 1 -- request header
	string uri;
	if (isVideo)
	{
		rs.video.uri.find("rtsp://") ? uri.assign(rs.uri).append(rs.video.uri) : (uri = rs.video.uri);
	}
	else
	{
		rs.audio.uri.find("rtsp://") ? uri.assign(rs.uri).append(rs.audio.uri) : (uri = rs.audio.uri);
	}
	ss << SETUP << " " << uri << " RTSP/1.0" << RN;
	//step 2 -- cseq
	ss << "CSeq: " << rs.cseq << RN;
	//step 3 -- user-Agent
	ss << "User-Agent: " << rs.user_agent << RN;
	//step 4 -- sessionID
	ss << "Session: " << rs.sessionID << RN;
	//step 5 --transport
	ss << "Transport: ";
	if (rs.sub_transport == RTSP_LOWER_TRANSPORT_UDP)
	{
		ss << "RTP/AVP;";
		ss << "client-port:" << (isVideo ? rs.video.crtp_port : rs.audio.crtp_port) << "-";
		ss << (isVideo ? rs.video.crtcp_port : rs.audio.crtcp_port) << ";";
	}
	else
	{
		ss << "RTP/AVP/TCP;";
		ss << "interleaved:" << (isVideo ? rs.video.crtp_port : rs.audio.crtp_port) << "-";
		ss << (isVideo ? rs.video.crtcp_port : rs.audio.crtcp_port) << ";";
	}
	
	ss << "unicast;" << RN;
	ss << RN;

	return ss.str();
}

std::string MgsRtsp::create_play(const MgsRtspState& rs)
{
	stringstream ss;
	ss << create_commond_headers(rs, PLAY);
	//step 7 -- range
	ss << "Range: npt=" << rs.range_start << "-" << rs.range_end << RN;
	ss << RN;

	return ss.str();
}
#define HAS_HEADER_EX(header_name) HAS_HEADER(strRtsp, header_name, posStart, posRN - posStart)

std::string MgsRtsp::create_teardown(const MgsRtspState& rs)
{
	stringstream ss;
	ss << create_commond_headers(rs, TEARDOWN);
	ss << RN;

	return ss.str();
}

int MgsRtsp::parse_rtsp_headers(const string& strRtsp, MgsRtspState &rs)
{
	if (!strRtsp.empty())
		return -1;
	MgsRtspState rsTmp{ 0 };
	int posRN = 0;
	int posStart = 0;
	int pkgLen = strRtsp.length();
	while (posRN < pkgLen)
	{
		posRN = strRtsp.find_first_of(RN, posStart);
		if (HAS_HEADER_EX("Session:"))
		{
			rsTmp.sessionID.assign(strRtsp.c_str() + posStart + 8, posRN - posStart - 9);
		}
		else if (HAS_HEADER_EX("Content-Length:"))
		{ 
			rsTmp.content_length = GET_SECTION_INT_VALUE(strRtsp, posStart, 15);
		}
		else if (HAS_HEADER_EX("Transport:"))
		{
			//parse_header_transport(package.substr())
		}
		else if (HAS_HEADER_EX("CSeq:"))
		{
			rsTmp.cseq = GET_SECTION_INT_VALUE(strRtsp, posStart, 5/*strlen("CSeq")*/);
		}
		else if (HAS_HEADER_EX("Range:"))
		{
			int tmpPos = strRtsp.find("npt", posStart + 6/*strlen("Range:")*/);		
			if (tmpPos != string::npos)
			{//npt time, fmt: npt:0.000-
				rsTmp.range_start = GET_SECTION_INT_VALUE(strRtsp, tmpPos, 0);
				tmpPos = strRtsp.find('-', posStart + 10);
				if (tmpPos != string::npos)
				{
					rsTmp.range_end = GET_SECTION_INT_VALUE(strRtsp, tmpPos, 0);
				}
			}
		}
		else if (HAS_HEADER_EX("RTP-Info:"))
		{

		}
		else if (HAS_HEADER_EX("Content-Type:"))
		{

		}
		else if (HAS_HEADER_EX("WWW-Authenticate:"))
		{

		}
		else if (HAS_HEADER_EX("Authentication-Info:"))
		{

		}
#ifndef NOT_NEED_NOW
		else if (HAS_HEADER_EX("RealChallenge1:"))
		{

		}
		else if (HAS_HEADER_EX("Server:"))
		{

		}
		else if (HAS_HEADER_EX("Location:"))
		{

		}
		else if (HAS_HEADER_EX("Content-Base:"))
		{

		}
		else if (HAS_HEADER_EX("Public:"))
		{

		}
		else if (HAS_HEADER_EX("x-Accept-Dynamic-Rate:"))
		{

		}
#endif

		posStart = posRN + 2/*strlen(RN)*/;
	}
	return 0;
}
int MgsRtsp::parse_header_transport(const string& vTransport, MgsRtspState &rs)
{
	int posStart = 0;
	int posEnd = 0;
	int pkgLen = vTransport.length();
	while (posEnd < pkgLen)
	{
		posEnd = vTransport.find_first_of(";", posStart);
		if (vTransport.find_first_of("RTP", posStart, posEnd) == 0)
		{
			rs.transport = RTSP_TRANSPORT_RTP;
			if (HAS_HEADER(vTransport, "TCP", posStart, posEnd))
			{
				rs.sub_transport = RTSP_LOWER_TRANSPORT_TCP;
			}
			else
			{
				rs.sub_transport = RTSP_LOWER_TRANSPORT_UDP;
			}
		}
		else if (HAS_HEADER(vTransport, "unicast", posStart, posEnd))
		{ 

		}
		else if (HAS_HEADER(vTransport, "interleaved", posStart, posEnd))
		{
			int p = vTransport.find_first_of("-", posStart + 11, posEnd);
			if (p != 0)
				return -1;

		}
		posStart = posEnd + 1/*strlen(";")*/;
	}
	return 0;
}

std::string MgsRtsp::create_commond_headers(const MgsRtspState& rs, const string& method)
{
	stringstream ss;
	//step 1 -- request header
	ss << method << " " << rs.uri << " RTSP/1.0" << RN;
	//step 2 -- cseq
	ss << "CSeq: " << rs.cseq << RN;
	//step 3 -- user-Agent
	ss << "User-Agent: " << rs.user_agent << RN;
	//step 4 -- hik dont need now
	char auth[512]{ 0 };
	//step 5 -- content-length
	if (!rs.content.empty())
	{
		ss << "Content-Length: " << rs.content.length() << RN;
	}
	//step 6 -- sessionID
	ss << "Session: " << rs.sessionID << RN;
	
	return ss.str();
}
