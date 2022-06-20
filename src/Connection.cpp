#include "../inc/Connection.hpp"
#include <sstream>

Connection::Connection() {}
Connection::Connection(Request req, Response resp, int fd) : request(req), response(resp), fd(fd)
{
	this->finished = false;
	this->position = 0;
}

Connection::~Connection() {}


Connection::Connection(Connection const & other)
{
	this->fd = other.fd;
	this->request = other.request;
	this->response = other.response;
	this->finished = other.finished;
	this->position = other.position;
}

Connection & Connection::operator=(Connection const & other)
{
	this->fd = other.fd;
	this->request = other.request;
	this->response = other.response;
	this->finished = other.finished;
	this->position = other.position;
	return *this;
}

int const Connection::getFd() const { return this->fd; }

Request & Connection::getRequest() { return this->request; }

Response & Connection::getResponse() { return this->response; }

bool Connection::isFinished() const { return this->finished; }

int Connection::getPosition() const { return this->position; }


void Connection::setResponse(Response response) { this->response = response; }

void Connection::setRequest(Request request) { this->request = request; }

void Connection::setFinished(bool finished) { this->finished = finished; }

void Connection::setPosition(int position) { this->position = position; }