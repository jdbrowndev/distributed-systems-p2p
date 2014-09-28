//
// service_request.h
//
// Author: Jordan Brown
// Date: Sep 3, 2014
//
// Data structure used for client/server
// communication

#ifndef SERVICE_REQUEST_H_
#define SERVICE_REQUEST_H_

namespace brown {
	struct service_request {
		char domainName[256];
		int portNumber;
		int requestType;
		char requestString[32];
		int requestId;
		char payload[256];
	};
}

#endif /* SERVICE_REQUEST_H_ */
