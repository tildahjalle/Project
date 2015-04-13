#ifndef MEMORYDATABASE_H
#define	MEMORYDATABASE_H

#include <iostream>
#include <set>
#include "Database.h"

class MemoryDatabase{
public:
    //constructor
    MemoryDatabase();
    
    //Returns const iterators.
    std::map<unsigned int,NewsGroup>::const_iterator  cbegin(){return database.cbegin();};
    std::map<unsigned int,NewsGroup>::const_iterator cend() {return database.cend();};
    
    //check if newsgroup is unique, then add to db implementation. Return success.
    bool add_newsgroup(std::string) ;
    
    //check if newsgroup exists, then delete. Return success.
    bool delete_newsgroup(unsigned int) ;
    
    //get reference to group for listing of articles. Read only.
    //returns nullptr if group doesn't exists
    /*const*/ std::pair<bool,NewsGroup>/*&*/ get_newsgroup(unsigned int) const  ;
    
    //Adds Article to group nbr int. Return success.
    bool add_article(unsigned int, const Article&) ;
    
    //Delete article
    bool delete_article(unsigned int, unsigned int)  ;
    
private:
    std::map<unsigned int,NewsGroup> database;
    unsigned int groupnumber;
};
#endif

