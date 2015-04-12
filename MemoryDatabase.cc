#include "MemoryDatabase.h"


//constructor

MemoryDatabase::MemoryDatabase() : groupnumber(0){};

bool MemoryDatabase::add_newsgroup(std::string title){
	groupnumber++;
	return ((database.emplace(groupnumber,NewsGroup(title))).second);	
}

bool MemoryDatabase::delete_newsgroup(unsigned int groupnbr){
	return database.erase(groupnbr) == 1;
}

const NewsGroup& MemoryDatabase::get_newsgroup(unsigned int groupnbr) const{
	return database.at(groupnbr);
}

bool MemoryDatabase::add_article(unsigned int groupnbr, const Article& a){
	return database.at(groupnbr).add_article(a);
}

bool MemoryDatabase::delete_article(unsigned int groupnbr, unsigned int articlenbr){
	return database.at(groupnbr).delete_article(articlenbr);
}
