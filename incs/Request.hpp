#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Utils.hpp"
# include "Config.hpp"

# include "Server.hpp"

// https://people.eecs.berkeley.edu/~kubitron/courses/cs162-S19/sp19/static/hw/hw2.pdf
class Request
{
private:
	std::string							_message;
	
	/* Data from request line: method, path component of the URL for the request, HTTP version number. */
	std::string							_request_line;
	std::vector<std::string>			_request_line_vec;
	std::string							_method;
	std::string							_URL_component;
	std::string							_HTTP_version;

	/* Data from HTTP headers in form of dictionary (map). */
	std::map<std::string, std::string>	_header;

	/* Message body*/
	std::string							_body;

	bool								_isBadRequest;

	Location							_location;
	std::string							_path;
	std::string							_query;
	std::string							_index;

	Request(); // just for canonical form, will not be used
	
public:
	Request(std::string const& str);
	Request(t_client_info req);
	Request(Request const& other);
	~Request();

	Request &operator=(Request const& other);

	void	parseRequest(); // main part
	void	createPath(ServerConfig &server_conf);
	void	chooseLocation(ServerConfig &server_conf);

	/* Getters */
	std::string							getMessage(void) const;
	std::string							getRequestLine(void) const;
	std::vector<std::string>			getRequestLineVec(void) const;
	std::string							getMethod(void) const;
	std::string							getURL(void) const;
	std::string							getHttpVersion(void) const;
	std::map<std::string, std::string>	getHeader(void) const;
	std::string							getBody(void) const;
	bool								getIsBadRequest(void) const;
	Location							getLocation(void) const;
	std::string							getPath(void) const;
	std::string							getQuery(void) const;
};

#endif