#include "DiskDatabase.h"
#include "connection.h"
#include "message.h"
#include "server.h"
#include "protocol.h"
#include <stdexcept>
#include "connectionclosedexception.h"

using namespace std;
using p = Protocol;

int main(int argc, char* argv[]){
    //skapa server med invariabel som port. felhantering!
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << endl;
        exit(1);
    }
    
    int port = -1;
    try {
        port = stoi(argv[1]);
    } catch (exception& e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(1);
    }
    
    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error." << endl;
        exit(1);
    }
    
    auto database = DiskDatabase();
    
    while (true) {
        auto conn = server.waitForActivity();
        if (conn != nullptr) {
            try {
                Message message(*conn);
                vector<int> intargs;
                vector<string> stringargs;
                vector<pair<int,string>> ngs;
                vector<pair<int,string>> arts;
                pair<bool, NewsGroup> pa;
                switch (message.command){
                    case p::COM_LIST_NG:
                        //skapa en lista på lämpligt vis, skapa ett message av detta, transmitta det.
                        ngs = database.list_newsgroup();
                        
                        for (pair<int,string> pa:ngs) {
                            intargs.push_back(pa.first);
                            stringargs.push_back(pa.second);
                        }
                        
                        /*for(auto s = database.cbegin(); s != database.cend(); s++){
                         intargs.push_back(s->first);
                         stringargs.push_back(s->second.get_name());
                         }*/
                        Message(p::ANS_LIST_NG, 0, intargs, stringargs).transmit(*conn);
                        break;
                    case p::COM_CREATE_NG:
                        if(database.add_newsgroup(message.stringargs[0])){
                            Message(p::ANS_CREATE_NG,p::ANS_ACK, intargs, stringargs).transmit(*conn);
                        }else{
                            intargs.push_back(p::ERR_NG_ALREADY_EXISTS);
                            Message(p::ANS_CREATE_NG, p::ANS_NAK, intargs, stringargs).transmit(*conn);
                        }
                        break;
                    case p::COM_DELETE_NG:
                        if(database.delete_newsgroup(message.intargs[0])){
                            Message(p::ANS_DELETE_NG, p::ANS_ACK).transmit(*conn);
                        }else{
                            intargs.push_back(p::ERR_NG_DOES_NOT_EXIST);
                            Message(p::ANS_DELETE_NG, p::ANS_NAK, intargs).transmit(*conn);
                        }
                        break;
                    case p::COM_LIST_ART:
                        if(database.get_newsgroup(message.intargs[0]).first != false){
                            arts = database.list_articles(message.intargs[0]);
                            for (pair<int,string> pa:arts) {
                                intargs.push_back(pa.first);
                                stringargs.push_back(pa.second);
                            }
                            /*for(auto article : database.get_newsgroup(message.intargs[0])){
                             intargs.push_back(article.first);
                             stringargs.push_back(article.second);
                             }*/
                            Message(p::ANS_LIST_ART, p::ANS_ACK, intargs,stringargs).transmit(*conn);
                        }else{
                            intargs.push_back(p::ERR_NG_DOES_NOT_EXIST);
                            Message(p::ANS_LIST_ART, p::ANS_NAK, intargs).transmit(*conn);
                        }
                        break;
                    case p::COM_CREATE_ART:
                        pa = database.get_newsgroup(message.intargs[0]);
                        if(pa.first == false){
                            intargs.push_back(p::ERR_NG_DOES_NOT_EXIST);
                            Message(p::ANS_CREATE_ART, p::ANS_NAK, intargs).transmit(*conn);
                        } else {
                            Article a = Article(message.stringargs[0],message.stringargs[1],message.stringargs[2]);
                            if (database.add_article(message.intargs[0], pa.second.get_name(),a)) {
                                Message(p::ANS_CREATE_ART,p::ANS_ACK).transmit(*conn);
                            } else {
                                intargs.push_back(p::ERR_ART_DOES_NOT_EXIST);
                                Message(p::ANS_CREATE_ART,p::ANS_NAK,intargs).transmit(*conn);
                            }
                        }
                        //Var skapas artiklar?
                        break;
                    case p::COM_DELETE_ART:
                        if(database.get_newsgroup(message.intargs[0]).first == false){
                            intargs.push_back(p::ERR_NG_DOES_NOT_EXIST);
                            Message(p::ANS_DELETE_ART, p::ANS_NAK, intargs).transmit(*conn);
                        }else if(database.delete_article(message.intargs[0],message.intargs[1]) == false){
                            intargs.push_back(p::ERR_ART_DOES_NOT_EXIST);
                            Message(p::ANS_DELETE_ART, p::ANS_NAK, intargs).transmit(*conn);	
                        }else{
                            Message(p::ANS_DELETE_ART,p::ANS_ACK).transmit(*conn);
                        }
                        break;
                    case p::COM_GET_ART:
                        //Group does not exist
                        if(database.get_newsgroup(message.intargs[0]).first == false){
                            intargs.push_back(p::ERR_NG_DOES_NOT_EXIST);
                            Message(p::ANS_GET_ART, p::ANS_NAK, intargs).transmit(*conn);
                            //Article does not exist
                        }else if(database.get_newsgroup(message.intargs[0]).second.get_article(message.intargs[1]).first==false){
                            intargs.push_back(p::ERR_ART_DOES_NOT_EXIST);
                            Message(p::ANS_GET_ART, p::ANS_NAK, intargs).transmit(*conn);
                            //All is well
                        } else{
                            Article a = ((((database.get_newsgroup(message.intargs[0])).second).get_article)(message.intargs[1])).second;
                            stringargs.push_back(a.getTitle());
                            stringargs.push_back(a.getAuthor());
                            stringargs.push_back(a.getText());
                            Message(p::ANS_GET_ART, p::ANS_ACK, intargs,stringargs).transmit(*conn);
                        }
                        
                        break;
                }
                
                
                
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }	
        } else {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }		
}
  
