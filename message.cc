#include "message.h"
#include "connection.h"
#include "protocol.h"
#include <iostream>

using namespace protocol ;
using namespace std;

/*
Connection constructor reads a message from the connection.
The first byte is stored separately, while the rest are sorted into vectors
according to type. 

*/
Message::Message(const Connection& conn){
	command = conn.read();
	char ch = 0;
	while(ch != (COM_END || ANS_END){
		ch = conn.read();	
		if(ch == PAR_STRING){
			stringargs.push_back(readString(conn,stringlength)
		}else if(ch == PAR_NUM){
			intargs.push_back(readNbr(conn);			
		}else if(ch==ANS_NAK){
			ack = ch;
			intargs.push_back(conn.read());
		}else if(ch==ANS_ACK){
			ack = ch; 
			}
	}
}

Message::Message(char newcommand, std::vector<int> ints = nullptr, 
											vector<string> strs = nullptr) 
: 	command(newcommand)
,	intargs(ints)
,	stringargs(strs); {}
 
void Message::transmit(const Connection& conn){
	//send the command
	conn.write(command);
	
	//If this is a reply from server it's time to handle
	//all cases, yay!
	if (command >= ANS_LIST_NG && command =< ANS_GET_ART){
		switch(command){
		case ANS_LIST_NG:
			writeNbr(conn,intargs.size());
			for(int i = 0; i != intargs.size(), i++){
				writeNbr(intargs[i]);
				writeString(stringargs[i]);
			}
			break;	

		case ANS_CREATE_NG: case ANS_DELETE_NG: case ANS_CREATE_ART: case ANS_DELETE_ART:
			conn.write(ack);
			if(ack == ANS_NAK){
				conn.write(intargs[0]);
			}
			break;
						
		case ANS_LIST_ART:
			conn.write(ack);
			if(ack=ANS_ACK){
				writeNbr(conn, intargs.size());
				for(int i = 0; i != intargs.size(), i++){
					writeNbr(intargs[i]);
					writeString(stringargs[i]);
				}
			} else {
			conn.write(intargs[0]);
			}
			break;
			
		case ANS_GET_ART:
			conn.write(ack);
			if(ack == ANS_ACK){
			for(auto s : stringargs){
				writeString(s,conn);
				}	
			}else{
			conn.write(intargs[0]);			
			}
					
		}
		conn.write(ANS_END);
		}
		
		
		
	if(command >= COM_LIST_NG && command =< COM_GET_ART){
		switch (command){
		// COM_LIST_NG needs no parameters
		case COM_CREATE_NG:
			writeString(conn,stringargs[0]);
			break;
		case COM_DELETE_NG:
			writeNbr(conn,intargs[0]);
			break;
		case COM_LIST_ART:
			writeNbr(conn,intargs[0]);
			break;
		case COM_CREATE_ART:
			writeNbr(conn,intargs[0]);
			for(auto s : stringargs){
				writeString(conn, s);
			}
			break;
		case COM_DELETE_ART: case COM_GET_ART:
			for(auto i : intargs){
				writeNbr(conn,i);
			}
			break;
		}
		conn.write(COM_END)
	}	
	}
	

 
//transmit an int over connection conn
void Message::writeNbr(const Connection& conn, int value) {
	conn.write(PAR_NUM);
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

//read an int from connection conn
int Message::readNbr(const Connection& conn) {
	int value =conn.read() << 8;
	value = (value | conn.read() << 8; 
	value = (value | conn.read()) << 8; 	
	value = (value | conn.read());
	return value;
}

//read a string of defined length from connection conn
string Message::readString(const Connection& conn) {
	char stringlength = conn.read()
	string s;
	for(int i = 0, i != stringlength, i++){
	s += conn.read();
	}
	return s;
}

//write a string to connection conn
void Message::writeString(const Connection& conn,std::string s){
	conn.write(PAR_STRING);
	conn.write(s.length());
	for(char c : s){
		conn.write(c);
	}
	
}