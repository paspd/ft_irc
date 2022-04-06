#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Client/Client.hpp"
#include "../Rand.hpp"

class Client;

class Channel
{
private:
	std::string _channelName;
	std::string _channelPassword;

	std::vector<Client> _channelOccupants;

	bool _created;

public:
	Channel &operator=(Channel const &rhs);

	Channel();
	~Channel();

	bool getCreated();

	std::string getChannelName() const;

	void resetChannel();

	void setChannel(std::string name, std::string password);

	void connectToChan(Client &newOccupant, std::string password);

private:
	void _cleanChannelOccupants();

	bool _addOccupant(Client const &newOccupant);

	void _delOccupant(Client const &newOccupant);

};

#endif