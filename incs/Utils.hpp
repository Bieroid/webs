#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <utility> // make_pair
# include <sstream>
# include <sys/stat.h>
# include <time.h> /* time_t, struct tm, time, localtime, strftime */
# include <cstring> // memset
# include <unistd.h> // getcwd

# define HEADER_END		"\r\n\r\n"
# define LINE_END		"\r\n"
# define HTTP_VER		"HTTP/1.1"
# define DEFAULT_LISTEN	"127.0.0.1:8080"
# define DEFAULT_IP		"127.0.0.1"
# define DEFAULT_PORT	"8080"

/*fonts color*/
# define FBLACK		"\033[0;30m"
# define FRED		"\033[0;31m"
# define FGREEN		"\033[0;32m"
# define FYELLOW	"\033[0;33m"
# define FBLUE		 "\033[0;34m"
# define FPURPLE	"\033[0;35m"
# define FCYAN		"\x1b[36m"

/*end color*/
# define NONE        "\033[0m"

#define CODE_100	"Continue"
#define CODE_101	"Switching Protocols"
#define CODE_200	"Ok"
#define CODE_201	"Created"
#define CODE_202	"Accepted"
#define CODE_203	"Non-Authoritative Information"
#define CODE_204	"No Content"
#define CODE_205	"Reset Content"
#define CODE_206	"Partial Content"
#define CODE_300	"Multiple Choices"
#define CODE_301	"Moved Permanently"
#define CODE_302	"Found"
#define CODE_303	"See Other"
#define CODE_304	"Not Modified"
#define CODE_305	"Use Proxy"
#define CODE_307	"Use Proxy"
#define CODE_400	"Bad Request"
#define CODE_401	"Unauthorized"
#define CODE_402	"Payment Required"
#define CODE_403	"Forbidden"
#define CODE_404	"Not Found"
#define CODE_405	"Method Not Allowed"
#define CODE_406	"Not Acceptable"
#define CODE_505	"HTTP Version not supported"

#define RETRY_TIME  10

// https://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
// Converting num to sting
template <typename T>
std::string NumberToString ( T Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

std::vector<std::string> ft_split(std::string str, char del);
void errorMessage(std::string msg);
void skipWS(size_t &i, std::string const& str);

#endif