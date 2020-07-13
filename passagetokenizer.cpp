#include "passagetokenizer.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Checks if there is another passage T/F
bool PassageTokenizer::hasNextPart(){
    if (current_position == (int) body.size())
        return false;

    // reset distances array
    distances.clear();
    /*
    for (int j=0; j<distances.size(); j++)
            distances[j] = body.size();
    */
    // check only the remainder of the body for another part
    sub_body = body.substr(current_position);
    //cout << "Sub Body:" << sub_body << endl;

    // check for LINK
    string check = "[[";
    size_t found = sub_body.find(check);
    if (found != string::npos){
        if (found == 0){
            nextPartType = LINK;
            return true;
        }
        else
            distances.push_back(found);
    }

    // check for COMMANDS
    for(int i=0; i < 5; i++){
        check = "(" + commands.at(i) + ":";
        found = sub_body.find(check);

        if (found != string::npos){
            if (found == 0){
                nextPartType = COMMAND; 
                return true;
            }
            else
                distances.push_back(found);
        }
    }

    // check for BLOCK
    check = '[';
    found = sub_body.find(check);
    if (found != string::npos){
        if (found == 0){
            nextPartType = BLOCK;
            return true;
        }
        else
            distances.push_back(found);
    }

    // If nothing else, it is TEXT
    nextPartType = TEXT;
    return true;
}


PartToken PassageTokenizer::nextPart(){
    int fin, text_len=0;
    string text;
    //cout << "Current Position: " << current_position << endl;
    //cout << nextPartType << endl;
    
    // If token is a TEXT
    if (nextPartType == TEXT){
        /*cout << "Distances: ";
        for (int j=0; j<3; j++)
            cout << distances[j] << ",";
        cout << endl;
        */
        if (distances.size() == 0){
            text_len = (int) body.size() - current_position;
        }
        else{
            text_len = distances.at(0);
            int s = distances.size();
            for (int i=1; i<s; i++){
                if (text_len > distances.at(i))
                    text_len = distances.at(i);
            }
        }
        
        text = body.substr(current_position, text_len);
        current_position += text_len;
        if (current_position > (int) body.size())
            current_position = body.size();

        return PartToken(text, nextPartType);
    }

    // If token is a LINK
    else if(nextPartType == LINK){
        fin = body.find("]]", current_position)+2;
        text_len = fin - current_position;

        text = body.substr(current_position, text_len);

        current_position += text_len;
        return PartToken(text, nextPartType);
    }

    // If token is a COMMAND
    else if(nextPartType == COMMAND){
        for (int i=0; i < 5; i++){
            int end = commands.at(i).size();
            string s1 = body.substr(current_position, end+2);
            string s2 = "(" + commands.at(i) + ":";
            if (s1 == s2){
                switch(i){
                case 0:
                    nextPartType = GOTO;
                    break;
                case 1:
                    nextPartType = SET;
                    break;
                case 2:
                    nextPartType = IF;
                    break;
                case 3:
                    nextPartType = ELSEIF;
                    break;
                case 4:
                    nextPartType = ELSE;
                    break;
                }
                fin = body.find(')', current_position)+1;
                text_len = fin - current_position;
                text = body.substr(current_position, text_len);

                current_position += text_len;
                return PartToken(text, nextPartType);
            }
        }
    }

    // If token is a BLOCK
    else if(nextPartType == BLOCK){
        int level = 0, start = 1, i=0;
        string ch;
        while(level!=0 || start == 1){
            text_len++;
            ch = body.at(current_position + i);
            if (ch == "["){
                start = 0;
                level += 1;
            }
            if (ch == "]"){
                start = 0;
                level -= 1;
                //cout << "In Loop Text Len: " << text_len << endl;
                //cout << "Level: " << level << ", start: " << start << endl;
            }
            i++;
        }
        //cout << "Text Len: " << text_len << endl;
        text = body.substr(current_position, text_len);
        current_position += text_len;
        return PartToken(text, nextPartType);
    }
    // If no type:
    cout << "Something went wrong! You got to the bottom of nextPart!" << endl;
    return PartToken();
}


PassageTokenizer::PassageTokenizer(string body="h"){
    this->body = body;
    cout << "Passage" << endl;
}