#include "../Server/Server.hpp"

void Server::pass(std::vector<std::string> command, int actualClient) {
	if (_clients[actualClient].getPassBool()) throw Exception::ERR_ALREADYREGISTERED();
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
	if (command.size() == 2 && command[1] != _password) throw Exception::ERR_PASSWDMISMATCH();

	_clients[actualClient].passBoolClient();
	std::cout << BOLD << ORANGE << "Client number " << actualClient << END << ORANGE << " has been accepted." << END << std::endl << std::endl;
}