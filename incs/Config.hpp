#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Utils.hpp"
#include <fstream>

struct Location {
	std::string					path;
	std::string					index;
	std::string					root;
	std::vector<std::string>	allowedMethods;
	std::string					autoindex;
};

struct ServerConfig {
	std::string				listen;
	std::string				ip;
	std::string				port_str;
	int						port;
	std::string				server_name;
	std::vector<Location>	location;
	int						max_body_size;
};

struct ConfigDuplicates {
	bool	listen;
	bool	ip;
	bool	port;
	bool	server_name;
	bool	max_body_size;
	bool	index;
	bool	root;
	bool	allowed_methods;
	bool	autoindex;
};

class Config
{
private:
	std::ifstream				_fd;
	std::string					_filename;
	std::string					_buff;
	std::vector<ServerConfig>	_serverConfig;

public:
	Config();
	Config(std::string filename);
	Config(Config const& other);
	~Config();

	Config &operator=(Config const& other);

	void checkExtension();
	void parseConfig();
	void parseServer(size_t &i);
	void parseHelp(size_t &i, size_t &idx_endl, std::string const str);
	void parseLocation(size_t &i, ServerConfig &serv);
	void intitDefaultServerConfig(ServerConfig &serv);
	void initDefaultLocation(Location &loc);

	void setPortIp(std::string listen, ServerConfig & conf);

	std::string getFileName();
	std::string	getBuff();
	std::vector<ServerConfig> getServerConfigVector();
};

#endif