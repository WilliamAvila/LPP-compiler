#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <string>
#include <map>
#include <list>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;
extern map<string, int> vars;

enum ExpressionKind
{
    ADD_EXPR,
    SUB_EXPR,
    DIV_EXPR,
    MOD_EXPR,
    MULT_EXPR,
    INT_EXPR,
    REAL_EXPR,
    STRING_EXPR,
    CHAR_EXPR,
    ID_EXPR,
    EXP_EXPR,
};
enum VarType
{
    INTEGER,
    BOOLEAN,
    STRINGS,
    CHARACTER,
    FUNCTION,
    ARRAY,
    REAL,
    REGISTER
};
struct VValue;
class Registro
{
    public:
    
     map<string, VValue> variables;;
};

struct VValue
{

    int IntValue()
    {

        return u.ivalue;
    }

    int RealValue()
    {

        return u.rvalue;
    }

    bool BoolValue()
    {
        if (type == INTEGER)
        {
            return true;
        }

        return u.bvalue;
    }

    char *StringValue()
    {
        return u.svalue;
    }

    char CharValue()
    {
        return u.cvalue;
    }

    void setIntArrayValue(int pos, int val)
    {
        if (pos >= ArraySize)
        {
            printf("Out of range.\n");
            exit(0);
        }
        u.a_ivalue[pos] = val;
    }

    void setIntBidiArrayValue(int pos, int pos2, int val)
    {
        if (pos >= ArraySize)
        {
            printf("Out of range.\n");
            exit(0);
        }
        u.ab_ivalue[pos][pos2] = val;
    }

    void setBoolArrayValue(int pos, bool val)
    {
        if (pos >= ArraySize)
        {
            printf("Out of range.\n");
            exit(0);
        }
        u.a_bvalue[pos] = val;
    }
    int getIntArrayValue()
    {
        if (Pos >= ArraySize)
        {
            printf("Out of range.\n");
            exit(0);
        }
        return u.a_ivalue[Pos];
    }

    bool getBoolArrayValue()
    {
        if (Pos >= ArraySize)
        {
            printf("Out of range.\n");
            exit(0);
        }
        return u.a_bvalue[Pos];
    }

    VarType type;
    bool isArray;
    int ArraySize;
    int Pos;

    union {
        Registro *registro;
        int ivalue;
        bool bvalue;
        char *svalue;
        char cvalue;
        double rvalue;

        int *a_ivalue;
        int **ab_ivalue;
        bool *a_bvalue;
    } u;
};

class ScopedSymbolTable{
    public:
        ScopedSymbolTable();
        string id;
        map<string, VValue> symbolTable;
        VValue getValue(string methodName);
};

class Expression
{
  public:
    virtual VValue Evaluate() = 0;
};
typedef list<Expression *> ExpressionList;

class BinaryExpression : public Expression
{
  public:
    BinaryExpression(Expression *expr1, Expression *expr2)
    {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }
    Expression *expr1;
    Expression *expr2;
};

class AddExpression : public BinaryExpression
{
  public:
    AddExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2) {}

    ExpressionKind getKind() { return ADD_EXPR; }
    VValue Evaluate();
};

class SubExpression : public BinaryExpression
{
  public:
    SubExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2) {}

    ExpressionKind getKind() { return SUB_EXPR; }
    VValue Evaluate();
};

class MultExpression : public BinaryExpression
{
  public:
    MultExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2) {}

    ExpressionKind getKind() { return MULT_EXPR; }
    VValue Evaluate();
};

class DivExpression : public BinaryExpression
{
  public:
    DivExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2) {}

    ExpressionKind getKind() { return DIV_EXPR; }
    VValue Evaluate();
};

class ModExpression : public BinaryExpression
{
  public:
    ModExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2) {}

    ExpressionKind getKind() { return MOD_EXPR; }
    VValue Evaluate();
};

class ExpExpression : public BinaryExpression
{
  public:
    ExpExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2) {}

    ExpressionKind getKind() { return EXP_EXPR; }
    VValue Evaluate();
};

class IntExpression : public Expression
{
  public:
    IntExpression(int value)
    {
        this->value.type = INTEGER;
        this->value.u.ivalue = value;
    }
    ExpressionKind getKind() { return INT_EXPR; }
    VValue Evaluate()
    {
        return value;
    }

    VValue value;
};
class StringExpression : public Expression
{
  public:
    StringExpression(char *value)
    {
        this->value.type = STRINGS;
        this->value.u.svalue = value;
    }

    ExpressionKind getKind() { return STRING_EXPR; }
    VValue Evaluate() { return value; }

    VValue value;
};
class CharExpression : public Expression
{
  public:
    CharExpression(char value)
    {
        this->value.type = CHARACTER;
        this->value.u.cvalue = value;
    }

    ExpressionKind getKind() { return CHAR_EXPR; }
    VValue Evaluate() { return value; }

    VValue value;
};
class RealExpression : public Expression
{
  public:
    RealExpression(double value)
    {
        this->value.type = CHARACTER;
        this->value.u.rvalue = value;
    }

    ExpressionKind getKind() { return REAL_EXPR; }
    VValue Evaluate() { return value; }

    VValue value;
};

class IdExpression : public Expression
{
  public:
    IdExpression(string id) { this->id = id; }

    ExpressionKind getKind() { return ID_EXPR; }
    VValue Evaluate();

    string id;
};

class RegisterIdExpression : public Expression
{
  public:
    RegisterIdExpression(string regId, string id) { this->regId= regId;this->id = id; }

    ExpressionKind getKind() { return ID_EXPR; }
    VValue Evaluate();

    string id;
    string regId;
};

class LessThanExpression : public BinaryExpression
{
  public:
    LessThanExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};

class GreaterThanExpression : public BinaryExpression
{
  public:
    GreaterThanExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};

class LessThanEqualExpression : public BinaryExpression
{
  public:
    LessThanEqualExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};

class GreaterThanEqualExpression : public BinaryExpression
{
  public:
    GreaterThanEqualExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};

class NotEqualExpression : public BinaryExpression
{
  public:
    NotEqualExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};

class EqualExpression : public BinaryExpression
{
  public:
    EqualExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};
class NegativeExpression : public Expression
{
  public:
    NegativeExpression(Expression *expr)
    {
        this->expr = expr;
    }

    VValue Evaluate();
    Expression *expr;
};

class NotExpression : public Expression
{
  public:
    NotExpression(Expression *expr)
    {
        this->expr = expr;
    }

    VValue Evaluate();
    Expression *expr;
};

class OrExpression : public BinaryExpression
{
  public:
    OrExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};

class AndExpression : public BinaryExpression
{
  public:
    AndExpression(Expression *expr1, Expression *expr2) : BinaryExpression(expr1, expr2)
    {
    }

    VValue Evaluate();
};

class MethodExpression : public Expression
{
  public:
    MethodExpression(string id, ExpressionList ls)
    {
        this->id = id;
        this->exprs = ls;
    }
    VValue Evaluate();

    string id;
    ExpressionList exprs;
};

class ArrayExpression : public Expression
{
  public:
    ArrayExpression(string id, ExpressionList pos)
    {
        this->id = id;
        this->pos = pos;
    }
    VValue Evaluate();

    string id;
    ExpressionList pos;
};

//------------------------------Statement------------------------------

enum StatementKind
{
    ASSIGN_STATEMENT,
    PRINT_ST,
    DECLARATION_ST,
    READ_STATEMENT,
    FOR_STATEMENT,
    WHILE_STATEMENT,
    DO_WHILE_STATEMENT,
    IF_STATEMENT,
    CASE_STATEMENT,
    CASELINE_STATEMENT,
    METHOD_STATEMENT,
    RETURN_STATEMENT
};
enum MethodType
{
    M_VOID,
    M_INT,
    M_BOOLEAN,
    M_REAL,
    M_CADENA,
    M_ARRAY
};

class Statement
{
  public:
    virtual void Execute() = 0;
    virtual StatementKind getKind() = 0;
};

typedef list<Statement *> StatementList;

class DeclItem
{
  public:
    DeclItem(string id)
    {
        this->id = id;
    }

    DeclItem(string id, int dimension)
    {
        this->id = id;
        this->dimension = dimension;
    }
    string id;
    int dimension;
};
typedef list<DeclItem *> DeclItemList;

class Declaration : public Statement
{
  public:
    Declaration(){

    }
    Declaration(VarType type, DeclItemList ids, ExpressionList dimensions)
    {
        this->ids = ids;
        this->type = type;
        this->dimension = dimensions.front();
    }

    void Execute();

    StatementKind getKind()
    {
        return DECLARATION_ST;
    }
    DeclItemList ids;
    VarType type;
    Expression *dimension;
};

class Param
{
  public:
    Param(VarType type, string id, ExpressionList dims)
    {
        this->id = id;
        this->type = type;
    }
    Param(VarType type, string id)
    {
        this->id = id;
        this->type = type;
    }
    void Execute();
    string id;
    VarType type;
    ExpressionList dimensions;
};

typedef list<Param *> ParamList;
typedef list<Declaration *> DeclList;

class DeclarationRegister : public Declaration
{
    public:
    //Registers declaration
    DeclarationRegister(string id, DeclList declarations) : Declaration()
    {
        this->id = id;
        this->declarations = declarations;
        this->type = REGISTER;
    }
    string id;
    DeclList declarations;
    VarType type;
    void Execute();
    StatementKind getKind()
    {
        return DECLARATION_ST;
    }
};

class Method
{
  public:
    Method()
    {
    }
    Method(MethodType type, string id, ParamList params, DeclList declare, list<Statement *> statementBlock)
    {
        this->id = id;
        this->type = type;
        this->statementBlock = statementBlock;
        this->params = params;
        this->declare = declare;
    }
    void Execute();
    string id;
    MethodType type;
    list<Statement *> statementBlock;
    ParamList params;
    DeclList declare;
    map<string, VValue> LTable;
};

typedef list<Method *> MethodList;

class ReturnStatement : public Statement
{
  public:
    ReturnStatement(Expression *expr)
    {
        this->expr = expr;
    }
    void Execute();

    StatementKind getKind()
    {
        return RETURN_STATEMENT;
    }
    Expression *expr;
};

class MethodStatement : public Statement
{
  public:
    MethodStatement(string id, ExpressionList ls)
    {
        this->id = id;
        this->exprs = ls;
    }
    void Execute();

    StatementKind getKind()
    {
        return METHOD_STATEMENT;
    }

    string id;
    ExpressionList exprs;
};

class AssignStatement : public Statement
{
  public:
    AssignStatement(string id, Expression *dim, Expression *dim2, Expression *expr)
    {
        this->id = id;
        this->dim = dim;
        this->dim2 = dim2;
        this->expr = expr;
    }

    void Execute();

    StatementKind getKind()
    {
        return ASSIGN_STATEMENT;
    }

    string id;
    Expression *expr;
    Expression *dim;
    Expression *dim2;
};

class AssignRegisterStatement : public Statement
{
  public:
    AssignRegisterStatement(string id, string var, Expression *expr)
    {
        this->id = id;
        this->registerVar = var;
        this->expr = expr;
    }
    
    void Execute();

    StatementKind getKind()
    {
        return ASSIGN_STATEMENT;
    }

    string id;
    string registerVar;
    Expression *expr;
};

class PrintStatement : public Statement
{
  public:
    PrintStatement(ExpressionList expr)
    {
        this->expr = expr;
    }

    void Execute();
    StatementKind getKind() { return PRINT_ST; }

    ExpressionList expr;
};

class ReadStatement : public Statement
{
  public:
    ReadStatement(DeclItemList ids)
    {
        this->ids = ids;
    }
    void Execute();

    StatementKind getKind()
    {
        return READ_STATEMENT;
    }
    DeclItemList ids;
};

class IfStatement : public Statement
{
  public:
    IfStatement(Expression *cond, list<Statement *> trueBlock, list<Statement *> falseBlock)
    {
        this->cond = cond;
        this->trueBlock = trueBlock;
        this->falseBlock = falseBlock;
    }
    void Execute();

    StatementKind getKind()
    {
        return IF_STATEMENT;
    }
    Expression *cond;
    list<Statement *> trueBlock;
    list<Statement *> falseBlock;
};

class CaseStatement : public Statement
{
  public:
    CaseStatement(Expression *variable, list<Statement *> stList, Statement *defaultCase)
    {
        this->variable = variable;
        this->stList = stList;
        this->defaultCase = defaultCase;
    }
    void Execute();

    StatementKind getKind()
    {
        return CASE_STATEMENT;
    }
    Expression *variable;
    list<Statement *> stList;
    Statement *defaultCase;
};

class CaseLineStatement : public Statement
{
  public:
    CaseLineStatement(list<Expression *> literals, list<Statement *> stList)
    {
        this->literals = literals;
        this->stList = stList;
    }
    void Execute();

    StatementKind getKind()
    {
        return CASELINE_STATEMENT;
    }
    list<Expression *> literals;
    list<Statement *> stList;
};

class WhileStatement : public Statement
{
  public:
    WhileStatement(Expression *cond, list<Statement *> statementBlock)
    {
        this->cond = cond;
        this->statementBlock = statementBlock;
    }
    void Execute();

    StatementKind getKind()
    {
        return WHILE_STATEMENT;
    }
    Expression *cond;
    list<Statement *> statementBlock;
};

class DoWhileStatement : public Statement
{
  public:
    DoWhileStatement(Expression *cond, list<Statement *> statementBlock)
    {
        this->cond = cond;
        this->statementBlock = statementBlock;
    }
    void Execute();

    StatementKind getKind()
    {
        return DO_WHILE_STATEMENT;
    }
    Expression *cond;
    list<Statement *> statementBlock;
};

class ForStatement : public Statement
{
  public:
    ForStatement(Statement *assignStatement, Expression *last, list<Statement *> statementBlock)
    {
        this->assignStatement = assignStatement;
        this->last = last;
        this->statementBlock = statementBlock;
    }
    void Execute();

    StatementKind getKind()
    {
        return FOR_STATEMENT;
    }
    Statement *assignStatement;
    Expression *last;
    list<Statement *> statementBlock;
};

extern map<string, VValue> sTable;
extern map<string, Method *> mTable;
#endif
