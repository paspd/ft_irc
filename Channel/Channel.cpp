#include "Channel.hpp"

Channel &Channel::operator=(Channel &rhs) {
	_channelName = rhs._channelName;
	_channelPassword = rhs._channelPassword;

	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		_channelOccupants[i] = rhs._channelOccupants[i];
	}
	
	return *this;
};

Channel::Channel() {};

Channel::~Channel() {};

std::string Channel::getChannelName() const {
	return _channelName;
};

std::string Channel::getChannelPassword() {
	return _channelPassword;
};


void Channel::resetChannel() {
	_channelName.erase();
	_channelPassword.erase();
	_cleanChannelOccupants();
};

void Channel::setChannel(std::string name, std::string password) {
	_channelName = name;
	_channelPassword = password;
};

void Channel::connectToChan(Client *newOccupant, std::string password) {
	if (password != _channelPassword) throw Exception::ERR_BADCHANNELKEY(_channelName);
	std::cout << "test" << std::endl;
	if (!_addOccupant(newOccupant)) throw Exception::ERR_CHANNELISFULL(_channelName);
};

void Channel::_cleanChannelOccupants() {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		_channelOccupants[i] = NULL;
	}
};

bool Channel::_addOccupant(Client *newOccupant) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
	{
		if (_channelOccupants[i] == NULL) {
			_channelOccupants[i] = newOccupant;
			return true;
		}
		if (i == MAX_OCCUPANTS_CHAN - 1)
			return false;
	}
	return false;
};

void Channel::delOccupant(Client const *newOccupant) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
	{
		if (_channelOccupants[i] == newOccupant)
			_channelOccupants[i] = NULL;
	}
};