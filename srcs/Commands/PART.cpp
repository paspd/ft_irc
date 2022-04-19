#include "../Server/Server.hpp"

void Server::part(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	std::vector<std::string> chanName = _split(command[1], ",");
	std::string info;

	info.clear();
	if (command.size() > 2) {
		info = _strcatArguments(command.begin() + 2, command.end());
		if (info[0] != ':')
			info = ":" + info;
	}

	int index = 0;
	for (size_t i = 0; i < chanName.size(); i++) {
		if ((index = _channelExist(chanName[i])) >= 0) {
			if (_channels[index].checkClientConnected(_clients[actualClient]) >= 0) {
				if (!info.empty())
					_channels[index].sendToAllChannel(RPL_PART_MESSAGE(_createClientPrompt(_clients[actualClient]), _channels[i].getChannelName(), info));
				else
					_channels[index].sendToAllChannel(RPL_PART_NOMESSAGE(_createClientPrompt(_clients[actualClient]), _channels[i].getChannelName()));
				_clients[actualClient].leaveChannel(chanName[i]);
				_channels[index].delOccupant(_clients[actualClient].getClientSocket());
			}
			else throw Exception::ERR_NOTONCHANNEL(chanName[i]);
		}
		else
			throw Exception::ERR_NOSUCHCHANNEL(chanName[i]);
	}
}