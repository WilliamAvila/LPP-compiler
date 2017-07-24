%option noyywrap

%{

#include <iostream>
#include <string>
#include <stdlib.h>
#include "Utils.h"
#include "tokens.h"

using namespace std;

string mtext;
int line = 1;


#define YY_DECL int nextToken(struct TokenInfo *info)

#define RETURN_TOKEN (tk) do{ \
                                info->lexem = yytext; \
                                return tk; \
                            } while (0)

%}

DIGIT [0-9]
LETTER [a-zA-Z]
CHARACTER "'"[^\\]"'"

%x str


%%


"<-" { return TK_EQUAL; }
"+" { return TK_PLUS; }
"*" { return TK_TIMES; }
"-" { return TK_MINUS; }
"/" { return TK_DIVIDE; }
"(" { return TK_OPAR; }
")" { return TK_CPAR; }
"[" { return TK_OBRACKET; }
"]" { return TK_CBRACKET; }
"," { return TK_COMMA; }
"<" { return TK_LT; }
"<=" { return TK_LE; }
">" { return TK_GT; }
">=" { return TK_GE; }
"<>" { return TK_NE; }
"=" { return TK_EQ; }
"^" { return TK_EXP; }
":" { return TK_COLON; }
"verdadero" { return TK_VERDADERO; }
"falso" { return TK_FALSO; }
"y" { return TK_AND; }
"o" { return TK_OR; }
"div" { return TK_DIV; }
"." { return TK_DOT; }
"escriba" { return TK_ESCRIBA; }
"lea" { return TK_LEA; }
\" { BEGIN(str);}

"inicio"   { return TK_INICIO; }
"fin"   { return TK_FIN; }
"entero" { return TK_ENTERO; }
"real" { return TK_REAL; }
"booleano" { return TK_BOOLEANO; } 
"caracter" { return TK_CARACTER; } 
"haga" { return TK_HAGA; }   
"hasta" { return TK_HASTA; } 
"entonces" { return TK_ENTONCES; } 
"si" { return TK_SI; }  
"sino" { return TK_SINO; }  
"mientras" { return TK_MIENTRAS; }  
"para" { return TK_PARA; }  
"repita" { return TK_REPITA; }  
"caso" { return TK_CASO; }
"funcion" { return TK_FUNCION; }
"procedimiento" { return TK_PROCEDIMIENTO; }
"var" { return TK_VAR; }
"retorne" { return TK_RETORNE; }
"llamar" { return TK_LLAMAR; }
"arreglo" { return TK_ARREGLO; }
"registro" { return TK_REGISTRO; }
"cadena" { return TK_CADENA; }
"de" { return TK_DE; }
[ \t]
\n  { line++; }

{CHARACTER} 		{ info->char_v = yytext[1]; info->type=CHAR; return TK_CARACTER; }
<str>[^\"]* 		{  info->string_v  = strdup(yytext); return TK_STRING;}
<str>\" 			{ BEGIN(INITIAL);}

{DIGIT}+ { info->value = atoi(yytext); info->type=INT; return TK_ENTERO; }
{DIGIT}+"."{DIGIT}*  { info->value = atoi(yytext); info->type=DOUBLE; return TK_REAL; }
{LETTER}({DIGIT}|{LETTER}|_)* { info->lexem = yytext; info->type=STRING; return TK_ID; }
"//"[^\n]*

.   { printf(" Linea: %d Simbolo no valido: %s\n", line, yytext); }
<<EOF>> { return TK_EOF; }





%%