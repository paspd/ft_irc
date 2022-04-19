for i in {1..10}
do
    make
    clear
    ./ircserv $i "hello"
done