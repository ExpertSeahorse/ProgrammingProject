#include "story.h"


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
void Story::print(){
    for (auto i : passages_vec){
        cout << "Passage " << i.getName() << ":" << endl;

        PassageTokenizer pt(i.getText());
        while (pt.hasNextPart()){
            PartToken stok = pt.nextPart();
            
            switch (stok.getType()){
            case TEXT:
                cout << "Text:  \"" << stok.getText() << "\"" << endl;
                break;

            case LINK:{
                cout << "Link:  display=";
                string text = stok.getText();

                // Test if display != target
                size_t found = text.find("-&gt;");
                if (found != string::npos){

                    // If test != target: 
                    cout << text.substr(2, found-2) << ", target=";
                    cout << text.substr(found+5, text.size()-(found+5+2)) << endl;
                }

                else{
                    // If test == target:
                    string s = text.substr(2, text.size()-4);
                    cout << s << ", target=" << s << endl;
                }

                break;
            }
            
            case GOTO:{
                cout << "Goto:  target=";
                string text = stok.getText();
                
                // Print all chars after "(go-to: &quot;" until the next &
                cout << text.substr(14, text.find('&')) << endl;
                break;
            }

            case SET:{
                cout << "Set:  var=";
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
                cout << var << ", ";
                    
                if(found != string::npos) {
                    val = true;
                    cout << "value=true" << endl;
                    //cout << "value=[" << val << "]" << endl;
                }
                else {
                    val = false;
                    cout << "value=false" << endl;
                }
                break;
            }

            case IF: {
                cout << "If:  var=";
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
                cout << var << ", ";
                    
                if(found != string::npos) {
                    val = true;
                    cout << "value=true" << endl;
                }
                else {
                    val = false;
                    cout << "value=false" << endl;
                }
                break;
            }

            case ELSEIF: {
                cout << "Else-if:  var=";
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
                cout << var << ", ";
                    
                if(found != string::npos) {
                    val = true;
                    cout << "value=true" << endl;
                }
                else {
                    val = false;
                    cout << "value=false" << endl;
                }
                break;
            }

            case ELSE: 
                // Will output else
                cout << "Else"<< endl;
                break;

            case BLOCK:{
                string text = stok.getText();
    
                size_t i = 0, j=0;
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
                            if (text[i + 2] == '['){
                                cout << "START BLOCK" << endl;
                                i++;
                            }
                            link = true;
                            break;
                        }
                        else if(text[i] == '['){
                            cout << "START BLOCK" << endl;
                        }

                        else if(text[i] == ']'){
                            cout << "END BLOCK" << endl;
                        }

                        else if(isalpha(text[i]) || isspace(text[i]) || text[i] == '?' ){
                            words = true;
                            break;
                        }

                        else if(text[i] == '('){
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

                        cout << "Link:  display=";

                        string newtext = text.substr(i, (j - i) + 1);
                        size_t found = newtext.find("-&gt;");

                        if (found != string::npos){
                            cout << newtext.substr(2, found-2) << ", target=";
                            cout << newtext.substr(found+5, newtext.size()-(found+5+2)) << endl;
                        }
                        else{
                            string s = newtext.substr(2, newtext.size() - 4);
                            cout << s << ", target=" << s << endl;
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
                        cout << "Text:  \"" << newtext << '\"' << endl;
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
                            cout << "If:  var=" << var << ", value=";
                            size_t found2 = newtext.find("true");

                            if(found2 != string::npos){
                                val = true;
                                cout << "true" << endl;
                            }
                            else {
                                val = false;
                                cout << "false" << endl;
                            }

                        }

                        //Will check for "set" in parantheses
                        //and output as if set function
                        found = newtext.find("set");

                        if(found != string::npos) {
                            var = newtext.substr(newtext.find("$"));
                            var = var.substr(var.find("$"), var.find(" "));
                            cout << "Set:  var=" << var <<", value=";
                            size_t found2 = newtext.find("true");

                            if(found2 != string::npos){
                                val = true;
                                cout << "true" << endl;
                            }
                            else {
                                val = false;
                                cout << "false" << endl;
                            }

                        }

                        //Will check for "go-to" in parantheses
                        //and will output like goto function
                        found = newtext.find("go-to");

                        if(found != string::npos) {
                            var = newtext.substr(14, newtext.find("&"));
                            cout << "Goto:  target=" << var << endl;
                        }

                        //Will check for "else-if" in parantheses
                        //will output like else-if function
                        found = newtext.find("else-if");

                        if(found != string::npos) {
                            var = newtext.substr(newtext.find("$"));
                            var = var.substr(var.find("$"), var.find(" "));
                            cout << "Else-if:  var=" << var <<", value=";
                            size_t found2 = newtext.find("true");

                            if(found2 != string::npos){
                                val = true;
                                cout << "true" << endl;
                            }
                            else {
                                val = false;
                                cout << "false" << endl;
                            }
                        }

                        //Will check for "else:" in the parantheses
                        //and output else 
                        found = newtext.find("else:");

                        if(found != string::npos) {
                            cout << "ELSE" << endl;
                        }

                        //Resets bool para to be used once more in the for loop if theres more paranthetical statements
                        para = false;
                        //Sets i to j as the new starting point for the for loop
                        i = j;
                    }

                }
                break;
            }
            
            default:
                cout << "  Unknown token:  " << stok.getType() << endl;
                break;
            }
        }
    }
}


void Story::play(){
    vector<pair<string, string>> link_destinations;
    string nextPassage = passages_vec.at(0).getName();  // Sets nextPassage to first passage
    PassageToken passage;

    bool gotoFlag = false;

    // TODO: Input exit condition maybe
    while (true){
        passage = passages_map[nextPassage];

        PassageTokenizer pt(passage.getText());
        while (pt.hasNextPart() || !gotoFlag){
            PartToken stok = pt.nextPart();
            
            switch (stok.getType()){
            case TEXT:
                cout << stok.getText();
                break;

            case LINK:{
                string text = stok.getText();
                string target;

                target = text.substr(2, text.size()-4);
                cout << target;

                // Test if link display != target
                size_t found = text.find("-&gt;");
                if (found != string::npos){
                    // appends the target of the link to the vector if target != display
                    string destination = text.substr(found+5, text.size()-(found+5+2));
                    link_destinations.push_back(make_pair(target, destination));
                }

                else{
                    // If link display == target:
                    link_destinations.push_back(make_pair(target, target));
                }
                break;
            }
            
            case GOTO:{
                string text = stok.getText();

                // send all chars after "(go-to: &quot;" until the next & into nextPassage
                nextPassage = text.substr(14, text.find('&'));
                gotoFlag = true;

                break;
            }

            case SET:{
                string text = stok.getText();
                /*  Just in case my code doesn't work

                // Will cut out everything before '$'
                string var = text.substr(text.find("$"));
                
                // Will assign chars from '$' until next whitespace to var
                var = var.substr(var.find("$"), var.find(" "));
                */

               string var = text.substr(text.find("$"), text.find(" "));

                // Will get value and assign to vars map        
                size_t found = text.find("true");             
                if(found != string::npos) 
                    vars.emplace(var, true);
                
                else 
                    vars.emplace(var, false);
                
                break;
            }

            case IF: {
                cout << "If:  var=";
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
                cout << var << ", ";
                    
                if(found != string::npos) {
                    val = true;
                    cout << "value=true" << endl;
                }
                else {
                    val = false;
                    cout << "value=false" << endl;
                }
                break;
            }

            case ELSEIF: {
                cout << "Else-if:  var=";
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
                cout << var << ", ";
                    
                if(found != string::npos) {
                    val = true;
                    cout << "value=true" << endl;
                }
                else {
                    val = false;
                    cout << "value=false" << endl;
                }
                break;
            }

            case ELSE: 
                // Will output else
                cout << "Else"<< endl;
                break;

            case BLOCK:{
                string text = stok.getText();
    
                size_t i = 0, j=0;
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
                            if (text[i + 2] == '['){
                                cout << "START BLOCK" << endl;
                                i++;
                            }
                            link = true;
                            break;
                        }
                        else if(text[i] == '['){
                            cout << "START BLOCK" << endl;
                        }

                        else if(text[i] == ']'){
                            cout << "END BLOCK" << endl;
                        }

                        else if(isalpha(text[i]) || isspace(text[i]) || text[i] == '?' ){
                            words = true;
                            break;
                        }

                        else if(text[i] == '('){
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

                        cout << "Link:  display=";

                        string newtext = text.substr(i, (j - i) + 1);
                        size_t found = newtext.find("-&gt;");

                        if (found != string::npos){
                            cout << newtext.substr(2, found-2) << ", target=";
                            cout << newtext.substr(found+5, newtext.size()-(found+5+2)) << endl;
                        }
                        else{
                            string s = newtext.substr(2, newtext.size() - 4);
                            cout << s << ", target=" << s << endl;
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
                        cout << "Text:  \"" << newtext << '\"' << endl;
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
                            cout << "If:  var=" << var << ", value=";
                            size_t found2 = newtext.find("true");

                            if(found2 != string::npos){
                                val = true;
                                cout << "true" << endl;
                            }
                            else {
                                val = false;
                                cout << "false" << endl;
                            }

                        }

                        //Will check for "set" in parantheses
                        //and output as if set function
                        found = newtext.find("set");

                        if(found != string::npos) {
                            var = newtext.substr(newtext.find("$"));
                            var = var.substr(var.find("$"), var.find(" "));
                            cout << "Set:  var=" << var <<", value=";
                            size_t found2 = newtext.find("true");

                            if(found2 != string::npos){
                                val = true;
                                cout << "true" << endl;
                            }
                            else {
                                val = false;
                                cout << "false" << endl;
                            }

                        }

                        //Will check for "go-to" in parantheses
                        //and will output like goto function
                        found = newtext.find("go-to");

                        if(found != string::npos) {
                            var = newtext.substr(14, newtext.find("&"));
                            cout << "Goto:  target=" << var << endl;
                        }

                        //Will check for "else-if" in parantheses
                        //will output like else-if function
                        found = newtext.find("else-if");

                        if(found != string::npos) {
                            var = newtext.substr(newtext.find("$"));
                            var = var.substr(var.find("$"), var.find(" "));
                            cout << "Else-if:  var=" << var <<", value=";
                            size_t found2 = newtext.find("true");

                            if(found2 != string::npos){
                                val = true;
                                cout << "true" << endl;
                            }
                            else {
                                val = false;
                                cout << "false" << endl;
                            }
                        }

                        //Will check for "else:" in the parantheses
                        //and output else 
                        found = newtext.find("else:");

                        if(found != string::npos) {
                            cout << "ELSE" << endl;
                        }

                        //Resets bool para to be used once more in the for loop if theres more paranthetical statements
                        para = false;
                        //Sets i to j as the new starting point for the for loop
                        i = j;
                    }

                }
                break;
            }
            
            default:
                cout << "  Unknown token:  " << stok.getType() << endl;
                break;
            }
        }

        // End of the PartToken switch statement

        // If the Passage stopped because of Go-To: just continue,
        // don't print out links or terminate
        if (gotoFlag){
            gotoFlag = false;
            continue;
        }

        cout << endl;
        // If there are no links, terminate the session
        if (link_destinations.empty())
            break;

        // print out the links + 
        int choice;
        for (size_t i=0; i < link_destinations.size(); i++){
            cout << i+1 << ":\t" << link_destinations.at(i).first << endl;
        }

        cin >> choice;
        nextPassage = link_destinations[choice-1].second;
    }
}
