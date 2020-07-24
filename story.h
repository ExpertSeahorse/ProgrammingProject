#ifndef STORY_H
#define STORY_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map> 
#include <map>

#include "storytokenizer.h"

using namespace std;

class Story
{
private:
    string story;
    unordered_map<string, bool> vars;
    unordered_map<string, PassageToken> passages_map;
    vector<PassageToken> passages_vec;
public:
    Story();
    Story(string);
    void print();
    void play();

    string tokenizePlay(string, vector< pair<string, string> >&, bool&);
};

#endif