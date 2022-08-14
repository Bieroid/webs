#include <iostream>
#include <csignal>
#include "Webserv.hpp"
#include "../incs/Utils.hpp"

int main(int argc, char **argv)
{
	std::string	config;

	if (argc > 2)
	{
		std::cerr << FRED << "Usage: ./webserv [configuration_file]" << NONE << std::endl;
		return (1);
	}
	config = argc == 1 ? config = "./config/default.conf" : argv[1];

	t_input x, y;
	x.host = htonl(INADDR_ANY);
	x.port = htons(8080);
	y.host = htonl(INADDR_ANY);
	y.port = htons(8090);
	
	std::vector<t_input> test(2);
	test.at(0) = x;
	test.at(1) = y;
	
	
	//Server	my_serv(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY);
	Server	my_serv(test, config);
	my_serv.run();
	std::cout << "Hello, world!\n";
	(void) argv;
	(void) argc;
	return (0);
}
