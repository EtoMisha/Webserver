#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include "Location.hpp"

namespace ft {
	class Location;
	class Server {
		private:
		
		std::string host;
		int port;
		std::string server_name;
		bool autoindex;
		std::string index;
		std::string root;
		std::string uploadPath;
		int maxBodySize;
		std::vector<std::string> methods;
		std::vector<Location> locations;
		std::map<int, std::string> errorPages;
		std::string redirection;
		int redirection_code;

		public:
		Server();
		~Server();
		// Server(Server & server);
		Server &operator=(const Server &rhs);

		void setHost(const std::string &_host);
		const std::string &getHost() const;

		void setPort(const int &_port);
		const int &getPort() const;

		void setName(const std::string &_serverName);
		const std::string &getName() const;

		void setRoot(const std::string &_root);
		const std::string &getRoot() const;

		void setIndex(const std::string &_index);
		const std::string &getIndex() const;

		void setUploadPath(const std::string &_uploadPath);
		const std::string &getUploadPath() const;

		void setAutoIndex(const bool &_autoIndex);
		const bool &getAutoIndex() const;

		void setMaxBodySize(const int &_maxBodySize);
		const int &getMaxBodySize() const;

		void setMethods(const std::vector<std::string> &_methods);
		const std::vector<std::string> &getMethods() const;

		void setLocations(const std::vector<Location> &_locations);
		std::vector<Location>  &getLocations();

		void setErrorPages(const std::map<int, std::string> &_errorPages);
		const std::map<int, std::string> &getErrorPages();

		void setRedirection(const std::string &_redirection);
		const std::string &getRedirection() const;

		void setRedirectionCode(const int &_code);
		const int &getRedirectionCode() const;

		void setErrorPageVal(const int& code, const std::string& path);

	};
}

#endif
