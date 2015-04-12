#ifndef MEMORYDATABASE_H
#define	MEMORYDATABASE_H

#include <iostream>
#include <set>
#include "Database.h"

class MemoryDatabase : public Database{
public:
	//constructor
	MemoryDatabase();
	
	//Returns const iterators.
	std::map<unsigned int,NewsGroup>::const_iterator  cbegin(){return database.cbegin();};
	std::map<unsigned int,NewsGroup>::const_iterator cend() {return database.cend();};
		
	//check if newsgroup is unique, then add to db implementation. Return success.
	bool add_newsgroup(std::string) override;
	
	//check if newsgroup exists, then delete. Return success.
	bool delete_newsgroup(unsigned int) override;
	
	//get reference to group for listing of articles. Read only.
	//returns nullptr if group doesn't exists
	const NewsGroup& get_newsgroup(unsigned int) const override ;
	
	//Adds Article to group nbr int. Return success.
	bool add_article(unsigned int, const Article&) override;
	
	//Delete article 
	bool delete_article(unsigned int, unsigned int) override ;
	
private:
	std::map<unsigned int,NewsGroup> database;
	unsigned int groupnumber;
};
#endif
