#include "../Server/Server.hpp"

void Server::quit(std::vector<std::string> command, int actualClient) {
	std::string msg;
	if (command.size() > 1)
		msg = _strcatArguments(command.begin() + 1, command.end());

	if (msg.empty()) {
		_clients[actualClient].sendToAllCurrent(RPL_QUIT(_clients[actualClient].createClientPrompt()));
		sendMessage(CLIENT_SOCKET, RPL_QUIT(_clients[actualClient].createClientPrompt()));
	}
	else {
		_clients[actualClient].sendToAllCurrent(RPL_QUIT_CUSTOM(_clients[actualClient].createClientPrompt(), msg));
		sendMessage(CLIENT_SOCKET, RPL_QUIT_CUSTOM(_clients[actualClient].createClientPrompt(), msg));
	}

	_clients[actualClient].leaveAllChannels();
	close(_clients[actualClient].getClientSocket());
	_clients[actualClient].resetClient();
}