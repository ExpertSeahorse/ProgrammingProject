#include "parttoken.h"

#ifndef PASSAGETOKENIZER_H
#define PASSAGETOKENIZER_H

#include <string>
#include <vector>
using namespace std;

class PassageTokenizer
{
private:
  int current_position = 0;
  string body;
  part nextPartType;

  vector<int> distances;
  const vector<string> commands{"go-to", "set", "if", "else-if", "else"};
  string sub_body;
public:
  bool hasNextPart();
  PartToken nextPart();

  PassageTokenizer(string);
};

#endif