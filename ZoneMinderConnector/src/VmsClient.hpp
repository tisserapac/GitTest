/*
 * VmsClient.hpp
 *
 *  Created on: Oct 7, 2014
 *      Author: Lakindu Boteju
 */

#ifndef VMSCLIENT_HPP_
#define VMSCLIENT_HPP_

#include <vector>

using namespace std;
class VmsClient
{
public:
	VmsClient() {}
	virtual ~VmsClient(){}
	virtual bool init() = 0;
	virtual int produceImageObject(vector<char>* pCurrentImageData) = 0;
};

// the type definitions for the class factories
typedef VmsClient* create_t();
typedef void destroy_t(VmsClient*);

#endif /* VMSCLIENT_HPP_ */
