#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>

namespace ft {

	class Location {
	private:
		std::string name;
		std::string root;
		std::string index;
		std::string upload_path;
		std::string redirection;
		int redirection_code;
		bool autoindex;
		std::vector<std::string> methods;
		std::map<int, std::string> errorPages;
		std::map<std::string, std::string> scripts;

	public:
		Location();
		Location &operator=(const Location &other);
		~Location();

		void setName(const std::string &_path);
		const std::string &getName() const;

		void setRoot(const std::string &_root);
		const std::string &getRoot() const;

		void setIndex(const std::string &_index);
		const std::string &getIndex() const;

		void setUploadPath(const std::string &_uploadPath);
		const std::string &getUploadPath() const;

		void setRedirection(const std::string &_redirection);
		const std::string &getRedirection() const;

		void setRedirectionCode(const int &_code);
		const int &getRedirectionCode() const;

		void setAutoIndex(const bool &_autoIndex);
		const bool &getAutoIndex() const;

		void setMethods(const std::vector<std::string> &_methods);
		const std::vector<std::string> &getMethods() const;

		void setErrorPages(const std::map<int, std::string> &_errorPages);
		const std::map<int, std::string> &getErrorPages() const;

		void setErrorPageVal(const int& code, const std::string& path);

		void setScripts(const std::map<std::string, std::string> &_scripts);
		const std::map<std::string, std::string> &getScripts() const;
		void setScriptsVal(const std::string& script, const std::string& path);
	};
}

#endif 
