#ifndef DATABASE_H 
#define	DATABASE_H

#include <iostream>
#include <ifstream>
#include <string>
#include "article.h"
#include "newsgroup.h"

class Database{
public:
	//constructor
	Database();
	
	//Returns const iterators.
	iterator cbegin() virtual;
	iterator cend() virtual;
		
	//check if newsgroup is unique, then add to db implementation. Return success.
	bool add_newsgroup(Newsgroup) virtual;
	
	//check if newsgroup exists, then delete. Return success.
	bool delete_newsgroup(unsigned int) virtual;
	
	//get reference to group for listing of articles. Read only.
	//returns nullptr if group doesn't exists
	Newsgroup& get_newsgroup(unsigned int) const virtual;
	
	//Adds Article to group nbr int. Return success.
	bool add_article(unsigned int, Article) virtual;
	
	//Delete article 
	bool delete_article(unsigned int, unsigned int) virtual;
	}
#endif