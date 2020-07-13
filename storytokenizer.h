#include "passagetoken.h"

#ifndef STORYTOKENIZER_H
#define STORYTOKENIZER_H

#include <string>
using namespace std;

class StoryTokenizer
{
    private:
        int current_position;
        string story;
    public:
        StoryTokenizer();
        StoryTokenizer(string);

        bool hasNextPassage() const;
        PassageToken nextPassage();
};

#endif