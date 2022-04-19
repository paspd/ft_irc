#include "../Server/Server.hpp"

void Server::ping(std::vector<std::string> command, int actualClient) {
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	sendMessage(CLIENT_SOCKET, RPL_PONG());
}