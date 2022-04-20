#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Rand/Rand.hpp"
#include "../Mode/OccupantChannelMode.hpp"
#include "../Mode/ChannelMode.hpp"

class Client;

class Channel
{
private:
	std::string _channelName;
	std::string _channelPassword;

	std::string _channelTopic;

	Client *_channelOccupants[MAX_OCCUPANTS_CHAN];
	OccupantChannelMode _channelOccupantsMode[MAX_OCCUPANTS_CHAN];

	std::vector<std::string> _banAddr;

	ChannelMode _channelMode;

public:
	Channel &operator=(Channel &rhs);

	Channel();
	~Channel();

	std::string getChannelName() const;

	std::string getChannelPassword() const;

	std::string getChannelTopic() const;

	Client *getChannelOccupant(int i) const;

	OccupantChannelMode getChannelOccupantMode(int i) const;

	void resetChannel();

	void setChannel(std::string name, std::string password);

	void setMode(std::string const &mode, Client const &client, std::string const &argument = std::string());

	void setChannelTopic(std::string channelTopic);

	void connectToChan(Client *newOccupant, std::string password);

	void delOccupant(int const socket);

	void sendToAllChannel(std::string const &msg);

	void userSendToChannel(int const socketSender, std::string const &msg);

	int checkIfClient();

	std::string getStrOccupant(int const &socketSender);

	bool getMode(char const &mode, Client const &client) const;

	int checkClientConnected(Client const &rhs);

private:
	void _cleanChannelOccupants();

	bool _addOccupant(Client *newOccupant);

	void _addBanAddr(std::string const &banAddr);
	
	void _delBanAddr(std::string const &banAddr);
};

std::ostream &	operator<<(std::ostream & o, Channel const & rhs);

#endif