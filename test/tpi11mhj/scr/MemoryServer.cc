#include "MemoryDatabase.h"
#include "connection.h"
#include "message.h"
#include "server.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace std;
using p = Protocol;

int main(int argc, char* argv[]){
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
    
    MemoryDatabase database = MemoryDatabase();
    
    while (true) {
        auto conn = server.waitForActivity();
        if (conn != nullptr) {
            vector<int> intargs = vector<int>();
            vector<string> stringargs = vector<string>();
            std::cout << "Message recieved!" << std::endl;
            try {
                //std::cout<< "hejhej" <<endl;
                Message message=Message(*conn);
                std::cout << "meddelande inläst" << std::endl;
                
                switch (message.command){
                    case p::COM_LIST_NG:
                        std::cout << "Servern fattar att det är ett list!" << endl;
                        //skapa en lista på lämpligt vis, skapa ett message av detta, transmitta det.
                        for(auto s = database.cbegin(); s != database.cend(); s++){
                            intargs.push_back(s->first);
                            stringargs.push_back(s->second.get_name());
                            std::cout << "nu loopar vi genom databasen" << std::endl;
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(200));
                        Message(p::ANS_LIST_NG, 0, intargs, stringargs).transmit(*conn);
                        break;
                        
                        
                        
                    case p::COM_CREATE_NG:
                        if(database.add_newsgroup(message.stringargs[0])){
                            std::cout << "lyckades skapa grupp" << std::endl;			      	    Message(p::ANS_CREATE_NG,p::ANS_ACK).transmit(*conn);
                        }else{
                            std::cout << "lyckades inte skapa grupp" << std::endl;
                            intargs.push_back(p::ERR_NG_ALREADY_EXISTS);
                            Message(p::ANS_CREATE_NG, p::ANS_NAK, intargs).transmit(*conn);
                        }
                        std::cout << "lyckades skicka meddelande" << std::endl;
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
                            int count = 0;
                            for(auto article : database.get_newsgroup(message.intargs[0]).second.get_articles()){
                                intargs.push_back(count);
                                stringargs.push_back(article.getTitle());
                                stringargs.push_back(article.getAuthor());
                                stringargs.push_back(article.getText());
                                ++count;
                            }
                            Message(p::ANS_LIST_ART, p::ANS_ACK, intargs,stringargs).transmit(*conn);
                        } else{
                            intargs.push_back(p::ERR_NG_DOES_NOT_EXIST);
                            Message(p::ANS_LIST_ART, p::ANS_NAK, intargs).transmit(*conn);					
                        }
                        break;
                        
                        
                        
                        
                    case p::COM_CREATE_ART:
                        //Var skapas artiklar?
                        
                        
                        
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
                            Article a = database.get_newsgroup(message.intargs[0]).second.get_article(message.intargs[1]).second;
                            stringargs.push_back(a.getTitle());
                            stringargs.push_back(a.getAuthor());
                            stringargs.push_back(a.getText());
                        }
                        break;
                }
                
                
                
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }
            cout << "slut!" << std::endl;
            conn = nullptr;	
        } else {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }		
}
  
