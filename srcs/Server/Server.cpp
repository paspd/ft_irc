#include "Server.hpp"

Server::Server(int port, char* password): _port(port), _password(password) {
	try {
	_clients.reserve(MAX_CLIENTS);
	_channels.reserve(MAX_SERV_CHAN);
	cleanClientsList();
	createMasterSocket();
	bindMasterSocket();
	serverListen();
	acceptConnection();
	}
	catch (const std::exception &e) {
		std::cerr << e.what();
			exit(EXIT_FAILURE);
	};
}

Server::~Server() {}

void Server::createMasterSocket() {
	if((_masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
		throw Exception::MasterSocketCreationFailed();

	_masterAddress.sin_family = AF_INET;
	_masterAddress.sin_addr.s_addr = INADDR_ANY;
	_masterAddress.sin_port = htons(_port);

	_masterAddressLength = sizeof(_masterAddress);
}

void Server::bindMasterSocket() {
	if (bind(_masterSocket, (SOCKADDR *)&_masterAddress, sizeof(_masterAddress)) < 0)
		throw Exception::MasterSocketBindingFailed();
}

void Server::serverListen() {
	std::cout << BLUE << "Listening on port " << _port << " ..." << END << std::endl << std::endl;
	if (listen(_masterSocket, SOMAXCONN) < 0)
		throw Exception::ListenFailed();
}

void Server::cleanClientsList() {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		_clients[i].resetClient();
	}
}

void Server::cleanChannelsList() {
	for (size_t i = 0; i < MAX_SERV_CHAN; i++) {
		_channels[i].resetChannel();
	}
}

void Server::acceptConnection() {
	while (true) {
		resetSocketDescriptor();
		checkActivity();
		checkServerActivity();
		checkClientActivity();
		_checkClientInChannel();
		_affAllChannels();
		_affAllClients();
	}
}

void Server::resetSocketDescriptor() {
	FD_ZERO(&_socketDescriptor);

	FD_SET(_masterSocket, &_socketDescriptor);

	_maxSocket = _masterSocket;

	for (int actualClient = 0; actualClient < MAX_CLIENTS; actualClient++) {
		int s = CLIENT_SOCKET;
		if (s)
			FD_SET(s, &_socketDescriptor);
		if (s > _maxSocket)
			_maxSocket = s;
	}
}

void Server::checkActivity() {
	if (select(_maxSocket + 1, &_socketDescriptor ,NULL, NULL, NULL) < 0 && (errno != EINTR))
		throw Exception::SelectFailed();
}

void Server::checkServerActivity() {
	if (FD_ISSET(_masterSocket, &_socketDescriptor)) {
		int new_socket;
		SOCKADDR_IN new_address;
		SOCKLEN_T new_address_length = sizeof(new_address);
		if ((new_socket = accept(_masterSocket, (struct sockaddr *)&new_address, (socklen_t*)&new_address_length)) < 0)
			throw Exception::AcceptFailed();
			//err server full
		std::cout << BOLD << GREEN << "New connection at socket number " << new_socket << ", ip is : " << inet_ntoa(new_address.sin_addr) << ", port : " << ntohs(new_address.sin_port) << END << std::endl << std::endl;
		for (int actualClient = 0; actualClient < MAX_CLIENTS; actualClient++) {
			if (CLIENT_SOCKET == 0) {
				_clients[actualClient].setClientSocket(new_socket);
				_clients[actualClient].setClientAddress(new_address);
				std::cout << BOLD << GREEN << "Adding of new client at number: " << actualClient << END << std::endl << std::endl;
				break ;
			}
		}
	}
}


void Server::checkClientActivity() {
	for (int actualClient = 0; actualClient < MAX_CLIENTS; actualClient++) {
		int s = CLIENT_SOCKET;
		if (FD_ISSET(s, &_socketDescriptor)) {
			int retread;
			if ((retread = read(s, buffer, 2048)) == 0) {
				std::cout << BOLD << RED << "Host disconected, ip is : " << inet_ntoa(_clients[actualClient].getClientAddress().sin_addr) << ", port : " << ntohs(_clients[actualClient].getClientAddress().sin_port) << END << std::endl << std::endl;
				close(s);
				_clients[actualClient].resetClient();
			}
			else {
				buffer[retread] = '\0';
				std::string str_buffer = buffer;
				std::string lineSeparator = "\r\n";
				std::string cmdSeparator = " \t\v\n\r";
				std::vector<std::string> line = _split(buffer, lineSeparator);
				for (size_t actualLine = 0; actualLine < line.size(); actualLine++) {
					std::vector<std::string> command = _split(line[actualLine], cmdSeparator);
					if (!command.size())
						return ;
					std::cout << BOLD << YELLOW << "Client number " << actualClient << " has send a message : " << END << YELLOW;
					for (size_t i = 0; i < command.size(); i++) {
						std::cout << command[i] << " ";
					}
					std::cout << END << std::endl << std::endl;

					try {

					if (command[0] == "INVITE")
						invite(command, actualClient);
					else if (command[0] == "JOIN")
						join(command, actualClient);
					else if (command[0] == "MODE")
						mode(command, actualClient);
					else if (command[0] == "MSG" || command[0] == "PRIVMSG")
						msg(command, actualClient);
					else if (command[0] == "NAMES")
						names(command, actualClient);
					else if (command[0] == "NICK")
						nick(command, actualClient);
					else if (command[0] == "OPER")
						oper(command, actualClient);
					else if (command[0] == "PART")
						part(command, actualClient);
					else if (command[0] == "PASS")
						pass(command, actualClient);
					else if (command[0] == "PING")
						ping(command, actualClient);
					else if (command[0] == "TOPIC")
						topic(command, actualClient);
					else if (command[0] == "USER")
						user(command, actualClient);
					else if (command[0] == "QUIT")
						exit(0);
					else throw Exception::ERR_UNKNOWNCOMMAND(command[0]);
					
					if (!_clients[actualClient].getWelcomeBool() && _clients[actualClient].getPassBool() && _clients[actualClient].getNickBool() && _clients[actualClient].getUserBool()) {
						sendMessage(CLIENT_SOCKET, RPL_WELCOME(_clients[actualClient].getClientNickname(), _clients[actualClient].getClientUsername(), inet_ntoa(_clients[actualClient].getClientAddress().sin_addr)));
						sendMessage(CLIENT_SOCKET, RPL_YOURHOST(_clients[actualClient].getClientNickname()));
						sendMessage(CLIENT_SOCKET, RPL_CREATED(_clients[actualClient].getClientNickname()));
						sendMessage(CLIENT_SOCKET, RPL_MYINFO(_clients[actualClient].getClientNickname()));
						_clients[actualClient].welcomeBoolClient();
					}
					}
					catch (std::exception &e) {
						sendMessage(CLIENT_SOCKET, e.what());
						std::cout << BOLD << RED << "[ERROR] " << e.what() << END << std::endl;
					}
				}
			}
		}
	}
}



bool Server::_checkValidityNick(const std::string &nick) {
	return std::all_of(nick.begin(), nick.end(), ::isalnum);
}

std::vector<std::string> Server::_split(const std::string &str, const std::string &delimiters) {
	std::size_t prev_pos = 0, pos = 0;
	std::vector<std::string> output;

	while((pos = str.find_first_of(delimiters, prev_pos)) != std::string::npos) {
		if(pos > prev_pos) {
			output.push_back(str.substr(prev_pos, pos - prev_pos));
		}
		prev_pos = pos + 1;
	}
	if(prev_pos < str.length()) {
		output.push_back(str.substr(prev_pos, std::string::npos));
	}
	return output;
}

std::vector<std::string> Server::_splitWithDelimiters(const std::string &str, const std::string &delimiters) {
	std::size_t prev_pos = 0, pos = 0, nbDelimiters = 0;
	std::vector<std::string> output;

	while((pos = str.find_first_of(delimiters, prev_pos)) != std::string::npos) {
		if(pos > prev_pos) {
			output.push_back(str.substr(prev_pos - nbDelimiters, pos - prev_pos + nbDelimiters));
			nbDelimiters = 0;
		}
		nbDelimiters++;
		prev_pos = pos + 1;
	}
	if(prev_pos < str.length()) {
		output.push_back(str.substr(prev_pos - nbDelimiters, std::string::npos));
	}
	return output;
}

std::string	Server::_strcatArguments(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
	std::stringstream ss;

	while (begin != end) {
		ss << *begin;
		if (begin != end - 1)
			ss << " ";
		begin++;
	}
	return ss.str();
}

bool Server::_createChannel(std::string name, std::string password) {
	for (size_t i = 0; i < MAX_SERV_CHAN; i++) {
		if (!_channels[i].getChannelName().size()) {
			_channels[i].setChannel(name, password);
			return true;
		}
		if (i == MAX_SERV_CHAN - 1)
			return false;
	}
	return false;
}

int Server::_channelExist(std::string name) {
	for (size_t i = 0; i < MAX_SERV_CHAN; i++) {
		if (_channels[i].getChannelName().size())
			if (_channels[i].getChannelName() == name)
				return i;
	}
	return -1;
}

void Server::_checkClientInChannel() {
	for (size_t i = 0; i < MAX_SERV_CHAN; i++) {
		if (!_channels[i].checkIfClient())
			_channels[i].resetChannel();
	}
}

bool Server::_checkUserModeList(std::string const &modeList) {
	std::string::const_iterator it = modeList.begin(), its = modeList.end();
	std::string mode = (std::string)USER_MODE_AVAILABLE + CHAN_MODE_AVAILABLE;
	bool check = false;
	while (it != its) {
		if (*it == '+' || *it == '-') {
			it++;
			while (mode.find_first_of(*it) != std::string::npos) {
				it++;
				check = true;
			}
			if (!check)
				return false;
			else check = false;
		}
		else return false;
	}
	return true;
}

bool Server::_checkChannelModeList(std::string const &modeList) {
	return (modeList.size() == 2 && !modeList.find_first_of("+-") && modeList.find_first_of(CHAN_MODE_AVAILABLE) == 1);
}

void Server::_affAllClients() {
	std::cout << BOLD << CYAN << "All clients in server :" << END << std::endl;
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (_clients[i].getClientSocket())
			std::cout << BOLD << CYAN << "_clients[" << i << "] :" << std::endl << END << CYAN << _clients[i];
	}
	std::cout << END << std::endl;
}

void Server::_affAllChannels() {
	std::cout << BOLD << PURPLE << "All channels in server :" << END << std::endl;
	for (size_t i = 0; i < MAX_SERV_CHAN; i++)
	{
		if (_channels[i].getChannelName().size())
			std::cout << BOLD << PURPLE << "_channels[" << i << "] :" << std::endl << END << PURPLE << _channels[i];
	}
	std::cout << END << std::endl;
}

int Server::_clientExist(std::string name) {
	for (size_t i = 0; i < MAX_CLIENTS; i++) {
		if (_clients[i].getClientNickname().size())
			if (_clients[i].getClientNickname() == name)
				return i;
	}
	return -1;
}