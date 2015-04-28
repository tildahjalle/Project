#include "MemoryDatabase.h"
#include <string>
#include <vector>


//constructor

MemoryDatabase::MemoryDatabase() : groupnumber(0){};

bool MemoryDatabase::add_newsgroup(std::string title){
    for (auto p:database) {
        if (p.second.get_name() == title) {
            return false;
        }
    }
    groupnumber++;
    return ((database.emplace(groupnumber,NewsGroup(title))).second);
}

bool MemoryDatabase::delete_newsgroup(unsigned int groupnbr){
    return database.erase(groupnbr) == 1;
}

std::pair<bool,NewsGroup> MemoryDatabase::get_newsgroup(unsigned int groupnbr) const{
  std::pair<bool,NewsGroup> p;
  if (database.find(groupnbr) != database.end()) {
    p = std::pair<bool,NewsGroup>(true,database.at(groupnbr));
  } else{
    p = std::pair<bool,NewsGroup>(false,NewsGroup());
  }
  return p;
}

bool MemoryDatabase::add_article(unsigned int groupnbr, const Article& a){
    return database.at(groupnbr).add_article(a);
}

bool MemoryDatabase::delete_article(unsigned int groupnbr, unsigned int articlenbr){
    return database.at(groupnbr).delete_article(articlenbr);
}
