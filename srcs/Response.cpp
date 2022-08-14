#include "../incs/Response.hpp"
#include <fstream>

Response::Response() {}

Response::Response(Request &other) {
	_request = &other;
	_response = "";
	_statusLine = "";
	_header_str = "";
	_body = "";
	_HttpVersion = "HTTP/1.1";
	_statusCode = 200;
	_reasonPhrase = CODE_200;
	_pathURL = _request->getPath();
	// std::cout << _pathURL << std::endl;
	_statReturn = stat(_pathURL.c_str(), &_buf); // https://pubs.opengroup.org/onlinepubs/9699919799/functions/stat.html
	_allowedMethods = _request->getLocation().allowedMethods;
	handleRequest();
}

Response::Response(Response const& other) {
	*this = other;
}

Response &Response::operator=(Response const&other) {
	if (this != &other)
	{
		_request = other._request;
		_response = other._response;
		_statusLine = other._statusLine;
		_header = other._header;
		_header_str = other._header_str;
		_body = other._body;
		_HttpVersion = other._HttpVersion;
		_statusCode = other._statusCode;
		_reasonPhrase = other._reasonPhrase;
	}
	return (*this);
}

Response::~Response() {}

bool Response::isValidMethod() {
	std::string method = _request->getMethod();

	if (method != "GET" && method != "POST" && method != "DELETE") return false;
	std::vector<std::string>::iterator it = _allowedMethods.begin();
	for (; it != _allowedMethods.end(); ++it)
		if (*it == method)
			return true;
	return false;
}

void Response::handleRequest() {
	if (_request->getIsBadRequest() == true) return (handleErrorPage(400));
	if (!isValidMethod()) return (handleErrorPage(405));
	if (_request->getHttpVersion() != HTTP_VER) return (handleErrorPage(505));
	
	// else - create default response
	setStatusLine();
	setContentType(getContentType(_request->getLocation().index));
	/* Origin servers MUST include a Date header field in all responses.
	** Example: Date: Tue, 15 Nov 1994 08:12:31 GMT 
	** https://datatracker.ietf.org/doc/html/rfc2616#section-3.3.1 */
	getTime("Date");
	/* HTTP/1.1 servers SHOULD send Last-Modified whenever feasible. */
	getTime("Last-Modified");
	headerMapToStr();
	
	// add methods - GET, POST, DELETE

	std::stringstream	ss;
	
	std::ifstream		file(_pathURL.c_str());
	if (file.is_open())
	{
		ss << file.rdbuf();
		file.close();
	}
	_body = ss.str();

	setResponse();
}

void Response::handleErrorPage(int code) {
	_statusCode = code;
	setAllow();
	setContentType("text/html");
	setRetryAfter();
	setWWWAuthenticate();

	setReasonPhrase();

	setStatusLine(); // get first line of response
	headerMapToStr(); // convert map of headers to string
	setErrorBody(); // get body
	setResponse(); // get full response
}

void Response::headerMapToStr() {
	std::map<std::string, std::string>::iterator	it = _header.begin();
	
	for (; it != _header.end(); ++it)
		_header_str = _header_str + it->first + ": " + it->second + LINE_END;
	_header_str += LINE_END; // header should end with '\r\n\r\n'
}

/*
Time of last access: struct timespec st_atim;
Time of last modification: struct timespec st_mtim;
Time of last status change: struct timespec st_ctim;
*/

/* An origin server MUST NOT send a Last-Modified date which is later
** than the server's time of message origination. */

void Response::getTime(std::string Key) {
	char buffer [80];
	memset(buffer, 0, 80);
	if (_statReturn == 0 && Key == "Last-Modified")
		strftime(buffer,80," %a, %d %b %Y %X %Z", localtime(&(_buf.st_mtime)));
	else
	{
		time_t current_time;
		time(&current_time);
		strftime(buffer,80," %a, %d %b %Y %X %Z", localtime(&current_time));
	}
	_header[Key] = buffer;
}

/*
** https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header
** https://www.iana.org/assignments/media-types/media-types.xhtml
*/
std::string Response::getContentType(std::string fileName) {
	std::string fileExtention;

	if (fileName.find(".") == std::string::npos)
		return "text/plain";
	fileExtention = fileName.substr(fileName.find(".") + 1, fileName.length());
	/* text */
	if (fileExtention == "css") return ("text/css");
	if (fileExtention == "csv") return ("text/csv");
	if (fileExtention == "html") return ("text/html");
	if (fileExtention == "javascript") return ("text/javascript");
	if (fileExtention == "xml") return ("text/xml");
	if (fileExtention == "csv") return ("text/csv");
	/* video/audio */	
	if (fileExtention == "mp4") return ("application/mp4");
	if (fileExtention == "mpeg") return ("audio/mpeg");
	if (fileExtention == "quicktime") return ("video/quicktime");
	/* image */	
	if (fileExtention == "gif") return ("image/gif");
	if (fileExtention == "jpeg") return ("image/jpeg");
	if (fileExtention == "png") return ("image/png");
	if (fileExtention == "tiff") return ("image/tiff");
	if (fileExtention == "vnd.microsoft.icon") return ("image/vnd.microsoft.icon");
	if (fileExtention == "svg+xml") return ("image/svg+xml");
	return ("text/plain");
}

/* GETTERS -------------------------------------------------------------------------------------*/
Request *Response::getRequest() {return _request;}
std::string	Response::getResponse() {return _response;}
std::string	Response::getStatusLine() {return _statusLine;}
std::map<std::string, std::string> Response::getHeaderMap() {return _header;}
std::string Response::getHeader() {return _header_str;}
std::string	Response::getBody() {return _body;}
std::string	Response::getHttpVersion() {return _HttpVersion;}
std::string	Response::getStatusCode() {return (NumberToString(_statusCode));}
int Response::getStatusCodeInt() {return _statusCode;}
std::string Response::getReasonPhrase() {return _reasonPhrase;}
std::vector<std::string> Response::getAllowedMethods() {return _allowedMethods;}

/* SETTERS FOR HEADER --------------------------------------------------------------------------*/
void Response::setAllow() {
	std::string								methods;
	std::vector<std::string>::iterator it = _allowedMethods.begin();
	if (_statusCode == 405)
	{
		for (; it != _allowedMethods.end(); ++it)
		{
			if (it != _allowedMethods.begin())
				methods += ", ";
			methods += *it; 
		}
		_header["Allow"] = methods;
	}
}

void Response::setContentType(std::string type) {
	_header["Content-Type"] = type;
}

/*
** The Retry-After response-header field can be used with a 503 (Service
** Unavailable) response to indicate how long the service is expected to
   be unavailable to the requesting client. This field MAY also be used
   with any 3xx (Redirection) response to indicate the minimum time the
   user-agent is asked wait before issuing the redirected request. The
   value of this field can be either an HTTP-date or an integer number
   of seconds (in decimal) after the time of the response.
*/
void Response::setRetryAfter() {
	if (_statusCode == 503 || (_statusCode >= 300 && _statusCode <= 399))
		_header["Retry-After"] = RETRY_TIME;
}

/* The WWW-Authenticate response-header field MUST be included in 401
   (Unauthorized) response messages. */
void Response::setWWWAuthenticate() {
	if (_statusCode == 401)
		_header["WWW-Authenticate"] = "Basic realm=\"User Visible Realm\", charset=\"UTF-8\"";
}

/* SETTERS FOR PRIVATE ATTRIBUTES --------------------------------------------------------------*/
void Response::setReasonPhrase() {
	if (_statusCode == 200) _reasonPhrase = CODE_200;
	else if (_statusCode == 100) _reasonPhrase = CODE_100;
	else if (_statusCode == 101) _reasonPhrase = CODE_101;
	else if (_statusCode == 201) _reasonPhrase = CODE_201;
	else if (_statusCode == 202) _reasonPhrase = CODE_202;
	else if (_statusCode == 203) _reasonPhrase = CODE_203;
	else if (_statusCode == 204) _reasonPhrase = CODE_204;
	else if (_statusCode == 205) _reasonPhrase = CODE_205;
	else if (_statusCode == 206) _reasonPhrase = CODE_206;
	else if (_statusCode == 300) _reasonPhrase = CODE_300;
	else if (_statusCode == 301) _reasonPhrase = CODE_301;
	else if (_statusCode == 302) _reasonPhrase = CODE_302;
	else if (_statusCode == 303) _reasonPhrase = CODE_303;
	else if (_statusCode == 304) _reasonPhrase = CODE_304;
	else if (_statusCode == 305) _reasonPhrase = CODE_305;
	else if (_statusCode == 307) _reasonPhrase = CODE_307;
	else if (_statusCode == 400) _reasonPhrase = CODE_400;
	else if (_statusCode == 401) _reasonPhrase = CODE_401;
	else if (_statusCode == 402) _reasonPhrase = CODE_402;
	else if (_statusCode == 403) _reasonPhrase = CODE_403;
	else if (_statusCode == 404) _reasonPhrase = CODE_404;
	else if (_statusCode == 405) _reasonPhrase = CODE_405;
	else if (_statusCode == 406) _reasonPhrase = CODE_406;
	else if (_statusCode == 505) _reasonPhrase = CODE_505;
	else
		_reasonPhrase = "UNKNOWN ERROR";
}

void Response::setStatusLine() {
	_statusLine = _HttpVersion + " " + getStatusCode() + " " + _reasonPhrase + LINE_END;
}

void Response::setErrorBody() {
	std::stringstream	ss;
	std::string			error_page; // path to default error_page (will be used if img for statusCode was not found)
	std::string			error_img; // path to img with statusCode
	std::string			currDir;
	char				buf[1024];
	
	getcwd(buf, sizeof(buf));
	currDir = buf;
	currDir = currDir[currDir.size() - 1] == '/' ? currDir.substr(0, currDir.size() - 1) : currDir;
	error_img = currDir + "/www/static/images/" + getStatusCode() + + ".jpg";
	error_page = currDir + "/www/templates/error_page.html";

	std::ifstream		fd(error_img.c_str());
	if (fd.fail() == true)
	{
		std::ifstream	file(error_page.c_str());
		if (file.is_open())
		{
			ss << file.rdbuf();
			file.close();
			_body = ss.str();
		}
		return;
	}
	fd.close();
	ss << "<!DOCTYPE html>\n"
	"<html>\n"
	"\t<head>\n"
		"\t\t<title>\n" 
		"\t\t"<< 
		getStatusCode() << " " << _reasonPhrase << "\n" 
		"\t\t</title>\n"
	"\t</head>\n"
	"\t<body style=\"background-color: black;\">\n"
    "\t\t<img src=\"" + error_img + "\" alt=" + "\"" + getStatusCode() + " " + _reasonPhrase + "\" " + "style=\"display: block; margin-left: auto; margin-right: auto; width: 50%;\">\n"
    "\t</body>\n"
	"</html>";

	_body = ss.str();
}

void Response::setResponse() {
	_response = _response + _statusLine + _header_str + _body;
}
