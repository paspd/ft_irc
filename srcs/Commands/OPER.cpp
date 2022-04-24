#include "../Server/Server.hpp"

void Server::oper(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (_clients[actualClient].getClientMode()._restricted) throw Exception::ERR_RESTRICTED(_clients[actualClient].getClientNickname());
	if (command.size() < 3) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
	if (command[1] != _clients[actualClient].getClientNickname()) throw Exception::ERR_USERSDONTMATCH(_clients[actualClient].getClientNickname());
	if (command[2] != OPER_MDP) throw Exception::ERR_PASSWDMISMATCH();

	_clients[actualClient].setOP(true);

	sendMessage(CLIENT_SOCKET, RPL_UMODEIS(_clients[actualClient].getClientNickname(), _clients[actualClient].getStrMode()));
	sendMessage(CLIENT_SOCKET, RPL_YOUREOPER(_clients[actualClient].createClientPrompt()));
}