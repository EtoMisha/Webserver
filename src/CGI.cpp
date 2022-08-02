
#include "../inc/CGI.hpp"

using namespace std;

//request - header and body
static const string strRequestBody = "===this is request body===\n";
static const string strRequestHeader = "Content-Length=" + to_string((long long)strRequestBody.length());

//env variables that will be sent used by the script
static const char *pszChildProcessEnvVar[4] = {strRequestHeader.c_str(), "VARIABLE2=2", "VARIABLE3=3", 0};

//needed for execution, the script itself
// static const char *pszChildProcessArgs[4] = {"./a.out", "first argument", "second argument", 0};
CGI::CGI(Request request)
{
	//_scriptPath = ; // parse to get it?
	std::map<std::string, std::string> body;
	int argCount = body.size();

	_argv = (char **)malloc(argCount + 1);
	std::map<std::string, std::string>::iterator it_begin = body.begin();
	std::map<std::string, std::string>::iterator it_end = body.end();
	std::string temp;

	while (it_begin != it_end)
	{
		temp = it_begin->first + it_begin->second;
		*_argv = temp.c_str();
		_argv++;
		it_begin++;
	}
	*_argv = NULL;

	
}

int CGI::spawnProcess()
{
    int pid = fork();
    if(pid == 0)
    {
		execve("python", (char* const*)_script, (char* const*)_argv);
		exit(EXIT_SUCCESS);
	}
    return pid;
}

void CGI::launchScript()
{
		
	const int fd = open ("test.txt", ios::app);

	int fdStdInPipe[2], fdStdOutPipe[2];
	
	fdStdInPipe[0] = fdStdInPipe[1] = fdStdOutPipe[0] = fdStdOutPipe[1] = -1;
	if (pipe(fdStdInPipe) != 0 || pipe(fdStdOutPipe) != 0)
	{
		cout << "Cannot create CGI pipe";
		return; //error
	}
	int fdOldStdIn = dup(fileno(stdin));
	int fdOldStdOut = dup(fileno(stdout));
	fdStdOutPipe[1] = dup(fd);
	if ((dup2(fdStdOutPipe[1], fileno(stdout)) == -1) || (dup2(fdStdInPipe[0], fileno(stdin)) == -1))
		return; //error
	close(fdStdInPipe[0]);
	close(fdStdOutPipe[1]);

	const int nChildProcessID = spawnProcess();

	dup2(fdOldStdIn, fileno(stdin));
	dup2(fdOldStdOut, fileno(stdout));

	close(fdOldStdIn);
	close(fdOldStdOut);

	// write(fdStdInPipe[1], strRequestBody.c_str(), strRequestBody.length());

}