#include <iostream>
#include <fstream>
#define PATH_INFO ""

void	CGI(std::string filename, int csock, char *envp[])
{
	pid_t cpid = fork();
	if (cpid < 0) exit(1);
	if ( cpid ) {
		close( csock );
		waitpid( cpid, NULL, 0 );
	} else {
		dup2( csock, STDOUT_FILENO );
		close( csock );
		execve( filename, NULL, envp );
	}
	// open socket after it has been closed?
}
