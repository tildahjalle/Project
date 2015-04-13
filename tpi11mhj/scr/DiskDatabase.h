#ifndef DISKDATABASE_H
#define	DISKDATABASE_H

#include <iostream>
#include <fstream>
#include <string>
//#include "article.h"
//#include "newsgroup.h"
#include "Database.h"

class DiskDatabase{
public:
    //constructor
    DiskDatabase();
    
    //Returns const iterators
    //iterator cbegin(){return database.cbegin()};
    //iterator cend(){return database.cend()};
    
    //check if newsgroup is unique, then add to db implementation. Return success.
    bool add_newsgroup(std::string);
    
    //check if newsgroup exists, then delete. Return success.
    bool delete_newsgroup(unsigned int);
    
    //get reference to group for listing of articles. Read only.
    //returns nullptr if group doesn't exists
    /*const*/ std::pair<bool,NewsGroup>/*&*/ get_newsgroup(unsigned int) const;
    
    //Adds Article to group nbr int. Return success.
    bool add_article(unsigned int, const Article&);
    
    //Delete article
    bool delete_article(unsigned int, unsigned int);
    
    //List NewGroups
    std::vector<std::pair<int,std::string>> list_newsgroup();
    
    //List Articles in a specific Newsgroup
    std::vector<std::pair<int,std::string>> list_articles(unsigned int);
    
private:
    unsigned int groupnbr;
    std::string root = "database";
    
    
};

#endif