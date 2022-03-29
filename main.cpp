#include "Server/Server.hpp"

int main (int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> " << "<password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		Server server(atoi(argv[1]), argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	
	return (EXIT_SUCCESS);
}