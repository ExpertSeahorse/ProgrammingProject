#include <string>
#include <iostream>
#include "story.h"


#include <fstream>
using namespace std;

const string FILENAME = "if.html";

int main()
{
  /*
  Story* story = new Story(FILENAME);
  story->print();

  delete story;
  */
  ofstream out;
  out.open("output.txt");

  Story s(FILENAME);
  out << s.print();

  out.close();
  return 0;
}
