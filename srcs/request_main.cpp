// #include "../incs/Request.hpp"
// #include "../incs/Response.hpp"
// #include <fstream>  


// int main() {
// 	std::string request;

// 	/// BAD REQUEST (NO HOST WITH HTTP 1.1) -------------------------------------
// // 	request = "GET /hello.html HTTP/1.1\r\n\
// // Connection: keep-alive\r\n\
// // Accept:   			  text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\
// // User-Agent: Chrome/45.0.2454.93\r\n\
// // Accept-Encoding: gzip,deflate,sdch\r\n\
// // Accept-Language: en-US,en;q=0.8\r\n\
// // \r\n";
	
// 	/* METHOD NOT ALLOWED -------------------------------------*/
// 	request = "LOL /hello.html HTTP/1.1\r\n\
// Host: 127.0.0.1:8080\r\n\
// Connection: keep-alive\r\n\
// Accept:   			  text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\
// User-Agent: Chrome/45.0.2454.93\r\n\
// Accept-Encoding: gzip,deflate,sdch\r\n\
// Accept-Language: en-US,en;q=0.8\r\n\
// \r\n";

// /* HTTP VERSION NOT SUPPORTES -------------------------------------*/
// // 	request = "GET /hello.html HTTP/1.0\r\n\
// // Host: 127.0.0.1:8000\r\n\
// // Connection: keep-alive\r\n\
// // Accept:   			  text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\
// // User-Agent: Chrome/45.0.2454.93\r\n\
// // Accept-Encoding: gzip,deflate,sdch\r\n\
// // Accept-Language: en-US,en;q=0.8\r\n\
// // \r\n";

// /* NORM -------------------------------------*/
// // 	request = "GET /hello.html HTTP/1.1\r\n\
// // Host: 127.0.0.1:8000\r\n\
// // Connection: keep-alive\r\n\
// // Accept:   			  text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\
// // User-Agent: Chrome/45.0.2454.93\r\n\
// // Accept-Encoding: gzip,deflate,sdch\r\n\
// // Accept-Language: en-US,en;q=0.8\r\n\
// // \r\n";

// 	Request req(request);

// 	// std::cout << req.getBody() << std::endl;

// 	std::cout << "Body: " << req.getBody() << std::endl;
// 	std::cout << "First line: " << req.getRequestLine() << std::endl;
// 	std::cout << "Method: " << req.getMethod() << std::endl;
// 	std::cout << "URL: " << req.getURL() << std::endl;
// 	std::cout << "HTTPv : " << req.getHttpVersion() << std::endl;

// 	std::cout << "map_size: " << req.getHeader().size() << std::endl; 
	
// 	std::map<std::string, std::string> m = req.getHeader();
// 	std::map<std::string, std::string>::iterator it = m.begin();
// 	for (; it != m.end(); ++it)
// 		std::cout << it->first << ": " << it->second << std::endl;
// 	// std::cout << request;

// 	std::cout << "------------------------------------------------------\n";
// 	Response res(req);
// 	std::cout << res.getResponse();
// 	// // // std::ofstream { "../templates/example.html" };

// 	// // // std::ofstream outfile("templates\\example.html");
// 	// // std::ofstream outfile("templates/example.html");
// 	// outfile << res.getBody() << std::endl;
// 	// // outfile.close();
// 	// // std::cout << "------------------------------------------------------\n";
// 	// std::cout << res.getHeader();
// }