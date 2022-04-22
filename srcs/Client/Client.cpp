#include "Client.hpp"

Channel *Client::getCurrentChannel(int i) const {
	return _currentChannels[i];
}

int Client::getClientSocket() const {
	return _clientSocket;
}

SOCKADDR_IN Client::getClientAddress() const {
	return _clientAddress;
}

bool Client::getNickBool() const {
	return _nickBool;
}

bool Client::getPassBool() const {
	return _passBool;
}

bool Client::getUserBool() const {
	return _userBool;
}

bool Client::getWelcomeBool() const {
	return _welcomeBool;
}

std::string Client::getClientNickname() const {
	return _nickname;
}

std::string Client::getClientUsername() const {
	return _username;
}

std::string Client::getClientRealname() const {
	return _realname;
}

std::vector<std::string> *Client::getChannelInvited() const {
	if (_channelInvited)
		return _channelInvited;
	return NULL;
}

ClientMode Client::getClientMode() const {
	return _clientMode;
}

void Client::setClientSocket(int newSocketID) {
	_clientSocket = newSocketID;
}

void Client::setClientNickname(std::string nickname) {
	_nickname = nickname;
}

void Client::setClientUsername(std::string username) {
	_username = username;
}

void Client::setClientRealname(std::string realname) {
	_realname = realname;
}

void Client::setClientMode(std::string const &mode) {
	if (mode.size() < 2)
			return ;
	bool val = (mode[0] == '+' ? true : false);
	if (mode[2] == 'r' && val)
		return ;
	_clientMode.setMode(mode);
}

void Client::setClientAddress(SOCKADDR_IN newAdress) {
	_clientAddress = newAdress;
	_clientAddressLength = sizeof(_clientAddress);
}

void Client::resetClient() {
	_clientMode.reset();
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
	if (_channelInvited)
		_channelInvited->clear();
	_cleanCurrentList();
}

void Client::passBoolClient() {
	_passBool = true;
}

void Client::userBoolClient() {
	_userBool = true;
}

void Client::nickBoolClient() {
	_nickBool = true;
}

void Client::welcomeBoolClient() {
	_welcomeBool = true;
	std::stringstream ss;
	ss << _nickname << "!" << _username << "@" << inet_ntoa(_clientAddress.sin_addr);
	_clientID = ss.str();
}

bool Client::addChannel(Channel *channel) {
	for (size_t i = 0; i < MAX_CURRENT_CHAN; i++)
	{
		if (_currentChannels[i] == NULL || _currentChannels[i]->getChannelName().empty()) {
			_currentChannels[i] = channel;
			return true;
		}
	}
	return false;
}

void Client::leaveChannel(std::string channelName) {
	for (size_t i = 0; i < MAX_CURRENT_CHAN; i++)
	{
		if (_currentChannels[i] != NULL && _currentChannels[i]->getChannelName() == channelName) {
			_currentChannels[i] = NULL;
		}
	}
}

void Client::leaveAllChannels() {
	for (size_t i = 0; i < MAX_CURRENT_CHAN; i++)
	{
		if (_currentChannels[i] != NULL) {
			_currentChannels[i]->delOccupant(_clientSocket);
			_currentChannels[i] = NULL;
		}
	}
}

void Client::addInvitation(std::string invitation) {
	_channelInvited->push_back(invitation);
	return ;
}

Client &Client::operator=(Client const &rhs) {
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
	_channelInvited = rhs._channelInvited;

	return *this;
}

Client::Client() {};

Client::~Client() {};

void Client::_cleanCurrentList() {
	for (size_t i = 0; i < MAX_CURRENT_CHAN; i++) {
		_currentChannels[i] = NULL;
	}
}

std::string Client::createClientPrompt() const {
	std::stringstream ss;
	ss << ":" << _nickname << "!~" << _username << "@" << inet_ntoa(_clientAddress.sin_addr);
	return ss.str();
}

bool Client::checkConnected(Channel *channel) {
	for (size_t i = 0; i < MAX_CURRENT_CHAN; i++) {
		if (_currentChannels[i] != NULL && _currentChannels[i]->getChannelName() == channel->getChannelName())
			return true;
	}
	return false;
}

std::string Client::getStrMode() {
	std::stringstream ss;
	ss << "+" << (_clientMode.getMode('i') ? "i" : "") << (_clientMode.getMode('r') ? "r" : "") << (_clientMode.getMode('o') ? "o" : "");
	if (ss.str().size() == 1)
		ss.clear();
	return ss.str();
}

std::ostream &	operator<<(std::ostream & o, Client const & rhs) {
	o << "nickname :" << rhs.getClientNickname() << ", username :" << rhs.getClientUsername() << ", realname :" << rhs.getClientRealname() << std::endl;
	for (size_t i = 0; i < MAX_CURRENT_CHAN; i++) {
		if (rhs.getCurrentChannel(i) != NULL)
			o << "channel number " << i << " name :" << rhs.getCurrentChannel(i)->getChannelName() << " password :" << rhs.getCurrentChannel(i)->getChannelPassword() << std::endl;
	}
	return o;
}
