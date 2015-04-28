#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

class MessageHandler{
	public:
		MessageHandler(std::shared_ptr<Connection>&);
		int readCode();
		int readInt();
		int readIntPar();
		std::string readStringPar();	
		void sendCode(int);
		void sendInt(int);
		void sendIntPar(int);
		void sendStringPar(std::string s);


	private:
		std::shared_ptr<Connection>& conn;
		unsigned char readByte();	
		
		void sendByte(unsigned char);
};



#endif
