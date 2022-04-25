#include "../Server/Server.hpp"

void Server::invite(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 3) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	std::string clientName = command[1];
	std::string channelName = command[2];

	int clientIndex = 0;
	if ((clientIndex = _clientExist(clientName)) < 0) throw Exception::ERR_NOSUCHNICK(clientName);

	int chanIndex = 0;
	if ((chanIndex = _channelExist(channelName)) >= 0) {
		if (_channels[chanIndex].checkClientConnected(_clients[actualClient]) < 0)
			throw Exception::ERR_NOTONCHANNEL(channelName);
		if (_channels[chanIndex].checkClientConnected(_clients[clientIndex]) >= 0)
			throw Exception::ERR_USERONCHANNEL(clientName, channelName);
	}
	if (_channels[chanIndex].getMode('i', _clients[actualClient]))
		if (!_channels[chanIndex].getMode('C', _clients[actualClient]) && !_channels[chanIndex].getMode('o', _clients[actualClient]))
			throw Exception::ERR_CHANOPRIVSNEEDED(channelName);
	_clients[clientIndex].addInvitation(channelName);
	sendMessage(_clients[actualClient].getClientSocket(), RPL_INVITING(_clients[actualClient].createClientPrompt(), channelName, clientName, _clients[actualClient].getClientNickname()));
	sendMessage(_clients[clientIndex].getClientSocket(), RPL_INVITED(_clients[actualClient].createClientPrompt(), clientName, channelName));
}
