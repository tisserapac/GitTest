/*
 * TcpMpegDecoder.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#ifndef TCPMPEGDECODER_HPP_
#define TCPMPEGDECODER_HPP_

#include <vector>
#include "../tcpsocket/HttpClient.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace tcpsocket;

namespace zoneminder {

class TcpMpegDecoder {
private:
	//vector<char> _vCurrentImageData;
	unsigned int _iCurrentContentLength;
	string _sCurrentImageTimeStamp;
	HttpClient *_pHttpClientPtr;
	bool _bDecodeEnable;
	bool _bStillProcessing;
	const string CONT_LEN_HEADER = "Content-Length: ";
	const string CUR_TIME_HEADER = "Current: ";
	unsigned int _iDecodedImageCount;

	char* prevBytePtr;
	char* curBytePtr;
	ReceivedDataStruct* dataStructPtr;
	//ostringstream strOtherThanImageData;
	size_t i;;

	void extractStreamHeaderData(const string& streamHeaders);

public:
	TcpMpegDecoder(HttpClient* pHttpClient);
	int decodeImageData(vector<char>* pCurrentImageData);
	//void startDecoding(ThreadSafeQueue<Image>& queuePtr, Stream &stream, unsigned int imageCount);
	bool isStillProcessing();
	virtual ~TcpMpegDecoder();
};

} /* namespace zoneminder */

#endif /* TCPMPEGDECODER_HPP_ */
