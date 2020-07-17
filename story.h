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

    string print();
    void play();

    string printLink(PartToken);
    string printSet(PartToken);
    string printGoTo(PartToken);
    string printIf(PartToken);
    string printElseIf(PartToken);
    string printBlock(PartToken);



};

#endif
