#include "Channel.hpp"

Channel &Channel::operator=(Channel const &rhs) {
	_channelName = rhs._channelName;
	_channelPassword = rhs._channelPassword;
	_channelOccupants = rhs._channelOccupants;
	
	return *this;
}

Channel::Channel() {};

Channel::~Channel() {};

bool Channel::getCreated() {
	return _created;
}

std::string Channel::getChannelName() const {
	return _channelName;
}

void Channel::resetChannel() {
	_created = false;
	_channelName.erase();
	_channelPassword.erase();
	_cleanChannelOccupants();
}

void Channel::setChannel(std::string name, std::string password = NULL) {
	_created = true;
	_channelName = name;
	_channelPassword = password;
	_channelOccupants.reserve(MAX_OCCUPANTS_CHAN);
}

void Channel::connectToChan(Client &newOccupant, std::string password = NULL) {
	if (password != _channelPassword) throw Exception::ERR_BADCHANNELKEY(_channelName);
	if (!_addOccupant(newOccupant)) throw Exception::ERR_CHANNELISFULL(_channelName);
	if (!newOccupant.addChannel(*this)){
		_delOccupant(newOccupant);
		throw Exception::ERR_TOOMANYCHANNELS(_channelName);
	}
}

void Channel::_cleanChannelOccupants() {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
		_channelOccupants[i].resetClient();
	}
}

bool Channel::_addOccupant(Client const &newOccupant) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
	{
		if (!_channelOccupants[i].getClientSocket()) {
			_channelOccupants[i] = newOccupant;
			return true;
		}
		if (i == MAX_OCCUPANTS_CHAN - 1)
			return false;
	}
	return false;
}

void Channel::_delOccupant(Client const &newOccupant) {
	for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
	{
		if (_channelOccupants[i].getClientSocket() == newOccupant.getClientSocket())
			_channelOccupants[i].resetClient();
	}
}