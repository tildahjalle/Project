#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

/*
 * Send an integer to the server as four bytes.
 */
void writeNumber(const Connection& conn, int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

/*
 * Read a string from the server.
 */
string readString(const Connection& conn) {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
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
	
	cout << "Welcome, you can use commands to list, create or delete newsgroups. Write exit to terminate program.";
	//Om man väljer ett, jämföra strängar, cin >> choice if(choice == blabla) gör det är alternativet. 
	
	string str;
	while(cin >> str){
	  try{
	    if(str == "list"){
            Message(Protocol::COM_LIST_NG).transmit(*conn); //hur löser man det i message
	      //lista alla newsgroups
	      cout << "To create, delete and read articles, list the articles in the newsgroup you wish to alter. Write exit to terminate the program.";
	      cin >> str;
	      if(str == "list"){
              cin >> str;
              Message(Protocol::COM_LIST_ART,str).transmit(*conn);
              
		cin >> str;
		//lista alla articles i str newsgroup. Kolla om newsgroup finns annars exception..
		string ngroup = str; 
		while(cin >> str){
		  if(str == "create"){

		  }else if(str == "delete"){
		  }else if(str == "read" ){
		  }else if(str == "exit"){
		  }else{
		    //exception finns inget sådant kommando, terminate.
		  }
		}
	      }else if(str == "exit"){
	      }
	      //här måste man också kunna lista artiklar i en specifik newsgroup. När man har gjort det kan man create, delete och read artiklar. Måste därför spara undan vilken newsgroup det är.
	    }else if (str == "create"){
	      
	      
	    }else if (str == "delete"){

	    }else if (str == "exit"){
	      
	      
	    }else{
	      //kan inte använda det kommandot här, skriv ut en hjälplista eller liknande..
	    }
	  }catch(){
	  }
	}

	int nbr;
	while (cin >> nbr) {
		try {
			cout << nbr << " is ...";
			writeNumber(conn, nbr);
			string reply = readString(conn);
			cout << " " << reply << endl;
			cout << "Type another number: ";
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}
