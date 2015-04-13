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
    cout << "Command Recieved!: " << static_cast<int>(command) << endl;
    char ch = 0;
    while(ch != Protocol::COM_END && ch !=  Protocol::ANS_END){
        cout << "a" << endl;
        ch = conn.read();
        cout << "b" << endl;
        if(ch == Protocol::PAR_STRING){
            cout << "c" << endl;
            stringargs.push_back(readString(conn));
            cout << "d" << endl;
        }else if(ch == Protocol::PAR_NUM){
            cout << "e" << endl;
            intargs.push_back(readNbr(conn));
            cout << "f" << endl;
        }else if(ch==Protocol::ANS_NAK){
            cout << "g" << endl;
            ack = ch;
            intargs.push_back(conn.read());
            cout << "h" << endl;
        }else if(ch==Protocol::ANS_ACK){
            cout << "i" << endl;
            ack = ch;
        }
    }
}

Message::Message(char newcommand, char newack, vector<int> ints, vector<string> strs): 	command(newcommand),ack(newack),intargs(ints),stringargs(strs){}

void Message::transmit(Connection& conn){
    //send the command
    cout << "1.here" << endl;
    conn.write(command);
    //cout << "New message created by server, command =" << static_cast<int>(command) << std::endl;
    cout << "2.here" << endl;
    cout << command << endl;
    
    if (command >= Protocol::ANS_LIST_NG && command <= Protocol::ANS_GET_ART){
        cout << "here" << endl;
        switch(command){
            case Protocol::ANS_LIST_NG:
                writeNbr(conn,intargs.size());
                for(unsigned int i = 0; i != intargs.size(); i++){
                    
                    writeNbr(conn,intargs[i]);
                    writeString(conn,stringargs[i]);
                }
                std::cout << "Lista skickad" << std::endl;
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
        //std::cout << "svar färdigskickat" << std::endl;
    }
    
    
    
    if(command >= Protocol::COM_LIST_NG && command <= Protocol::COM_GET_ART){
        //std::cout << "NU FÖRSÖKER SERVERN SKICKA ETT KOMMANDO!" << std::endl;
        cout << "4.here" << endl;
        switch (command){
                // COM_LIST_NG needs no parameters
            case Protocol::COM_CREATE_NG:
                cout << "3.here" << endl;
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
        //std::cout << "Kommando  färdigskickat" << std::endl;
    }
}



//transmit an int over connection conn
void Message::writeNbr(const Connection& conn, int value) {
    conn.write(Protocol::PAR_NUM);
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)	 & 0xFF);
    conn.write(value & 0xFF);
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
    cout << "c.1" << endl;
    int stringlength = readNbr(conn);
    cout << "c.2" << endl;
    string s;
    for(int i = 0; i < stringlength; i++){
        cout << "c.3" << endl;
        s += conn.read();
        cout << "c.4" << endl;
    }
    cout << "c.5" << endl;
    //std::cout << "stringlength: " << static_cast<int>(stringlength) << "sträng läst från connection: " << s << "." << std::endl;
    return s;
}

//write a string to connection conn
void Message::writeString(const Connection& conn,std::string s){
    cout << "5.here" << endl;
    conn.write(Protocol::PAR_STRING);
    cout << "6.here" << endl;
    conn.write(s.length());
    cout << "7.here" << endl;
    for(char c : s){
        conn.write(c);
    }
    cout << "8.here" << endl;
    //std::cout << "s" << s;
}