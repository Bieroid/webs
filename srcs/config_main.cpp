// #include "../incs/Config.hpp"

// int main(void)
// {
//     Config config("./config/default.conf");
//     // Config config("./config/error_conf");
//     std::vector<ServerConfig>           serv = config.getServerConfigVector();
//     std::vector<ServerConfig>::iterator it = serv.begin();
    
//     std::cout << "Size: " << serv.size() << std::endl;

//     for (; it != serv.end(); ++it)
//     {
//         std::cout << "NAME: " << it->server_name << std::endl;
// 		std::cout << "listen: " << it ->listen << std::endl;
// 		std::cout << "port: " << it ->port << std::endl;
// 		std::cout << "ip: " << it ->ip << std::endl;
// 		std::cout << "client_max_body_size: " << it->max_body_size << std::endl;
//         std::vector<Location> loc = it->location;
//         std::vector<Location>::iterator it_loc = loc.begin();
//         for (; it_loc != loc.end(); ++it_loc)
// 		{
// 			std::cout << "path: " << it_loc->path << std::endl;
// 			std::cout << "ind: " << it_loc->index << std::endl;
// 			std::cout << "root: " << it_loc->root << std::endl;
// 			std::cout << "autoindex: " << it_loc->autoindex << std::endl;
// 			std::vector<std::string>::iterator it_meth = it_loc->allowedMethods.begin();
// 			for (; it_meth != it_loc->allowedMethods.end(); ++it_meth)
// 				std::cout << *it_meth << std::endl;
// 		}
//     }
//     return (0);
// }