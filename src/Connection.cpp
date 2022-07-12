#include "../inc/Connection.hpp"

Connection::Connection() {}

Connection::Connection(int fd, int listen_fd) : fd(fd), listen_fd(listen_fd), status(READ), position(0) {
}

Connection::~Connection() {}


Connection::Connection(Connection const & other)
{
	this->fd = other.fd;
	this->request = other.request;
	this->response = other.response;
	this->position = other.position;
	this->status = other.status;
	this->filename = other.filename;
}

Connection & Connection::operator=(Connection const & other)
{
	this->fd = other.fd;
	this->request = other.request;
	this->response = other.response;
	this->position = other.position;
	this->status = other.status;
	this->filename = other.filename;
	return *this;
}

int Connection::getFd() const { return this->fd; }

int Connection::getListenFd() const { return this->listen_fd; }

Request & Connection::getRequest() {
	request.setFilename(filename);
	return this->request; 
}

Response & Connection::getResponse() { return this->response; }

int Connection::getPosition() const { return this->position; }

ConStatus Connection::getStatus() { return this->status; }

void Connection::setRequest(Request request) { this->request = request; }

void Connection::setResponse(Response response) { this->response = response; }

void Connection::setPosition(int position) { this->position = position; }

void Connection::setStatus(ConStatus status) { this->status = status; }


int Connection::readRequest()
{
	char buf[BUFFER_SIZE + 1];
	int bytes_read = recv(fd, buf, BUFFER_SIZE, 0);
	printf("read %d bytes\n", bytes_read);	
	if (bytes_read == 0)
	{
		status = READ_DONE;
		return 0;
	} else if (bytes_read > 0) {
		buf[bytes_read] = '\0';
		request = Request(&buf[0], bytes_read);
		if (request.getFilename() != "")
			filename = request.getFilename();
		if (request.getType() == DATA_START || request.getType() == DATA_CONT)
			status = READ;
		else
			status = READ_DONE;
	}
	return bytes_read;
}

int Connection::sendHeaders()
{
	// std::cout << "*** RESPONSE ***\n" << response.toString() << "\n***\n";
	if (status == READ_DONE) {
		std::cout << "send headers" << std::endl;
		if (send(fd, response.toString().c_str(), response.toString().length(), 0) < 0)
			return -1;
	}
	return 0;
}

int Connection::sendBody()
{
	if (response.getBodyFile() != "")
	{
		std::cout << "send file " << response.getBodyFile() << std::endl;
		FILE* file = fopen(response.getBodyFile().c_str(), "rb");
		if (file != NULL) {
			fseek(file, position, SEEK_SET);
		
			char buffer[BUFFER_SIZE];
			int bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file);
			position += bytes_read;
			if (send(fd, buffer, bytes_read, 0) < 0) {
				std::cout << "Sending error, file " << response.getBodyFile() << std::endl;
				return -1;
			}
			fclose(file);
			std::cout << "file sent ok: " << bytes_read << " bytes, position " << position 
				<< " length " << response.getLength() << std::endl;
		} else {
			std::cout << "CAN NOT SEND FILE \n"; 
		}
		
	}
	if (position == response.getLength() || response.getBodyFile() == "")
	{
		std::string delimeter = "\r\n\r\n";
		if (send(fd, delimeter.c_str(), delimeter.length(), 0) < 0)
			return -1;
		status = WRITE_DONE;
	}
	else
		status = WRITE;
	if (response.getStatus() > 400 && response.getBodyFile() == "")
		return -1;
	return 0;
}