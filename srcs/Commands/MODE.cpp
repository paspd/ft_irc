#include "../Server/Server.hpp"

void Server::mode(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	if (command[1] == _clients[actualClient].getClientNickname()) {
		if (command.size() == 2)
			sendMessage(CLIENT_SOCKET, RPL_UMODEIS(_clients[actualClient].getClientNickname(), _clients[actualClient].getStrMode()));
		else if (_checkUserModeList(command[2])) {
			std::vector<std::string> modeList = _splitWithDelimiters(command[2], "+-");
			for (size_t i = 0; i < modeList.size(); i++)
				_clients[actualClient].setClientMode(modeList[i]);
			sendMessage(CLIENT_SOCKET, RPL_UMODEIS(_clients[actualClient].getClientNickname(), _clients[actualClient].getStrMode()));
		}
		else throw Exception::ERR_UMODEUNKNOWNFLAG();
	}
	else if (!command[1].find_first_of("#!&+")) {
		int chanIndex = 0;
		if ((chanIndex = _channelExist(command[1])) >= 0) {
			if (_channels[chanIndex].checkClientConnected(_clients[actualClient]) >= 0) {
				if (_clients[actualClient].getClientMode()._restricted) throw Exception::ERR_RESTRICTED(_clients[actualClient].getClientNickname());
				else if (command.size() == 2) sendMessage(CLIENT_SOCKET, RPL_CHANNELMODEIS(_clients[actualClient].getClientNickname(), _channels[chanIndex].getChannelName(), _channels[chanIndex].getStrMode()));
				else if (_checkChannelModeList(command[2])) {
					if (_channels[chanIndex].getMode('o', _clients[actualClient]) || _channels[chanIndex].getMode('C', _clients[actualClient])) {
						if (command.size() == 2)
							sendMessage(CLIENT_SOCKET, RPL_CHANNELMODEIS(_clients[actualClient].getClientNickname(), command[1], _channels[chanIndex].getStrMode()));
						else if (command[2].find_first_of("bko") != std::string::npos) {
							_channels[chanIndex].setMode(command[2], _clients[actualClient], command[3]);
							_channels[chanIndex].sendToAllChannel(RPL_MODE_CUSTOM_CMD(_clients[actualClient].createClientPrompt(), command[1], command[2], command[3]));
							return ;
						}
						_channels[chanIndex].setMode(command[2], _clients[actualClient]);
						_channels[chanIndex].sendToAllChannel(RPL_MODE_CMD(_clients[actualClient].createClientPrompt(), command[1], command[2]));
						return ;
					}
					else throw Exception::ERR_CHANOPRIVSNEEDED(command[1]);
				}
				else throw Exception::ERR_UMODEUNKNOWNFLAG();
			}
			else throw Exception::ERR_NOTONCHANNEL(command[1]);
		}
		else throw Exception::ERR_NOSUCHCHANNEL(command[1]);
	}
	else throw Exception::ERR_USERSDONTMATCH(_clients[actualClient].getClientNickname());
}