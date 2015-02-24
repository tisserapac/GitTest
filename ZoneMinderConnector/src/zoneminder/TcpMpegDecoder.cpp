/*
 * TcpMpegDecoder.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#include "TcpMpegDecoder.hpp"

namespace zoneminder {

TcpMpegDecoder::TcpMpegDecoder(HttpClient* pHttpClientPtr) {
	_iCurrentContentLength = 0;
	_sCurrentImageTimeStamp = "";
	this->_pHttpClientPtr = pHttpClientPtr;
	_bDecodeEnable = false;
	_bStillProcessing = false;
	_iDecodedImageCount = 0;

	prevBytePtr = NULL;
	curBytePtr = NULL;
	dataStructPtr = NULL;
	i = 0;
}

int TcpMpegDecoder::decodeImageData(vector<char>* pCurrentImageData)
{
	int iResult = 1;
	_bDecodeEnable = true;
	_bStillProcessing = true;

	while (_bDecodeEnable)
	{
		if(i == 0)
		{
			dataStructPtr = (*_pHttpClientPtr).readBytes();
		}

		ostringstream strOtherThanImageData;

		/*dataStructPtr = (*_pHttpClientPtr).readBytes();
		ostringstream strOtherThanImageData;
		size_t i = 0;*/

		while (i < (*dataStructPtr).length)
		{
			curBytePtr = (*dataStructPtr).dataPtr + i;
			if (prevBytePtr != NULL && (*prevBytePtr) == -1 && (*curBytePtr) == -40) //start of jpeg
					{
				string metaDataStr = strOtherThanImageData.str();
				extractStreamHeaderData(metaDataStr);

				if (_iCurrentContentLength > ((*dataStructPtr).length - i)) {
					(*pCurrentImageData).insert((*pCurrentImageData).end(), prevBytePtr,(prevBytePtr + (*dataStructPtr).length - i + 1));
					i = (*dataStructPtr).length;
				} else {
					(*pCurrentImageData).insert((*pCurrentImageData).end(), prevBytePtr, (prevBytePtr + _iCurrentContentLength));
					i += _iCurrentContentLength - 1;
				}
			} else if ((*pCurrentImageData).size() > 0) {
				if ((*pCurrentImageData).size() < _iCurrentContentLength) {
					if (((*dataStructPtr).length - i) <= (_iCurrentContentLength - (*pCurrentImageData).size())) {
						(*pCurrentImageData).insert((*pCurrentImageData).end(),	curBytePtr, (curBytePtr + ((*dataStructPtr).length - i - 1 + 1)));
						i = (*dataStructPtr).length;
					} else {
						(*pCurrentImageData).insert((*pCurrentImageData).end(), curBytePtr, (curBytePtr + (_iCurrentContentLength - (*pCurrentImageData).size() - 1 + 1)));
						i += (_iCurrentContentLength - (*pCurrentImageData).size());
					}
				} else if ((*pCurrentImageData).size() == _iCurrentContentLength) {
					if ((*pCurrentImageData)[_iCurrentContentLength - 2] == -1 && (*pCurrentImageData)[_iCurrentContentLength - 1] == -39) //end of jpeg
									{
						//got jpeg
						_iDecodedImageCount++;
						_bDecodeEnable = false;
						break;

						//Image image(stream.getWidth(), stream.getHeight(), _vCurrentImageData, stream.getId(), _sCurrentImageTimeStamp, "");
						//queuePtr.push(image);
						//cout << "TcpMpegDecoder::startDecoding: Produced image from stream " << stream.getId() << " at " << _sCurrentImageTimeStamp << endl;
					} else //error
					{
						cerr << "TcpMpegDecoder:startDecoding: Error in decoding Mpeg stream." << std::endl;
					}
					//_vCurrentImageData.clear();
				}
			} else if ((*pCurrentImageData).size() == 0) {
				strOtherThanImageData << *curBytePtr;
				i++;
			} else {
				cerr << "TcpMpegDecoder:startDecoding: Error - cannot reach this part of the program." << std::endl;
			}
			prevBytePtr = curBytePtr;
		}

		i = 0;
	}





	return iResult;
}

void TcpMpegDecoder::extractStreamHeaderData(const string& streamHeaders) {
	bool extractedContentLength = false;
	bool extractedTimestamp = false;
	vector<string> subStrs;
	size_t strPos;
	boost::split(subStrs, streamHeaders, boost::is_any_of("\n\r"));
	for (std::vector<std::string>::iterator subStrPtr = subStrs.begin();
			subStrPtr != subStrs.end(); ++subStrPtr) {
		//TODO : substr can be simplified???
		if ((strPos = (*subStrPtr).find(CONT_LEN_HEADER)) != string::npos) {
			string dataStr;
			dataStr = (*subStrPtr).substr((strPos + CONT_LEN_HEADER.length()),
					((*subStrPtr).length() - CONT_LEN_HEADER.length()));
			_iCurrentContentLength = boost::lexical_cast<unsigned int>(dataStr);
			extractedContentLength = true;
		}

		if ((strPos = (*subStrPtr).find(CUR_TIME_HEADER)) != string::npos) {
			_sCurrentImageTimeStamp = (*subStrPtr).substr(
					(strPos + CUR_TIME_HEADER.length()),
					((*subStrPtr).length() - CUR_TIME_HEADER.length()));
			extractedTimestamp = true;
		} else //if the time stamp is not in the image header, set it to the image decoded time stamp
		{
			ostringstream oss;
			oss << time(0);
			_sCurrentImageTimeStamp = oss.str();
			extractedTimestamp = true;
		}
	}
	if (!(extractedContentLength && extractedTimestamp)) {
		std::cerr
				<< "TcpMpegDecoder:extractStreamHeaderData: Error - cannot reach this part of the program."
				<< std::endl;
	}
}

bool TcpMpegDecoder::isStillProcessing() {
	return _bStillProcessing;
}

TcpMpegDecoder::~TcpMpegDecoder() {
}

} /* namespace zoneminder */
