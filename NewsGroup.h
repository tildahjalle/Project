
#ifndef ____NewsGroup__
#define ____NewsGroup__

#include <stdio.h>
#include "Article.h"

class NewsGroup {

public:
    NewsGroup(std::string, unsigned int);
    
    std::string getName() const;
    unsigned int getId() const;
    
    Article getArticle(unsigned int) const;
    std::vector<Article> getArticles() const;
    bool createArticle(std::string, std::string, std::string);
    bool deleteArticle(unsigned int);
    
private:
    std::vector<Article> articles;
    std::string name;
    unsigned int id;
    unsigned int count;
    
    





#endif /* defined(____NewsGroup__) */


