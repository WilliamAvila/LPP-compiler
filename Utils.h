#include <iostream>
#include <string>

using namespace std;

typedef struct TokenInfo TokenInfo;

enum Type{
  INT,
  STRING,
  BOOL,
  CHAR,
  DOUBLE
};

struct TokenInfo {
  int value;
  char* string_v;
  double real_v;
  char char_v;
  string lexem;
  Type type;
};