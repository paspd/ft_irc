#include "../Server/Server.hpp"

void Server::user(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getPassBool()) throw Exception::ERR_NOTREGISTERED();
	if (_clients[actualClient].getUserBool()) throw Exception::ERR_ALREADYREGISTERED();
	if (command.size() < 5) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	_clients[actualClient].setClientUsername(command[1]);
	for (size_t i = 0; i < command[2].size(); i++)
		_clients[actualClient].setClientMode(command[2]);
	_clients[actualClient].setClientRealname(_strcatArguments(command.begin() + 4, command.end()));
	_clients[actualClient].userBoolClient();

	std::cout << BOLD << ORANGE << "Client number " << actualClient << END \
	<< ORANGE << " has been change his username to : " << _clients[actualClient].getClientUsername() \
	<< " and his realname to :" << _clients[actualClient].getClientRealname() << END << std::endl << std::endl;
}
