#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../Channel/Channel.hpp"
#include "../Mode/ClientMode.hpp"
#include "../Rand/Rand.hpp"

class Channel;

class ClientMode;

class Client
{
private:
	SOCKET _clientSocket;
	SOCKADDR_IN _clientAddress;
	SOCKLEN_T _clientAddressLength;
	
	ClientMode _clientMode;

	std::string _clientID;

	bool _passBool;
	bool _nickBool;
	bool _userBool;

	bool _welcomeBool;

	std::string _nickname;
	std::string _username;
	std::string _realname;

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

	void setClientSocket(int newSocketID);

	void setClientMode(std::string const &mode);

	void setClientNickname(std::string nickname);

	void setClientUsername(std::string username);

	void setClientRealname(std::string realname);

	void setClientAddress(SOCKADDR_IN newAdress);

	void resetClient();

	void passBoolClient();

	void userBoolClient();
	
	void nickBoolClient();

	void welcomeBoolClient();

	bool addChannel(Channel *channel);

	void leaveAllChannels();

	void leaveChannel(std::string channelName);
	
	bool checkConnected(Channel *channel);

	std::string createClientPrompt() const;

	Client &operator=(Client const &rhs);

	Client();

	~Client();

private:

	void _cleanCurrentList();

};

std::ostream &	operator<<(std::ostream & o, Client const & rhs);

#endif