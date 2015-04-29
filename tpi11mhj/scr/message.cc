#include "message.h"
#include "connection.h"
#include "protocol.h"
#include <iostream>

using std::cout;
using std::string;
using std::vector;
using std::endl;

/*
 Connection constructor reads a message from the connection.
 The first byte is stored separately, while the rest are sorted into vectors
 according to type.
 
 */
Message::Message(){};
Message::Message(Connection& conn){
    command = conn.read();
    char ch = 0;
    while(ch != Protocol::COM_END && ch !=  Protocol::ANS_END){
        ch = conn.read();
        if(ch == Protocol::PAR_STRING){
            stringargs.push_back(readString(conn));
        }else if(ch == Protocol::PAR_NUM){
            intargs.push_back(readNbr(conn));
        }else if(ch==Protocol::ANS_NAK){
            ack = ch;
            intargs.push_back(conn.read());
        }else if(ch==Protocol::ANS_ACK){
            ack = ch;
        }
    }
}

Message::Message(char newcommand, char newack, vector<int> ints, vector<string> strs): 	command(newcommand),ack(newack),intargs(ints),stringargs(strs){}

void Message::transmit(Connection& conn){
    //send the command
    conn.write(command);
    cout << command << endl;
    
    if (command >= Protocol::ANS_LIST_NG && command <= Protocol::ANS_GET_ART){
        switch(command){
            case Protocol::ANS_LIST_NG:
                writeNbr(conn,intargs.size());
                for(unsigned int i = 0; i != intargs.size(); i++){
                    
                    writeNbr(conn,intargs[i]);
                    writeString(conn,stringargs[i]);
                }
                conn.write(Protocol::ANS_END);
                break;
                
            case Protocol::ANS_CREATE_NG: case Protocol::ANS_DELETE_NG: case Protocol::ANS_CREATE_ART: case Protocol::ANS_DELETE_ART:
                conn.write(ack);
                if(ack == Protocol::ANS_NAK){
                    conn.write(intargs[0]);
                }
                conn.write(Protocol::ANS_END);
                break;
                
            case Protocol::ANS_LIST_ART:
                conn.write(ack);
                if(ack==Protocol::ANS_ACK){
                    writeNbr(conn, intargs.size());
                    for(unsigned int i = 0; i != intargs.size(); i++){
                        writeNbr(conn,intargs[i]);
                        writeString(conn,stringargs[i]);
                    }
                } else {
                    conn.write(intargs[0]);
                }
                conn.write(Protocol::ANS_END);
                break;
                
            case Protocol::ANS_GET_ART:
                conn.write(ack);
                if(ack == Protocol::ANS_ACK){
                    for(auto s : stringargs){
                        writeString(conn,s);
                    }
                }else{
                    conn.write(intargs[0]);
                }
                conn.write(Protocol::ANS_END);
                
        }
    }
    
    
    
    if(command >= Protocol::COM_LIST_NG && command <= Protocol::COM_GET_ART){
        switch (command){
                // COM_LIST_NG needs no parameters
            case Protocol::COM_CREATE_NG:
                writeString(conn,stringargs[0]);
                break;
            case Protocol::COM_DELETE_NG:
                writeNbr(conn,intargs[0]);
                break;
            case Protocol::COM_LIST_ART:
                writeNbr(conn,intargs[0]);
                break;
            case Protocol::COM_CREATE_ART:
                writeNbr(conn,intargs[0]);
                for(auto s : stringargs){
                    writeString(conn, s);
                }
                break;
            case Protocol::COM_DELETE_ART: case Protocol::COM_GET_ART:
                for(auto i : intargs){
                    writeNbr(conn,i);
                }
                break;
        }
        conn.write(Protocol::COM_END);
    }
}



//transmit an int over connection conn
void Message::writeNbr(const Connection& conn, int value) {
    conn.write(Protocol::PAR_NUM);
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)	 & 0xFF);
    conn.write(value & 0xFF);
    /*cout << " writenbr Complete" << endl;*/
}

//read an int from connection conn
int Message::readNbr(const Connection& conn) {
    int value =conn.read() << 8;
    value = (value | conn.read()) << 8;
    value = (value | conn.read()) << 8; 	
    value = (value | conn.read());
    return value;
}

//read a string of defined length from connection conn
string Message::readString(const Connection& conn) {
    int stringlength = readNbr(conn);
    string s;
    /*cout << "stringlength: " << stringlength << endl;*/
    for(int i = 0; i < stringlength; i++){
        s += conn.read();
        /*cout << "Conn.read nbr: " << i << endl;*/
    }
    return s;
}

//write a string to connection conn
void Message::writeString(const Connection& conn,std::string s){
    conn.write(Protocol::PAR_STRING);
    auto value = s.length();
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)	 & 0xFF);
    conn.write(value & 0xFF);
    for(char c : s){
        conn.write(c);
    }
}