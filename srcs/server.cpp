#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <cstring>

Server::Server(std::vector<t_input> test, std::string const& config_file): nfds(0)
{
	Config conf(config_file);
	config = conf;
	int t_size = test.size();
	int i = -1;
	t_listener temp;
	while (++i < t_size)
	{
		temp.port = test[i].port;
		temp.host = test[i].host;
		listen_sockets.push_back(temp);
	}
	FD_ZERO(&rfds_all);
	FD_ZERO(&wfds_all);
}

Server::~Server()
{
}

void Server::run()
{
	struct timeval time = {2, 0};
	initServer();

	int retval;

	for(;;)
	{
		//because select is destructive
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		rfds = rfds_all;
		wfds = wfds_all;
		retval = select(nfds + 1, &rfds, &wfds, NULL, NULL); //&time
		if (retval < 0)
		{
			perror("select");
			//continue ;
			exit(EXIT_FAILURE); //continue??
			//continue ;
		}
		else if (retval)
		{
			int lsize = listen_sockets.size();
			for (int i = 0; i < lsize; ++i)
			{
				if (FD_ISSET(listen_sockets[i].socket, &rfds))
				{
					//this is a new connection
					initClient(listen_sockets[i]);
				}
			}
			send_or_request();
		}
		else
			std::cout << "No data\n";
	}
}


void Server::initServer()
{
	struct sockaddr_in server;
	int l_size = listen_sockets.size();
	for(size_t i = 0; i < l_size; ++i)
	{
		listen_sockets[i].socket = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_sockets[i].socket == -1)
		{
			perror("socket");
			exit(EXIT_FAILURE);
		}
		int flag = 1;
		if (-1 == setsockopt(listen_sockets[i].socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)))
		{
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		server.sin_family = AF_INET;

		server.sin_port = listen_sockets[i].port;
		server.sin_addr.s_addr = listen_sockets[i].host;
		if (bind(listen_sockets[i].socket, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) == -1)
		{
			perror("bind");
			exit(EXIT_FAILURE);
		}
		if (listen(listen_sockets[i].socket, SOMAXCONN) == -1)  //FIX. variable?
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}
		FD_SET(listen_sockets[i].socket, &rfds_all);
		if (listen_sockets[i].socket > nfds)
			nfds = listen_sockets[i].socket;
	}
}

void Server::initClient(t_listener listen_s)
{
	struct sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	t_client_info temp;

	client.sin_family = AF_INET;
	client.sin_port = temp.port = listen_s.port;
	client.sin_addr.s_addr = temp.host = listen_s.host;
	int conn_fd = accept(listen_s.socket, reinterpret_cast<struct sockaddr *>(&client), &clientSize);
	if (conn_fd == -1)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	FD_SET(conn_fd, &rfds_all);
	temp.cfd = conn_fd;
	temp.m_response = "Hello, world!";
	client_sockets.push_back(temp);
	if (conn_fd > nfds)
		nfds = conn_fd;
	fcntl(conn_fd, F_SETFL, O_NONBLOCK);
}


void Server::send_or_request()
{
	std::list<t_client_info>::iterator it_end = client_sockets.end();
	std::list<t_client_info>::iterator it = client_sockets.begin();
	int i = 0;
	for (; it != it_end; ++it)
	{
		//std::cout << "i = " << i++ << std::endl;
		if (FD_ISSET((*it).cfd, &rfds))
		{
			do_recv(&it);
			continue ;
		}
		if (FD_ISSET((*it).cfd, &wfds))
		{
			do_send(&it);
		}
	}
}

void Server::do_recv(std::list<t_client_info>::iterator *it)
{
	std::list<t_client_info>::iterator tmp;
	char buf[BUFFER_SIZE + 1];
	bzero(buf, BUFFER_SIZE + 1);
	int res = recv((*(*it)).cfd, buf, BUFFER_SIZE, 0);
	bool con_len = false;
	if (res == -1)
	{
		perror("close connection, recv");
		exit(1);
		tmp = (*it)--;
		close_connection(&tmp);
	}
	else if (res == 0)
	{
		tmp = (*it)--;
		FD_CLR((*tmp).cfd, &rfds_all);
		FD_CLR((*tmp).cfd, &wfds_all);
		close_connection(&tmp);
	}
	else
	{
		recieveRequest(it, buf);
		long long s;
		if ((s = (*(*it)).m_request.find("\r\n\r\n")) != std::string::npos)
		{
			long long body = 0;
			if (body = (*(*it)).m_request.find("Content-Length") != std::string::npos)
			{
				con_len = true;
				body = atoll((*(*it)).m_request.substr(body + 15, (*(*it)).m_request.find("\r\n", body) - body).c_str());
			}
			if (//(*(*it)).m_request.substr(0, 5).find("PUT") != std::string::npos ||
						(*(*it)).m_request.substr(0, 5).find("POST") != std::string::npos)
			{
				if ((con_len && ((*(*it)).m_request.substr(s + 4).size() >= body)) || ((*(*it)).m_request.substr(s + 4).find("\r\n\r\n") != std::string::npos))
				{
					//fill_request(it);
					FD_SET((*(*it)).cfd, &wfds_all);
					FD_CLR((*(*it)).cfd, &rfds_all);
				}
				else
					return ;
			}
			else
			{
				//fill_request(it);
				FD_SET((*(*it)).cfd, &wfds_all);
				FD_CLR((*(*it)).cfd, &rfds_all);
			}
		}
		else
			return ;
	}
	std::cout << "Client: " << (*(*it)).cfd << " had a sent request.\n"
		<< (*(*it)).m_request;
	//std::map<std::string, std::string>::iterator its = (*(*it)).m_q.header.find("content-length");
	//std::cout << its->second;


	Request req((*(*it)));
	std::vector<ServerConfig>	servers = config.getServerConfigVector(); 
	ServerConfig				serv = servers[0]; // первый сервер - сервер по умолчаниюж
	// Выбираю нужный сервер из конфига по названию хоста - по хорошему нужно еще сверить port
	// https://nginx.org/ru/docs/http/request_processing.html#mixed_name_ip_based_servers
	std::vector<ServerConfig>::iterator	it_serv = servers.begin();
	for (; it_serv != servers.end(); ++it_serv)
	{
		if (it_serv->server_name == req.getHeader()["Host"])
		{
			serv = *it_serv;
			break;
		}
	}
	req.createPath(serv);
	Response resp(req);

	(*(*it)).m_response = resp.getResponse();
}


void Server::do_send(std::list<t_client_info>::iterator *it)
{
	char buf[BUFFER_SIZE + 1];
	std::list<t_client_info>::iterator tmp;

	ssize_t n = send((*(*it)).cfd, (*(*it)).m_response.c_str(), (*(*it)).m_response.size(), 0);
	if (n <= 0)
	{
		FD_CLR((*(*it)).cfd, &rfds_all);
		FD_CLR((*(*it)).cfd, &wfds_all);
		tmp = (*it)--;
		close_connection(&tmp);
	}
	if (n < (*(*it)).m_response.size())
		;//do something;
	else
	{
		FD_CLR((*(*it)).cfd, &rfds_all);
		FD_CLR((*(*it)).cfd, &wfds_all);
		
		tmp = (*it)--;
		close_connection(&tmp);
	}
	
}

void Server::recieveRequest(std::list<t_client_info>::iterator *it, const char* buf)
{
	(*(*it)).m_request.append(buf);
}
/*
void Server::fill_request(std::list<t_client_info>::iterator *it)
{
	size_t f1, f2;
	std::string s;

	f1 = (*(*it)).m_request.find(" ");
	s = (*(*it)).m_request.substr(0, f1);
	(*(*it)).m_q.header.insert(std::pair<std::string, std::string>("method", s));

	f2 = (*(*it)).m_request.find(" ", f1 + 1);
	s = (*(*it)).m_request.substr(f1 + 1, f2 - f1 - 1);
	(*(*it)).m_q.header.insert(std::pair<std::string, std::string>("uri", s));

	if ((f1 = (*(*it)).m_request.find("Content-Length")) != std::string::npos)
	{
		f1 += 15;
		if (f2 = (*(*it)).m_request.find("\r\n", f1) != std::string::npos)
			s = (*(*it)).m_request.substr(f1, f2 - f1 - 1);
		(*(*it)).m_q.header.insert(std::pair<std::string, std::string>("content-length", s));
	}
	
	f1 = (*(*it)).m_request.find("\r\n\r\n");
	if ((*(*it)).m_request.size() != f1 + 4)
	{
		f1 += 4;
		s = (*(*it)).m_request.substr(f1, (*(*it)).m_request.size() - f1);
		(*(*it)).m_q.header.insert(std::pair<std::string, std::string>("body", s));
	}
}
*/
void Server::close_connection(std::list<t_client_info>::iterator *it)
{
	close ((*(*it)).cfd);
	client_sockets.erase(*it);
}
