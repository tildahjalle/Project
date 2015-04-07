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
			switch (message->command)
			case COM_LIST_NG:
				//skapa en lista på lämpligt vis, skapa ett message av detta, transmitta det.		
			case COM_CREATE_NG
			case COM_DELETE_NG
			case LIST_ART
			case CREATE_ART
			case GET_ART
			


			
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