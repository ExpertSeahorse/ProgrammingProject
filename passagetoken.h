#include "passagetokenizer.h"

#ifndef PASSAGETOKEN_H
#define PASSAGETOKEN_H

#include <string>
using namespace std;

class PassageToken
{
private:
  string name;
  string text;
public:
  string getName() const { return name; }
  string getText() const { return text; }

  PassageToken();
  PassageToken(string, string);
};

#endif