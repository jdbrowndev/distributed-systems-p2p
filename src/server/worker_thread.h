//
// worker_thread.h
//
// Author: Jordan Brown
// Date: Sep 6, 2014
//
// Handles connection reads/writes

#ifndef WORKER_THREAD_H_
#define WORKER_THREAD_H_

#include "../service_request.h"

namespace brown {
	class worker_thread {
	private:
		int connection;
		int port;
		service_request request;
		bool exit;
		void readRequest();
		void writeResponse();
	public:
		worker_thread(int connection, int port);
		void handleRequests();
	};
}

#endif /* WORKER_THREAD_H_ */
