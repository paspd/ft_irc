#include "../Server/Server.hpp"

void Server::mode(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 3) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	if (command[1] == _clients[actualClient].getClientNickname()) {
		if (_checkModeList(command[2])) {
			std::vector<std::string> modeList = _splitWithDelimiters(command[2], "+-");
			for (size_t i = 0; i < modeList.size(); i++)
				_clients[actualClient].setClientMode(modeList[i]);

			for (int i = 0; USER_MODE_AVAILABLE[i]; i++) { std::cout << "| " << USER_MODE_AVAILABLE[i] << ": " << _clients[actualClient].getClientMode().getMode(USER_MODE_AVAILABLE[i]) << " "; } std::cout << " |" << std::endl;
		}
		else throw Exception::ERR_UMODEUNKNOWNFLAG();
	}
	else throw Exception::ERR_USERSDONTMATCH();
}