#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Rand.hpp"

class Client;

class Channel
{
private:
	std::string _channelName;
	std::string _channelPassword;

	Client *_channelOccupants[MAX_OCCUPANTS_CHAN];

public:
	Channel &operator=(Channel &rhs);

	Channel();
	~Channel();

	std::string getChannelName() const;

	void resetChannel();

	void setChannel(std::string name, std::string password);
	
	std::string getChannelPassword();

	void connectToChan(Client *newOccupant, std::string password);

	void delOccupant(Client const *newOccupant);

private:
	void _cleanChannelOccupants();

	bool _addOccupant(Client *newOccupant);


};

#endif