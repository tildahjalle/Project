#ifndef MESSAGE_H
#define MESSAGE_H

#include "connection.h"
#include <string>
#include <vector>
class Message{
public:
    //connection (message reading) constructor
    Message();
    Message(Connection&);
    //command list constructor (message writing)
    Message(char, char = 0, std::vector<int> = std::vector<int>(), std::vector<std::string> = std::vector<std::string>());
    void transmit(Connection&);
    
    char command, ack;
    std::vector<std::string> stringargs;
    std::vector<int> intargs;
    
private:
    int readNbr(const Connection&);
    void writeNbr(const Connection&, int);
    std::string readString(const Connection&);
    void writeString(const Connection&,std::string);
};
#endif
