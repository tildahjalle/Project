#ifndef MESSAGE_H
#define MESSAGE_H

#include "connection.h"
class Message{
public:
//connection (message reading) constructor
	Message(const Connection&);
	//command list constructor (message writing)
	Message(char, std::vector<int> = nullptr, std::vector<std::String> = nullptr);
	void transmit(const Connection&);
	
	private:
	char command ack;
	std::vector<std::String> stringargs;
	std::vector<int> intargs;
	int readNbr(const Connection&);
	void writeNbr(const Connection&, int);
	std::String readString(const Connection&,int);
	void writeString(const Connection&,std::string);
}