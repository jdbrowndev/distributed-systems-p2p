//
// request_handler.cc
//
// Author: Jordan Brown
// Date: Sep 4, 2014
//
// Accepts and handles client connections

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "../globals.h"
#include "worker_thread.h"
#include "request_handler.h"

namespace brown {

	request_handler::request_handler(int socketdesc, char* port): socketdesc(socketdesc),
			port(port) { }

	void request_handler::serviceRequests() {
		for(;;) {
			connection = accept(socketdesc, NULL, NULL);
			if (connection < 0) {
				std::cout << "request_handler: Error in accept" << std::endl;
				exit(0);
			} else {
				// create worker thread to handle connection
				pthread_t workerThread;
				worker_thread_args wargs;
				wargs.connection = connection;
				wargs.port = atoi(port);
				pthread_create(&workerThread, NULL, launchWorkerThread, (void*)&wargs);
			}
		}
	}

	void* request_handler::launchWorkerThread(void* args) {
		worker_thread_args wargs = *((worker_thread_args*)args);
		worker_thread worker(wargs.connection, wargs.port);
		worker.handleRequests();
		pthread_exit(0);
		return NULL;
	}
}
