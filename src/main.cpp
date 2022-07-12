#include <iostream>
#include "../inc/Webserver.hpp"
#include "../inc/Parser.hpp"
#include "../inc/Server.hpp"


int main(int ac, char **av) {
	if (ac != 2){
		std::cout << "Error set configuration file" << std::endl;
		return 0;
	}
	ft::Parser parser(av[1]);
	try
	{
		parser.parse();
	}
   	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	Webserver webserver(parser.getServers());
	for(size_t serv_id = 0; serv_id < webserver.getServers().size(); serv_id++) {
		webserver.prepare(serv_id);
	}
	// for(size_t serv_id = 0; serv_id < webserver.getServers().size(); serv_id++) {
		webserver.run();
	// }
	
}