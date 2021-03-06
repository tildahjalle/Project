
#include "NewsGroup.h"
#include <string>
#include <vector>


using namespace std;

NewsGroup::NewsGroup() {};
NewsGroup::NewsGroup(string n): name(n) {}

string NewsGroup::get_name() const { return name; }


pair<bool,Article> NewsGroup::get_article(unsigned int nbr) const {
  pair<bool,Article> p;
  if (articles.find(nbr) != articles.end()) {
    p.first = true;
    p.second = articles.at(nbr);
  }else{
    p.first = false;
}

  return p;
  
}

map<int,Article> NewsGroup::get_articles() const {
  return articles;   
}

bool NewsGroup::add_article(Article a) {
  bool status =articles.emplace(count,a).second;
  count += 1;
  return status;
}

bool NewsGroup::delete_article(unsigned int nbr){
    return articles.erase(nbr) == 1;
    return false;
}

bool NewsGroup::add_article(unsigned int id, Article a){
    return articles.emplace(id,a).second;
}


