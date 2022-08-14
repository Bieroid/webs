#ifndef SERVER_HPP
# define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>

#include <vector>
#include <list>
#include <map>

#include <string>

#include "Config.hpp"

#define BUFFER_SIZE 2048

typedef struct s_input
{
	u_short port;
	in_addr_t host;
} t_input;

typedef struct s_listener
{
	int	socket;
	u_short port;
	in_addr_t host;
} t_listener;

typedef struct s_request
{
	std::map<std::string, std::string> header;
	std::string req;
} t_request;

typedef struct s_client_info
{
	int cfd;
	
	std::string m_request;
	u_short port;
	in_addr_t host;
	std::string m_response;
	//t_request m_q;
} t_client_info;

class Server
{
private:
	Config config;
	fd_set rfds_all;
	fd_set wfds_all;
	fd_set rfds;
	fd_set wfds;
	int nfds;

	std::vector<t_listener> listen_sockets;
	std::list<t_client_info> client_sockets;

public:
	Server(std::vector<t_input> test, std::string const& config_file);
	~Server();

	void run();

	void initServer();
	void initClient(t_listener listen_s);

	void send_or_request();

	void do_recv(std::list<t_client_info>::iterator *it);
	void recieveRequest(std::list<t_client_info>::iterator *it, const char *buf);

	void do_send(std::list<t_client_info>::iterator *it);

	void fill_request(std::list<t_client_info>::iterator *it);

	void close_connection(std::list<t_client_info>::iterator *it);
};

#endif // SERVER_HPP