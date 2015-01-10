# Overview

This C/C++ program can be used to start a simple peer-to-peer network. It was developed by myself, Jordan Brown, and another student, Jesse Rowland, as part of a graduate-level distributed systems class. The program features TCP/IP messaging, POSIX threads, mutex locks, and a distributed depth-first graph traversal algorithm. A summary of the requirements met by the program is listed below:

1. The program shall allow users to specify, via a config file, an acceptable range of port numbers that the program may listen to.
  1. The program shall use a port stepping algorithm to find an open port to listen to. The chosen port must be within the range of port numbers specified by the user.
2. The program shall allow users to specify, via a config file, peers (i.e. neighbors) that the program may send messages to.
  1. The program shall prompt users to specify a peer if no peers are given.
3. The program shall handle requests sent by other peers (i.e. clients) in the network 
  1. When the program receives a request from a new client, the program shall begin a session by creating a new thread to handle that request and all future requests from the client.
      1. The program shall add the new client to its list of known neighbors if it is not already present.
  2. When the program receives an exit request from a client, the program shall end the client's session by closing the thread created to handle the client's requests. 
4. The program shall allow users to send messages to other peers via a command-line interface. The command-line interface shall provide the following commands:
  - List: print an ordered list of all neighbors known by the program
  - System-wide List: print an unordered list of all peers in the network
  - Select: begin a new session with a selected neighbor
  - Share: send neighbors known by the program to the selected neighbor
  - File: query the selected neighbor for a file
  - System-wide File: query all peers in the network for a file 
  - Exit: exit the command-line interface but allow the program to continue listening for messages

# Installation

To compile the program, you will need g++ 4.4.7 or an equivalent compiler that supports the C++03 standard. Pthreads is the only external library required and should be installed by default on most operating systems.

Use GNU Make to build the program. Run `make` in the root of the program's directory to start a build. Once complete, create a file named `portnums.txt` inside of the `config` directory. The first two lines should be used to specify an inclusive range of ports that the program may listen to, such as the following:

```
2000
4000
```

Optionally, you can create a file named `neighbors.txt` inside of the `config` directory to specify peers that the program may contact. Each peer should be written on a separate line in the form `host:port`. If you do not create a file, the program will ask for a neighbor when it starts and create the file automatically. Below is an example:

```
localhost:3182
an.example.host:2110
```

Finally, place any files that you want to make available to other peers inside of the `content` directory.

# Usage

Run `bin/peer -c` to start the program and its command-line interface. If you want to start the program without its command-line interface (i.e. as a daemon), leave off the `-c` flag.

Once the program starts, run `help` to view a list of commands. If a command requires arguments, you can type in the command without arguments to view its usage.

# MIT License

Copyright (c) 2015 Jordan Brown, Jesse Rowland

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
