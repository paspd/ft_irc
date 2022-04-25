#include "../Server/Server.hpp"

void Server::join(std::vector<std::string> command, int actualClient) {
	if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_NOTREGISTERED();
	if (!(command.size() >= 2 && command.size() <= 3)) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

	if (command.size() == 2 && command[1] == "0") {
		_clients[actualClient].leaveAllChannels();
		return ;
	}

	std::vector<std::string> chanName = _split(command[1], ",");
	std::vector<std::string> chanPass;
	if (command.size() == 3)
		chanPass = _split(command[2], ",");

	bool newChan = false;

	for (size_t j = 0; j < chanName.size(); j++)
	{
		if (_channelExist(chanName[j]) < 0)
			if (chanName[0].find_first_of("#!&+") != 0 || (!_createChannel(chanName[j], (chanPass.size() >= j && command.size() == 3 ? chanPass[j] : "")) && (newChan = true))) throw Exception::ERR_NOSUCHCHANNEL(chanName[j]);
		// int chanIndex = _channelExist(chanName[j]);
		// if (chanIndex >= 0) {
		// 	bool invited = false;
		// 	if (_channels[chanIndex].getMode('i', _clients[actualClient])) {
		// 		for (int i = 0; _clients[actualClient].getChannelInvited()[i].size() > 0; i++) {
		// 			if (_clients[actualClient].getChannelInvited()[i] == chanName[j])
		// 				invited = true;
		// 		}
		// 		if (!invited)
		// 	}
		// }
		for (size_t k = 0; k < MAX_SERV_CHAN; k++) {
			if (_channels[k].getChannelName().size() && _channels[k].getChannelName() == chanName[j]) {
				if (_channels[k].getMode('i', _clients[actualClient]) && !_clients[actualClient].checkInvited(chanName[j]))
					throw Exception::ERR_INVITEONLYCHAN(chanName[0]);
				if (_clients[actualClient].checkConnected(&_channels[k]))
					break ;
				_channels[k].connectToChan(&_clients[actualClient], (chanPass.size() >= j && command.size() == 3 ? chanPass[j] : ""));
				if (!_clients[actualClient].addChannel(&_channels[k])){
					_channels[k].delOccupant(_clients[actualClient].getClientSocket());
					throw Exception::ERR_TOOMANYCHANNELS(_clients[actualClient].getClientNickname(), chanName[j]);
				}
				else {
				_channels[k].setMode("+n", _clients[actualClient]);
				sendMessage(CLIENT_SOCKET, _clients[actualClient].createClientPrompt() + " JOIN " + _channels[k].getChannelName() + CRLF);
				_channels[k].userSendToChannel(_clients[actualClient].getClientSocket(), _clients[actualClient].createClientPrompt() + " JOIN " + _channels[k].getChannelName() + CRLF);
				sendMessage(CLIENT_SOCKET, RPL_CHAN_MODE(_channels[k].getChannelName(), _channels[k].getStrMode()));
				sendMessage(CLIENT_SOCKET, RPL_NAMREPLY(_clients[actualClient].getClientNickname(), _channels[k].getChannelName(), _channels[k].getStrOccupant(CLIENT_SOCKET)));
				sendMessage(CLIENT_SOCKET, RPL_ENDOFNAMES(_clients[actualClient].getClientNickname(), _channels[k].getChannelName()));
				if (!_channels[k].getChannelTopic().empty())
					sendMessage(CLIENT_SOCKET, RPL_TOPIC(_channels[k].getChannelName(), _channels[k].getChannelTopic()));
				else
					sendMessage(CLIENT_SOCKET, RPL_NOTOPIC(_channels[k].getChannelName()));
				}

			}
		}
	}
}
