CC = g++
# -Wall prints compiler warnings; -g adds debug info to object files; -MD generates .d files
CFLAGS = -Wall -g -MD
LIBS = -lpthread
ODIR = obj
SDIR = src
INC = -Iinc 
OUT = bin/peer

_OBJS = main.o server_connection.o request_handler.o worker_thread.o client_interface.o client_connection.o \
	graph_traversal.o network_connection.o neighbors_vector.o print_helper.o neighbor_serializer.o \
	service_request.o file_manager.o globals.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS)) # Prefixes all object files with object directory

all: $(OUT)

$(OUT): $(OBJS) 
	$(CC) $^ $(LIBS) -o $(OUT)

$(ODIR)/%.o: $(SDIR)/%.cc
	$(CC) $(CFLAGS) -c $(INC) -o $@ $< 

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(OUT)

# -MD compiler flag and the line below are required to recompile changes in header files
include $(wildcard obj/*.d)
