#include "../Server/Server.hpp"

void Server::oper(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 3) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
	if (command[1] != _clients[actualClient].getClientNickname()) throw Exception::ERR_USERSDONTMATCH(_clients[actualClient].getClientNickname());
	if (command[2] != OPER_MDP) throw Exception::ERR_PASSWDMISMATCH();

	_clients[actualClient].getClientMode().setMode("+o");
	sendMessage(CLIENT_SOCKET, RPL_YOUREOPER(_clients[actualClient].createClientPrompt()));
}