
#ifndef ____NewsGroup__
#define ____NewsGroup__

#include <string>
#include <map>
#include "Article.h"
#include <vector>

class NewsGroup {

public:
    NewsGroup();
    NewsGroup(std::string);
    
    std::string get_name() const;
    //unsigned int getId() const;
    
    std::pair<bool,Article> get_article(unsigned int) const;
    std::vector<Article> get_articles() const;
    bool add_article(Article a);
    bool delete_article(unsigned int);
    bool add_article(unsigned int,Article);
    
private:
    std::map<int,Article> articles;
    std::string name;
    //unsigned int id;
    unsigned int count;
    
};

#endif /* defined(____NewsGroup__) */


