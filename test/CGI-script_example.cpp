#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <vector>

#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
	//Отдаем в stdout переменные командной строки, которые получили от родителя
	cout << "Child process started\n";
	for (int n=0; n<argc; n++)
		cout << "argv[" << n << "] = " << argv[n] << "\n";

	//Отдаем в stdout переменные окружения, которые получили от родителя
	const int nContentLength = atoi(getenv("Content-Length"));
	cout << 
		"\n" << 
		"Content-Length = " << nContentLength << "\n" <<
		"VARIABLE2 = " << getenv("VARIABLE2") << "\n" <<
		"VARIABLE3 = " << getenv("VARIABLE3") << "\n" <<
		"\n\n";
	fflush(stdout);

	sleep(5); //Паузы сделаны для наглядности

	vector<unsigned char> vBuffer(nContentLength);

	//Получаем из stdin все, что прислал туда родительский процесс
    const size_t nBytes = fread(&vBuffer[0],  1, nContentLength, stdin);

	//Отдаем в stdout то, что только что получили от родителя и добавляем свое
	cout << "Request body:\n";
    fwrite(&vBuffer[0], 1, nBytes, stdout);
    fflush(stdout);

	sleep(5); //Паузы сделаны для наглядности

	return 0;
}