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
};

void Channel::setChannel(std::string name, std::string password) {
	_channelName = name;
	_channelPassword = password;
};

void Channel::setChannelTopic(std::string channelTopic) {
	_channelTopic = channelTopic;
}

void Channel::connectToChan(Client *newOccupant, std::string password) {
	if (password != _channelPassword) throw Exception::ERR_BADCHANNELKEY(_channelName);
	if (!_addOccupant(newOccupant)) throw Exception::ERR_CHANNELISFULL(_channelName);
};

OccupantChannelMode *Channel::getClientMode(Client const &rhs) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket() == rhs.getClientSocket())
			return &_channelOccupantsMode[i];
	}
	return NULL;
}

int Channel::checkClientConnected(Client const &rhs) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket() == rhs.getClientSocket())
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
			if (!checkIfClient())
				_channelOccupantsMode[i].setCreator(true);
			_channelOccupants[i] = newOccupant;
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
			return ;
		}
	}
	throw Exception::ERR_NOTONCHANNEL(_channelName);
};

bool Channel::checkIfClient() {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket())
			return true;
	}
	return false;
}

void Channel::sendToAllChannel(int const &socketSender, std::string const &msg) {
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

std::string Channel::getStrOccupant() {
	std::stringstream ss;
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (_channelOccupants[i] != NULL && _channelOccupants[i]->getClientSocket())
		ss << (i != 0 ? " " : "") << (_channelOccupantsMode[i].getCreator() ? "@" : "") << _channelOccupants[i]->getClientNickname();
	}
	return ss.str();
}


std::ostream &	operator<<(std::ostream & o, Channel const & rhs) {
	o << "channel name :" << rhs.getChannelName() << ", password :" << rhs.getChannelPassword() << std::endl;
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		if (rhs.getChannelOccupant(i) != NULL && rhs.getChannelOccupant(i)->getClientSocket())
			o << "client number " << i << " socket :" << rhs.getChannelOccupant(i)->getClientSocket() << " username :" << rhs.getChannelOccupant(i)->getClientUsername() << (rhs.getChannelOccupantMode(i).getCreator() ? " is creator of channel." : "") << std::endl;
	}
	return o;
}
