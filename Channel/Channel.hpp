#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Rand.hpp"
#include "../Client/Client.hpp"

class Client;

class Channel
{
private:
	std::string _channelName;
	std::string _channelPassword;

	std::vector<Client> _channelOccupants;

	bool _created;

public:
	Channel &operator=(Channel const &rhs) {
		_channelName = rhs._channelName;
		_channelPassword = rhs._channelPassword;
		_channelOccupants = rhs._channelOccupants;
		
		return *this;
	}

	Channel() {};
	~Channel() {};

	bool getCreated() {
		return _created;
	}

	std::string getChannelName() const {
		return _channelName;
	}

	void resetChannel() {
		_created = false;
		_channelName.erase();
		_channelPassword.erase();
		_cleanChannelOccupants();
	}

	void setChannel(std::string name, std::string password = NULL) {
		_created = true;
		_channelName = name;
		_channelPassword = password;
		_channelOccupants.reserve(MAX_OCCUPANTS_CHAN);
	}

	void connectToChan(Client &newOccupant, std::string password = NULL) {
		if (password != _channelPassword) throw Exception::ERR_BADCHANNELKEY(_channelName);
		if (!_addOccupant(newOccupant)) throw Exception::ERR_CHANNELISFULL(_channelName);
		if (!newOccupant.addChannel(*this)){
			_delOccupant(newOccupant);
			throw Exception::ERR_TOOMANYCHANNELS(_channelName);
		}
	}

private:
	void _cleanChannelOccupants() {
		for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++) {
			_channelOccupants[i].resetClient();
		}
	}

	bool _addOccupant(Client const &newOccupant) {
		for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
		{
			if (!_channelOccupants[i].getClientSocket()) {
				_channelOccupants[i] = newOccupant;
				return true;
			}
			if (i == MAX_OCCUPANTS_CHAN - 1)
				return false;
		}
		
	}

	void _delOccupant(Client const &newOccupant) {
		for (size_t i = 0; i < MAX_OCCUPANTS_CHAN; i++)
		{
			if (_channelOccupants[i].getClientSocket() == newOccupant.getClientSocket())
				_channelOccupants[i].resetClient();
		}
	}

};

#endif