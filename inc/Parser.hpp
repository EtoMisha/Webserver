#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include "Server.hpp"
#include "Location.hpp"
#include "ServerParameters.hpp"

enum ValidRootKeys {
	Host_port,
	Server_name,
	Autoindex,
	Root,
	Index_page,
	Methods,
	Client_max_body_size,
	Error_page,
	UploadPath,
	Redirection
};

enum ValidLocKeys {
	Location_name,
	Location_methods,
	Location_root,
	Location_redirection,
	Location_error_page,
	Bin_path_py,
	Bin_path_sh,
	Path_cgi,
	Location_index,
	LocationUploadPath,
	LocationAutoindex
};

namespace ft {
	class Parser {
		private:
		
		const char* _config;
		// std::vector<Port> _ports; //??
		std::vector<Server> _servers;

		public:
		Parser();
		Parser(const char* config);
		Parser(Parser const  &other);
		~Parser();
		ft::ValidConfigKeys _validConfigParams;
		std::vector<ft::Server> &getServers();

		void parse();
		std::vector<std::string> checkAndClean(std::string argv);
		int isEmptyLine(std::string str);
		void cleanSpaces(std::string *string);
		std::vector<std::string> lineBreaks(std::string string);
		int validKeys(std::string config);
		std::vector<std::string> checkHost(std::string host);
		void checkAllKeys(std::vector<std::string> config);
		int easyFind(std::string word, std::string line);
		int checkBrackets(std::vector<std::string> file, size_t *line);
		std::vector<std::string> splitString(std::string key, std::string line);
		int validHost(std::string value);
		int checkPortVal (std::string str);
		void serversInfo(size_t index, std::vector<std::string> file, size_t start, size_t end);
		void fillConfig(std::string key, std::string line, size_t index, size_t caseKey);
		void fillHostPort(std::string key, std::string line, size_t index);
		void fillServerName(std::string key, std::string line, size_t index);
		void fillAutoindex(std::string key, std::string line, size_t index);
		void fillServerRoot(std::string key, std::string line, size_t index);
		void fillIndex(std::string key, std::string line, size_t index);
		void fillRootMethods(std::string key, std::string line, size_t index);
		void fillRootMaxBodySize(std::string key, std::string line, size_t index);
		void fillUploadPath(std::string key, std::string line, ssize_t index);
		void fillRootErrorPages(std::string key, std::string line, ssize_t index);
		std::pair<int,std::string> fillErrorPage(std::vector<std::string> value);
		void locationsInfo(std::vector<std::string> file, size_t index, size_t *start, size_t end);
		void fillLocation(std::string key, std::string line, ft::Location& location, size_t caseKey);
		void fillLocationName(std::string key, std::string line, ft::Location& location);
		void fillLocationRoot(std::string key, std::string line, ft::Location& location);
		void fillLocationRedirection(std::string key, std::string line, ft::Location& location);
		void fillLocationMethods(std::string key, std::string line, ft::Location& location);
		void fillLocationIndex(std::string key, std::string line, ft::Location& location);
		void fillLocationAutoindex(std::string key, std::string line, ft::Location& location);
		void fillLocationUploadPath(std::string key, std::string line, ft::Location& location);
		void fillLocationErrorsPages(std::string key, std::string line, ft::Location& location);
		void fillLocationScripts(std::string key, std::string line, ft::Location& location);


	};
}


#endif