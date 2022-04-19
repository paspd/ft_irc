#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"
#include "../Rand.hpp"

class Client;
class Channel;

class Server
{
private:
	SOCKET _masterSocket;
	SOCKADDR_IN _masterAddress;
	SOCKLEN_T _masterAddressLength;
	int _port;
	std::vector<Client> _clients;
	std::vector<Channel> _channels;
	std::string _password;
	fd_set _socketDescriptor;
	SOCKET _maxSocket;
	char buffer[2048];

public:
	Server(int port, char* password);

	~Server();

	void createMasterSocket();

	void bindMasterSocket();

	void serverListen();

	void cleanClientsList();

	void cleanChannelsList();

	void acceptConnection();

	void resetSocketDescriptor();

	void checkActivity();

	void checkServerActivity();

	void checkClientActivity();

	void sendMessage(SOCKET sock, std::string msg);

public: //commands

	void pass(std::vector<std::string> command, int actualClient);

	void nick(std::vector<std::string> command, int actualClient);

	void user(std::vector<std::string> command, int actualClient);

	void join(std::vector<std::string> command, int actualClient);

	void topic(std::vector<std::string> command, int actualClient);

	void msg(std::vector<std::string> command, int actualClient);

	void invite(std::vector<std::string> command, int actualClient);

	void kill(std::vector<std::string> command, int actualClient);

	void part(std::vector<std::string> command, int actualClient);

	void names(std::vector<std::string> command, int actualClient);

	void mode(std::vector<std::string> command, int actualClient);

	void ping(std::vector<std::string> command, int actualClient);

private:

	bool _checkValidityNick(const std::string &nick);

	std::vector<std::string> _split(const std::string &str, const std::string &delimiters);

	std::vector<std::string> _splitWithDelimiters(const std::string &str, const std::string &delimiters);

	std::string	_strcatArguments(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

	bool _createChannel(std::string name, std::string password);

	int _channelExist(std::string name);

	int _clientExist(std::string name);

	void _checkClientInChannel();

	bool _checkModeList(std::string const &modeList);

	void _affAllChannels();
	
	void _affAllClients();

	std::string _createClientPrompt(Client const &rhs);

};

#endif
