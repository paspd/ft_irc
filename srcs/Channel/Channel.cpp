#include "Channel.hpp"

Channel &Channel::operator=(Channel &rhs) {
	_channelName = rhs._channelName;
	_channelPassword = rhs._channelPassword;

	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		_channelOccupants[i] = rhs._channelOccupants[i];
		_channelOccupantsMode[i] = rhs._channelOccupantsMode[i];
	}
	
	return *this;
};

Channel::Channel() {};

Channel::~Channel() {};

std::string Channel::getChannelName() const {
	return _channelName;
};

std::string Channel::getChannelPassword() const{
	return _channelPassword;
};

Client *Channel::getChannelOccupant(int i) const {
	return _channelOccupants[i];
}

OccupantChannelMode Channel::getChannelOccupantMode(int i) const {
	return _channelOccupantsMode[i];
}

std::string Channel::getChannelTopic() const {
	return _channelTopic;
}

void Channel::resetChannel() {
	_channelName.erase();
	_channelPassword.erase();
	_cleanChannelOccupants();
	_channelMode.reset();
};

void Channel::setChannel(std::string name, std::string password) {
	_channelName = name;
	if (password.size()) {
		_channelPassword = password;
		_channelMode.setMode("+k");
	}
};

void Channel::setChannelTopic(std::string channelTopic) {
	if (channelTopic[0] == ':')
		channelTopic.erase(channelTopic.begin(), channelTopic.begin() + 1);
	_channelTopic = channelTopic;
}

void Channel::connectToChan(Client *newOccupant, std::string password) {
	if (password != _channelPassword) throw Exception::ERR_BADCHANNELKEY(_channelName);
	if (!_addOccupant(newOccupant)) throw Exception::ERR_CHANNELISFULL(_channelName);
};

bool Channel::getMode(char const &mode, Client const &client = Client()) const {
	if (mode == 'C' || mode == 'o' || mode == 'v')
		for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
			if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket() == client.getClientSocket())
				return _channelOccupantsMode[i].getMode(mode);
	return _channelMode.getMode(mode);
}

void Channel::setMode(std::string const &mode, Client const &client, std::string const &argument) {
	switch (mode[1])
	{
	case 'C':
		for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
			if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket() == client.getClientSocket())
				_channelOccupantsMode[i].setMode(mode);
		break;
	case 'o':
		for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
			if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientNickname() == argument)
				_channelOccupantsMode[i].setMode(mode);
		return ;
	case 'v':
		for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
			if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientNickname() == argument)
				_channelOccupantsMode[i].setMode("+c");
			return ;
	case 'k':
		(mode[0] == '+' ? _channelPassword = argument : _channelPassword.erase());
		break;
	default:
		break;
	}
	_channelMode.setMode(mode);
}

int Channel::checkClientConnected(Client const &rhs) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket() == rhs.getClientSocket())
			return i;
	}
	return -1;
}

int Channel::checkClientConnected(std::string const &client) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientNickname() == client)
			return i;
	}
	return -1;
}

void Channel::_cleanChannelOccupants() {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		_channelOccupants[i] = NULL;
		_channelOccupantsMode[i].reset();
	}
};

bool Channel::_addOccupant(Client *newOccupant) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
	{
		if (_channelOccupants[i] == NULL) {
			_channelOccupants[i] = newOccupant;
			if (checkIfClient() == 1)
				setMode("+C", *newOccupant);
			return true;
		}
		if (i == MAX_OCCUPANTS_CHAN - 1)
			return false;
	}
	return false;
};

void Channel::delOccupant(int const socket) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
	{
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket() == socket) {
			_channelOccupants[i] = NULL;
			_channelOccupantsMode[i].reset();
			return ;
		}
	}
	throw Exception::ERR_NOTONCHANNEL(_channelName);
};

void Channel::delOccupant(std::string const name) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
	{
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientNickname() == name) {
			_channelOccupants[i] = NULL;
			_channelOccupantsMode[i].reset();
			return ;
		}
	}
	throw Exception::ERR_NOTONCHANNEL(_channelName);
};

int Channel::checkIfClient() {
	int nb_client = 0;
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket())
			nb_client++;
	}
	return nb_client;
}

void Channel::sendToAllChannel(std::string const &msg) {
	int msgSize = msg.length();
	ssize_t ret_send = 0;
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket()) {
			std::cout << "MSG :" << msg << std::endl;
			ret_send = send(_channelOccupants[i]->getClientSocket(), msg.c_str(), msgSize, 0);
			if (ret_send != (ssize_t)msgSize)
				throw Exception::SendFailed();
		}
	}
	
}

void Channel::userSendToChannel(int const socketSender, std::string const &msg) {
	int msgSize = msg.length();
	ssize_t ret_send = 0;
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket() && _channelOccupants[i]->getClientSocket() != socketSender) {
			ret_send = send(_channelOccupants[i]->getClientSocket(), msg.c_str(), msgSize, 0);
			if (ret_send != (ssize_t)msgSize)
				throw Exception::SendFailed();
		}
	}
	
}

std::string Channel::getStrOccupant(int const &socketSender) {
	std::stringstream ss;
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket()) {
			if (!_channelOccupants[i]->getClientMode()._invisible || _channelOccupants[i]->getClientSocket() == socketSender)
				ss << (i != 0 ? " " : "") << (_channelOccupantsMode[i].getMode('C') ? "@" : "") << (_channelOccupantsMode[i].getMode('o') ? "+" : "") << _channelOccupants[i]->getClientNickname();
		}
	}
	return ss.str();
}

std::string Channel::getStrMode() {
	std::stringstream ss;
	ss << "+" << (_channelMode.getMode('i') ? "i" : "") << (_channelMode.getMode('p') ? "p" : "") << (_channelMode.getMode('t') ? "t" : "") << (_channelMode.getMode('n') ? "n" : "") << (_channelMode.getMode('m') ? "m" : "") << (_channelMode.getMode('k') ? "k" : "");
	if (ss.str().size() == 1)
		ss.clear();
	return ss.str();
}

std::ostream &	operator<<(std::ostream & o, Channel const & rhs) {
	o << "channel name :" << rhs.getChannelName() << ", password :" << rhs.getChannelPassword() << std::endl;
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (rhs.getChannelOccupant(i) != NULL && rhs.getChannelOccupant(i)->getClientSocket())
			o << "client number " << i << " socket :" << rhs.getChannelOccupant(i)->getClientSocket() << " username :" << rhs.getChannelOccupant(i)->getClientUsername() << (rhs.getMode('C', *rhs.getChannelOccupant(i)) ? " is creator of channel." : "") << std::endl;
	}
	return o;
}
