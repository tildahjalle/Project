
#ifndef ____Article__
#define ____Article__

#include <stdio.h>

class Article {

public:
    Article();
    Article(std::string, std:string, std::string, unsigned int) const;
    
    std::string getAuthor() const;
    std::string getTitle() const;
    std::string getText() const;
    unsigned int getId() const;
    
    
private:
    std::string author;
    std::string title;
    std::string text;
    unsigned int id;
    
};



#endif /* defined(____Article__) */
