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
                cout << "  Text:  \"" << stok.getText() << "\"" << endl;
                break;

            case LINK:{
                cout << "  Link:  display=";
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
                cout << "  Go-to:  target=";
                string text = stok.getText();
                
                // Print all chars after "(go-to: &quot;" until the next &
                cout << text.substr(14, text.find('&')) << endl;
                break;
            }

            case SET:{
                cout << "  Set: var=";
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
                    cout << "value=[" << val << "]" << endl;
                }
                else {
                    val = false;
                    cout << "value=[" << val << "]" << endl;
                }
                break;
            }
            case IF: {
                cout << "  If: var=";
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
                cout << "  Else-if:  var=";
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

            case ELSE: {
                // Will output else
                cout << "  Else  "<< endl;
                break;
            }
            case BLOCK:{
                //int i to keep track of where in the string we are indexed
                //int j is used for the next position of '(' '[' or ')'
                //bools are used to continue making text and if outputs in case more than
                //one but keeps them from repeating the output in the for loop
                
                int i = 0;
                int j = 0;
                bool para = false;
                bool words = false;

                string text = stok.getText();
                
                // i will continue to be incremented in the for loop in this while loop
                // it will break out completely when program has iterated through entire text
      
                while(i < text.size()) {
                // for loop is used to iterate through the text
                // 4 if statements in the for loop will output the start of a block if
                // '[' is found and will out put the end of the block if ']' is found
                // loop will break if the next character is an alphabetical letter or
                // a '(' and assign the bool for words indicating text or para for parantheses
                    
                for (; i < text.size(); i++) {
                    if(text[i] == '['){
                        cout << "START BLOCK" << endl;
                    }
                    if(text[i] == ']'){
                        cout << "END BLOCK" << endl;
                    }
                    if(isalpha(text[i])) {
                        words = true;
                        break;
                    }
                    if(text[i] == '(') {
                        para = true;
                        break;
                    }
                }
                        
                // if statements are used with the bool values as only one bool value will be on at any
                // given time.
                    
                if(words == true) {
                    //j will be set equal to i as that is the beginning of the text in the previous for loop
                    //this for loop will look for the end of text by finding the corresponding symbols for
                    //brackets or parantheses and i and j will be used to cut the original text and output the text
         
                    for (j = i; j < text.size(); j++) {
                        if(text[j] == '(' || text[j] == '[' || text[j] == ']') {
                            break;
                        }
                    }
                    string newtext = text.substr(i, j - i);
                    cout << "Text: \"" << newtext << '\"' << endl;
                    
                    // words is set to false so that the original for loop can activate it again if there are more than one
                    // text passage
                    // i is also set to j because that would be our new starting point in the original text due to the fact
                    // that we output all the text until j
                    words = false;
                    i = j;
                }

                if(para == true) {
                    // for loop will set j to i as that is the beginning of the parantheses text
                    // the for loop will iterate until it finds a closing ')' and break
                    // the original text will be cut using i and j as delimeters for newtext
                    for (j = i; j < text.size(); j++) {
                        if(text[j] == ')') {
                        break;
                        }
                    }
                    
                    string newtext = text.substr(i, j - (i - 1));
                    string var = newtext.substr(newtext.find("$"));
                    bool val;    
                    
                    //Will cut the newtext for var starting with '$" and ending with
                    //the next whitespace character
                    var = var.substr(var.find("$"), var.find(" "));
                
                    //test to search is "true" is in the parantheses or not
                    size_t found = text.find("true");
                    
                    // will output var value if true or false
                    cout << "If: var=" << var << ", ";

                    if(found != string::npos) {
                        val = true;
                        cout << "value=true" << endl;
                    }
                    else {
                        val = false;
                        cout << "value=false" << endl;
                    }
                    // sets para to false for the original for loop to be able to flip it to true if another '('
                    // is found and i is updated with j because it will be our new starting position in the string
                    para = false;
                    i = j;
                }
               }
            break;
            }

            default:
                cout << "  Unknown token:  ";
            }
        }
    }
}


void Story::play(){
    // Wait for Part 5
}
