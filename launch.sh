for i in {1..10}
do
    clang++ -Werror -Wall -Wextra -std=c++98 -g Server/Server.cpp Client/Client.cpp Channel/Channel.cpp main.cpp
    ./a.out $i "hello"
done