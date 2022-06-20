#include "../inc/Webserver.hpp"
#include "../inc/Request.hpp"
#include "../inc/Response.hpp"
#include "../inc/Handler.hpp"

Webserver::Webserver() {}
Webserver::Webserver(Config config) : config(config) {}
Webserver::~Webserver() {}

void Webserver::run()
{
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(config.getPort());
	addr_in.sin_len = sizeof(addr_in);
	addr_in.sin_addr.s_addr = inet_addr(config.getHost().c_str());
	bzero(&(addr_in.sin_zero), 8);
	
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket < 0)
		err("socket");
	int on = 1;
	if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		err("setcockopt");
	if (bind(listen_socket, (struct sockaddr *) &addr_in, sizeof(struct sockaddr)) == -1)
		err("bind");
	if (listen(listen_socket, SOMAXCONN) == -1)
		err("listen");

	memset(fds, 0, sizeof(fds));
	fds[0].fd = listen_socket;
	fds[0].events = POLLIN;

	end_server = false;
	nfds = 1;
	while (end_server == false)
	{
		listenLoop();
	}
	for (int i = 0; i < nfds; i ++)
	{
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
}

void Webserver::listenLoop()
{
	int timeout = (3 * 60 * 1000);
	int new_sd = -1;
	std::cout << "--- wainting on poll" << std::endl;
	int rc = poll(fds, nfds, timeout);
	if (rc < 0)
		err("poll");
	if (rc == 0)
		err("poll timeout");
	int current_size = nfds;
	for (int i = 0; i < current_size; i++)
	{
		if (fds[i].revents == 0)
			continue;
		if (fds[i].revents != POLLIN && fds[i].revents != POLLOUT)
		{
			std::cout << "Error in revents" << std::endl;
			end_server = true;
			break;
		}
		if (fds[i].fd == listen_socket)
		{
			printf("Listening socket is readable\n");
			do
			{
				int new_sd = accept(listen_socket, NULL, NULL);
				if (new_sd < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << "accept error" << std::endl;
						end_server = true;
					}
					break;
				}
				std::cout << "new incoming connection " << new_sd << std::endl;
				fds[nfds].fd = new_sd;
				fds[nfds].events = POLLIN;
				nfds++;
			} while (new_sd != -1);
		}
		else
		{
			printf("Descriptor %d is readable\n", fds[i].fd);
			if (sendAndReceive(fds[i].fd, i) == -1)
				closeConnection(i);
		}
	}
}

int Webserver::sendAndReceive(int fd, int i)
{
	std::cout << "Send and receive, fd " << fd << std::endl;
	if (connections.find(fd) == connections.end())
	{
		fds[i].events = POLLIN;
		Request request(readRequest(fd), config);
		Handler handler(request, config);
		Response response = handler.getResponse();
		Connection *connection = new Connection(request, response, fd); // удалять в дескрипторе
		connections[fd] = connection;
		fds[i].events = POLLOUT;
		if (send(fd, response.toString().c_str(), response.toString().length(), 0) < 0)
			return -1;
		std::cout << "***\n" << response.toString() << "\n***\n" << std::endl;
	}
	fds[i].events = POLLOUT;
	
	if (connections[fd]->getResponse().getBodyFile() != "" && connections[fd]->isFinished() == false)
	{
		if (sendFile(*connections[fd]) < 0)
			return -1;
	}
	std::string delimeter = "\r\n\r\n";
	if (connections[fd]->isFinished())
	{
		if (send(fd, delimeter.c_str(), delimeter.length(), 0) < 0)
			return -1;
		closeConnection(i);
		connections.erase(fd);
	}
	return 0;
}

std::string Webserver::readRequest(int fd)
{
	char buf[config.getBufferSize()];
	size_t bytes_read = recv(fd, buf, sizeof(buf), 0);
	printf("read %zu bytes\n", bytes_read);	
	if (bytes_read == 0)
	{
		std::cout << "Can't receive client's request" << std::endl;
	}
	buf[bytes_read] = '\0';
	std::string requestData = buf;
	
	return requestData;
}


int Webserver::sendFile(Connection & connection)
{
	std::cout << "send file" << std::endl;
	FILE* file = fopen(connection.getResponse().getBodyFile().c_str(), "rb");
	fseek(file, connection.getPosition(), SEEK_SET);
	
	int bufferSize = config.getBufferSize();
	char buffer[bufferSize];
	int bytes_read = fread(buffer, sizeof(char), bufferSize, file);
	connection.setPosition(connection.getPosition() + bytes_read);
	int l = send(connection.getFd(), buffer, bytes_read, 0);
	if (l < 0)
	{
		std::cout << "SEND ERROR" << std::endl;
		return -1;
	}
	fclose(file);
	std::cout << "file sent ok: " << bytes_read << " bytes, position " << connection.getPosition() 
		<< " length " << connection.getResponse().getLength() << std::endl;

	if (connection.getPosition() == connection.getResponse().getLength())
		connection.setFinished(true);
	else
		connection.setFinished(false);
	return 0;
}

void Webserver::closeConnection(int i)
{
	close(fds[i].fd);
	fds[i].fd = -1;
	for (int i = 0; i < nfds; i++)
	{
		if (fds[i].fd == -1)
		{
			for (int j = 0; j < nfds - 1; j++)
				fds[i].fd = fds[j + 1].fd;
			i--;
			nfds--;
		}
	}
}

int Webserver::err(std::string msg) // добавить код возврата 500-какой-то в случае такой ошибки
{
	std::cerr << "Error " << errno << " in " << msg << std::endl;
	close(this->listen_socket);
	exit(errno);
}
