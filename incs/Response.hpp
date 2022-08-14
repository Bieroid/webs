#ifndef RESPONSE_HPP
# define RESPONSE_HPP

// #include "Utils.hpp"
# include "Request.hpp"

class Response
{
private:
	Request								*_request;
	std::string							_response;
	/* Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF */
	std::string							_statusLine;
	/* These header fields give information about the server and about
	** further access to the resource identified by the Request-URI */
	std::map<std::string, std::string>	_header;
	std::string							_header_str;
	/* Body obtained from the message-body by decoding any Transfer-Encoding that 
	** might have been applied to ensure safe and proper transfer of the message.*/
	std::string							_body;

	std::string							_HttpVersion;
	int									_statusCode;
	std::string							_reasonPhrase;

	struct stat							_buf;
	std::string							_pathURL;
	int									_statReturn;

	std::vector<std::string>			_allowedMethods;
	
	Response();

	
public:
	Response(Request &other);
	Response(Response const& other);

	~Response();

	Response &operator=(Response const&other);

	void handleRequest();
	void handleErrorPage(int code);
	void headerMapToStr();
	void getTime(std::string Key);
	bool isValidMethod();

	std::string getContentType(std::string fileName);
	/* Setters for header */
	void setAllow();
	void setContentType(std::string type);
	void setRetryAfter();
	void setWWWAuthenticate();
	/* Setters for private attributes */
	void setReasonPhrase();
	void setStatusLine();
	void setErrorBody();
	void setResponse();

	void getMethod();
	void postMethod();
	void deleteMethod();

	/* Getters */
	Request *getRequest();
	std::string	getResponse();
	std::string	getStatusLine();
	std::map<std::string, std::string> getHeaderMap();
	std::string getHeader();
	std::string	getBody();
	std::string getHttpVersion();
	std::string	getStatusCode();
	int getStatusCodeInt();
	std::string getReasonPhrase();
	std::vector<std::string> getAllowedMethods();
};

#endif