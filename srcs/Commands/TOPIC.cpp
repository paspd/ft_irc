#include "../Server/Server.hpp"

void Server::topic(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (_clients[actualClient].getClientMode()._restricted) throw Exception::ERR_RESTRICTED(_clients[actualClient].getClientNickname());
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	int indexChan = 0;
	if ((indexChan = _channelExist(command[1])) >= 0) {
		if (_channels[indexChan].checkClientConnected(_clients[actualClient]) >= 0) {
			if (command.size() == 2) {
				if (!_channels[indexChan].getChannelTopic().empty())
					sendMessage(CLIENT_SOCKET, RPL_TOPIC(_channels[indexChan].getChannelName(), _channels[indexChan].getChannelTopic()));
				else sendMessage(CLIENT_SOCKET, RPL_NOTOPIC(_channels[indexChan].getChannelName()));
			}
			else if ((_channels[indexChan].getMode('o', _clients[actualClient]) || _channels[indexChan].getMode('C', _clients[actualClient])) && !_channels[indexChan].getMode('t', _clients[actualClient])) {
				_channels[indexChan].setChannelTopic(_strcatArguments(command.begin() + 2, command.end()));
				_channels[indexChan].sendToAllChannel(RPL_TOPIC_CMD(_clients[actualClient].createClientPrompt(), _channels[indexChan].getChannelName(), _channels[indexChan].getChannelTopic()));
			}
			else throw Exception::ERR_CHANOPRIVSNEEDED(command[1]);
		}
		else throw Exception::ERR_NOTONCHANNEL(command[1]);
	}
	else throw Exception::ERR_NOSUCHCHANNEL(command[1]);
}