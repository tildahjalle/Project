//
//  Article.cpp
//  
//
//  Created by Matilda Hjälle on 2015-04-07.
//
//

#include "Article.h"
#include <string>

using namespace std;


Article::Article() {};

Article::Article(string t, string a, string txt/*, unsigned int nbr*/) : author(a), title(t), text(txt) /*,id(nbr)*/{
    //id= ++count;
}

string Article::getAuthor() const {return author;};

string Article::getTitle() const {return title;};

string Article::getText() const {return text;};

//unsigned int Article::getId() const (return id);

