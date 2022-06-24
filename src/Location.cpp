#include "../inc/Location.hpp"

ft::Location::Location() {
	name = "";
	root = "";
	index = "";
	upload_path = "";
	redirection = "";
	redirection_code = 0;
	autoindex = false;
	methods = std::vector<std::string>();
	errorPages = std::map<int, std::string>();
	scripts = std::map<std::string, std::string>();
}

ft::Location &ft::Location::operator=(const Location &rhs) {
	name = rhs.name;
	root = rhs.root;
	index = rhs.index;
	upload_path = rhs.upload_path;
	redirection = rhs.redirection;
	redirection_code = rhs.redirection_code;
	autoindex = rhs.autoindex;
	methods = rhs.methods;
	errorPages = rhs.errorPages;
	scripts = rhs.scripts;
	return *this;
}

ft::Location::~Location() {}

void ft::Location::setName(const std::string &_name) {
	name = _name;
}
const std::string &ft::Location::getName() const {
	return name;
}
void ft::Location::setRoot(const std::string &_root) {
	root = _root;
}
const std::string &ft::Location::getRoot() const {
	return root;
}
void ft::Location::setIndex(const std::string &_index) {
	index = _index;
}
const std::string &ft::Location::getIndex() const {
	return index;
}
void ft::Location::setUploadPath(const std::string &_uploadPath) {
	upload_path = _uploadPath;
}
const std::string &ft::Location::getUploadPath() const {
	return upload_path;
}
void ft::Location::setRedirection(const std::string &_redirection){
	redirection = _redirection;
}
const std::string &ft::Location::getRedirection() const {
	return redirection;
}
void ft::Location::setRedirectionCode(const int &_code){
	redirection_code = _code;
}
const int &ft::Location::getRedirectionCode() const {
	return redirection_code;
}
void ft::Location::setAutoIndex(const bool &_autoIndex) {
	autoindex = _autoIndex;
}
const bool &ft::Location::getAutoIndex() const {
	return autoindex;
}
void ft::Location::setMethods(const std::vector<std::string> &_methods) {
	methods = _methods;
}
const std::vector<std::string> &ft::Location::getMethods() const{
	return methods;
}

void ft::Location::setErrorPages(const std::map<int, std::string> &_errorPages) {
	errorPages = _errorPages;
}
const std::map<int, std::string> &ft::Location::getErrorPages() const {
	return errorPages;
}

void ft::Location::setErrorPageVal(const int& code, const std::string& path){
	errorPages[code] = path;
}

void ft::Location::setScripts(const std::map<std::string, std::string> &_scripts) {
	scripts = _scripts;
}
const std::map<std::string, std::string> &ft::Location::getScripts() const {
	return scripts;
}
void ft::Location::setScriptsVal(const std::string& script, const std::string& path) {
	scripts[script] = path;
}
