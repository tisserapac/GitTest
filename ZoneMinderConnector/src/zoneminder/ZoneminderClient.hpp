/*
 * ZoneminderClient.hpp
 *
 *  Created on: Feb 3, 2015
 *      Author: anjana
 */

#ifndef ZONEMINDERCLIENT_HPP_
#define ZONEMINDERCLIENT_HPP_

#include <iostream>
#include <string>
#include "../VmsClient.hpp"
#include "../tcpsocket/HttpClient.hpp"
#include "TcpMpegDecoder.hpp"
#include "ZoneminderMessage.hpp"


using namespace tcpsocket;
using namespace std;

namespace zoneminder {

class ZoneminderClient : public VmsClient
{
private:
	TcpMpegDecoder* _pDecoderPtr;
	string _sUsername;
	string _sPassword;
	string _sServerName;
	int _iServerPort;
	HttpClient* _pHttpClientPtr;

public:
	ZoneminderClient();
	virtual ~ZoneminderClient();
	int produceImageObject(vector<char>* pCurrentImageData);
	bool init();
};

// the class factories
extern "C" VmsClient* create() {
    return new ZoneminderClient;
}

extern "C" void destroy(VmsClient* p) {
    delete p; p = 0;
}

} /* namespace zoneminder */

#endif /* ZONEMINDERCLIENT_HPP_ */
