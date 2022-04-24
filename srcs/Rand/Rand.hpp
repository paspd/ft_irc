#ifndef RAND_HPP
#define RAND_HPP

#define AFF(thing) (std::cout << thing << std::endl)

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
#include <algorithm>


typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef socklen_t SOCKLEN_T;
typedef struct sockaddr SOCKADDR;

// void sendMessage(SOCKET sock, std::string msg);

#define GREY	"\033[30m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define PINK	"\033[35m"
#define CYAN	"\033[36m"
#define ORANGE	"\033[38;5;166m"
#define PURPLE	"\033[38;5;128m"
#define WHITE	"\033[37m"
#define END		"\033[0m"
#define BOLD	"\033[1m"
#define UNDER	"\033[4m"
#define SUR		"\033[7m"

#define SERVER_NAME "serverIRC"
#define SERVER_NAME_PROMPT (static_cast<std::string>(":") + static_cast<std::string>(SERVER_NAME))

#define VERSION "v 0.1.0"
#define CREATION_DATE "28.03.2022"

#define USER_MODE_AVAILABLE "iro"
#define CHAN_MODE_AVAILABLE "iptnmkovC"

#define OPER_MDP "mdp"

#define MAX_CLIENTS 1024
#define MAX_OCCUPANTS_CHAN 16
#define MAX_CURRENT_CHAN 8
#define MAX_SERV_CHAN 256

#define CLIENT_SOCKET _clients[actualClient].getClientSocket()

#define PASS (char *)"PASS"
#define NICK (char *)"NICK"

#define CRLF "\r\n"

#define RPL_WELCOME(nickname, username, hostname) (SERVER_NAME_PROMPT + " 001 " + nickname + " Welcome to the Internet Relay Network " + CRLF)
#define RPL_YOURHOST(nickname) (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 002 ") + static_cast<std::string>(nickname) + static_cast<std::string>(" :Your host is ") + static_cast<std::string>(SERVER_NAME) + static_cast<std::string>(", running version ") + static_cast<std::string>(VERSION) + static_cast<std::string>(CRLF))
#define RPL_CREATED(nickname) (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 003 ") + static_cast<std::string>(nickname) + static_cast<std::string>(" This server was created ") + static_cast<std::string>(CREATION_DATE) + static_cast<std::string>(CRLF))
#define RPL_MYINFO(nickname) (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 004 ") + static_cast<std::string>(nickname) + static_cast<std::string>(" ") + static_cast<std::string>(SERVER_NAME) + static_cast<std::string>(" ") + static_cast<std::string>(VERSION) + static_cast<std::string>(" ") + static_cast<std::string>(USER_MODE_AVAILABLE) + static_cast<std::string>(" ") + static_cast<std::string>(CHAN_MODE_AVAILABLE) + static_cast<std::string>(" ") + static_cast<std::string>(CRLF))
#define RPL_UMODEIS(nickname, clientmode) ("221 " + nickname + " " + clientmode + CRLF)
#define RPL_CHANNELMODEIS(nickname, channelname, channelmode) ("324 " + nickname + " " + channelname + " " + channelmode + CRLF)
#define RPL_NOTOPIC(channel) (SERVER_NAME_PROMPT + " 331 " + channel + " :No topic is set" + CRLF)
#define RPL_TOPIC(channel, topic) (SERVER_NAME_PROMPT + " 332 " + channel + " :" + topic + CRLF)
#define RPL_NAMREPLY(nickname, channel, occupants) (SERVER_NAME_PROMPT + " 353 " + nickname + " @ " + channel + " :" + occupants + CRLF)
#define RPL_ENDOFNAMES(nickname, channel) (SERVER_NAME_PROMPT + " 366 " + nickname + " " + channel + " :End of /NAMES list" CRLF)
#define RPL_BANLIST(nickname, channel, ban, clientprompt) (SERVER_NAME_PROMPT + " 367 " + nickname + " " + channel + " " + ban + " " + clientprompt + CRLF)
#define RPL_ENDOFBANLIST(channel) (SERVER_NAME_PROMPT + " 368 " + channel + " :End of channel ban list" + CRLF)
#define RPL_YOUREOPER(clientprompt) (clientprompt + " 381 " + " :You are now an IRC operator" + CRLF)
#define ERR_NOSUCHNICK_BUILDER(nickname) (SERVER_NAME_PROMPT + " 401 " + nickname + " :No such nick/channel" + CRLF)
#define ERR_NOSUCHCHANNEL_BUILDER(channel) (SERVER_NAME_PROMPT + " 403 " + channel + " :No such channel" + CRLF)
#define ERR_TOOMANYCHANNELS_BUILDER(nickname, channel) (SERVER_NAME_PROMPT + " 404 " + nickname + " " + channel + " :You have joined too many channels" + CRLF)
#define ERR_CANNOTSENDTOCHAN_BUILDER(nickname, channel) (SERVER_NAME_PROMPT + " 405 " + nickname + " " + channel + " :Cannot send to channel" + CRLF)
#define ERR_NOTEXTTOSEND_BUILDER(nickname) (SERVER_NAME_PROMPT + " 412 " + nickname + " :No text to send" + CRLF)
#define ERR_UNKNOWNCOMMAND_BUILDER(command) (static_cast<std::string>(SERVER_NAME_PROMPT) + static_cast<std::string>(" 421 ") + static_cast<std::string>(command) + static_cast<std::string>(" :Unknown command") + static_cast<std::string>(CRLF))
#define ERR_ERRONEOUSNICKNAME_BUILDER(nickname) (SERVER_NAME_PROMPT + " 432 " + nickname + " :Erroneous nickname" + CRLF)
#define ERR_NICKNAMEINUSE_BUILDER(nickname) (SERVER_NAME_PROMPT + " 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_NICKCOLLISION_BUILDER(nickname) (SERVER_NAME_PROMPT + " 436 " + nickname + " :Nickname collision" + CRLF)
#define ERR_USERNOTINCHANNEL_BUILDER(nickname, channel) (SERVER_NAME_PROMPT + " 441 " + nickname + " " + channel + " :They aren't on that channel" + CRLF)
#define ERR_NOTONCHANNEL_BUILDER(channel) (SERVER_NAME_PROMPT + " 442 " + channel + " :You're not on that channel" + CRLF)
#define ERR_USERONCHANNEL_BUILDER(nickname, channel) (SERVER_NAME_PROMPT + " 443 " + channel + " :User " + nickname + " is already on the channel" + CRLF)
#define ERR_NEEDMOREPARAMS_BUILDER(command) (SERVER_NAME_PROMPT + " 461 " + command + " :Not enough parameters" + CRLF)
#define ERR_CHANNELISFULL_BUILDER(channel) (SERVER_NAME_PROMPT + " 471 " + channel + " :Cannot join channel (+l)" + CRLF)
#define ERR_BADCHANNELKEY_BUILDER(channel) (SERVER_NAME_PROMPT + " 475 " + channel + " :Cannot join channel (+k)" + CRLF)
#define ERR_CHANOPRIVSNEEDED_BUILDER(channel) (SERVER_NAME_PROMPT + " 482 " + channel + " :You're not channel operator" + CRLF)
#define ERR_RESTRICTED_BUILDER(nickname) (SERVER_NAME_PROMPT + " 482 " + nickname + " :Your connection is restricted!" + CRLF)
#define ERR_USERSDONTMATCH_BUILDER(nickname) (SERVER_NAME_PROMPT + " 502 " + nickname + " :Can't change mode for other users" + CRLF)

#define RPL_PRIVMSG_MESSAGE(clientPrompt, nickname, message) (clientPrompt + " PRIVMSG " + nickname + " " + message + CRLF)
#define RPL_TOPIC_CMD(clientPrompt, channel, topic) (clientPrompt + " TOPIC " + channel + " :" + topic + CRLF)
#define RPL_MODE_CMD(clientPrompt, channel, mode) (clientPrompt + " MODE " + channel + " :" + mode + CRLF)
#define RPL_MODE_CUSTOM_CMD(clientPrompt, channel, mode, param) (clientPrompt + " MODE " + channel + " :" + mode + " " + param + CRLF)
#define RPL_CHAN_MODE(channel, mode) (SERVER_NAME_PROMPT + " MODE " + channel + " " + mode + CRLF)
#define RPL_PART_MESSAGE(clientPrompt, channel, message) (clientPrompt + " PART " + channel + (message[0] == ':' ? " " : " :") + message + CRLF)
#define RPL_KICK_CMD(clientPrompt, nickname, channel, message) (clientPrompt + " KICK " + nickname + " " + channel + (message.empty() ? "" : (message[0] == ':' ? " " : " :")) + message + CRLF)
#define RPL_PART_NOMESSAGE(clientPrompt, channel) (clientPrompt + " PART " + channel + CRLF)
#define RPL_PONG() (SERVER_NAME_PROMPT + " PONG " + SERVER_NAME  + " " + SERVER_NAME_PROMPT +  CRLF)
#define RPL_INVITING(clientPrompt, channel) (clientPrompt + " INVITE " + channel + CRLF)
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
				static std::string message;																		\
				message = exceptionMessageBuilder(function, std::strerror(errno));								\
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

#define IRC_EXCEPTION_CUSTOM_2(name, messageBuilder)															\
class name : public std::exception {																			\
	private:																									\
		std::string param1;																						\
		std::string param2;																						\
	public:																										\
		name(const std::string param1, const std::string param2) : param1(param1), param2(param2) {}			\
		~name() throw() {}																						\
		virtual const char* what() const throw() {																\
			static std::string msg;																				\
			msg = messageBuilder(param1, param2);																\
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
		IRC_EXCEPTION(ERR_UMODEUNKNOWNFLAG, " 501 :Unknown MODE flag\r\n")
		IRC_EXCEPTION(ERR_NOPRIVILEGES, " 481 :Permission Denied- You're not an IRC operator\r\n")
		IRC_EXCEPTION_CUSTOM(ERR_UNKNOWNCOMMAND, ERR_UNKNOWNCOMMAND_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NICKCOLLISION, ERR_NICKCOLLISION_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_ERRONEOUSNICKNAME, ERR_ERRONEOUSNICKNAME_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NICKNAMEINUSE, ERR_NICKNAMEINUSE_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_BADCHANNELKEY, ERR_BADCHANNELKEY_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_CHANNELISFULL, ERR_CHANNELISFULL_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NOSUCHCHANNEL, ERR_NOSUCHCHANNEL_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NOTONCHANNEL, ERR_NOTONCHANNEL_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NOTEXTTOSEND, ERR_NOTEXTTOSEND_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NOSUCHNICK, ERR_NOSUCHNICK_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_CHANOPRIVSNEEDED, ERR_CHANOPRIVSNEEDED_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_USERSDONTMATCH, ERR_USERSDONTMATCH_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_RESTRICTED, ERR_RESTRICTED_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_QUIT, ERR_QUIT_BUILDER)
		IRC_EXCEPTION_CUSTOM_2(ERR_USERONCHANNEL, ERR_USERONCHANNEL_BUILDER)
		IRC_EXCEPTION_CUSTOM_2(ERR_TOOMANYCHANNELS, ERR_TOOMANYCHANNELS_BUILDER)
		IRC_EXCEPTION_CUSTOM_2(ERR_CANNOTSENDTOCHAN, ERR_CANNOTSENDTOCHAN_BUILDER)
		IRC_EXCEPTION_CUSTOM_2(ERR_USERNOTINCHANNEL, ERR_USERNOTINCHANNEL_BUILDER)
};

#include "../Server/Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"

void sendMessage(SOCKET sock, std::string msg);

#endif
