#include "Memorydatabase.h"
#include "connection.h"
#include "message.h"
#include "server.h"
#include "protocol.h"

using namespace Protocol;

int main(int argc char* argv[]){
	//skapa server med invariabel som port. felhantering!
	auto database = Memorydatabase();
	
	while (true) {
		auto conn = server.waitForActivity();
		Message message;
		if (conn != nullptr) {
			try {
			message = Message(conn);
			vector<int> intargs;
			vector<string> stringargs;
			switch (message->command){
			case COM_LIST_NG:
				//skapa en lista på lämpligt vis, skapa ett message av detta, transmitta det.
				for(auto s : database){
					intargs.push_back(s.first);
					stringargs.push_back(s.second);
				}
				Message(ANS_LIST_NG, intargs, stringargs).transmit(conn);
				break;
			case COM_CREATE_NG:
				if(database.add_newsgroup(Newsgroup(message->stringargs[0]))){
					Message(ANS_CREATE_NG,ANS_ACK).transmit(conn);
				}else{
					intargs.push_back(ERR_NG_ALREADY_EXISTS);
					Message(ANS_CREATE_NG, ANS_NAK, intargs).transmit(conn);
				}
				break;
			case COM_DELETE_NG:
				if(database.delete_newsgroup(message->intargs[0]){
					Message(ANS_DELETE_NG, ANS_ACK).transmit(conn);
					}else{
						intargs.push_back(ERR_NG_DOES_NOT_EXISTS);
						Message(ANS_DELETE_NG, ANS_NAK, intargs).transmit(conn);
					}
				break;
			case COM_LIST_ART
				if(database.get_newsgroup(message_intargs[0]) != nullptr){
					for(auto article : database.get_newsgroup(message->intargs[0]){
						intargs.push_back(article.first);
						stringargs.push_back(article.second);
					}
					Message(ANS_LIST_ART, ANS_ACK, intargs,stringargs).transmit();
				} else{
					intargs.push_back(ERR_NG_DOES_NOT_EXIST);
					Message(ANS_LIST_ART, ANS_NAK, intargs).transmit(conn);					
				}
				break;
				
			case COM_CREATE_ART:
				//Var skapas artiklar?
			case COM_GET_ART:
				//Group does not exist
				if(database.get_newsgroup(message_intargs[0]) == nullptr){
					intargs.push_back(ERR_NG_DOES_NOT_EXIST);
					Message(ANS_GET_ART, ANS_NAK, intargs).transmit(conn);	
				//Article does not exist
				}else if(database.get_newsgroup(message_intargs[0]).get_article(message.intargs[1])==nullptr){
					intargs.push_back(ERR_ART_DOES_NOT_EXIST);
					Message(ANS_GET_ART, ANS_NAK, intargs).transmit(conn);	
				//All is well
				} else{
					Article a = database.get_newsgroup(message_intargs[0]).get_article(message.intargs[1]);
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
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}		
}