#include "story.h"

#include <sstream>

// if no input file chosen
Story::Story(){
    cout << "You must pass a filename!" << endl;
}

// Preferred constructor
Story::Story(string filename){
    string line;  
    //Open input.txt for reading  
    ifstream in(filename);  
    if (!in.is_open())  {
        cout << "Couldn't open " << filename << " for reading!\n";    
        abort();
    }  
    
    //Read in the story from input.txt  
    getline(in, line);  
    while (in && line != "</html>"){    
        story += line + '\n';    
        getline(in, line);  
    }

    //Construct the StoryTokenizer object
    StoryTokenizer st(story);

    //Iterate through all of the passages in the story
    while (st.hasNextPassage()){
        PassageToken ptok = st.nextPassage();
        passages_map.insert({ptok.getName(), ptok});
        passages_vec.push_back(ptok);
    }
}

// Prints out all passages in order of appearence
string Story::print(){
    ostringstream ooutstring;
    for (auto i : passages_vec){
        ooutstring << "Passage " << i.getName() << ":" << endl;

        PassageTokenizer pt(i.getText());
        while (pt.hasNextPart()){
            PartToken stok = pt.nextPart();
            
            switch (stok.getType()){
            case TEXT:
                ooutstring << "Text:  \"" << stok.getText() << "\"" << endl;
                break;

            case LINK:
                ooutstring << printLink(stok);
                break;
            
            case GOTO:
                ooutstring << printGoTo(stok);
                break;

            case SET:
                ooutstring << printSet(stok);
                break;

            case IF: 
                ooutstring << printIf(stok);
                break;

            case ELSEIF: 
                ooutstring << printElseIf(stok);
                break;

            case ELSE: 
                // Will output else
                ooutstring << "Else"<< endl;
                break;

            case BLOCK:
                 ooutstring << printBlock(stok);
                 break;
            
            default:
                ooutstring << "  Unknown token:  " << stok.getType() << endl;
                break;
            }
        }
    }
    return ooutstring.str();
}


void Story::play(){
    // Wait for Part 5
}

string Story::printLink(PartToken stok){
    ostringstream ooutstring;
    ooutstring << "Link:  display=";
    string text = stok.getText();

    // Test if display != target
    size_t found = text.find("-&gt;");
    if (found != string::npos){

        // If test != target: 
        ooutstring << text.substr(2, found-2) << ", target=";
        ooutstring << text.substr(found+5, text.size()-(found+5+2)) << endl;
    }
    else{
        // If test == target:
        string s = text.substr(2, text.size()-4);
        ooutstring << s << ", target=" << s << endl;
    }
    return ooutstring.str();
}

string Story::printSet(PartToken stok){
    ostringstream ooutstring;
    ooutstring << "Set:  var=";
    string text = stok.getText();
    
    // Will cut out everything before '$'
    string var = text.substr(text.find("$"));
    bool val;    
    // Will assign chars from '$' until next whitespace to var
    var = var.substr(var.find("$"), var.find(" "));
    
    // Test to see if true or false in text and assigns bool to val
    // then prints out value
    size_t found = text.find("true");
        
    // Will output variable and value and assign bool 
    ooutstring << var << ", ";
        
    if(found != string::npos) {
        val = true;
        ooutstring << "value=true" << endl;
        //ooutstring << "value=[" << val << "]" << endl;
    }
    else {
        val = false;
        ooutstring << "value=false" << endl;
    }
    return ooutstring.str();
}

string Story::printGoTo(PartToken stok){
    ostringstream ooutstring;
    ooutstring << "Goto:  target=";
    string text = stok.getText();
    
    // Print all chars after "(go-to: &quot;" until the next &
    ooutstring << text.substr(14, text.find('&')) << endl;
    return ooutstring.str();
}

string Story::printIf(PartToken stok){
    ostringstream ooutstring;
    ooutstring << "If:  var=";
    string text = stok.getText();
    
    //Will cut out all characters before '$'
    string var = text.substr(text.find("$"));
    bool val;    
    
    // Will assign chars from '$' until next whitespace to var
    var = var.substr(var.find("$"), var.find(" "));
    
    // Test to see if true or false in text and assigns bool to val
    // then prints out value
    size_t found = text.find("true");
        
    // Will output variable and value and assign bool 
    ooutstring << var << ", ";
        
    if(found != string::npos) {
        val = true;
        ooutstring << "value=true" << endl;
    }
    else {
        val = false;
        ooutstring << "value=false" << endl;
    }
    return ooutstring.str();
}

string Story::printElseIf(PartToken stok){
    ostringstream ooutstring;
    ooutstring << "Else-if:  var=";
    string text = stok.getText();

    //Will cut out all characters before '$'
    string var = text.substr(text.find("$"));
    bool val;
    
    // Will assign chars from '$' until next whitespace to var
    var = var.substr(var.find("$"), var.find(" "));
    
    // Test to see if true or false in text and assigns bool to val
    // then prints out value
    size_t found = text.find("true");
        
    // Will output variable and value and assign bool
    ooutstring << var << ", ";
        
    if(found != string::npos) {
        val = true;
        ooutstring << "value=true" << endl;
    }
    else {
        val = false;
        ooutstring << "value=false" << endl;
    }
    return ooutstring.str();
}

string Story::printBlock(PartToken stok){
    ostringstream ooutstring;
    string text = stok.getText();
    
    int i = 0;
    int j = 0;
    bool para = false;
    bool words = false;
    bool link = false;
    
    //int i to keep track of where in the string we are indexed and will be used in while loop to end the function
    //int j is used for the next position of '(' '[' or ')'
    //bools are used to continue making outputs of links, sets, ifs etc in case more than
    //one but keeps them from repeating the output in the for loop
    
    while(i < text.size()) {
    // for loop is used to iterate through the text
    // 4 if statements in the for loop will output the start of a block if
    // '[' is found and will out put the end of the block if ']' is found
    // loop will break if the next character is an alphabetical letter or
    // a '(' and assign the bool for words indicating text or para for parantheses

        for (; i < text.size(); i++) {
            if(i > 0 && text[i] == '[' && text[i + 1] == '[') {
                link = true;
                break;
            }
            if(text[i] == '['){
            ooutstring << "START BLOCK" << endl;
            }
            if(text[i] == ']'){
            ooutstring << "END BLOCK" << endl;
            }
            if(isalpha(text[i]) || isspace(text[i])){
            words = true;
            break;
            }
            if(text[i] == '('){
            para = true;
            break;
            }
        }

        // if statements are used with the bool values as only one bool value will be on at any
        // given time.
        
        //Will check for both cases of links in the block passage
        if(link == true) {
            for(j = i; j < text.size(); j++) {
                if(text[j] == ']' && text[j + 1] == ']'){
                    j = j + 1;
                    break;
                }
            }

            string newtext = text.substr(i, (j - i) + 1);
            size_t found = newtext.find("-&gt;");

            if (found != string::npos){
                ooutstring << "Link: display=";
                ooutstring << newtext.substr(2, found-2) << ", target=";
                ooutstring << newtext.substr(found+5, newtext.size()-(found+5+2)) << endl;
            }
            else{
                ooutstring << "Link: display=";
                string s = newtext.substr(2, newtext.size() - 4);
                ooutstring << s << ", target=" << s << endl;
            }
            link = false;
            //In this case i is set to j + 1 to not output an extra "END BLOCK" statement in the for loop
            i = j + 1;
        }


        if(words == true) {

            //j will be set equal to i as that is the beginning of the text in the previous for loop
            //this for loop will look for the end of text by finding the corresponding symbols for
            //brackets or parantheses and i and j will be used to cut the original text and output the text

            for (j = i; j < text.size(); j++) {
                if(text[j] == '(' || text[j] == '[' || text[j] == ']') {
                    break;
                }
            }

            // words is set to false so that the original for loop can activate it again if there are more than one
            // text passage
            // i is also set to j because that would be our new starting point in the original text due to the fact
            // that we output all the text until j

            string newtext = text.substr(i, j - i);
            ooutstring << "Text: \"" << newtext << '\"' << endl;
            words = false;
            i = j;
        }

        if(para == true) {
            
            // for loop will set j to i as that is the beginning of the parantheses text
            // the for loop will iterate until it finds a closing ')' and break
            // the original text will be cut using i and j as delimeters for newtext

            //size_t found is used to check the paratheses for if, else, if-else or goto
            //When either of the 4 is found the if staments will appropritately extract the
            //values as they are inteded to for their type

            //found2 will be used in all but goto and else to find if the value is either true or false

            for (j = i; j < text.size(); j++) {
                if(text[j] == ')') {
                    break;
                }
            }

            string newtext = text.substr(i, j - (i - 1));
            string var;
            bool val;   
                

            //Will check for "if" in the parantheses
            //and output according to if function
            size_t found = newtext.find("if");
                    
            if(found != string::npos) {
                var = newtext.substr(newtext.find("$"));
                var = var.substr(var.find("$"), var.find(" "));
                ooutstring << "If: var=" << var << ", value=";
                size_t found2 = newtext.find("true");

                if(found2 != string::npos){
                    val = true;
                    ooutstring << "true" << endl;
                }
                else {
                    val = false;
                    ooutstring << "false" << endl;
                }

            }

            //Will check for "set" in parantheses
            //and output as if set function
            found = newtext.find("set");

            if(found != string::npos) {
                var = newtext.substr(newtext.find("$"));
                var = var.substr(var.find("$"), var.find(" "));
                ooutstring << "Set: var=" << var <<", value=";
                size_t found2 = newtext.find("true");

                if(found2 != string::npos){
                    val = true;
                    ooutstring << "true" << endl;
                }
                else {
                    val = false;
                    ooutstring << "false" << endl;
                }

            }

            //Will check for "go-to" in parantheses
            //and will output like goto function
            found = newtext.find("go-to");

            if(found != string::npos) {
                var = newtext.substr(14, newtext.find("&"));
                ooutstring << "Goto: target=" << var << endl;
            }

            //Will check for "else-if" in parantheses
            //will output like else-if function
            found = newtext.find("else-if");

            if(found != string::npos) {
                var = newtext.substr(newtext.find("$"));
                var = var.substr(var.find("$"), var.find(" "));
                ooutstring << "Else-if: var=" << var <<", value=";
                size_t found2 = newtext.find("true");

                if(found2 != string::npos){
                    val = true;
                    ooutstring << "true" << endl;
                }
                else {
                    val = false;
                    ooutstring << "false" << endl;
                }
            }

            //Will check for "else:" in the parantheses
            //and output else 
            found = newtext.find("else:");

            if(found != string::npos) {
                ooutstring << "ELSE" << endl;
            }

            //Resets bool para to be used once more in the for loop if theres more paranthetical statements
            para = false;
            //Sets i to j as the new starting point for the for loop
            i = j;
        }

    }
    return ooutstring.str();   
}
    
