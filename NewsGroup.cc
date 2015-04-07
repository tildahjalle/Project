
#include "NewsGroup.h"
#include <stdio.h>

using namespace std;


NewsGroup::(string n, unsigned int nbr): name(n),id(nbr) {}

string NewsGroup::getName() const { return name; }

unsigned int NewsGroup::getId() const { return id; }

Article NewsGroup::getArticle(unsigned int nbr) {
    for (size_t k = 0; k < articles.size(); ++k) {
        if (articles.at(k).getId() == nbr) {
            return articles.at(k);
        }
    }
    return nullptr;
}

vector<Article> getArticles() const {
    
    return articles;
    
}

bool NewsGroup::createArticle(string name,string title,string txt) {
    // when will this return false?
    articles.push_back(Article(name,title,txt,count));
    count += count;
    return true;
}

bool NewsGroup::deleteArticle(unsigned int nbr){
    for (size_t k = 0; k < articles.size(); ++k) {
        if (articles.at(k).getId() == nbr) {
            articles.erase(k);
            return true;
        }
    }
    return false;
}


