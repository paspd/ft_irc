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
	bool _accepted;

	std::string _nickname;
public:
	
	int getClientSocket() {
		return _clientSocket;
	}

	SOCKADDR_IN getClientAddress() {
		return _clientAddress;
	}

	bool getAccepted() {
		return _accepted;
	}

	std::string getClientNickname() {
		return _nickname;
	}

	void setClientSocket(int newSocketID) {
		_clientSocket = newSocketID;
	}

	void setClientNickname(std::string nickname) {
		_nickname = nickname;
	}

	void setClientAddress(SOCKADDR_IN newAdress) {
		_clientAddress = newAdress;
		_clientAddressLength = sizeof(_clientAddress);
	}

	void resetClient() {
		_clientSocket = 0;
		_clientAddress.sin_family = 0;
		_clientAddress.sin_addr.s_addr = 0;
		_clientAddress.sin_port = 0;
		_clientAddressLength = 0;
		_accepted = false;
		_nickname.erase();
	}

	void acceptClient() {
		_accepted = true;
	}

	Client() {};

	~Client() {};
};

#endif