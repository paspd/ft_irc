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

#define MAX_CLIENTS 1024

#define CLIENT_SOCKET _clients[i].getClientSocket()

#define PASS (char *)"PASS"
#define NICK (char *)"NICK"

#define CRLF "\r\n"


// GLOBAL
#define ERR_UNKNOWNCOMMAND_BUILDER(command) ("421 * " + command + " :Unknown command" + CRLF)
#define ERR_NEEDMOREPARAMS_BUILDER(command) ("461 * " + command + " :Not enough parameters" + CRLF)

//NICK
#define ERR_NICKCOLLISION_BUILDER(nick) (" * " + nick + " :Nickname collision" + CRLF)
#define ERR_NICKNAMEINUSE_BUILDER(nickname) ("433 * " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEOUSNICKNAME_BUILDER(nickname) ("432 * " + nickname + " :Erroneous nickname" + CRLF)

#define SERVER_EXCEPTION(name, function)																		\
class name : public std::exception {																			\
		private:																								\
			std::string exceptionMessageBuilder(const std::string &func, const std::string &message) const {	\
				std::stringstream ss;																			\
				ss << func << ": " << message;																	\
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
				static std::string msg = message;																\
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
			static std::string msg = messageBuilder(param);														\
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


		IRC_EXCEPTION(ERR_NONICKNAMEGIVEN, "431 PASS :Password incorrect\r\n")
		IRC_EXCEPTION(ERR_NOTREGISTERED, "451 :No nickname given\r\n")
		IRC_EXCEPTION(ERR_PASSWDMISMATCH, "464 * PASS :Password incorrect\r\n")
		IRC_EXCEPTION(ERR_ALREADYREGISTERED, "462 * PASS :Unauthorized command (already registered)\r\n")
		IRC_EXCEPTION(ERR_RESTRICTED, "484 :Your connection is restricted!\r\n")
		IRC_EXCEPTION_CUSTOM(ERR_UNKNOWNCOMMAND, ERR_UNKNOWNCOMMAND_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_ERRONEOUSNICKNAME, ERR_ERRONEOUSNICKNAME_BUILDER)
		IRC_EXCEPTION_CUSTOM(ERR_NICKNAMEINUSE, ERR_NICKNAMEINUSE_BUILDER)
};

#endif