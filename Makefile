NAME			=		ircserv

CC				=		@clang++

CFLAGS			=		-Wall -Werror -Wextra -std=c++98 -g

INCLUDES		=		srcs/Server/Server.hpp \
						srcs/Client/Client.hpp \
						srcs/Channel/Channel.hpp \
						srcs/Mode/OccupantChannelMode.hpp \
						srcs/Mode/ChannelMode.hpp \
						srcs/Mode/ClientMode.hpp \
						srcs/Rand/Rand.hpp

SRCS			=		srcs/main.cpp \
						srcs/Server/Server.cpp \
						srcs/Client/Client.cpp \
						srcs/Channel/Channel.cpp \
						srcs/Commands/INVITE.cpp \
						srcs/Commands/JOIN.cpp \
						srcs/Commands/KICK.cpp \
						srcs/Commands/MODE.cpp \
						srcs/Commands/MSG.cpp \
						srcs/Commands/NAMES.cpp \
						srcs/Commands/NICK.cpp \
						srcs/Commands/NOTICE.cpp \
						srcs/Commands/OPER.cpp \
						srcs/Commands/PART.cpp \
						srcs/Commands/PASS.cpp \
						srcs/Commands/PING.cpp \
						srcs/Commands/QUIT.cpp \
						srcs/Commands/TOPIC.cpp \
						srcs/Commands/USER.cpp \
						srcs/Rand/Rand.cpp

OBJS			=		$(SRCS:.cpp=.o)

all				:		$(NAME)


$(NAME)		: $(OBJS)
		@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
		@echo "\033[34m[$(NAME)] Compiled\033[0m"

%.o: %.cpp $(INCLUDES)
		@$(CC) $(CFLAGS) -c $< -o $@

clean		:
		@echo "\033[32m[$(NAME)] Clean\033[0m"
		@rm -f $(OBJS)

fclean		:
		@echo "\033[32m[$(NAME)] Clean\033[0m"
		@rm -f $(OBJS)
		@rm -f $(NAME)

re			:		fclean all

.PHONY: all bonus clean fclean re