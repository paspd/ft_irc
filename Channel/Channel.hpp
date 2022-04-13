#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Rand.hpp"
#include "../Mode/OccupantChannelMode.hpp"
// #include "../Mode/Mode.hpp"

class Client;

class Channel
{
private:
	std::string _channelName;
	std::string _channelPassword;

	std::string _channelTopic;

	Client *_channelOccupants[MAX_OCCUPANTS_CHAN];
	OccupantChannelMode _channelOccupantsMode[MAX_OCCUPANTS_CHAN];

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

	void setChannelTopic(std::string channelTopic);

	void connectToChan(Client *newOccupant, std::string password);

	void delOccupant(int const socket);

	void sendToAllChannel(int const &socketSender, std::string const &msg);

	bool checkIfClient();

	std::string getStrOccupant();

private:
	void _cleanChannelOccupants();

	bool _addOccupant(Client *newOccupant);


};

std::ostream &	operator<<(std::ostream & o, Channel const & rhs);

#endif