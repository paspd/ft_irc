#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Rand.hpp"
#include "../Client/Client.hpp"

class Channel
{
private:
	std::string _channelName;
	std::string _channelPassword;

	std::vector<Client> _channelOccupants;
public:
	Channel() {};
	~Channel() {};

	void resetChannel() {
		_channelName.erase();
		_channelPassword.erase();
		_cleanChannelOccupants();
	}

	void setChannel(std::string name, std::string password = NULL) {
		_channelName = name;
		_channelPassword = password;
		_channelOccupants.reserve(MAX_CLIENTS_CHAN);
	}

private:
	void _cleanChannelOccupants() {
		for (size_t i = 0; i < MAX_CLIENTS_CHAN; i++) {
			_channelOccupants[i].getClientAddress();
		}
	}

};

#endif