#include "../incs/Config.hpp"
#include <cstdlib>

Config::Config() {}

Config::Config(std::string filename) : _filename(filename) {
	checkExtension();
	_fd.open(_filename.c_str());
	if (_fd.fail() == true)
	{
        std::cout << FRED << "Can not open config" << NONE << std::endl;
        exit(EXIT_FAILURE);
    }
	parseConfig();
	_fd.close();
}

Config::Config(Config const& other) {
	*this = other;
}

Config::~Config() {}

Config &Config::operator=(Config const& other) {
	if (this != &other)
	{
		_filename = other._filename;
		_serverConfig = other._serverConfig;
	}
	return (*this);
}

void Config::checkExtension() {
	if (_filename.find(".conf") == std::string::npos || _filename == ".conf")
	{
		std::cout << FRED << "Wrong extention" << NONE << std::endl;
		exit(EXIT_FAILURE);
	}   
}

void Config::parseConfig() {
	std::getline(_fd, _buff, '\0');
	// std::cout << _buff << std::endl;

	for (size_t i = 0; i < _buff.size(); ++i) {
		i = _buff.find("server", i);
		if (i == std::string::npos)
			break;
		i += std::strlen("server");
		skipWS(i, _buff);
		if (i > _buff.length() || _buff[i] != '{')
			errorMessage("Bad config");
		i += 2;
		parseServer(i);
		if (_buff[i] != '}') 
			errorMessage("Bad config");
		while (_buff[i] == '\n')
			i++;
	}
}

void Config::intitDefaultServerConfig(ServerConfig &serv) {
	serv.listen = DEFAULT_LISTEN;
	serv.ip = DEFAULT_IP;
	serv.port_str = DEFAULT_PORT;
	serv.port = std::atoi(serv.port_str.c_str());
	serv.server_name = "";
	serv.max_body_size = 100;
}

void Config::initDefaultLocation(Location &loc) {
	loc.path = "";
	loc.index  = "";
	loc.root = "";
	loc.autoindex = "off";
}

void Config::parseServer(size_t &i) {
	ServerConfig		serv;
	ConfigDuplicates	configDuplicates;
	size_t				idx_endl = 0;

	memset(&configDuplicates, 0, sizeof(ConfigDuplicates));
	intitDefaultServerConfig(serv);
	for (; i < _buff.size() && _buff[i] != '}'; ++i)
	{
		while (_buff[i] == 32 || _buff[i] == 9)
			i++;
		if (_buff.compare(i, std::strlen("server_name "), "server_name ") == 0 ) {
			parseHelp(i, idx_endl, "server_name ");
			if (configDuplicates.server_name == true) errorMessage("Bad config - duplicates");
			configDuplicates.server_name = true;
			serv.server_name = _buff.substr(i, idx_endl - i - 1); // minus ';' and '\n'	
			i = idx_endl + 1; // skip '\n'
		}
		else if (_buff.compare(i, std::strlen("listen "), "listen ") == 0 ) {
			parseHelp(i, idx_endl, "listen ");
			if (configDuplicates.listen == true) errorMessage("Bad config - duplicates");
			configDuplicates.listen = true;
			serv.listen = _buff.substr(i, idx_endl - i - 1);	
			i = idx_endl + 1;
			setPortIp(serv.listen, serv);
		}
		else if (_buff.compare(i, std::strlen("client_max_body_size "), "client_max_body_size ") == 0 ) {
			parseHelp(i, idx_endl, "client_max_body_size ");
			if (configDuplicates.max_body_size == true) errorMessage("Bad config - duplicates");
			configDuplicates.max_body_size = true;
			serv.max_body_size = std::atoi(_buff.substr(i, idx_endl - i - 1).c_str());
			i = idx_endl + 1; 
		}
		else if (_buff.compare(i, std::strlen("location "), "location ") == 0 ) {
			i += std::strlen("location ");
			parseLocation(i, serv);
			// std::cout << FRED << "Here! "<< _buff.substr(i) << NONE << std::endl;
			idx_endl = _buff.find("\n", i);
			i = idx_endl + 1; 
			// std::cout << FRED << "Here! "<< _buff.substr(i) << NONE << std::endl;
			// std::cout << FRED << "Here1! "<< _buff[i] << "i: " << i << " bsize: " << _buff.size() << NONE << std::endl;
		}
		if (i >= _buff.size() || _buff[i] == '}')
			break;
	}
	// std::cout << FRED << "Here2! "<< _buff[i] << "i: " << i << NONE << std::endl;
	_serverConfig.push_back(serv);
}

void Config::parseHelp(size_t &i, size_t &idx_endl, std::string const str) {
	i += str.length();
	skipWS(i, _buff);
	idx_endl = _buff.find("\n", i); // search for '\n' from i-th symbol
	if (idx_endl == std::string::npos || _buff[idx_endl - 1] != ';')
		errorMessage("Bad config");
}

void Config::parseLocation(size_t &i, ServerConfig &serv) {
	size_t				idx = _buff.find('{', i);
	size_t				idx_endl = _buff.find('\n', i);
	Location			loc;
	ConfigDuplicates	configDuplicates;

	memset(&configDuplicates, 0, sizeof(ConfigDuplicates));
	initDefaultLocation(loc);
	if (idx == std::string::npos || idx_endl == std::string::npos || idx_endl != idx + 1)
		errorMessage("Bad config");
	loc.path = _buff.substr(i, idx - i - 1);
	i = idx;
	for (; i < _buff.size() && _buff[i] != '}'; ++i)
	{
		if (_buff.compare(i, std::strlen("index "), "index ") == 0 ) {
			parseHelp(i, idx_endl, "index ");
			if (configDuplicates.index == true) errorMessage("Bad config - duplicates");
			configDuplicates.index = true;
			loc.index = _buff.substr(i, idx_endl - i - 1);
			i = idx_endl + 1; 
		}
		else if (_buff.compare(i, std::strlen("root "), "root ") == 0 ) {
			parseHelp(i, idx_endl, "root ");
			if (configDuplicates.root == true) errorMessage("Bad config - duplicates");
			configDuplicates.root = true;
			loc.root = _buff.substr(i, idx_endl - i - 1);
			i = idx_endl + 1; 
		}
		else if (_buff.compare(i, std::strlen("autoindex "), "autoindex ") == 0 ) {
			parseHelp(i, idx_endl, "autoindex ");
			if (configDuplicates.autoindex == true) errorMessage("Bad config - duplicates");
			configDuplicates.autoindex = true;
			loc.autoindex = _buff.substr(i, idx_endl - i - 1);
			if (loc.autoindex != "on" && loc.autoindex != "off") errorMessage("Bad config - autoindex");
			i = idx_endl + 1;
		}
		else if (_buff.compare(i, std::strlen("allowed_methods "), "allowed_methods ") == 0 ) {
			parseHelp(i, idx_endl, "allowed_methods ");
			if (configDuplicates.allowed_methods == true) errorMessage("Bad config - duplicates");
			configDuplicates.allowed_methods = true;
			// std::cout << "Here\n";
			std::vector<std::string>			methods = ft_split(_buff.substr(i, idx_endl - i - 1).c_str(), '|');
			std::vector<std::string>::iterator	it = methods.begin();
			if (methods.empty())
				errorMessage("Bad config - fill allowed methods");
			for (; it != methods.end(); ++it)
			{
				// std::cout << *it << std::endl;
				if (*it != "GET" && *it != "POST" && *it != "DELETE") 
					errorMessage("Bad config - allowed methods: GET, POST, DELETE");
			}
			loc.allowedMethods = methods;
			methods.clear();
			i = idx_endl + 1;
		}
	}
	serv.location.push_back(loc);
}

void Config::setPortIp(std::string listen, ServerConfig & conf) {
	std::vector<std::string> tmp = ft_split(listen, ':');

	conf.ip = tmp[0];
	conf.port_str = tmp[1];
	conf.port = std::atoi(tmp[1].c_str());
}

std::string Config::getFileName() {return _filename;}
std::string	Config::getBuff() {return _buff;}
std::vector<ServerConfig> Config::getServerConfigVector() {return _serverConfig;}