#include "../Server/Server.hpp"

void Server::names(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	std::vector<std::string> chanName = _split(command[1], ",");
	int index = 0;
	for (size_t i = 0; i < chanName.size(); i++) {
		if ((index = _channelExist(chanName[i])) >= 0) {
			sendMessage(CLIENT_SOCKET, RPL_NAMREPLY(_clients[actualClient].getClientNickname(), _channels[index].getChannelName(), _channels[index].getStrOccupant(CLIENT_SOCKET)));
		}
		else throw Exception::ERR_NOSUCHCHANNEL(chanName[i]);
	}
}