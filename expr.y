%token_type {TokenInfo *}



%include{
    #include <cstdio>
    #include <string>
    #include <map>
    #include <list>
    #include <vector>
    #include <stdlib.h>
    #include <sstream>
    #include "Utils.h"
    #include "Expression.h"
    #include "tokens.h"

    using namespace std;

    extern int line;
    extern char* mtext;

    extern StatementList* stMain;

    void addToList(DeclList* regDecl, DeclList* declList, MethodList* methods, StatementList* source, StatementList* target){

        DeclList::iterator itR = regDecl->begin();
        while(itR != regDecl->end()){
            Statement* s = *itR;
            s->Execute();
            itR++;
        }

        DeclList::iterator itD = declList->begin();
        while(itD != declList->end()){
            Statement* n = *itD;
            n->Execute();
            itD++;
        }
        
        if (methods != 0) {
           
             MethodList::iterator itM = methods->begin();

             while (itM != methods->end()) {
                 Method* m = *itM;
                 m->Execute();
                 itM++;
             }
       }

        StatementList::iterator it = source->begin();
        while (it!=source->end()){
          Statement* n = *it;
          target->push_back(n);
          it++;
        }

    }
}

%token_destructor { if ($$ != NULL) delete $$; }
%default_type {int}
%token_prefix TK_

%type expr    {Expression*}
%type expresion_simple {Expression*}
%type term    {Expression*}
%type factor  {Expression*}
%type exprl   {ExpressionList*}
%type potencia  {Expression*}
%type variable    {Expression*}
%type variable_simple    {Expression*}
%type lista_literales    {ExpressionList*}
%type literal    {Expression*}



%type declaration {Declaration *}
%type opt_declarations {DeclList *}
%type declaration_list {DeclList *}
%type id_listP {DeclItemList *}
%type type {VarType}
%type method_type {MethodType}
%type reg_definition {DeclarationRegister *}
%type register_definitions {DeclList *}
%type opt_register_definitions {DeclList *}


%type st  {Statement*}
%type list_st {StatementList*}
%type prop_asignar {Statement*}
%type prop_escriba {Statement*}
%type prop_lea {Statement*}
%type prop_si {Statement*}
%type prop_mientras {Statement*}
%type prop_para {Statement*}
%type prop_caso {Statement*}
%type linea_caso {Statement*}
%type caso_sino {Statement*}
%type prop_return {Statement*}
%type lista_casos {StatementList*}
%type proposicion_compuesta {StatementList*}
%type declaraciones_subprogramas {MethodList*}
%type declaracion_sub {Method*}
%type declaracion_parametros {ParamList*}
%type argumentos {ParamList*}
%type decl_parametro {Param*}
%type opt_methods {MethodList*}
%type method_call_statement {Statement*}

%type prop_repita {Statement*}


%type text_list {Statement*}
%type lista_enteros {ExpressionList*}

%syntax_error{
    cout << "Syntax Error. Line: " << line << endl;
   
}

%start_symbol start

start ::= input EOF. 


input ::= opt_register_definitions(B) opt_declarations(C) opt_methods(D) proposicion_compuesta(E). {addToList(B, C, D, E, stMain); }

proposicion_compuesta(A) ::= INICIO list_st(B) FIN. { A = B; }

opt_methods(A) ::= declaraciones_subprogramas(B).  {A = B; }
opt_methods(A) ::= . { A = new MethodList(); }

reg_definition(A) ::= REGISTRO ID(B) declaration_list(C) FIN REGISTRO. { A = new DeclarationRegister(B->lexem,(*C)); }           


register_definitions(A) ::= register_definitions(B) reg_definition(C). { A = B;  A->push_back(C); }
register_definitions(A) ::= reg_definition(B).{ A = new DeclList();  A->push_back(B); }


opt_register_definitions(A) ::=  register_definitions(C). { A = C; }
opt_register_definitions(A) ::= . { A = new DeclList(); }

declaraciones_subprogramas(A) ::= declaraciones_subprogramas(B)  declaracion_sub(C).  { A = B; A->push_back(C); }
declaraciones_subprogramas(A) ::= declaracion_sub(B).  { A = new MethodList(); A->push_back(B); }

declaracion_sub(A) ::= FUNCION ID(B) argumentos(C) COLON method_type(D) declaration_list(E) proposicion_compuesta(F). { A = new Method(D,B->lexem,(*C),(*E),(*F)); }
declaracion_sub(A) ::= PROCEDIMIENTO ID(B) argumentos(C) opt_declarations(D) proposicion_compuesta(E). { A = new Method(M_VOID,B->lexem,(*C),(*D),(*E));}


argumentos(A) ::= OPAR declaracion_parametros(B) CPAR. { A = B; }
argumentos(A) ::=  . { A = new ParamList(); }

declaracion_parametros(A) ::= declaracion_parametros(B) COMMA decl_parametro(C).  { A = B; A->push_back(C); }
declaracion_parametros(A) ::= decl_parametro(B).  { A = new ParamList(); A->push_back(B);}

decl_parametro(A) ::= VAR type(B) ID(C).  { A = new Param(B, C->lexem); }
decl_parametro(A) ::= type(B) ID(C). { A = new Param(B, C->lexem); }
decl_parametro(A) ::=  ARREGLO OBRACKET lista_enteros(B) CBRACKET DE type(C) ID(D). { A = new Param(C, D->lexem, (*B)); }


opt_declarations(A) ::= declaration_list(C). { A = C; }
opt_declarations(A) ::= . { A = new DeclList(); }


declaration_list(A) ::= declaration_list(B) declaration(C).   { A = B;  A->push_back(C); }
declaration_list(A) ::= declaration(B). { A = new DeclList();  A->push_back(B); }


declaration(A) ::= type(B) id_listP(C).	{ A = new Declaration(B,(*C),list<Expression *>()); }
declaration(A) ::=  ARREGLO OBRACKET lista_enteros(B) CBRACKET DE type(C) id_listP(D). { A = new Declaration(C,(*D),(*B)); }



lista_enteros(A) ::= lista_enteros(B) COMMA ENTERO(C) . { A = B; A->push_back(new IntExpression(C->value)); }
lista_enteros(A) ::= ENTERO(B). { A = new ExpressionList; A->push_back(new IntExpression(B->value)); }

id_listP(A) ::= id_listP(B) COMMA ID(C). { A = B; A->push_back(new DeclItem(C->lexem)); }
id_listP(A) ::=  ID(B). { A = new DeclItemList(); A->push_back(new DeclItem(B->lexem));}



method_type(A) ::= ENTERO.  { A = M_INT; }
method_type(A) ::= BOOLEANO. { A = M_BOOLEAN; }
method_type(A) ::= REAL. { A = M_REAL; }
method_type(A) ::= CADENA. { A = M_CADENA; }


type(A) ::= ENTERO.  { A = INTEGER; }
type(A) ::= BOOLEANO. { A = BOOLEAN; }
type(A) ::= REAL. { A = REAL; }
type(A) ::= CARACTER. { A = CHARACTER; }
type(A) ::= CADENA OBRACKET exprl CBRACKET. { A = STRINGS; }


list_st(A) ::=  list_st(B) st(C).  {A = B; A->push_back(C); }
list_st(A) ::=  st(B). {A = new StatementList; A->push_back(B); }

st(A) ::= prop_asignar(B). { A = B; }
st(A) ::= prop_escriba(B). { A = B; }
st(A) ::= prop_lea(B). { A = B; }
st(A) ::= prop_si(B). { A = B; }
st(A) ::= prop_mientras(B). { A = B; }
st(A) ::= prop_repita(B). { A = B; }
st(A) ::= prop_para(B). { A = B; }
st(A) ::= prop_caso(B). { A = B; }
st(A) ::= prop_return(B). { A = B; }  
st(A) ::= method_call_statement(B). { A = B; }


method_call_statement(A) ::= LLAMAR ID(B) OPAR exprl(C) CPAR. { A = new MethodStatement(B->lexem,(*C));}

prop_return(A) ::= RETORNE expr(B). 	{ A = new ReturnStatement(B); }

prop_caso(A) ::= CASO variable(B) lista_casos(C) caso_sino(D) FIN CASO. { A = new CaseStatement(B,(*C),D);  }

lista_casos(A) ::= linea_caso(B) lista_casos(C). { A = C; A->push_back(B); }
lista_casos(A) ::= linea_caso(B). { A = new StatementList; A->push_back(B); }

caso_sino(A) ::= SINO COLON list_st(B). { A = new CaseLineStatement(list<Expression *>(),(*B)); }
caso_sino(A) ::= . { A = 0; }

linea_caso(A) ::= lista_literales(B) COLON list_st(C).  { A = new CaseLineStatement((*B),(*C)); }

lista_literales(A) ::= literal(B) COMMA lista_literales(C). { A = C; A->push_back(B); }
lista_literales(A) ::= literal(B). { A = new ExpressionList; A->push_back(B); }

literal(A) ::= ENTERO(B). { A = new IntExpression(B->value); }
literal(A) ::= REAL(B). { A = new RealExpression(B->real_v); }
literal(A) ::= CARACTER(B). { A = new CharExpression(B->char_v); }
literal(A) ::= STRING(B). { A = new StringExpression(B->string_v); }


prop_asignar(A) ::= ID(B) EQUAL expr(C). { A = new AssignStatement(B->lexem,0,0,C); }
prop_asignar(A) ::= ID(B) OBRACKET expr(C) CBRACKET EQUAL expr(D). { A = new AssignStatement(B->lexem,C,0,D); }
prop_asignar(A) ::= ID(B)  OBRACKET expr(C) COMMA expr(D) CBRACKET EQUAL expr(E). {  A = new AssignStatement(B->lexem,C,D,E); }
prop_asignar(A) ::= ID(B) DOT ID(C) EQUAL expr(D).  { ;A = new AssignRegisterStatement(B->lexem,C->lexem, D); }

prop_escriba(A) ::= ESCRIBA  exprl(B). { A = new PrintStatement((*B)); }

prop_lea(A) ::= LEA  id_listP(B). { A = new ReadStatement((*B)); }

prop_si(A) ::= SI expr(B) ENTONCES list_st(C) FIN SI. { A = new IfStatement(B,(*C),list<Statement *>()); }
prop_si(A) ::= SI expr(B) ENTONCES list_st(C) SINO list_st(D) FIN SI. { A = new IfStatement(B,(*C),(*D)); }


prop_mientras(A) ::= MIENTRAS expr(B) HAGA list_st(C) FIN MIENTRAS. { A = new WhileStatement(B,(*C)); }

prop_repita(A) ::= REPITA list_st(B) HASTA  expr(C). { A = new DoWhileStatement(C,(*B));  }

prop_para(A) ::= PARA prop_asignar(B) HASTA expr(C) HAGA list_st(D) FIN PARA.  { A = new ForStatement(B,C,(*D)); }


exprl(A) ::= exprl(B) COMMA expr(C). { A = B; A->push_back(C); }
exprl(A) ::= expr(B). { A = new ExpressionList; A->push_back(B); }


expr(A) ::= expresion_simple(B) LT expresion_simple(C). { A = new LessThanExpression(B, C);  }
expr(A) ::= expresion_simple(B) GT expresion_simple(C). { A = new GreaterThanExpression(B, C);  }
expr(A) ::= expresion_simple(B) LE expresion_simple(C). { A = new LessThanEqualExpression(B, C);  }
expr(A) ::= expresion_simple(B) GE expresion_simple(C). { A = new GreaterThanEqualExpression(B, C);  }
expr(A) ::= expresion_simple(B) NE expresion_simple(C). { A = new NotEqualExpression(B, C);  }
expr(A) ::= expresion_simple(B) EQ expresion_simple(C). { A = new EqualExpression(B, C);  }

expr(A) ::= expresion_simple(B). { A = B; }


expresion_simple(A) ::= expresion_simple(B) PLUS term(C).   { A = new AddExpression(B, C); }
expresion_simple(A) ::= expresion_simple(B) MINUS term(C).   { A = new SubExpression(B, C); }
expresion_simple(A) ::= expresion_simple(B) OR term(C).   { A = new OrExpression(B, C); }
expresion_simple(A) ::= term(B).   { A = B; }

term(A) ::= term(B) TIMES potencia(C). { A = new MultExpression(B, C); }
term(A) ::= term(B) DIVIDE potencia(C). { A = new DivExpression(B, C); }
term(A) ::= term(B) MOD potencia(C). { A = new ModExpression(B, C); }
term(A) ::= term(B) DIV potencia(C). { A = new DivExpression(B, C); }
term(A) ::= term(B) AND potencia(C). { A = new AndExpression(B, C); }
term(A) ::= potencia(B).              { A = B; }

potencia(A) ::= factor(B) EXP potencia(C).  { A = new ExpExpression(B, C); }
potencia(A) ::= factor(B).              { A = B; }

factor(A) ::= variable(B).              { A = B; }
factor(A) ::= ID(B) OPAR exprl(C) CPAR. { A = new MethodExpression(B->lexem,(*C));}
factor(A) ::= OPAR expr(B) CPAR.  { A = B; }
factor(A) ::= ENTERO(B). {  A = new IntExpression(B->value); }
factor(A) ::= REAL(B).  { A = new RealExpression(B->real_v); }
factor(A) ::= STRING(B).  { A = new StringExpression(B->string_v); }
factor(A) ::= CARACTER(B).  { A = new CharExpression(B->char_v); }
factor(A) ::= VERDADERO(B).  { A = new IntExpression(B->value); }
factor(A) ::= FALSO(B).  { A = new IntExpression(B->value); }   

variable(A) ::= ID(B) DOT ID(C). { A = new RegisterIdExpression(B->lexem, C->lexem); }
variable(A) ::= variable_simple(B). { A = B; }

variable_simple(A) ::=  ID(B) OBRACKET exprl(C) CBRACKET. { A = new ArrayExpression(B->lexem,(*C));}
variable_simple(A) ::=  ID(B).   { A = new IdExpression(B->lexem); }