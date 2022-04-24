#include "../Server/Server.hpp"

void Server::nick(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getPassBool()) throw Exception::ERR_NOTREGISTERED();
	if (_clients[actualClient].getClientMode()._restricted) throw Exception::ERR_RESTRICTED(_clients[actualClient].getClientNickname());
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
	if (command.size() > 3) throw Exception::ERR_NONICKNAMEGIVEN();
	if (!_checkValidityNick(command[1])) throw Exception::ERR_ERRONEOUSNICKNAME(command[1]);
	if (command[1] == _clients[actualClient].getClientNickname()) throw Exception::ERR_NICKNAMEINUSE(command[1]);
	for (size_t j = 0; j < MAX_CLIENTS; j++) {
		if (command[1] == _clients[j].getClientNickname()) {
			throw Exception::ERR_NICKCOLLISION(command[1]);
			break ;
		}
		else if (j == MAX_CLIENTS - 1) {
			_clients[actualClient].setClientNickname(command[1]);
				_clients[actualClient].nickBoolClient();
				std::cout << BOLD << ORANGE << "Client number " << actualClient << END << ORANGE << " has been change his nickname to : " << _clients[actualClient].getClientNickname() << "." << END << std::endl << std::endl;
		}
	}
}
