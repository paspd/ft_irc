#include "../Server/Server.hpp"

void Server::notice(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) return ;
	if (command.size() < 2) return ;

	std::vector<std::string> clientName = _split(command[1], ",");
	std::string message = "";

	if (command.size() > 2) {
		message = _strcatArguments(command.begin() + 2, command.end());
		if (message[0] != ':')
			message = ":" + message;
	}

	int index = 0;
	for (size_t i = 0; i < clientName.size(); i++) {
		if (!clientName[i].find_first_of("#!&+")) {
			if ((index = _channelExist(clientName[i])) >= 0) {
				if (_channels[index].checkClientConnected(_clients[actualClient]) >= 0 || !_channels[index].getMode('n', _clients[actualClient])) {
					if (!_channels[index].getMode('m', _clients[actualClient]) || _channels[index].getMode('C', _clients[actualClient]) || _channels[index].getMode('o', _clients[actualClient]) || _channels[index].getMode('v', _clients[actualClient])) {
						if (!message.empty())
							_channels[index].userSendToChannel(_clients[actualClient].getClientSocket(), RPL_NOTICE_MESSAGE(_clients[actualClient].createClientPrompt(), clientName[i], message));
						}
						else return ;
				}
				else return ;
			}
			else
				return ;
		}
		else if ((index = _clientExist(clientName[i])) >= 0) {
			sendMessage(_clients[index].getClientSocket(), RPL_NOTICE_MESSAGE(_clients[actualClient].createClientPrompt(), clientName[i], message));
		}
		else
				return ;
	}
}
