all: bin/main
bin/main: bin/main.o bin/server_connection.o bin/request_handler.o bin/worker_thread.o bin/client_interface.o bin/client_connection.o bin/network_connection.o bin/service_request.o bin/globals.o
	g++ bin/main.o bin/server_connection.o bin/request_handler.o bin/worker_thread.o bin/client_interface.o bin/client_connection.o bin/network_connection.o bin/service_request.o bin/globals.o -lpthread -o bin/main
bin/main.o: src/main.cc src/server/server_connection.cc src/server/request_handler.cc src/client/client_interface.cc src/globals.cc src/service_request.cc
	g++ -c -g src/main.cc -o bin/main.o
bin/server_connection.o: src/server/server_connection.cc src/server/server_connection.h src/network_connection.cc src/globals.cc
	g++ -c -g src/server/server_connection.cc -o bin/server_connection.o
bin/request_handler.o: src/server/request_handler.cc src/server/request_handler.h src/globals.cc src/server/worker_thread.cc
	g++ -c -g src/server/request_handler.cc -o bin/request_handler.o
bin/worker_thread.o: src/server/worker_thread.cc src/server/worker_thread.h src/globals.cc src/service_request.cc
	g++ -c -g src/server/worker_thread.cc -o bin/worker_thread.o
bin/client_interface.o: src/client/client_interface.cc src/client/client_interface.h src/client/client_connection.h src/service_request.cc src/globals.cc
	g++ -c -g src/client/client_interface.cc -o bin/client_interface.o
bin/client_connection.o: src/client/client_connection.cc src/client/client_connection.h src/network_connection.cc src/service_request.cc
	g++ -c -g src/client/client_connection.cc -o bin/client_connection.o
bin/globals.o: src/globals.cc src/globals.h
	g++ -c -g src/globals.cc -o bin/globals.o
bin/network_connection.o: src/network_connection.cc src/network_connection.h
	g++ -c -g src/network_connection.cc -o bin/network_connection.o
bin/service_request.o: src/service_request.cc src/service_request.h
	g++ -c -g src/service_request.cc -o bin/service_request.o
clean:
	rm bin/*.o
	rm bin/main
