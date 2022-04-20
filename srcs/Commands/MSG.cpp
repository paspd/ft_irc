#include "../Server/Server.hpp"

void Server::msg(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

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
					if (message.empty())
						throw Exception::ERR_NOTEXTTOSEND(_clients[actualClient].getClientNickname());
					else
						_channels[index].userSendToChannel(_clients[actualClient].getClientSocket(), RPL_PRIVMSG_MESSAGE(_createClientPrompt(_clients[actualClient]), clientName[i], message));
					}
				}
				else throw Exception::ERR_CANNOTSENDTOCHAN(clientName[i]);
			}
			else
				throw Exception::ERR_NOSUCHNICK(clientName[i]);
		}
		else if ((index = _clientExist(clientName[i])) >= 0) {
			sendMessage(_clients[index].getClientSocket(), RPL_PRIVMSG_MESSAGE(_createClientPrompt(_clients[actualClient]), clientName[i], message));
		}
		else
				throw Exception::ERR_NOSUCHNICK(clientName[i]);
	}
}