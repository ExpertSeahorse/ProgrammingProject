#ifndef STORY_H
#define STORY_H

#include <string>
#include <unordered_map> 
#include <map>

#include "passagetoken.h"

using namespace std;

class Story
{
private:
    string story;
    unordered_map<string, bool> vars;
    map<string, PassageToken> passages;
public:
    Story();
    Story(string);
    void print();
    void play();
};

#endif