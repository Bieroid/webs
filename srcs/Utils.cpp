#include "../incs/Utils.hpp"
#include <cstdlib>

std::vector<std::string> ft_split(std::string str, char del) {
	std::vector<std::string>	vec;
	size_t						pos;
	std::string					tmp;
	
	if (str.length() == 0)
		return vec;
	pos = str.find(del);
	if (pos == std::string::npos)
		vec.push_back(str);
	while (pos != std::string::npos)
	{
		tmp = str.substr(0, pos);
		if (tmp.length() > 0)
			vec.push_back(tmp);
		str = str.substr(pos + 1, str.length());
		pos = str.find(del);
		if (pos == std::string::npos)
			break;
	}
	if (str.length() > 0)
		vec.push_back(str);
	return vec;
}

void errorMessage(std::string msg) {
	std::cout << FRED << msg << NONE << std::endl;
	exit(EXIT_FAILURE);
}

void skipWS(size_t &i, std::string const& str) {
	while (str[i] == 32 || str[i] == 9)
		i++;
}
