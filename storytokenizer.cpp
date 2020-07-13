#include "storytokenizer.h"

#include <iostream>
#include <string>
using namespace std;

// Checks if there is another passage T/F
bool StoryTokenizer::hasNextPassage() const{
    // String to get the position of the next passage in the document
    string start = "<tw-passagedata";

    // get the index of the pid in the passage
    size_t found = story.find(start, current_position);

    // if the find fails, it returns string::npos
    if (found == string::npos)
        return false;
    else
        return true;
}

PassageToken StoryTokenizer::nextPassage(){
    // Strings to get the position of the current chunk
    string start = "<tw-passagedata";
    string end = "</tw-passagedata>";

    // Get start of the token's header
    size_t found = story.find(start, current_position);

    // Get the body text of the token
    size_t text_end = story.find(end, found);
    size_t text_start = story.find(">", found) + 1;
    size_t text_len = text_end - text_start;

    string text = story.substr(text_start, text_len);

    // Find the name of the token in the header
    size_t name_start = story.find("name=\"", found)+6;
    size_t name_end = story.find("\"", name_start);
    size_t name_len = name_end - name_start;

    string name = story.substr(name_start, name_len);

    // Increment current position for next time
    current_position = text_end + end.size();

    return PassageToken(text, name);
}

StoryTokenizer::StoryTokenizer(){
    this->story = "cool story";
    this->current_position =1;
}

StoryTokenizer::StoryTokenizer(string story){
    this->story = story;
    this->current_position =1;
}


// to_find code first on line 9 adapted from: http://www.cplusplus.com/forum/beginner/235373/
// find code on lines 10-11 adapted from: https://www.geeksforgeeks.org/string-find-in-cpp/
