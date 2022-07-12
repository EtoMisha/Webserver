#include "../inc/Webserver.hpp"
#include "../inc/Request.hpp"
#include "../inc/Response.hpp"
#include "../inc/Handler.hpp"

Webserver::Webserver() {}
Webserver::Webserver(std::vector<ft::Server> &_servers) : servers(_servers) {}
Webserver::~Webserver() {}

void Webserver::prepare(int serv_id)
{
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(servers[serv_id].getPort());///
	addr_in.sin_len = sizeof(addr_in);
	addr_in.sin_addr.s_addr = inet_addr(servers[serv_id].getHost().c_str());///
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
	fds[serv_id].fd = listen_socket;
	fds[serv_id].events = POLLIN;
	sockets.push_back(listen_socket);
}

void Webserver::run()
{
	for(int i = 0; i < servers.size(); i++) {
		servs_fd[fds[i].fd] = (getServers()[i]);
	}	
	end_server = false;
	nfds = getServers().size();
	while (end_server == false)
	{
		// printFds();
		// printConnections();
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
	int new_sd = -1;
	std::cout << "--- wainting on poll" << std::endl;
	int rc = poll(fds, nfds, -1);
	if (rc < 0)
		err("poll");
	if (rc == 0)
		err("poll timeout");
	else {	
		for (int i = 0; i < nfds; i++) {
			if (fds[i].revents == 0)
				continue;
			if (std::find(sockets.begin(), sockets.end(), fds[i].fd) != sockets.end()) {
				do {
					listen_fds = fds[i];
					int new_sd = accept(fds[i].fd, NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "accept error" << std::endl;
							end_server = true;
						}
						break;
					}
					std::cout << "new incoming connection " << new_sd << std::endl;///
					fds[nfds].fd = new_sd;
					fds[nfds].events = POLLIN;
					connections[new_sd] = new Connection(new_sd, fds[i].fd);
					nfds++;
				}
				while (new_sd > 0);
			}
			else if (fds[i].revents & POLLIN ) {
				std::cout << fds[i].fd << " : new socket is readable; listening : "<< listen_fds.fd<< "\n";	
				int fd = fds[i].fd;
				int rcv = connections[fd]->readRequest();
				if (rcv > 0) {
					Handler handler(connections[fd]->getRequest(), servs_fd[listen_fds.fd]); // add root dir setup
					// try - catch на случай если в хэндлере не создался файл или папка
					connections[fd]->setResponse(handler.getResponse());
					if (connections[fd]->getStatus() == READ_DONE && connections[fd]->getRequest().getType() == HEADERS)
						fds[i].events = POLLOUT;
				}
				else if (rcv == 0) {
					connections[fd]->setStatus(READ_DONE);
					closeConnection(i);
					// connections.erase(fds[i].fd);
				}
				fds[i].revents = 0;
			}
			else if (fds[i].revents & POLLOUT) {
				std::cout << fds[i].fd << " : waiting for response\n";
				int fd = fds[i].fd;
				if (connections[fd]->sendHeaders() < 0)
					closeConnection(i);
				if (connections[fd]->sendBody() < 0)
					closeConnection(i);
				if (connections[fd]->getStatus() == WRITE_DONE)
					fds[i].events = POLLIN;
				fds[i].revents = 0;
			}
			else if (fds[i].revents != POLLIN && fds[i].revents != POLLOUT) {
				std::cout << "Close connection\n";
				closeConnection(i);
			}
			else {
				std::cout << "Error in revents " << fds[i].revents << std::endl;
				end_server = true;
				break;
			}
		}
	}
}


void Webserver::printFds() {
	std::cout << "***\nnfds " << nfds << std::endl;
	for (int i = 0; i < nfds; i++) {
		std::cout << "fd " << fds[i].fd << " event ";
		if (fds[i].events == POLLIN)
			std::cout << "POLLIN ";
		else if (fds[i].events == POLLOUT)
			std::cout << "POLLOUT ";
		else
			std::cout << fds[i].events << std::endl;

		if (fds[i].revents == POLLIN)
			std::cout << "POLLIN" << std::endl;
		else if (fds[i].revents == POLLOUT)
			std::cout << "POLLOUT" << std::endl;
		else
			std::cout << fds[i].revents << std::endl;
	}
	std::cout << "***\n";
}


void Webserver::printConnections() {
	std::cout << "total  " << connections.size() << " connections" << std::endl;
	if ( connections.size() > 0) {
		std::map<int, Connection*>::iterator it = connections.begin();
		while (it != connections.end())
		{
			std::cout << "conn " << it->first << " fd " << it->second->getFd()
				<< " listen " << it->second->getListenFd() << " status " << it->second->getStatus() << std::endl;
			it++;
		}
	}
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

const std::vector<ft::Server> &Webserver::getServers() {
	return servers;
}
