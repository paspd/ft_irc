#include "../Server/Server.hpp"

void Server::mode(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 3) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	if (command[1] == _clients[actualClient].getClientNickname()) {
		if (_checkUserModeList(command[2])) {
			std::vector<std::string> modeList = _splitWithDelimiters(command[2], "+-");
			for (size_t i = 0; i < modeList.size(); i++)
				_clients[actualClient].setClientMode(modeList[i]);

			for (int i = 0; USER_MODE_AVAILABLE[i]; i++) { std::cout << "| " << USER_MODE_AVAILABLE[i] << ": " << _clients[actualClient].getClientMode().getMode(USER_MODE_AVAILABLE[i]) << " "; } std::cout << " |" << std::endl;
		}
		else throw Exception::ERR_UMODEUNKNOWNFLAG();
	}
	else if (!command[1].find_first_of("#!&+")) {
		int chanIndex = 0;
		if ((chanIndex = _channelExist(command[1])) >= 0) {
			if (_channels[chanIndex].checkClientConnected(_clients[actualClient]) >= 0) {
				if (_checkChannelModeList(command[2])) {
					if (_channels[chanIndex].getMode('o', _clients[actualClient]) || _channels[chanIndex].getMode('C', _clients[actualClient])) {
						if (command[2].find_first_of("bk") != std::string::npos)
							return _channels[chanIndex].setMode(command[2], _clients[actualClient], command[3]);
						return _channels[chanIndex].setMode(command[2], _clients[actualClient]);
					}
					else throw Exception::ERR_CHANOPRIVSNEEDED(command[1]);
				}
				else throw Exception::ERR_UMODEUNKNOWNFLAG();
			}
			else throw Exception::ERR_NOTONCHANNEL(command[1]);
		}
		else throw Exception::ERR_NOSUCHCHANNEL(command[1]);
	}
	else throw Exception::ERR_USERSDONTMATCH();
}