
#include "NewsGroup.h"
#include <string>
#include <vector>


using namespace std;


NewsGroup::NewsGroup(string n): name(n) {}

string NewsGroup::get_name() const { return name; }

//unsigned int NewsGroup::getId() const { return id; }

Article NewsGroup::get_article(unsigned int nbr) const {
    return articles.at(nbr);
    
    /*for (size_t k = 0; k < articles.size(); ++k) {
        if (articles.at(k).getId() == nbr) {
            return articles.at(k);
        }
    }
    return nullptr;*/
}

<<<<<<< HEAD
vector<Article> NewsGroup::get_articles() const {
  //kvar att implementera, gör en dummy för att det ska kompilera
  return vector<Article>();
=======
map<int,Article> get_articles() const {
    
    return articles;
>>>>>>> 5848a3ce469c49c51727f2281a60bbeb911839e2
    
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


