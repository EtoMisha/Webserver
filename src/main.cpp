#include "../inc/Webserver.hpp"

int main ()
{
	Config config(	"127.0.0.1",
					8000,
					"res/",
					"index.html",
					25000	);
	
	Webserver server(config);
	server.run();

}

