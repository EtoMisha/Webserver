#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <vector>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

//Формируем в глобальных переменных тело запроса и его длинну
static const string strRequestBody = "===this is request body===\n";
static const string strRequestHeader = "Content-Length=" + to_string((long long)strRequestBody.length());

//Формируем переменные окружения которые будут отосланы дочернему процессу
static const char *pszChildProcessEnvVar[4] = {strRequestHeader.c_str(), "VARIABLE2=2", "VARIABLE3=3", 0};

//Формируем переменные командной строки для дочернего процесса. Первая переменная - путь к дочернему процессу.
static const char *pszChildProcessArgs[4] = {"./a.out", "first argument", "second argument", 0};
//При желании можно запустить интерпретатор какого-нибудь скрипта. 
//Тогда первый аргумент - путь к интерпретатору, второй - к скрипту
//static const char *pszChildProcessArgs[3] = {"python", "./test.py", 0};

//Это функция, которая породит дочерний процесс и передаст ему переменные из родительского
int spawn_process(const char *const *args, const char * const *pEnv)
{
    int pid = fork();
    if(pid == 0)
    {
		execve(args[0], (char* const*)args, (char* const*)pEnv);
		exit(EXIT_SUCCESS);
	}
    return pid;
}

int main()
{
		
	const int fd = open ("test.txt", ios::app);

	int fdStdInPipe[2], fdStdOutPipe[2];
	
	fdStdInPipe[0] = fdStdInPipe[1] = fdStdOutPipe[0] = fdStdOutPipe[1] = -1;
	if (pipe(fdStdInPipe) != 0 || pipe(fdStdOutPipe) != 0)
	{
		cout << "Cannot create CGI pipe";
		return 0;
	}
	int fdOldStdIn = dup(fileno(stdin));
	int fdOldStdOut = dup(fileno(stdout));
	fdStdOutPipe[1] = dup(fd);
	if ((dup2(fdStdOutPipe[1], fileno(stdout)) == -1) || (dup2(fdStdInPipe[0], fileno(stdin)) == -1))
		return 0;
	close(fdStdInPipe[0]);
	close(fdStdOutPipe[1]);

	const int nChildProcessID = spawn_process(pszChildProcessArgs, pszChildProcessEnvVar);

	dup2(fdOldStdIn, fileno(stdin));
	dup2(fdOldStdOut, fileno(stdout));

	close(fdOldStdIn);
	close(fdOldStdOut);

	write(fdStdInPipe[1], strRequestBody.c_str(), strRequestBody.length());

	// while (1)
	// {
	// 	//Читаем ответ от дочернего процесса
	// 	char bufferOut[100000];
	// 	int n = read(fdStdOutPipe[0], bufferOut, 100000);
	// 	if (n > 0)
	// 	{
	// 		//Выводим ответ на экран
	// 		fwrite(bufferOut, 1, n, stdout);
	// 		fflush(stdout);
	// 	}

	// 	//Если дочерний процесс завершился, то завершаем и родительский процесс
	// 	int status;
	// 	if (waitpid(nChildProcessID, &status, WNOHANG) > 0)
	// 		break;
	// }

	return 0;
}