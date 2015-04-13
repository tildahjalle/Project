#ifndef MESSAGE_H
#define MESSAGE_H

#include "connection.h"
#include <string>
#include <vector>
class Message{
public:
//connection (message reading) constructor
	Message(const Connection&);
	//command list constructor (message writing)
	Message(char, char, std::vector<int> = std::vector<int>(), std::vector<std::string> = std::vector<std::string>());
	void transmit(const Connection&);
	
	char command, ack;
	std::vector<std::string> stringargs;
	std::vector<int> intargs;
		
	private:
	int readNbr(const Connection&);
	void writeNbr(const Connection&, int);
	std::string readString(const Connection&/*,int*/);
	void writeString(const Connection&,std::string);
};
#endif
