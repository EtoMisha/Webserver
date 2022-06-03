#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Handler.hpp"

Server::Server() {}
Server::~Server() {}

void Server::run()
{
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(PORT);
	addr_in.sin_len = sizeof(addr_in);
	addr_in.sin_addr.s_addr = inet_addr(LOCALHOST);
	bzero(&(addr_in.sin_zero), 8);
	
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == -1)
	{
		std::cerr << "socket" << std::endl;
		exit(1);
	}
	
	int on = 1;
	setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (bind(listen_socket, (struct sockaddr *) &addr_in, sizeof(struct sockaddr)) == -1)
		err("bind", listen_socket);

	if (listen(listen_socket, SOMAXCONN) == -1)
		err("listen", listen_socket);

	kq = kqueue();
	EV_SET(&evSet, listen_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
		err("kevent", listen_socket);

	while (1)
		listenLoop();
	close(listen_socket);
}

void Server::listenLoop()
{
	int nev = kevent(kq, NULL, 0, evList, 32, NULL);
	if (nev < 1)
		err("kevent", listen_socket);
	for (int i = 0; i < nev; i++)
	{
		int fd = (int)evList[i].ident;
		
		if (evList[i].flags & EV_EOF)
		{
			std::cout << "Disconnect" << std::endl;
			close(fd);
		}
		else if (fd == listen_socket)
		{
			struct sockaddr_storage addr;
			socklen_t socklen = sizeof(addr);
			int client_fd = accept(fd, (struct sockaddr *)&addr, &socklen);				
			if (client_fd == -1)
				err("accept", listen_socket);
			EV_SET(&evSet, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
			if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
				err("kevent", listen_socket);
			printf("Connected\n");
			EV_SET(&evSet, client_fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
			if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
				err("kevent", listen_socket);
		}
		else if (evList[i].filter == EVFILT_READ || evList[i].filter == EVFILT_WRITE) 
		{
			Request request(readRequest(fd));
			Handler handler(request);
			Response response = handler.getResponse();
			sendResponse(fd, i, response.toString());
		}
	}
}

int Server::err(std::string msg, int socket)
{
	std::cerr << "Error " << errno << " in " << msg << std::endl;
	close(socket);
	exit(errno);
}

std::string Server::readRequest(int fd)
{
	char buf[BUFFER_SIZE];
	size_t bytes_read = recv(fd, buf, sizeof(buf), 0);
	printf("read %zu bytes\n", bytes_read);
	buf[bytes_read] = '\0';
	std::string requestData = buf;
	
	return requestData;
}

void Server::sendResponse(int fd, int i, std::string responseStr)
{
	off_t offset = (off_t)evList[i].udata;
	off_t len = 0;
	int send_result = send(fd, responseStr.c_str(), responseStr.length(), 0);
	if (send_result == -1)
		err("send", listen_socket);
	if (send_result)
	{
		if (errno == EAGAIN) 
		{
			EV_SET(&evSet, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, (void *)(offset + len));
			kevent(kq, &evSet, 1, NULL, 0, NULL);
		}
	}
	printf("wrote %d bytes\n", send_result);
}

// void Server::sendResponse(Request request, int fd, int i)
// {
	// off_t offset = (off_t)evList[i].udata;
	// off_t len = 0;
	// std::stringstream response_body;

	// int send_count = sendPage(fd, request.getUrl());
	// if (send_count)
	// {
	// 	if (errno == EAGAIN) 
	// 	{
	// 		EV_SET(&evSet, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, (void *)(offset + len));
	// 		kevent(kq, &evSet, 1, NULL, 0, NULL);
	// 	}
	// }
// 	printf("wrote %d bytes\n", send_count);
// }

int Server::sendPage(int client_socket, std::string page)
{
	std::stringstream response;
	std::stringstream response_body;
		
	std::ifstream file(page);
	if (file.is_open())
	{
		std::cout << "File opened OK" << std::endl;
		response_body << file.rdbuf();
		file.close();
	}
	else
	{
		std::cout << "Can't open file" << "|" << page << "|" << std::endl;
	}
	response << "HTTP/1.1 200 OK\r\n"
	<< "Version: HTTP/1.1\r\n"
	<< "Content-Type: text/html; charset=utf-8\r\n"
	<< "Content-Length: " << response_body.str().length()
	<< "\r\n\r\n"
	<< response_body.str();
	int send_result = send(client_socket, response.str().c_str(),
							response.str().length(), 0);
	if (send_result == -1)
	{
		std::cerr << "Sending error" << std::endl;
		return(0);
	}
	return (response.str().length());
}