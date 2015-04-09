#ifndef MESSAGE_H
#define MESSAGE_H

#include "connection.h"
class Message{
public:
//connection (message reading) constructor
	Message(Connection);
	//command list constructor (message writing)
	Message(int[]);
	void transmit(Connection);
	
	private:
	int command;
	vector<string> stringargs;
	vector<int> intargs;
}