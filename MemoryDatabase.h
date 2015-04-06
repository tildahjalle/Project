#ifndef MEMORYDATABASE_H
#define	MEMORYDATABASE_H

#include <iostream>
#include <set>
#include "database.h"

class MemoryDatabase : public Database{
public:
	//constructor
	MemoryDatabase();
	
	//Returns const iterators.
	iterator cbegin(){return database.cbegin()};
	iterator cend() {return database.cend()};
		
	//check if newsgroup is unique, then add to db implementation. Return success.
	bool add_newsgroup(string) override;
	
	//check if newsgroup exists, then delete. Return success.
	bool delete_newsgroup(unsigned int) override;
	
	//get reference to group for listing of articles. Read only.
	//returns nullptr if group doesn't exists
	Newsgroup& get_newsgroup(unsigned int) const override ;
	
	//Adds Article to group nbr int. Return success.
	bool add_article(unsigned int, Article) override;
	
	//Delete article 
	bool delete_article(unsigned int, unsigned int) override ;
	
private:
	std::map<unsigned int,Newsgroup> database;
	unsigned int groupnumber;
}
#endif