#include "passagetoken.h"

#include <iostream>

PassageToken::PassageToken(){
    // If no input: blank fields
    name = "";
    text = "";
}
PassageToken::PassageToken(string text, string name){
    // If expected input: 
    this->text = text;
    this->name = name;
}