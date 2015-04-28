#include "MessageHandler.h"
#include <string>

using namespace std;
Protocol protocol;
MessageHandler::MessageHandler(shared_ptr<Connection>& connection) : conn(connection){
	
}

/*
 * READING
 */

//PRIVATE
unsigned char MessageHandler::readByte(){
	try {
		unsigned char byte = conn->read();
		return byte;
	}catch(exception& e) {
		throw ConnectionClosedException();
 		return -1;
	}
}

//PUBLIC
int MessageHandler::readCode() {
	int code = readByte();
	return code;
}

int MessageHandler::readInt() {
	int intRec = 0;
	int b = readByte();
	intRec = intRec | b;
	b = readByte();
	intRec = (intRec << 8) | b;
	b = readByte();
	intRec = (intRec << 8) | b;
	b = readByte();
	intRec = (intRec << 8) | b;

	return intRec;
	
}

int MessageHandler::readIntPar() {
  int code = readCode();
  if(code != protocol.PAR_NUM)
    throw ConnectionClosedException(); // Another form of error handling maybe?
  return readInt();
}

string MessageHandler::readStringPar() {
  int code = readCode();
  string s = "";
  if(code != protocol.PAR_STRING) {
    //Not a string. error handling?
    throw ConnectionClosedException();
  }else {
    int n = readInt();
    for(int i = 0; i < n; ++i) {
      s += readByte();
    }
  }
  return s;
}

/*
 * SENDING
 */



//PRIVATE
void MessageHandler::sendByte(unsigned char byte){

	try {
		conn->write(byte);
	} catch(exception& e) {
		throw ConnectionClosedException();
	}
}

//PUBLIC
void MessageHandler::sendCode(int code) {
	unsigned char c = code;
	sendByte(c);
}

void MessageHandler::sendInt(int i) {
	unsigned char c = (i >> 24);
	sendByte(c);
	c = i >> 16;
	sendByte(c);
	c = i >> 8;
	sendByte(c);
	c = i;
	sendByte(c);
}

void MessageHandler::sendIntPar(int par) {
	
	unsigned char c = protocol.PAR_NUM;
	sendByte(c);
	sendInt(par);
}

void MessageHandler::sendStringPar(string s) {
	unsigned char c = protocol.PAR_STRING;
	sendByte(c);
	sendInt(s.size());
	for(size_t i = 0; i < s.size(); ++i) {
		sendByte(s[i]);
	}

}
