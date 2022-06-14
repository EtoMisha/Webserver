#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/event.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Request.hpp"
#include "Response.hpp"

#define PORT 8000
#define LOCALHOST "127.0.0.1"
#define BUFFER_SIZE 1024
#define NUSERS 10

class Server
{
	public:
		Server();
		~Server();
		
		void run();

	private:
		int listen_socket;
		int kq;
		struct kevent evSet;
		struct kevent evList[32];

		void listenLoop();
		std::string readRequest(int fd);
		void sendResponse(int fd, int i, Response response);

		// int sendPage(int client_socket, std::string page);
		int sendFile(int fd, std::string file_path);

		int err(std::string msg);

};