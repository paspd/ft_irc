#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Rand.hpp"
#include "../Client/Client.hpp"

class Server
{
private:
	SOCKET _masterSocket;
	SOCKADDR_IN _masterAddress;
	SOCKLEN_T _masterAddressLength;
	int _port;
	std::vector<Client> _clients;
	std::string _password;
	fd_set _socketDescriptor;
	SOCKET _maxSocket;
	char buffer[2048];

public:
	Server(int port, char* password): _port(port), _password(password) {
		try {
		_clients.reserve(MAX_CLIENTS);
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
	};

	~Server() {};



	void createMasterSocket(void) {
		if((_masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
			throw Exception::MasterSocketCreationFailed();

		_masterAddress.sin_family = AF_INET;
		_masterAddress.sin_addr.s_addr = INADDR_ANY;
		_masterAddress.sin_port = htons(_port);

		_masterAddressLength = sizeof(_masterAddress);
	}

	void bindMasterSocket() {
		if (bind(_masterSocket, (SOCKADDR *)&_masterAddress, sizeof(_masterAddress)) < 0)
			throw Exception::MasterSocketBindingFailed();
	}

	void serverListen() {
		std::cout << "Listening on port " << _port << " ..." << std::endl;
		if (listen(_masterSocket, SOMAXCONN) < 0)
			throw Exception::ListenFailed();
	}

	void cleanClientsList() {
		for (int i = 0; i < MAX_CLIENTS; i++) {
			_clients[i].resetClient();
		}
	}

	void acceptConnection() {
		while (true) {
			resetSocketDescriptor();
			checkActivity();
			checkServerActivity();
			checkClientActivity();
		}
	}

	void resetSocketDescriptor() {
		FD_ZERO(&_socketDescriptor);

		FD_SET(_masterSocket, &_socketDescriptor);

		_maxSocket = _masterSocket;

		for (int i = 0; i < MAX_CLIENTS; i++) {
			int s = CLIENT_SOCKET;
			if (s)
				FD_SET(s, &_socketDescriptor);
			if (s > _maxSocket)
				_maxSocket = s;
		}
	}

	void checkActivity() {
		if (select(_maxSocket + 1, &_socketDescriptor ,NULL, NULL, NULL) < 0 && (errno != EINTR))
			throw Exception::SelectFailed();

	}

	void checkServerActivity() {
		if (FD_ISSET(_masterSocket, &_socketDescriptor)) {
			int new_socket;
			SOCKADDR_IN new_address;
			SOCKLEN_T new_address_length = sizeof(new_address);
			if ((new_socket = accept(_masterSocket, (struct sockaddr *)&new_address, (socklen_t*)&new_address_length)) < 0)
				throw Exception::AcceptFailed();
				//err server full
			std::cout << "New connection at socket number " << new_socket << ", ip is : " << inet_ntoa(new_address.sin_addr) << ", port : " << ntohs(new_address.sin_port) << std::endl;
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (CLIENT_SOCKET == 0) {
					_clients[i].setClientSocket(new_socket);
					_clients[i].setClientAddress(new_address);
					std::cout << "Adding of new client at number: " << i << std::endl;
					break ;
				}
			}
		}
	}

	void checkClientActivity() {
		for (int i = 0; i < MAX_CLIENTS; i++) {
			int s = CLIENT_SOCKET;
			if (FD_ISSET(s, &_socketDescriptor)) {
				int retread;
				if ((retread = read(s, buffer, 2048)) == 0) {
					std::cout << "Host disconected, ip is : " << inet_ntoa(_clients[i].getClientAddress().sin_addr) << ", port : " << ntohs(_clients[i].getClientAddress().sin_port) << std::endl;
					close(s);
					_clients[i].resetClient();
				}
				else {
					// buffer[retread++] = ' ';
					buffer[retread] = '\0';
					std::string str_buffer = buffer;
					std::string separator = " \t\v\r\n";
					std::vector<std::string> command = _split(buffer, separator);
					if (!command.size())
						return ;
					std::cout << "Client number " << i << " has send a message : ";
					for (size_t i = 0; i < command.size(); i++) {
						std::cout << command[i] << " ";
					}
					std::cout << std::endl;

					try {

					if (command[0] == "PASS") {
						if (_clients[i].getLogged()) throw Exception::ERR_ALREADYREGISTERED();
						if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
						if (command.size() == 2 && command[1] != _password) throw Exception::ERR_PASSWDMISMATCH();

						_clients[i].logClient();
						std::cout << "Client number " << i << " has been accepted." << std::endl;
					}
					else if (command[0] == "NICK") {
						if (!_clients[i].getLogged()) throw Exception::ERR_NOTREGISTERED();
						if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);
						if (command.size() > 3) throw Exception::ERR_NONICKNAMEGIVEN();
						if (!_checkValidityNick(command[1])) throw Exception::ERR_ERRONEOUSNICKNAME(command[1]);
						if (command[1] == _clients[i].getClientNickname()) throw Exception::ERR_NICKNAMEINUSE(command[1]);

						else {
							for (size_t j = 0; j < MAX_CLIENTS; j++) {
								if (command[1] == _clients[j].getClientNickname()) {
									throw Exception::ERR_NICKCOLLISION(command[1]);
									break ;
								}
								else if (j == MAX_CLIENTS - 1) {
									_clients[i].setClientNickname(command[1]);
									_clients[i].nickClient();
									std::cout << "Client number " << i << " has been change his nickname to : " << _clients[i].getClientNickname() << "." << std::endl;
								}
							}
						}
					}
					else if (command[0] == "USER") {
						if (!_clients[i].getLogged()) throw Exception::ERR_NOTREGISTERED();
						if (_clients[i].getRegistred()) throw Exception::ERR_ALREADYREGISTERED();
						if (command.size() < 2) throw Exception::ERR_NEEDMOREPARAMS(command[0]);

						_clients[i].setClientUsername(command[1]);
						_clients[i].setClientMode(std::atoi(command[2].c_str()));
						_clients[i].setClientRealname(_catArguments(command.begin() + 4, command.end()));
						_clients[i].registreClient();

						std::cout << "Client number " << i << " has been change his username to : " << _clients[i].getClientUsername() << ", his mode to :" << _clients[i].getClientMode() << " and his realname to :" << _clients[i].getClientRealname() << std::endl;
					}
					else if (command[0] == "JOIN") { }
					else if (command[0] == "MSG") { }
					else if (command[0] == "PRIVMSG") {

					}
					else if (command[0] == "QUIT") {
						throw Exception::ERR_QUIT(inet_ntoa(_clients[i].getClientAddress().sin_addr));
					}
					else throw Exception::ERR_UNKNOWNCOMMAND(command[0]);
					if (!_clients[0].getWelcome() && _clients[i].getLogged() && _clients[i].getRegistred() && _clients[i].getNicked()) {
						sendMessage(CLIENT_SOCKET, RPL_WELCOME(_clients[i].getClientNickname(), _clients[i].getClientUsername(), inet_ntoa(_clients[i].getClientAddress().sin_addr)));
						sendMessage(CLIENT_SOCKET, RPL_YOURHOST);

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

	void sendMessage(SOCKET sock, std::string msg) {
		int msgSize = msg.length();
		ssize_t ret_send = send(sock, msg.c_str(), msgSize, 0);
		if (ret_send != (ssize_t)msgSize)
			throw Exception::SendFailed();
	}

private:

	bool _checkValidityNick(const std::string &nick) {
		return std::all_of(nick.begin(), nick.end(), ::isalnum);
	}

	std::vector<std::string> _split(const std::string &str, const std::string &delimiters) {
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

	std::string	_catArguments(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
		std::stringstream ss;

		while (begin != end) {
			ss << *begin;
			if (begin != end - 1)
				ss << " ";
			begin++;
		}
		return ss.str();
	}










































};

#endif
