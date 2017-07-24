#include <string>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <list>
#include <map>
#include "Utils.h"
#include "Expression.h"
#include "tokens.h"
#include <editline/readline.h>

using namespace std;
extern int line;
map<string, int> vars;
StatementList* stMain;

void *ParseAlloc(void *(*mallocProc)(size_t));
void ParseFree(void *p, void (*freeProc)(void*));
void Parse(void *yyp, int yymajor, TokenInfo *yyminor);
int nextToken(struct TokenInfo *info);

int main(){
  stMain = new StatementList;
  void *parser = ParseAlloc(malloc);
  struct TokenInfo *ti = new TokenInfo;


  
  int token = nextToken(ti);
  while (token != TK_EOF){
    Parse(parser, token, ti);
    ti = new TokenInfo;
    token = nextToken(ti);
  }

  Parse(parser, TK_EOF, ti);
  ParseFree(parser, free);

  StatementList::iterator it = stMain->begin();
  while(it != stMain->end()){

    Statement* n = *it;
    n->Execute();
    it++;
  }
  cout<<endl;
  return 0;
}