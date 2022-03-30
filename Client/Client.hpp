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

	bool _logged;
	bool _nicked;
	bool _registred;

	bool _welcome;

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

	bool getLogged() const {
		return _logged;
	}

	bool getRegistred() const {
		return _registred;
	}

	bool getNicked() const {
		return _registred;
	}

	bool getWelcome() const {
		return _registred;
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
		_logged = false;
		_nicked = false;
		_registred = false;
		_welcome = false;
		_nickname.erase();
		_username.erase();
		_realname.erase();
	}

	void logClient() {
		_logged = true;
	}

	void nickClient() {
		_logged = true;
	}
	
	void registreClient() {
		_registred = true;
	}

	void welcomeClient() {
		_welcome = true;
	}

	Client() {};

	~Client() {};
};

#endif