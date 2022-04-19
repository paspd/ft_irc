NAME			=		ircserv

CC				=		clang++

CFLAGS			=		-Wall -Werror -Wextra -std=c++98

SRCS			=		main.cpp \
						Server/Server.cpp \
						Client/Client.cpp \
						Channel/Channel.cpp \
						Commands/INVITE.cpp \
						Commands/JOIN.cpp \
						Commands/KILL.cpp \
						Commands/MODE.cpp \
						Commands/MSG.cpp \
						Commands/NAMES.cpp \
						Commands/NICK.cpp \
						Commands/PART.cpp \
						Commands/PASS.cpp \
						Commands/PING.cpp \
						Commands/TOPIC.cpp \
						Commands/USER.cpp

OBJS			=		$(SRCS:.cpp=.o)

all				:		$(NAME)


$(NAME)		: $(OBJS)
		@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
		@echo "\033[34m[$(NAME)] Compiled\033[0m"

%.o: %.cpp
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