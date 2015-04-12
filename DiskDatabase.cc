#include "DiskDatabase.h"
#include <algorithm>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <utility>

//constructor

using namespace std;

DiskDatabase::DiskDatabase(){
    auto dir = opendir(root.c_str());
    groupnbr = 0;
    if (dir == nullptr) {
        mkdir(root.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        auto entry = readdir(dir);
        while (entry != nullptr) {
            try {
                string id = entry->d_name;
                //cout<<id<<endl;
                auto pos = id.find_first_of(" ");
                int id_nbr = stoi(id.substr(0,pos));
                if (id_nbr > groupnbr) {
                    groupnbr = id_nbr;
                }
                
            } catch (invalid_argument e) {
                cerr << "Something is wrong with the folders" << endl;
            }
            entry = readdir(dir);
        }
        closedir(dir);
    }
    ++groupnbr;
    
}

 //check if newsgroup is unique, then add to db implementation. Return success.
bool DiskDatabase::add_newsgroup(NewsGroup ng) {
    string path = root + "/";
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            auto pos = id.find_first_of(" ");
            string name = id.substr(pos+1,string::npos);
            
            if (name == ng.get_name()) {
                return false;
            }
        } catch (invalid_argument e) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry.readdir();
    }
    closedir(dir);
    string new_path = path + to_string(groupnbr++) + " " + ng.get_name();
    mkdir(new_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return true;
}

//check if newsgroup exists, then delete. Return success.
bool DiskDatabase::delete_newsgroup(unsigned int id_nbr){
    string path = root + "/";
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            auto pos = id.find_first_of(" ");
            string ng_id = id.substr(0,pos);
            if (ng_id == id_nbr) {
                return remove(path + id);
            }
        } catch (invalid_argument e) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return false;
}

//get reference to group for listing of articles. Read only.
//returns nullptr if group doesn't exists
NewsGroup& get_newsgroup(unsigned int id_nbr) const{
    string path = root + "/";
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        return nullptr;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            auto pos = id.find_first_of(" ");
            int ng_id = stoi(id.substr(0,pos));
            if (ng_id == id_nbr) {
                string ng_name = id.substr(pos+1,string::npos);
                return NewsGroup(ng_name);
            }
        } catch (invalid_argument else) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return nullptr;
}

//Adds Article to group nbr int. Return success.
bool DiskDataBase::add_article(unsigned int nbr, Article a) {
    NewsGroup ng = get_newsgroup(nbr);
    if (ng == nullptr) {
        return false;
    }
    string path = root + "/"+ toString(nbr) + ng.get_name();
    unsigned int nbr_of_a = 0;
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        ++nbr_of_a;
        entry = readdir(dir);
    }
    closedir(dir);
    ofstream file(path + toString(++nbr_of_a) + a.getTitle() + ".txt");
    file << a.getAuthor() << '\n';
    file << a.getText();
    file.close;
    return true;
    
}

//Delete article
bool delete_article(unsigned int ng_nbr, unsigned int a_nbr) {
    NewsGroup ng = get_newsgroup(ng_nbr);
    if (ng == nullptr) {
        return false;
    }
    string ng_path = root + "/"+ toString(ng_nbr) + ng.get_name();
    auto dir = opendir(ng_path.c_str());
    if (dir == nullptr) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            auto pos = id.find_first_of(" ");
            int a_id = stoi(id.substr(0.pos));
            if (a_id == a_nbr) {
                return remove((ng_path + id).c_str()) == 0;
            }
        } catch (invalid_argument e) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return false;
    
}





