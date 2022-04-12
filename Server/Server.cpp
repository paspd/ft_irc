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
	std::cout << "Listening on port " << _port << " ..." << std::endl;
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
		std::cout << "New connection at socket number " << new_socket << ", ip is : " << inet_ntoa(new_address.sin_addr) << ", port : " << ntohs(new_address.sin_port) << std::endl;
		for (int actualClient = 0; actualClient < MAX_CLIENTS; actualClient++) {
			if (CLIENT_SOCKET == 0) {
				_clients[actualClient].setClientSocket(new_socket);
				_clients[actualClient].setClientAddress(new_address);
				std::cout << "Adding of new client at number: " << actualClient << std::endl;
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
				std::cout << "Host disconected, ip is : " << inet_ntoa(_clients[actualClient].getClientAddress().sin_addr) << ", port : " << ntohs(_clients[actualClient].getClientAddress().sin_port) << std::endl;
				close(s);
				_clients[actualClient].resetClient();
			}
			else {
				buffer[retread] = '\0';
				std::string str_buffer = buffer;
				std::string separator = " \t\v\r\n";
				std::vector<std::string> command = _split(buffer, separator);
				Client tmp;
				tmp.getClientAddress();
				if (!command.size())
					return ;
				std::cout << "Client number " << actualClient << " has send a message : ";
				for (size_t i = 0; i < command.size(); i++) {
					std::cout << command[i] << " ";
				}
				std::cout << std::endl;

				try {

				if (command[0] == "PASS") {
					if (_clients[actualClient].getPassBool()) throw Exception::ERR_ALREADYREGISTERED();
					if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
					if (command.size() == 2 && command[1] != _password) throw Exception::ERR_PASSWDMISMATCH();

					_clients[actualClient].passBoolClient();
					std::cout << "Client number " << actualClient << " has been accepted." << std::endl;
				}
				else if (command[0] == "NICK") {
					if (!_clients[actualClient].getPassBool()) throw Exception::ERR_NOTREGISTERED();
					if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
					if (command.size() > 3) throw Exception::ERR_NONICKNAMEGIVEN();
					if (!_checkValidityNick(command[1])) throw Exception::ERR_ERRONEOUSNICKNAME(command[1]);
					if (command[1] == _clients[actualClient].getClientNickname()) throw Exception::ERR_NICKNAMEINUSE(command[1]);

					else {
						for (size_t j = 0; j < MAX_CLIENTS; j++) {
							if (command[1] == _clients[j].getClientNickname()) {
								throw Exception::ERR_NICKCOLLISION(command[1]);
								break ;
							}
							else if (j == MAX_CLIENTS - 1) {
								_clients[actualClient].setClientNickname(command[1]);
								_clients[actualClient].nickBoolClient();
								std::cout << "Client number " << actualClient << " has been change his nickname to : " << _clients[actualClient].getClientNickname() << "." << std::endl;
							}
						}
					}
				}
				else if (command[0] == "USER") {
					if (!_clients[actualClient].getPassBool()) throw Exception::ERR_NOTREGISTERED();
					if (_clients[actualClient].getUserBool()) throw Exception::ERR_ALREADYREGISTERED();
					if (command.size() < 5) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

					_clients[actualClient].setClientUsername(command[1]);
					_clients[actualClient].setClientMode(std::atoi(command[2].c_str()));
					_clients[actualClient].setClientRealname(_strcatArguments(command.begin() + 4, command.end()));
					_clients[actualClient].userBoolClient();

					std::cout << "Client number " << actualClient << " has been change his username to : " << _clients[actualClient].getClientUsername() << ", his mode to :" << _clients[actualClient].getClientMode() << " and his realname to :" << _clients[actualClient].getClientRealname() << std::endl;
				}
				else if (command[0] == "JOIN") {
					_clients[actualClient].welcomeBoolClient();
					if (!_clients[actualClient].getWelcomeBool()) throw Exception::ERR_RESTRICTED();
					if (!(command.size() >= 2 && command.size() <= 3)) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

					if (command.size() == 2 && command[1] == "0") {
						_clients[actualClient].leaveAllChannels();
						return ;
					}

					std::vector<std::string> chanName = _split(command[1], ",");
					std::vector<std::string> chanPass;
					if (command.size() == 3)
						chanPass = _split(command[2], ",");

					for (size_t j = 0; j < chanName.size(); j++)
					{
						if (!_channelExist(chanName[j]))
							if (chanName[0].find_first_of("#!&+") > 0 || (!_createChannel(chanName[j], (chanPass.size() >= j && command.size() == 3 ? chanPass[j] : "")))) throw Exception::ERR_NOSUCHCHANNEL(chanName[j]);
						for (size_t k = 0; k < MAX_SERV_CHAN; k++) {
							if (_channels[k].getChannelName().size() && _channels[k].getChannelName() == chanName[j]) {
								if (_clients[actualClient].checkConnected(&_channels[k]))
									break ;
								_channels[k].connectToChan(&_clients[actualClient], (chanPass.size() >= j && command.size() == 3 ? chanPass[j] : ""));
								if (!_clients[actualClient].addChannel(&_channels[k])){
									_channels[k].delOccupant(_clients[actualClient].getClientSocket());
									throw Exception::ERR_TOOMANYCHANNELS(chanName[j]);
								}
								else {
								sendMessage(CLIENT_SOCKET, _createClientPrompt(_clients[actualClient]) + " JOIN " + _channels[k].getChannelName() + CRLF);
								sendMessage(CLIENT_SOCKET, RPL_CHAN_MODE(_channels[k].getChannelName()));
								if (!_channels[k].getChannelTopic().empty())
									sendMessage(CLIENT_SOCKET, RPL_TOPIC(_channels[k].getChannelName(), _channels[k].getChannelTopic()));
								else
									sendMessage(CLIENT_SOCKET, RPL_NOTOPIC(_channels[k].getChannelName()));
								}
								
							}
						}
					}
				}
				else if (command[0] == "MSG") {  }
				else if (command[0] == "PRIVMSG") { }
				else if (command[0] == "QUIT") {
					exit(0);
					throw Exception::ERR_QUIT(inet_ntoa(_clients[actualClient].getClientAddress().sin_addr));
				}
				else throw Exception::ERR_UNKNOWNCOMMAND(command[0]);	
				if (!_clients[actualClient].getWelcomeBool() && _clients[actualClient].getPassBool() && _clients[actualClient].getNickBool() && _clients[actualClient].getUserBool()) {
					sendMessage(CLIENT_SOCKET, RPL_WELCOME(_clients[actualClient].getClientNickname(), _clients[actualClient].getClientUsername(), inet_ntoa(_clients[actualClient].getClientAddress().sin_addr)));
					sendMessage(CLIENT_SOCKET, RPL_YOURHOST);
					sendMessage(CLIENT_SOCKET, RPL_CREATED);
					sendMessage(CLIENT_SOCKET, RPL_MYINFO);
					_clients[actualClient].welcomeBoolClient();
				}
				}
				catch (Exception::ERR_QUIT &e) {
					sendMessage(CLIENT_SOCKET, e.what());
				}
				catch (std::exception &e) {
					sendMessage(CLIENT_SOCKET, e.what());
				}
			}
		}
	}
}

void Server::sendMessage(SOCKET sock, std::string msg) {
	int msgSize = msg.length();
	ssize_t ret_send = send(sock, msg.c_str(), msgSize, 0);
	if (ret_send != (ssize_t)msgSize)
		throw Exception::SendFailed();
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

bool Server::_channelExist(std::string name) {
	for (size_t i = 0; i < MAX_SERV_CHAN; i++) {

		if (_channels[i].getChannelName().size())
			if (_channels[i].getChannelName() == name)
				return true;
	}
	return false;
}

void Server::_checkClientInChannel() {
	for (size_t i = 0; i < MAX_SERV_CHAN; i++) {
		if (!_channels[i].checkIfClient())
			_channels[i].resetChannel();
	}
}

void Server::_affAllClients() {
	std::cout << "All clients in server :" << std::endl;
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (_clients[i].getClientSocket())
			std::cout << "_clients[" << i << "] :" << std::endl << _clients[i];
	}
	std::cout << std::endl;
}

void Server::_affAllChannels() {
	std::cout << "All channels in server :" << std::endl;
	for (size_t i = 0; i < MAX_SERV_CHAN; i++)
	{
		if (_channels[i].getChannelName().size())
			std::cout << "_channels[" << i << "] :" << std::endl << _channels[i];
	}
	std::cout << std::endl;
}

std::string Server::_createClientPrompt(Client const &rhs) {
	std::stringstream ss;
	ss << ":" << rhs.getClientNickname() << "!~" << rhs.getClientUsername() << "@" << inet_ntoa(rhs.getClientAddress().sin_addr);
	return ss.str();
}