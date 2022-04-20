for i in {1..10}
do
    rm -rf ircserv
    make
    clear
    ./ircserv $i "hello"
done