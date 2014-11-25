//
// worker_thread.h
//
// Author: Jordan Brown
// Date: Sep 6, 2014
//
// Handles connection reads/writes

#ifndef WORKER_THREAD_H_
#define WORKER_THREAD_H_

#include <string>
#include "../service_request.h"

namespace brown {
	class worker_thread {
	private:
		int connection;
		int port;
		service_request request;
		std::string client;
		bool exit;
		void handleRequest();
		void handleNewClientRequest();
		void handleClientExitRequest();
		void handleClientQueryRequest();
		void handleClientShareRequest();
		void handleClientSystemQueryRequest();
		void writeResponse(char* requestString, char* payload);
		void writeResponse(char* requestString, char* payload, char* visited);
		void buildClientString();
		void appendSharedNeighbors();
		bool tryAppendNeighbor(std::string neighbor);
	public:
		worker_thread(int connection, int port);
		void handleRequests();
	};
}

#endif /* WORKER_THREAD_H_ */
