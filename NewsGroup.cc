
#include "NewsGroup.h"
#include <string>
#include <vector>


using namespace std;

NewsGroup::NewsGroup() {};
NewsGroup::NewsGroup(string n): name(n) {}

string NewsGroup::get_name() const { return name; }

//unsigned int NewsGroup::getId() const { return id; }

pair<bool,Article> NewsGroup::get_article(unsigned int nbr) const {
    pair<bool,Article> p;
    p.first = false;
    if (articles.find(nbr) != articles.end()) {
        p.first = true;
    }
    p.second = articles.at(nbr);
    return p;
    
    /*for (size_t k = 0; k < articles.size(); ++k) {
        if (articles.at(k).getId() == nbr) {
            return articles.at(k);
        }
    }
    return nullptr;*/
}

vector<Article> NewsGroup::get_articles() const {
  //kvar att implementera, gör en dummy för att det ska kompilera
  return vector<Article>();   
}

bool NewsGroup::add_article(Article a) {
    // when will this return false?
    //articles.push_back(a);
    articles.emplace(count,a);
    count += 1;
    return true;
}

bool NewsGroup::delete_article(unsigned int nbr){
    return articles.erase(nbr) == 1;
    
    /*for (size_t k = 0; k < articles.size(); ++k) {
        if (articles.at(k).getId() == nbr) {
            articles.erase(k);
            return true;
        }
    }*/
    return false;
}


