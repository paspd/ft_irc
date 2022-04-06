#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "../Channel/Channel.hpp"
#include "../Rand.hpp"

class Channel;

class Client
{
private:
	SOCKET _clientSocket;
	SOCKADDR_IN _clientAddress;
	SOCKLEN_T _clientAddressLength;
	
	int _clientMode;

	bool _passBool;
	bool _nickBool;
	bool _userBool;

	bool _welcomeBool;

	std::string _nickname;
	std::string _username;
	std::string _realname;

	std::vector<Channel> _currentChannels;

public:
	
	int getClientSocket() const {
		return _clientSocket;
	}

	int getClientMode() const {
		return _clientMode;
	}

	SOCKADDR_IN getClientAddress() const {
		return _clientAddress;
	}

	bool getNickBool() const {
		return _nickBool;
	}

	bool getPassBool() const {
		return _passBool;
	}

	bool getUserBool() const {
		return _userBool;
	}

	bool getWelcomeBool() const {
		return _welcomeBool;
	}

	std::string getClientNickname() const {
		return _nickname;
	}

	std::string getClientUsername() const {
		return _username;
	}

	std::string getClientRealname() const {
		return _realname;
	}

	void setClientSocket(int newSocketID) {
		_clientSocket = newSocketID;
	}

	void setClientMode(int newMode) {
		_clientMode = newMode;
	}

	void setClientNickname(std::string nickname) {
		_nickname = nickname;
	}

	void setClientUsername(std::string username) {
		_username = username;
	}

	void setClientRealname(std::string realname) {
		_realname = realname;
	}

	void setClientAddress(SOCKADDR_IN newAdress) {
		_clientAddress = newAdress;
		_clientAddressLength = sizeof(_clientAddress);
	}

	void resetClient() {
		_clientMode = 0;
		_clientSocket = 0;
		_clientAddress.sin_family = 0;
		_clientAddress.sin_addr.s_addr = 0;
		_clientAddress.sin_port = 0;
		_clientAddressLength = 0;
		_passBool = false;
		_userBool = false;
		_nickBool = false;
		_welcomeBool = false;
		_nickname.erase();
		_username.erase();
		_realname.erase();
	}

	void passBoolClient() {
		_passBool = true;
	}

	void userBoolClient() {
		_userBool = true;
	}
	
	void nickBoolClient() {
		_nickBool = true;
	}

	void welcomeBoolClient() {
		_welcomeBool = true;
	}

	bool addChannel(Channel const &channel) {
		for (size_t i = 0; i < MAX_CURRENT_CHAN; i++)
		{
			if (!_currentChannels[i].getCreated()) {
				_currents[i] = channel;
				return true;
			}
			if (i == MAX_CURRENT_CHAN - 1)
				return false;
		}
		
	}

	Client &operator=(Client const &rhs) {
		_clientMode = rhs._clientMode;
		_clientSocket = rhs._clientSocket;
		_clientAddress.sin_family = rhs._clientAddress.sin_family;
		_clientAddress.sin_addr.s_addr = rhs._clientAddress.sin_addr.s_addr;
		_clientAddress.sin_port = rhs._clientAddress.sin_port;
		_clientAddressLength = rhs._clientAddressLength;
		_passBool = rhs._passBool;
		_userBool = rhs._userBool;
		_nickBool = rhs._nickBool;
		_welcomeBool = rhs._welcomeBool;
		_nickname = rhs._nickname;
		_username = rhs._username;
		_realname = rhs._realname;
		
		return *this;
	}

	Client() {};

	~Client() {};
};



#endif