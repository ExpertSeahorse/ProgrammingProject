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
                
                string var = text.substr(text.find("$"));
                bool val;    
                
                var = var.substr(var.find("$"), var.find(" "));
                
                size_t found = text.find("true");
                    
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
                
                string var = text.substr(text.find("$"));
                bool val;    
                
                var = var.substr(var.find("$"), var.find(" "));
                
                size_t found = text.find("true");
                    
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
            
                string var = text.substr(text.find("$"));
                bool val;
            
                var = var.substr(var.find("$"), var.find(" "));
                
                size_t found = text.find("true");
                    
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
                cout << "  Else  "<< endl;
                break;
            }
            case BLOCK:{
               
                int i = 0;
                int j = 0;
                bool para = false;
                bool words = false;

                string text = stok.getText();

                while(i < text.size()) {

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
    
                if(words == true) {
                    for (j = i; j < text.size(); j++) {
                        if(text[j] == '(' || text[j] == '[' || text[j] == ']') {
                            break;
                        }
                    }
                    string newtext = text.substr(i, j - i);
                    cout << "Text: \"" << newtext << '\"' << endl;
                    words = false;
                    i = j;
                }

                if(para == true) {
                    for (j = i; j < text.size(); j++) {
                        if(text[j] == ')') {
                        break;
                        }
                    }

                    string newtext = text.substr(i, j - (i - 1));
                    string var = newtext.substr(newtext.find("$"));
                    bool val;    
                
                    var = var.substr(var.find("$"), var.find(" "));
                
                    size_t found = text.find("true");
                    
                    cout << "If: var=" << var << ", ";

                    if(found != string::npos) {
                        val = true;
                        cout << "value=true" << endl;
                    }
                    else {
                        val = false;
                        cout << "value=false" << endl;
                    }
                    para = false;
                    i = j;
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
