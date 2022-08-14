#include "../incs/Request.hpp"

Request::Request() {}

Request::Request(std::string const& str) : _message(str) {
	_request_line = "";
	_method = "";
	_URL_component = "";
	_HTTP_version = "";
	_body = "";
	_isBadRequest = false;
	_path = "";
	_query = "";
	_index = "";
	parseRequest();
}


Request::Request(t_client_info req) : _message(req.m_request) {
	_request_line = "";
	_method = "";
	_URL_component = "";
	_HTTP_version = "";
	_body = "";
	_isBadRequest = false;
	_path = "";
	_query = "";
	_index = "";
	parseRequest();
}





// CHECK
void Request::createPath(ServerConfig &server_conf) {
	chooseLocation(server_conf);
	size_t	pos = _URL_component.find("?");
	if (pos != std::string::npos)
		_query = _URL_component.substr(pos + 1);
	_URL_component = _URL_component.substr(0, pos);
	char	currDir[1024];
	getcwd(currDir, sizeof(currDir));
	std::string Dir = currDir;
	std::string root = _location.root;
	// currdir + root in location
	Dir = Dir[Dir.size() - 1] == '/' ? Dir.substr(0, Dir.size() - 1) : Dir;
	root = root[0] == '/' ? root.substr(1, root.size()) : root;
	_path = Dir + "/" + root;
	// path + _location
	_path = _path[_path.size() - 1] == '/' ? _path.substr(0, _path.size() - 1) : _path;
	_URL_component = _location.path[0] == '/' ? _location.path.substr(1, _location.path.size()) : _location.path;
	_path = _path + "/" + _URL_component;
	// path + index
	_path = _path[_path.size() - 1] == '/' ? _path.substr(0, _path.size() - 1) : _path;
	_index = _location.index[0] == '/' ? _location.index.substr(1, _location.index.size()) : _location.index;
	_path = _path + "/" + _index;
}

void Request::chooseLocation(ServerConfig &server_conf) {
	_location.path = "";
	// chose location
	std::vector<Location>::iterator it = server_conf.location.begin();
	for (; it != server_conf.location.end(); ++it)
	{
		if (it->path == _URL_component.substr(0, it->path.size()))
		{
			if (it->path == _URL_component || it->path == "/" || _URL_component[it->path.size()] == '/')
			{
				if (_location.path.size() < it->path.size())
					_location = *it;
			}
		}
	}
}

Request::Request(Request const& other) {
	*this = other;
}

Request::~Request() {}

Request &Request::operator=(Request const& other) {
	if (this != &other) {
		_message = other.getMessage();
		_request_line = other.getRequestLine();
		_method = other.getMethod();
		_URL_component = other.getURL();
		_HTTP_version = other.getHttpVersion();
		_header = other.getHeader();
		_body = other.getBody();
		_isBadRequest = false;
	}
	return (*this);
}

void Request::parseRequest() {
	size_t		header_end;
	size_t		line_end;
	size_t		colon; // colon position in header
	std::string	tmp; // request line + header info
	std::string	line; // line in header
	std::string	key;
	std::string	value;

	/* Get body */
	header_end = _message.find(HEADER_END);
	tmp = _message.substr(0, header_end);
	_body = header_end != std::string::npos ?_message.substr(header_end + 4, _message.length()) : "";

	/* Get request line (first line in request) */
	line_end = _message.find(LINE_END);
	_request_line = tmp.substr(0, line_end);
	tmp = tmp.substr(line_end + 2, tmp.length()); // tmp contains only header now

	/* Get method, URL and HTTP v from request line. */
	_request_line_vec = ft_split(_request_line, ' ');
	_method = _request_line_vec[0];
	_URL_component = _request_line_vec[1];
	_HTTP_version = _request_line_vec[2];

	if (_request_line_vec.size() != 3)
		_isBadRequest = true;
	/* Parse header
	** Each header field consists of a name followed by a colon (":") and the field value.
	** The field value MAY be preceded by any amount of LWS, though a single SP is preferred.
	*/
	line_end = tmp.find(LINE_END);
	while (1)
	{
		line = tmp.substr(0, line_end);
		tmp = line_end != std::string::npos ? tmp.substr(line_end + 2, tmp.length()) : "";
		colon = line.find(":");
		key = line.substr(0, colon);
		colon += 1;
		while (line[colon] == 32 || line[colon] == 9) // while char in line is space or tab - skip them
			colon++;
		value = line.substr(colon);
		if (!key.length() || !value.length())
			_isBadRequest = true;
		else
			_header.insert(make_pair(key, value));
		if (tmp.length() != 0)
			line_end = tmp.find(LINE_END);
		else
			break;
	}
	/*  All Internet-based HTTP/1.1 servers MUST respond with a 400 (Bad Request)
	** status code to any HTTP/1.1 request message which lacks a Host header field. */
	if (_HTTP_version == "HTTP/1.1" && !_header.count("Host"))
		_isBadRequest = true;
	/* If a request contains a message-body and a Content-Length is not given,
	** the server SHOULD respond with 400 (bad request). */
	if (_body.length() > 0 && !_header.count("Content-Length"))
		_isBadRequest = true;
	// if (_isBadRequest == true)
	// 	std::cout << "true\n";
	// else
	// 	std::cout << "Fake\n";
}

std::string	Request::getMessage(void) const {return _message;}
std::string Request::getRequestLine(void) const { return _request_line;}
std::vector<std::string> Request::getRequestLineVec(void) const {return _request_line_vec;}
std::string Request::getMethod(void) const {return _method;}
std::string	Request::getURL(void) const {return _URL_component;}
std::string Request::getHttpVersion(void) const {return _HTTP_version;}
std::map<std::string, std::string>  Request::getHeader(void) const {return _header;}
std::string Request::getBody(void) const {return _body;}
bool Request::getIsBadRequest(void) const {return _isBadRequest;}
Location Request::getLocation(void) const {return _location;}
std::string Request::getPath(void) const {return _path;}
std::string Request::getQuery(void) const {return _query;}


