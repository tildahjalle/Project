#ifndef MESSAGE_H
#define MESSAGE_H

#include "connection.h"
class Message{
public:
//connection (message reading) constructor
	Message(const Connection&);
	//command list constructor (message writing)
	Message(char, char=0; std::vector<int> = nullptr, std::vector<std::String> = nullptr);
	void transmit(const Connection&);
	
	char command ack;
	std::vector<std::string> stringargs;
	std::vector<int> intargs;
		
	private:
	int readNbr(const Connection&);
	void writeNbr(const Connection&, int);
	std::string readString(const Connection&,int);
	void writeString(const Connection&,std::string);
}