#include "Server.hpp"

int main ()
{
	Server server;
	server.run();

	// struct sockaddr_in addr_in;
	// addr_in.sin_family = AF_INET;
	// addr_in.sin_port = htons(PORT);
	// addr_in.sin_len = sizeof(addr_in);
	// addr_in.sin_addr.s_addr = inet_addr(LOCALHOST);
	// bzero(&(addr_in.sin_zero), 8);
	
	// int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	// if (listen_socket == -1)
	// {
	// 	std::cerr << "socket" << std::endl;
	// 	return 1;
	// }
	
	// int on = 1;
	// setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	// if (bind(listen_socket, (struct sockaddr *) &addr_in, sizeof(struct sockaddr)) == -1)
	// 	err("bind", listen_socket);

	// if (listen(listen_socket, SOMAXCONN) == -1)
	// 	err("listen", listen_socket);

	// int kq = kqueue();
	// struct kevent evSet;
	// EV_SET(&evSet, listen_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	// if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
	// 	err("kevent", listen_socket);

	// struct kevent evList[32];
	// while (1)
	// {
	// 	int nev = kevent(kq, NULL, 0, evList, 32, NULL);
	// 	if (nev < 1)
	// 		err("kevent", listen_socket);
	// 	for (int i = 0; i < nev; i++)
	// 	{
	// 		std::string message;
	// 		int fd = (int)evList[i].ident;
			
	// 		if (evList[i].flags & EV_EOF)
	// 		{
	// 			std::cout << "Disconnect" << std::endl;
	// 			close(fd);
	// 		}
	// 		else if (fd == listen_socket)
	// 		{
	// 			struct sockaddr_storage addr;
	// 			socklen_t socklen = sizeof(addr);
	// 			int client_fd = accept(fd, (struct sockaddr *)&addr, &socklen);				
	// 			if (client_fd == -1)
	// 				err("accept", listen_socket);
	// 			EV_SET(&evSet, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	// 			if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
	// 				err("kevent", listen_socket);
	// 			printf("Connected\n");
	// 			EV_SET(&evSet, client_fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
	// 			if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
	// 				err("kevent", listen_socket);
	// 		}
	// 		else if (evList[i].filter == EVFILT_READ || evList[i].filter == EVFILT_WRITE) 
	// 		{
	// 			char buf[BUFFER_SIZE];
	// 			size_t bytes_read = recv(fd, buf, sizeof(buf), 0);
	// 			buf[bytes_read] = '\0';
	// 			if (strlen(buf) > 0)
	// 				message = buf;
	// 			printf("--- read %zu bytes\n", bytes_read);
	// 			// std::cout << "--- message in WRITE: " << message << std::endl;
	// 			std::string pageName = getPageName(buf);
	// 			off_t offset = (off_t)evList[i].udata;
	// 			off_t len = 0;
	// 			int send_count = sendMsg(fd, message);
	// 			if (send_count)
	// 			{
	// 				if (errno == EAGAIN) 
	// 				{
	// 					EV_SET(&evSet, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, (void *)(offset + len));
	// 					kevent(kq, &evSet, 1, NULL, 0, NULL);
	// 				}
	// 			}
	// 			printf("---- wrote %d bytes\n", send_count);
	// 		}
	// 	}
	// }
	// close(listen_socket);
}

