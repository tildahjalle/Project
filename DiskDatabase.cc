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
                unsigned int id_nbr = stoi(id.substr(0,pos));
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
bool DiskDatabase::add_newsgroup(string ng_name) {
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
            
            if (name == ng_name) {
                return false;
            }
        } catch (invalid_argument e) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    string new_path = path + to_string(groupnbr++) + " " + ng_name;
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
            unsigned int ng_id = stoi(id.substr(0,pos));
            if (ng_id == id_nbr) {
                return remove((path + id).c_str());
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
const NewsGroup& DiskDatabase::get_newsgroup(unsigned int id_nbr) const{
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
            unsigned int ng_id = stoi(id.substr(0,pos));
            if (ng_id == id_nbr) {
                string ng_name = id.substr(pos+1,string::npos);
                return NewsGroup(ng_name);
            }
        } catch (invalid_argument e) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return nullptr;
}

//Adds Article to group nbr int. Return success.
bool DiskDatabase::add_article(unsigned int nbr, const Article& a) {
    NewsGroup ng = get_newsgroup(nbr);
    if (ng == nullptr) {
        return false;
    }
    string path = root + "/"+ to_string(nbr) + ng.get_name();
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
    ofstream file(path + to_string(++nbr_of_a) + a.getTitle() + ".txt");
    file << a.getAuthor() << '\n';
    file << a.getText();
    file.close();
    return true;
    
}

//Delete article
bool DiskDatabase::delete_article(unsigned int ng_nbr, unsigned int a_nbr) {
    NewsGroup ng = get_newsgroup(ng_nbr);
    if (ng == nullptr) {
        return false;
    }
    string ng_path = root + "/"+ to_string(ng_nbr) + ng.get_name();
    auto dir = opendir(ng_path.c_str());
    if (dir == nullptr) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            auto pos = id.find_first_of(" ");
            unsigned int a_id = stoi(id.substr(0,pos));
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

vector<pair<int,string>> DiskDatabase::list_newsgroup(){
    string path = root + "/";
    auto dir = opendir(path.c_str());
    vector<pair<int,string>> ngs;
    if (dir == nullptr) {
        return ngs;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            auto pos = id.find_first_of(" ");
            int ng_id = stoi(id.substr(0,pos));
            string name = id.substr(pos+1,string::npos);
            pair<int,string> p;
            p.first = ng_id;
            p.second = name;
            ngs.push_back(p);
            
        } catch (invalid_argument e) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return ngs;
}

vector<pair<int,string>> DiskDatabase::list_articles(unsigned int ng_nbr) {
    string path = root + "/";
    vector<pair<int,string>> a_list;
    string ng_path;
    bool exists = false;
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        return a_list;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            auto pos = id.find_first_of(" ");
            unsigned int ng_id = stoi(id.substr(0,pos));
            if (ng_id == ng_nbr) {
                ng_path = path + id + "/";
            }
        } catch (invalid_argument e) {
            cerr << "Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    if (exists) {
        auto ng_dir = opendir(ng_path.c_str());
        if (ng_dir == nullptr) {
            return a_list;
        }
        entry = readdir(ng_dir);
        while (entry != nullptr) {
            try {
                string a_id = entry->d_name;
                auto pos = a_id.find_first_of(" ");
                int a_nbr = stoi(a_id.substr(0,pos));
                auto end_pos = a_id.find(".txt");
                string art_name = a_id.substr(pos+1,end_pos-1);
                pair<int,string> p;
                p.first = a_nbr;
                p.second = art_name;
                a_list.push_back(p);
            } catch (invalid_argument e) {
                cerr << "Something is wrong with the folders" << endl;
            }
            entry = readdir(ng_dir);
        }
        closedir(ng_dir);
    }
    return a_list;
    
}







