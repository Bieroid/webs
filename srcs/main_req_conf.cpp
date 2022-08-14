// #include "../incs/Request.hpp"
// #include "../incs/Response.hpp"
// #include "../incs/Config.hpp"
// #include <fstream>  

// int main() {
//     Config config("./config/default.conf");
// 	std::string request;

// 	/* METHOD NOT ALLOWED -------------------------------------*/
// 	request = "GET /info HTTP/1.1\r\n\
// Host: 127.0.0.1:8080\r\n\
// Connection: keep-alive\r\n\
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\
// User-Agent: Chrome/45.0.2454.93\r\n\
// Accept-Encoding: gzip,deflate,sdch\r\n\
// Accept-Language: en-US,en;q=0.8\r\n\
// \r\n";
// 	Request req(request);
// 	std::vector<ServerConfig>	servers = config.getServerConfigVector(); 
// 	ServerConfig				serv = servers[0]; // первый сервер - сервер по умолчаниюж
// 	// Выбираю нужный сервер из конфига по названию хоста - по хорошему нужно еще сверить port
// 	// https://nginx.org/ru/docs/http/request_processing.html#mixed_name_ip_based_servers
// 	std::vector<ServerConfig>::iterator	it = servers.begin();
// 	for (; it != servers.end(); ++it)
// 	{
// 		if (it->server_name == req.getHeader()["Host"])
// 		{
// 			serv = *it;
// 			break;
// 		}
// 	}
// 	std::cout << "Server name:" << serv.server_name << std::endl;
// 	// call method that creates path in request
// 	req.createPath(serv);
// 	std::cout << "URLComponent :" << req.getURL() << std::endl;
// 	std::cout << "Path_loc :" << req.getLocation().path << std::endl;
// 	std::cout << "Path :" << req.getPath() << std::endl;
// 	std::cout << "------------------------------------------------------\n";
// 	Response res(req);
// 	std::cout << res.getResponse();
	
// 	// std::ofstream { "../templates/example.html" };

// 	// // // std::ofstream outfile("templates\\example.html");
// 	std::ofstream outfile("templates/example.html");
// 	outfile << res.getBody() << std::endl;
// 	outfile.close();
// 	// // std::cout << "------------------------------------------------------\n";
// 	// std::cout << res.getHeader();
// }