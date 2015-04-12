#ifndef DATABASE_H 
#define	DATABASE_H

#include <iostream>
#include <fstream>
#include <string>
#include "Article.h"
#include "NewsGroup.h"

class Database{
public:
	//constructor
	Database();
	
	//Returns const iterators.
	//std::iterator cbegin() virtual;
	//std::iterator cend() virtual;
		
	//check if newsgroup is unique, then add to db implementation. Return success.
	virtual	bool add_newsgroup(std::string); 
	
	//check if newsgroup exists, then delete. Return success.
	virtual bool delete_newsgroup(unsigned int);
	
	//get reference to group for listing of articles. Read only.
	//returns nullptr if group doesn't exists
	virtual const NewsGroup& get_newsgroup(unsigned int) const;
	
	//Adds Article to group nbr int. Return success.
	virtual bool add_article(unsigned int, const Article&);
	
	//Delete article 
	virtual bool delete_article(unsigned int, unsigned int);
};
#endif
