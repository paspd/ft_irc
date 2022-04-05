#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "../Rand.hpp"

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

	Client() {};

	~Client() {};
};

#endif