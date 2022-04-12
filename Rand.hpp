#ifndef RAND_HPP
#define RAND_HPP

#include <stdio.h> 
#include <string.h>   //strlen 
#include <string>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef socklen_t SOCKLEN_T;
typedef struct sockaddr SOCKADDR;

#define SERVER_NAME "serverIRC"
#define SERVER_NAME_PROMPT (static_cast<std::string>(":") + static_cast<std::string>(SERVER_NAME))

#define VERSION "v 0.0.0"
#define CREATION_DATE "28.03.2022"

#define USER_MODE_AVAILABLE ""
#define CHAN_MODE_AVAILABLE ""

#define MAX_CLIENTS 1024
#define MAX_OCCUPANTS_CHAN 16
#define MAX_CURRENT_CHAN 8
#define MAX_SERV_CHAN 256


#define CLIENT_SOCKET _clients[actualClient].getClientSocket()

#define PASS (char *)"PASS"
#define NICK (char *)"NICK"

#define CRLF "\r\n"


//WELCOME
#define RPL_WELCOME(nick, user, host) (SERVER_NAME_PROMPT + " 001 Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + CRLF)
#define RPL_YOURHOST (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 002 Your host is ") + static_cast<std::string>(SERVER_NAME) + static_cast<std::string>(", running version ") + static_cast<std::string>(VERSION) + static_cast<std::string>(CRLF))
#define RPL_CREATED (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 003 This server was created ") + static_cast<std::string>(CREATION_DATE) + static_cast<std::string>(CRLF))
#define RPL_MYINFO (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 004 ") + static_cast<std::string>(SERVER_NAME) + static_cast<std::string>(" ") + static_cast<std::string>(VERSION) + static_cast<std::string>(" ") + static_cast<std::string>(USER_MODE_AVAILABLE) + static_cast<std::string>(" ") + static_cast<std::string>(CHAN_MODE_AVAILABLE) + static_cast<std::string>(" ") + static_cast<std::string>(CRLF))

// GLOBAL
#define ERR_UNKNOWNCOMMAND_BUILDER(command) (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 421 * ") + static_cast<std::string>(command) + static_cast<std::string>(" :Unknown command") + static_cast<std::string>(CRLF))
#define ERR_NEEDMOREPARAMS_BUILDER(command) (SERVER_NAME_PROMPT + " 461 * " + command + " :Not enough parameters" + CRLF)

//NICK
#define ERR_NICKCOLLISION_BUILDER(nick) (SERVER_NAME_PROMPT + " 436 * " + nick + " :Nickname collision" + CRLF)
#define ERR_NICKNAMEINUSE_BUILDER(nickname) (SERVER_NAME_PROMPT + " 433 * " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEOUSNICKNAME_BUILDER(nickname) (SERVER_NAME_PROMPT + " 432 * " + nickname + " :Erroneous nickname" + CRLF)

//JOIN
#define RPL_NOTOPIC(channel) (SERVER_NAME_PROMPT + " 331 * " + channel + " :No topic is set" + CRLF)
#define RPL_CHAN_MODE(channel) (SERVER_NAME_PROMPT + " MODE " + channel + " " + CHAN_MODE_AVAILABLE + CRLF)
#define RPL_TOPIC(channel, topic) (SERVER_NAME_PROMPT + " 332 * " + channel + " :" + topic + CRLF)
#define ERR_NOSUCHCHANNEL_BUILDER(channel) (SERVER_NAME_PROMPT + " 403 * " + channel + " :No such channel" + CRLF)
#define ERR_TOOMANYCHANNELS_BUILDER(channel) (SERVER_NAME_PROMPT + " 405 * " + channel + " :You have joined too many channels" + CRLF)
#define ERR_CHANNELISFULL_BUILDER(channel) (SERVER_NAME_PROMPT + " 471 * " + channel + " :Cannot join channel (+l)" + CRLF)
#define ERR_BADCHANNELKEY_BUILDER(channel) (SERVER_NAME_PROMPT + " 475 * " + channel + " :Cannot join channel (+k)" + CRLF)


//QUIT
#define ERR_QUIT_BUILDER(ip) (SERVER_NAME_PROMPT + " ERROR :Closing link: " + ip + "(Client Quit)" + CRLF)


#define SERVER_EXCEPTION(name, function)																		\
class name : public std::exception {																			\
		private:																								\
			std::string exceptionMessageBuilder(const std::string &func, const std::string &message) const {	\
				std::stringstream ss;																			\
				ss << func << ": " << message << CRLF;															\
				return (ss.str());																				\
			}																									\
		public:																									\
			virtual const char* what() const throw() {															\
				static std::string message = exceptionMessageBuilder(function, std::strerror(errno));			\
				return (message.c_str());																		\
			}																									\
};

#define IRC_EXCEPTION(name, message)																			\
class name : public std::exception {																			\
		public:																									\
			virtual const char* what() const throw() {															\
				static std::string msg;																			\
				msg = message;																					\
				return (msg.c_str());																			\
			}																									\
};

#define IRC_EXCEPTION_CUSTOM(name, messageBuilder)																\
class name : public std::exception {																			\
	std::string param;																							\
	public:																										\
		name(const std::string &param) : param(param) {}														\
		~name() throw() {}																						\
		virtual const char* what() const throw() {																\
			static std::string msg;																				\
			msg = messageBuilder(param);																		\
			return (msg.c_str());																				\
		}																										\
};

namespace Exception {
		SERVER_EXCEPTION(MasterSocketBindingFailed, "bind")
		SERVER_EXCEPTION(MasterSocketCreationFailed, "socket");
		SERVER_EXCEPTION(ListenFailed, "listen")
		SERVER_EXCEPTION(AcceptFailed, "accept")
		SERVER_EXCEPTION(SelectFailed, "select")
		SERVER_EXCEPTION(SendFailed, "send")
		SERVER_EXCEPTION(ReadFailed, "read")


		IRC_EXCEPTION(ERR_NONICKNAMEGIVEN, "431 :No nickname given\r\n")
		IRC_EXCEPTION(ERR_NOTREGISTERED, "451 :You have not registered\r\n")
		IRC_EXCEPTION(ERR_PASSWDMISMATCH, "464 * PASS :Password incorrect\r\n")
		IRC_EXCEPTION(ERR_ALREADYREGISTERED, "462 * PASS :Unauthorized command (already registered)\r\n")
		IRC_EXCEPTION(ERR_RESTRICTED, "484 :Your connection is restricted!\r\n")
		IRC_EXCEPTION_CUSTOM(ERR_UNKNOWNCOMMAND, ERR_UNKNOWNCOMMAND_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NICKCOLLISION, ERR_NICKCOLLISION_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_ERRONEOUSNICKNAME, ERR_ERRONEOUSNICKNAME_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NICKNAMEINUSE, ERR_NICKNAMEINUSE_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_BADCHANNELKEY, ERR_BADCHANNELKEY_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_CHANNELISFULL, ERR_CHANNELISFULL_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_TOOMANYCHANNELS, ERR_TOOMANYCHANNELS_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NOSUCHCHANNEL, ERR_NOSUCHCHANNEL_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_QUIT, ERR_QUIT_BUILDER)
};

#include "Server/Server.hpp"
#include "Channel/Channel.hpp"
#include "Client/Client.hpp"
#include "Mode/Mode.hpp"

#endif