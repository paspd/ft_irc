#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../Channel/Channel.hpp"
#include "../Rand/Rand.hpp"

class Channel;

struct ClientMode
{
	//i
	bool _invisible;

	//r
	bool _restricted;

	//o
	bool _OP;
};


class Client
{
private:
	SOCKET _clientSocket;
	SOCKADDR_IN _clientAddress;
	SOCKLEN_T _clientAddressLength;

	std::string _clientID;

	ClientMode _clientMode;

	bool _passBool;
	bool _nickBool;
	bool _userBool;

	bool _welcomeBool;

	std::string _nickname;
	std::string _username;
	std::string _realname;

	std::vector<std::string> _channelInvited;

	Channel *_currentChannels[MAX_CURRENT_CHAN];

public:

	Channel *getCurrentChannel(int i) const;

	int getClientSocket() const;

	ClientMode getClientMode() const;

	SOCKADDR_IN getClientAddress() const;

	bool getNickBool() const;

	bool getPassBool() const;

	bool getUserBool() const;

	bool getWelcomeBool() const;

	std::string getClientNickname() const;

	std::string getClientUsername() const;

	std::string getClientRealname() const;

	std::vector<std::string> getChannelInvited() const;

	void setClientSocket(int newSocketID);

	void setClientMode(std::string const &mode);

	void setClientNickname(std::string nickname);

	void setClientUsername(std::string username);

	void setClientRealname(std::string realname);

	void setChannelInvited(std::vector<std::string> *channelInvited);

	void setClientAddress(SOCKADDR_IN newAdress);

	void setOP(bool const &val);
	
	void resetClient();

	void passBoolClient();

	void userBoolClient();

	void nickBoolClient();

	void welcomeBoolClient();

	bool addChannel(Channel *channel);

	void leaveAllChannels();

	void leaveChannel(std::string channelName);

	void addInvitation(std::string invitation);

	bool checkConnected(Channel *channel);

	bool checkInvited(std::string channelName);

	std::string createClientPrompt() const;

	Client &operator=(Client const &rhs);

	std::string getStrMode();

	void sendToAllCurrent(std::string msg);

	Client();

	~Client();

private:

	void _cleanCurrentList();
};

std::ostream &	operator<<(std::ostream & o, Client const & rhs);

#endif
