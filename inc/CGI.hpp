#pragma once

#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <vector>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "Handler.hpp"

class CGI{
	public:
		CGI();
		CGI(Request request);
		CGI(CGI const & other);
		~CGI();

		CGI operator=(CGI const & other);
		

		void launchScript();
	private:
		// Request _request;
		std::string _script;
		const char *_argv; //get args from url?
		const char *_scriptPath; //get script 
		int spawnProcess();
};