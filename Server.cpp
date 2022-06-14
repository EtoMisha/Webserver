#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Handler.hpp"

#define HOME_DIR "files/"

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
		err("bind");

	if (listen(listen_socket, SOMAXCONN) == -1)
		err("listen");

	kq = kqueue();
	EV_SET(&evSet, listen_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
		err("kevent");

	while (1)
		listenLoop();
	close(listen_socket);
}

void Server::listenLoop()
{
	int nev = kevent(kq, NULL, 0, evList, 32, NULL);
	if (nev < 1)
		err("kevent");
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
				err("accept");
			EV_SET(&evSet, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
			if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
				err("kevent");
			printf("Connected\n");
			EV_SET(&evSet, client_fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
			if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
				err("kevent");
		}
		else if (evList[i].filter == EVFILT_READ || evList[i].filter == EVFILT_WRITE) 
		{
			Request request(readRequest(fd));
			Handler handler(request);
			Response response = handler.getResponse();
			sendResponse(fd, i, response);
		}
	}
}

int Server::err(std::string msg) // добавить код возврата 500-какой-то в случае такой ошибки
{
	std::cerr << "Error " << errno << " in " << msg << std::endl;
	close(this->listen_socket);
	exit(errno);
}


std::string Server::readRequest(int socket)
{
	char buf[BUFFER_SIZE];
	size_t bytes_read = recv(socket, buf, sizeof(buf), 0);
	printf("read %zu bytes\n", bytes_read);
	buf[bytes_read] = '\0';
	std::string requestData = buf;
	
	return requestData;
}

int Server::sendFile(int fd, std::string file_path)
{
	FILE* fin = fopen(file_path.c_str(), "rb");  // r for read, b for binary
	if (fin == NULL)
		return -1;
	
	int bufferSize = BUFFER_SIZE;
	char *buffer = (char *) malloc(bufferSize);
	int i = 0;
	while (i < bufferSize)
	{
		fread(buffer, sizeof(char), bufferSize, fin);
		int l = send(fd, &buffer[i], bufferSize, 0);
		std::cout << "SENT " << l << " bytes" << std::endl; 
		if (l < 0)
			return -1;
		i += l;
	}
	fclose(fin);
	std::cout << "FILE SENT, OK" << std::endl; 
	return i;
}

// void Handler::readFile(unsigned char* buffer, size_t size, const char* file_path)
// {
// 	FILE* fin = fopen(file_path, "rb");  // r for read, b for binary
// 	if (fin == NULL) {
// 		printf("open %s failed.", file_path);
// 		return;
// 	}

// 	fread(buffer, sizeof(unsigned char), size, fin); // read sizeof(buffer) elements to our buffer
// }

// int SendBuffer(SOCKET s, const char* buffer, int bufferSize, int chunkSize = 4 * 1024) {

//     int i = 0;
//     while (i < bufferSize) {
//         const int l = send(s, &buffer[i], __min(chunkSize, bufferSize - i), 0);
//         if (l < 0) { return l; } // this is an error
//         i += l;
//     }
//     return i;
// }

void Server::sendResponse(int fd, int i, Response response)
{
	off_t offset = (off_t)evList[i].udata;
	off_t len = 0;
	std::string delimeter = "\r\n\r\n";

	int send_result = send(fd, response.toString().c_str(), response.toString().length(), 0);
	if (send_result == -1)
		err("send");
	send_result = sendFile(fd, response.getBodyFile());
	if (send_result == -1)
		err("send");
	send_result = send(fd, delimeter.c_str(), delimeter.length(), 0);
	if (send_result == -1)
		err("send");
	if (send_result)
	{
		if (errno == EAGAIN) 
		{
			EV_SET(&evSet, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, (void *)(offset + len));
			kevent(kq, &evSet, 1, NULL, 0, NULL);
		}
	}
	// printf("wrote %d bytes\n", send_result);
}
