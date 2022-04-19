#include "../Server/Server.hpp"

void Server::kill(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (!_clients[actualClient].getClientMode().getMode('o')) throw Exception::ERR_NOPRIVILEGES();

	(void)command;
}