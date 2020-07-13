#include "parttoken.h"

#include <iostream>

PartToken::PartToken(){
    // If no input: blank fields
    text = "";
    type = TEXT;
}
PartToken::PartToken(string text, part type){
    // If expected input: 
    this->text = text;
    this->type = type;
}