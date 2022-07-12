#include "../inc/Server.hpp"
#include "../inc/Parser.hpp"


ft::Server::Server() {
	host = "";
	port = 0;
	server_name = "";
	maxBodySize = 0;
	autoindex = false;
	index = "";
	root = "";
	uploadPath = "";
	methods = std::vector<std::string>();
    locations = std::vector<ft::Location>();
	errorPages = std::map<int, std::string>();
	redirection = "";
	redirection_code = 0;
}
ft::Server::~Server() {}

// ft::Server::Server(Server & rhs)
// {
// 	this->host = rhs.host;
// 	this->port = rhs.port;
// 	this->server_name = rhs.server_name;
// 	this->maxBodySize = rhs.maxBodySize;
// 	this->autoindex = rhs.autoindex;
// 	this->index = rhs.index;
// 	this->root = rhs.root;
// 	this->uploadPath = rhs.uploadPath;
// 	this->methods = rhs.methods;
// 	this->locations = rhs.locations;
// 	this->errorPages = rhs.errorPages;
// }


ft::Server &ft::Server::operator=(const Server &rhs) {
	this->host = rhs.host;
	this->port = rhs.port;
	this->server_name = rhs.server_name;
	this->maxBodySize = rhs.maxBodySize;
	this->autoindex = rhs.autoindex;
	this->index = rhs.index;
	this->root = rhs.root;
	this->uploadPath = rhs.uploadPath;
	this->methods = rhs.methods;
	this->locations = rhs.locations;
	this->errorPages = rhs.errorPages;
	this->redirection = rhs.redirection;
	this->redirection_code = rhs.redirection_code;
	return *this;
}

void ft::Server::setHost(const std::string &_host)
{
	host = _host;
}
const std::string &ft::Server::getHost() const {
	return host;
}
void ft::Server::setPort(const int &_port) {
	port = _port;
}
const int &ft::Server::getPort() const {
	return port;
}
void ft::Server::setName(const std::string &_serverName) {
	server_name = _serverName;
}
const std::string &ft::Server::getName() const {
	return server_name;
}
void ft::Server::setRoot(const std::string &_root) {
	root = _root;
}
const std::string &ft::Server::getRoot() const {
	return root;
}
void ft::Server::setIndex(const std::string &_index) {
	index = _index;
}
const std::string &ft::Server::getIndex() const {
	return index;
}
void ft::Server::setUploadPath(const std::string &_uploadPath) {
	uploadPath = _uploadPath;
}
const std::string &ft::Server::getUploadPath() const {
	return uploadPath;
}
void ft::Server::setAutoIndex(const bool &_autoIndex) {
	autoindex = _autoIndex;
}
const bool &ft::Server::getAutoIndex() const {
	return autoindex;
}
void ft::Server::setMaxBodySize(const int &_maxBodySize) {
	maxBodySize = _maxBodySize;
}
const int &ft::Server::getMaxBodySize() const {
	return maxBodySize;
}
void ft::Server::setMethods(const std::vector<std::string> &_methods) {
	methods = _methods;
}
const std::vector<std::string> &ft::Server::getMethods() const {
	return methods;
}
void ft::Server::setLocations(const std::vector<Location> &_locations) {
	locations = _locations;
}
std::vector<ft::Location>  &ft::Server::getLocations() {
	return locations;
}
void ft::Server::setErrorPages(const std::map<int, std::string> &_errorPages) {
	errorPages = _errorPages;
}
const std::map<int, std::string> &ft::Server::getErrorPages() {
	return errorPages;
}

void ft::Server::setErrorPageVal(const int& code, const std::string& path){
	errorPages[code] = path;
}

void ft::Server::setRedirection(const std::string &_redirection){
	redirection = _redirection;
}
const std::string &ft::Server::getRedirection() const {
	return redirection;
}
void ft::Server::setRedirectionCode(const int &_code){
	redirection_code = _code;
}
const int &ft::Server::getRedirectionCode() const {
	return redirection_code;
}