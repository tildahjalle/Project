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
    if (dir == NULL) {
        mkdir(root.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        auto entry = readdir(dir);
        while (entry != nullptr) {
            try {
                string id = entry->d_name;
                if (id.at(0) != '.') {                    auto pos = id.find_first_of(" ");
                    unsigned int id_nbr = stoi(id.substr(0,pos));
                    if (id_nbr > groupnbr) {
                        groupnbr = id_nbr;
                    }
                }
                
            } catch (invalid_argument e) {
                cerr << "1. Something is wrong with the folders" << endl;
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
            if (id.at(0) != '.') {
                auto pos = id.find_first_of(" ");
                string name = id.substr(pos+1,string::npos);
            
                if (name == ng_name) {
                    return false;
                }
            }
        } catch (invalid_argument e) {
            cerr << "2.Something is wrong with the folders" << endl;
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
    if (groupnbr == 0) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            if (id.at(0) != '.') {
                auto pos = id.find_first_of(" ");
                unsigned int ng_id = stoi(id.substr(0,pos));
                if (ng_id == id_nbr) {
                    return remove((path + id).c_str()) == 0;
                }
            }
        } catch (invalid_argument e) {
            cerr << "3.Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return false;
}

//get reference to group for listing of articles. Read only.
//returns nullptr if group doesn't exists
/*const*/ pair<bool,NewsGroup>/*&*/ DiskDatabase::get_newsgroup(unsigned int id_nbr) const{
    pair<bool,NewsGroup> p;
    p.first = false;
    string path = root + "/";
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        return p;
    }
    if (groupnbr == 0) {
        return p;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            if (id.at(0) != '.') {
                auto pos = id.find_first_of(" ");
                unsigned int ng_id = stoi(id.substr(0,pos));
                if (ng_id == id_nbr) {
                    string ng_name = id.substr(pos+1,string::npos);
                    p.second = NewsGroup(ng_name);
                    auto ng_dir = opendir((path+id+"/").c_str());
                    if (dir == nullptr) {
                        return p;
                    }
                    auto ng_entry = readdir(ng_dir);
                    while (ng_entry!= nullptr) {
                        try {
                            string art_id = ng_entry->d_name;
                            if (art_id.at(0) != '.') {
                                auto pos = id.find_first_of(" ");
                                unsigned int a_id = stoi(art_id.substr(0,pos));
                                auto end_pos = art_id.find(".");
                                string title = art_id.substr(pos+1,end_pos-2);
                                ifstream file(path+id+"/" + art_id);
                                string author;
                                getline(file,author);
                                string text;
                                string line;
                                if(getline(file,line)){
                                    text += line;
                                    int count = 0;
                                    while(getline(file, line)){
                                        text += "\n" + line;
                                        count++;			
                                    }		
                                    if(count > 1){
                                        text += "\n";
                                    }
                                }
                                file.close();
                                Article a = Article(title,author,text);
                                p.second.add_article(a_id,a);
                            }
                        
                        } catch (invalid_argument) {
                            cerr << "4.Something is wrong with the files" << endl;
                        }
                        ng_entry = readdir(ng_dir);
                    }
                    p.first = true;
                    return p;
                }
            }
        } catch (invalid_argument e) {
            cerr << "5.Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return p;
}

//Adds Article to group nbr int. Return success.
bool DiskDatabase::add_article(unsigned int nbr, string ng_name, const Article& a) {
    if (groupnbr == 0) {
        return false;
    }
    string path = root + "/"+ to_string(nbr) + " "+ ng_name + "/";
    unsigned int nbr_of_a = 0;
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            if (id.at(0) != '.') {
                auto pos = id.find_first_of(" ");
                unsigned int id_nbr = stoi(id.substr(0,pos));
                if (id_nbr > nbr_of_a) {
                    nbr_of_a = id_nbr;
                }
            }
            
        } catch (invalid_argument e) {
            cerr << "6.Something is wrong with the folders" << endl;
        }
        entry = readdir(dir);
    }
    ++nbr_of_a;
    string name =path + "/" + to_string(nbr_of_a) + " " + a.getTitle() + ".txt";
    ofstream file;
    file.open(name);
    file << a.getAuthor() << '\n';
    file << a.getText();
    file.close();
    closedir(dir);
    return true;
    
}

//Delete article
bool DiskDatabase::delete_article(unsigned int ng_nbr, unsigned int a_nbr) {
    pair<bool, NewsGroup> p =get_newsgroup(ng_nbr);
    NewsGroup ng = p.second;
    if (p.first == false) {
        return false;
    }
    if (groupnbr == 0) {
        return false;
    }
    string ng_path = root + "/"+ to_string(ng_nbr) +" "+ ng.get_name() + "/";
    auto dir = opendir(ng_path.c_str());
    if (dir == nullptr) {
        return false;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            if (id.at(0) != '.') {
                auto pos = id.find_first_of(" ");
                unsigned int a_id = stoi(id.substr(0,pos));
                if (a_id == a_nbr) {
                    return remove((ng_path + id).c_str()) == 0;
                }
            }
        } catch (invalid_argument e) {
            cerr << "7.Something is wrong with the folders" << endl;
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
    if (groupnbr == 0) {
        return ngs;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            if (id.at(0) != '.') {
                auto pos = id.find_first_of(" ");
                int ng_id = stoi(id.substr(0,pos));
                string name = id.substr(pos+1,string::npos);
                pair<int,string> p;
                p.first = ng_id;
                p.second = name;
                ngs.push_back(p);
            }
            
        } catch (invalid_argument e) {
            cerr << "8.Something is wrong with the folders" << endl;
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
    if (groupnbr == 0) {
        return a_list;
    }
    auto entry = readdir(dir);
    while (entry != nullptr) {
        try {
            string id = entry->d_name;
            if (id.at(0) != '.') {
                auto pos = id.find_first_of(" ");
                unsigned int ng_id = stoi(id.substr(0,pos));
                if (ng_id == ng_nbr) {
                    ng_path = path + id + "/";
                    exists = true;
                }
            }
        } catch (invalid_argument e) {
            cerr << "9.Something is wrong with the folders" << endl;
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
                if (a_id.at(0) != '.') {
                    auto pos = a_id.find_first_of(" ");
                    int a_nbr = stoi(a_id.substr(0,pos));
                    auto end_pos = a_id.find(".");
                    string art_name = a_id.substr(pos+1,end_pos-2);
                    pair<int,string> p;
                    p.first = a_nbr;
                    p.second = art_name;
                    a_list.push_back(p);
                }
            } catch (invalid_argument e) {
                cerr << "10.Something is wrong with the folders" << endl;
            }
            entry = readdir(ng_dir);
        }
        closedir(ng_dir);
    }
    return a_list;
    
}







