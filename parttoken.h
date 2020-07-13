#ifndef PARTTOKEN_H
#define PARTTOKEN_H

#include <string>
using namespace std;
enum part{LINK, GOTO, SET, IF, ELSEIF, ELSE, BLOCK, TEXT, COMMAND};

class PartToken
{
private:
  string text;
  part type;
public:
  string getText() const { return text; }
  part getType() const { return type; }

  PartToken();
  PartToken(string, part);
};

#endif