#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "message.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

void help(){
    cout << "You can use the following commands:" << endl;
    cout << "List Newsgroups" << endl;
    cout << "Create Newsgroup <name>" << endl;
    cout << "Delete Newsgroup <IDnumber>" << endl;
    cout << "List Articles <IDNumber>" << endl;
    cout << "Create Article <IDNumber> <title> <author>" << endl;
    cout << "Delete Article <IDNumber> <IDNumber>" << endl;
    cout << "Read Article <IDNumber> <IDNumber>" << endl;
    cout << "Help" << endl;
    cout << "Exit" << endl;
}

int switchStr(string s){
    if (s == "List" || s == "L") {
        return 1;
    }
    if (s == "Create" || s == "C") {
        return 2;
    }
    if (s == "Delete" || s == "D") {
        return 3;
    }
    if (s == "Read" || s == "R") {
        return 4;
    }
    if (s == "Exit" || s == "E") {
        return 5;
    }
    if (s == "Help" || s == "H") {
        return 6;
    } else {
        return -1;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }
    
    int port = -1;
    try {
        port = stoi(argv[2]);
    } catch (exception& e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(1);
    }
    
    Connection conn(argv[1], port);
    if (!conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    
    cout << "Welcome, you can use the following commands:" << endl;
    cout << "List Newsgroups" << endl;
    cout << "Create Newsgroup <name>" << endl;
    cout << "Delete Newsgroup <IDnumber>" << endl;
    cout << "List Articles <IDNumber>" << endl;
    cout << "Create Article <IDNumber> <title> <author>" << endl;
    cout << "Delete Article <IDNumber> <IDNumber>" << endl;
    cout << "Read Article <IDNumber> <IDNumber>" << endl;
    cout << "Help" << endl;
    cout << "Exit" << endl;
    
    /*string line;*/
    string str;
    int arg;
    while(/*get(cin,line)*/cin >> str){
        try{
            arg = /*switchStr(line)*/ switchStr(str);
            switch(arg){
                case 1:
                {
                    cin >> str;
                    if(str == "Newsgroups" || str == "N"){
                        Message mh = Message(Protocol::COM_LIST_NG);
                        mh.transmit(conn);
                        mh = Message(conn);
                        for(unsigned int i = 0; i < mh.stringargs.size(); ++i){
                            cout << mh.intargs[i+1] << ". " << mh.stringargs[i] << endl;
                        }
                    }else if(str == "Articles" || str == "A"){
                        string IDNbr;
                        IDNbr = "-1";
                        cin >> IDNbr;
                        if(IDNbr == "-1"){
                            help();
                        }else{
                            vector<int> intargs;
                            intargs.push_back(stoi(IDNbr));
                            Message mh = Message(Protocol::COM_LIST_ART, 0, intargs);
                            mh.transmit(conn);
                            mh = Message(conn);
                            if(mh.ack == Protocol::ANS_NAK){
                                cout << "The newsgroup does not exist" << endl;
                                //}else if((mh.intargs).size() != 0/*mh.intargs[0] == Protocol::ERR_ART_DOES_NOT_EXIST*/){
                                // cout << "The article does not exist" << endl;
                            }else if(mh.ack == Protocol::ANS_ACK){
                                for(unsigned int i = 0; i < mh.intargs.size(); i+=3){
                                    cout << to_string(mh.intargs[(i/3)]) + "." + " " + mh.stringargs[i] + " From: " + mh.stringargs[i+1] << endl;
                                }
                            }else{
                                help();
                            }
                        }
                    }else{
                        help();
                    }
                }
                    break;
                case 2:
                {
                    cin >> str;
                    if(str == "Newsgroup" || str == "N"){
                        string name;
                        name = "-1";
                        cin >> name;
                        if(name == "-1"){
                            help();
                        }else{
                            vector<string> stringargs;
                            vector<int> intargs;
                            stringargs.push_back(name);
                            Message mh = Message(Protocol::COM_CREATE_NG, 0,intargs, stringargs);
                            mh.transmit(conn);
                            mh = Message(conn);
                            //cout << "before segmentaionfault: " << (mh.intargs).size() << endl;
                            if((mh.intargs).size() != 0/*mh.intargs[0] == Protocol::ERR_NG_ALREADY_EXISTS*/){
                                cout << "The Newsgroup already exists." << endl;
                            }else{
                                cout << "The newsgroup has been succesfully added." << endl;
                            }
                        }
                    }else if(str == "Article" || str == "A"){
                        string IDNbr, title, author, text;
                        IDNbr = title = author = text = "-1";
                        cin >> IDNbr >> title >> author >> text;
                        if(text == "-1"){
                            help();
                        }else{
                            vector<int> intargs;
                            vector<string> stringargs;
                            stringargs.push_back(title);
                            stringargs.push_back(author);
                            stringargs.push_back(text);
                            intargs.push_back(stoi(IDNbr));
                            Message mh = Message(Protocol::COM_CREATE_ART, 0, intargs, stringargs);
                            mh.transmit(conn);
                            mh = Message(conn);
                            if((mh.intargs).size() != 0/*mh.intargs[0] == Protocol::ERR_NG_DOES_NOT_EXIST*/){
                                cout << "The Newsgroup does not exist." << endl;
                            }else{
                                cout << "The article has been succesfully added." << endl;
                            }
                        }
                        
                    }else{
                        help();
                    }
                }
                    break;
                case 3:
                {
                    cin >> str;
                    if(str == "Newsgroup" || str == "N"){
                        string IDNbr;
                        IDNbr = "-1";
                        cin >> IDNbr;
                        if(IDNbr == "-1"){
                            help();
                        }else{
                            vector<int> intargs;
                            intargs.push_back(stoi(IDNbr));
                            Message mh = Message(Protocol::COM_DELETE_NG, 0, intargs);
                            mh.transmit(conn);
                            mh = Message(conn);
                            if((mh.intargs).size() != 0/*mh.intargs[0] == Protocol::ERR_NG_DOES_NOT_EXIST*/){
                                cout << "The Newsgroup does not exist." << endl;
                            }else{
                                cout << "The newsgroup has been succesfully deleted." << endl;
                            }
                        }
                    }else if(str == "Article" || str == "A"){
                        string IDNbrN, IDNbrA;
                        IDNbrN = IDNbrA = "-1";
                        cin >> IDNbrN >> IDNbrA;
                        if(IDNbrA == "-1"){
                            help();
                        }else{
                            vector<int> intargs;
                            intargs.push_back(stoi(IDNbrN));
                            intargs.push_back(stoi(IDNbrA));
                            Message mh = Message(Protocol::COM_DELETE_ART, 0, intargs);
                            mh.transmit(conn);
                            mh = Message(conn);
                            if((mh.intargs).size() != 0){
                                if(mh.intargs[0] == Protocol::ERR_NG_DOES_NOT_EXIST){
                                    cout << "The newsgroup does not exist." << endl;
                                }else if(mh.intargs[0] == Protocol::ERR_ART_DOES_NOT_EXIST){
                                    cout << "The article does not exist." << endl;
                                }
                            }else{
                                cout << "The article has been succesfully deleted." << endl;
                            }
                        }
                    }else{
                        help();
                    }
                }
                    break;
                case 4:
                {
                    string IDNbrN, IDNbrA;
                    IDNbrN = IDNbrA = "-1";
                    cin >> IDNbrN >> IDNbrA;
                    if(IDNbrA == "-1"){
                        help();
                    }else{
                        vector<int> intargs;
                        intargs.push_back(stoi(IDNbrN));
                        intargs.push_back(stoi(IDNbrA));
                        Message mh = Message(Protocol::COM_GET_ART, 0, intargs);
                        mh.transmit(conn);
                        mh = Message(conn);
                        if((mh.intargs).size() != 0){
                            if(mh.intargs[0] == Protocol::ERR_NG_DOES_NOT_EXIST){
                                cout << "The newsgroup does not exist." << endl;
                            }else if(mh.intargs[0] == Protocol::ERR_ART_DOES_NOT_EXIST){
                                cout << "The article does not exist." << endl;
                            }
                        }else{
                            cout << mh.stringargs[0] + "   " + mh.stringargs[1] << endl;
                            cout << mh.stringargs[2] << endl;
                        }
                    }
                }
                    break;
                case 5:
                {
                    exit(1);
                }
                    break;
                case 6:
                {
                    help();
                }
                    break;
                case -1:
                {
                    help();
                }
                    break;
            }
            
        }catch(ConnectionClosedException&){
            cout << " no reply from server. Exiting." << endl;
            exit(1);
        }
    }
    
}
