#include "../Server/Server.hpp"

void Server::kick(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (_clients[actualClient].getClientMode()._restricted) throw Exception::ERR_RESTRICTED(_clients[actualClient].getClientNickname());
	if (command.size() < 3) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	std::vector<std::string> chanName = _split(command[1], ",");
	std::vector<std::string> clientName = _split(command[2], ",");

	int indexChan = 0;
	int indexClient = 0;

	for (size_t i = 0; i < chanName.size(); i++) {
		if ((indexChan = _channelExist(chanName[i])) >= 0) {
			if (clientName.size() >= i) {
				if ((indexClient = _clientExist(clientName[i])) >= 0) {
					if (_channels[indexChan].checkClientConnected(clientName[i]) >= 0) {
						if (_channels[indexChan].checkClientConnected(clientName[i]) >= 0) {
							_channels[indexChan].delOccupant(clientName[i]);
							_clients[indexClient].leaveChannel(chanName[i]);
							AFF(_clients[indexClient].getClientSocket());
							sendMessage(_clients[indexClient].getClientSocket(), RPL_KICK_CMD(_clients[actualClient].createClientPrompt(), clientName[i], chanName[i], (command.size() >= 4 ? _strcatArguments(command.begin() + 3, command.end()) : "")));
							_channels[indexChan].sendToAllChannel(RPL_KICK_CMD(_clients[actualClient].createClientPrompt(), clientName[i], chanName[i], (command.size() >= 4 ? _strcatArguments(command.begin() + 3, command.end()) : clientName[i])));
						}
					}
					else throw Exception::ERR_USERNOTINCHANNEL(clientName[i], chanName[i]);
				}
				else throw Exception::ERR_NOSUCHNICK(clientName[i]);
			}
			else throw Exception::ERR_NOTONCHANNEL(chanName[i]);
		}
		else throw Exception::ERR_NOSUCHCHANNEL(command[1]);
	}
}
