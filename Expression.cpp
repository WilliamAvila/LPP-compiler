#include <sstream>
#include <string>
#include "Expression.h"
#include "Statement.h"

using namespace std;


extern map<string, int> vars;
map<string, VValue> sTable;
map<string, Method*> mTable;
VValue returnValue;

string getTextForEnum(int enumVal) {
    switch (enumVal) {
        case BOOLEAN:
            return "Boolean";
        case INTEGER:
            return "Int";
        case CHARACTER:
            return "Char";

        default:
            return "Not recognized..";
    }
}

#define IMPLEMENT_RELATIONAL_OPERATION(CLASS,OP) 								\
    VValue CLASS::Evaluate()													\
    {																			\
        VValue val;																\
        VValue e1,e2;															\
        val.type=BOOLEAN;														\
        e1=expr1->Evaluate();													\
        e2=expr2->Evaluate();													\
        if(e1.type==INTEGER && e2.type==INTEGER){										\
                val.type=BOOLEAN;													\
                val.u.bvalue=e1.IntValue() OP e2.IntValue(); 						\
        }else if(e1.type==INTEGER && e2.type==BOOLEAN){									\
                val.u.bvalue=e1.IntValue() OP e2.BoolValue(); 						\
                                                                                                                                                        \
        }else if(e2.type==INTEGER && e1.type==BOOLEAN){									\
                val.u.bvalue=e2.IntValue() OP e1.BoolValue(); 						\
                                                                                                                                                        \
        }else if(e2.type==BOOLEAN && e1.type==BOOLEAN){									\
                val.u.bvalue=e2.BoolValue() OP e1.BoolValue(); 						\
                                                                                                                                                        \
        }else if(e2.type==CHARACTER && e1.type==CHARACTER){									\
                val.u.cvalue=e2.CharValue() OP e1.CharValue(); 						\
                                                                                                                                                        \
        }else{																	\
                printf("Se esperaba tipo Booleano, Entero o Caracter"); \
                exit(0);								\
        }																		\
        return val;																\
    }	

#define IMPLEMENT_ARITHMETIC_OPERATION(CLASS,OP) 								\
	VValue CLASS::Evaluate()													\
	{																			\
		VValue val;																\
		VValue e1,e2;															\
																				\
		e1=expr1->Evaluate();													\
		e2=expr2->Evaluate();													\
		val.type=INTEGER;																	\
	 	if(e1.type==INTEGER && e2.type==INTEGER){										\
	 																\
                        val.isArray=false;                                                                        \
                        val.u.ivalue=e1.IntValue() OP e2.IntValue(); 						\
                }																		\
	 	return val;																\
 	}
IMPLEMENT_RELATIONAL_OPERATION(LessThanExpression, <)
IMPLEMENT_RELATIONAL_OPERATION(GreaterThanExpression, >)
IMPLEMENT_RELATIONAL_OPERATION(LessThanEqualExpression, <=)
IMPLEMENT_RELATIONAL_OPERATION(GreaterThanEqualExpression, >=)
IMPLEMENT_RELATIONAL_OPERATION(NotEqualExpression, !=)
IMPLEMENT_RELATIONAL_OPERATION(EqualExpression, ==)
IMPLEMENT_RELATIONAL_OPERATION(OrExpression, ||)
IMPLEMENT_RELATIONAL_OPERATION(AndExpression, &&)

IMPLEMENT_ARITHMETIC_OPERATION(AddExpression, +)
IMPLEMENT_ARITHMETIC_OPERATION(SubExpression, -)
IMPLEMENT_ARITHMETIC_OPERATION(MultExpression, *)
IMPLEMENT_ARITHMETIC_OPERATION(DivExpression, /)
IMPLEMENT_ARITHMETIC_OPERATION(ModExpression, %)

string MethodName;
list<VValue> ActualVariables;

VValue ExpExpression::Evaluate()													
{																	
    VValue val;														
    VValue e1,e2;													
                                                                    
    e1=expr1->Evaluate();											
    e2=expr2->Evaluate();											
    val.type=INTEGER;															
    if(e1.type==INTEGER && e2.type==INTEGER){								
                                                        
                    val.isArray=false;                                                                        \
                    val.u.ivalue= pow(e1.IntValue(), e2.IntValue()); 				
    }																
    return val;														
}

VValue IdExpression::Evaluate() {
    if (sTable.find(id) == sTable.end()) {
        if (mTable.find(MethodName) != mTable.end()) {
            if (mTable[MethodName]->LTable.find(id) != mTable[MethodName]->LTable.end()) {
                return mTable[MethodName]->LTable[id];
            }
        }
        printf("Variable %s no ha sido declarada\n", id.c_str());
        exit(0);
    }

    return sTable[id];
}

VValue RegisterIdExpression::Evaluate() {
    
    if (sTable.find(regId) == sTable.end()) {
        printf("Variable %s no ha sido declarada\n", id.c_str());
        exit(0);
    }

    return (sTable[regId]).u.registro->variables[id];
}

VValue MethodExpression::Evaluate() {
    string previous=MethodName;
    
    if (mTable.find(id) == mTable.end()) {
        printf("\nNo hay un metodo \"%s\"\n", id.c_str());
        exit(0);
    }
    
    Method* m = mTable[id];
    map<string, VValue> vars;
    vars=m->LTable;
    
    list<Expression*>::iterator it5=exprs.begin();
    ActualVariables.clear();
    while(it5!=exprs.end())
    {
        VValue v=(*it5)->Evaluate();
        ActualVariables.push_back(v);
        it5++;
    }
    
    
    if (exprs.size() != m->params.size()) {
        printf("Expected %lu parameters.\n", m->params.size());
        exit(0);
    }

    list<VValue>::iterator it2 = ActualVariables.begin();
    ParamList::iterator it3 = m->params.begin();
    while (it2 != ActualVariables.end()) {
        
        VValue v = (*it2);
        Param * p = *it3;
        if (v.type != p->type) {
            printf("Se esperaba tipo %s.\n", getTextForEnum(p->type).c_str());
            exit(0);
        }
        m->LTable[p->id] = v;

        it2++;
        it3++;
    }
    ActualVariables.clear();
    MethodName = this->id;
    //mTable[id]=m;
    
    StatementList::iterator it = m->statementBlock.begin();
    while (it != m->statementBlock.end()) {
        Statement* s = *it;
        s->Execute();
        if(s->getKind()==RETURN_STATEMENT){
            MethodName = previous;
            return returnValue;
        }
        it++;
    }
    MethodName = previous;
    m->LTable=vars;
    return returnValue;
}

VValue ArrayExpression::Evaluate() {
    
    if (sTable.find(id) == sTable.end()) {
        if (mTable.find(MethodName) != mTable.end()) {
            if (mTable[MethodName]->LTable.find(id) != mTable[MethodName]->LTable.end()) {
                return mTable[MethodName]->LTable[id];
            }
        }
        printf("Variable %s no ha sido declarada\n", id.c_str());
        exit(0);
    }
    VValue v=sTable[id];
    v.Pos=(pos.front())->Evaluate().IntValue();
    sTable[id]=v;

    return sTable[id];
}

//--------------------Statement------------------------


VValue st={st.type = INTEGER,st.u.ivalue = 0, st.isArray=false,st.ArraySize=0};

void Declaration::Execute() {
    VValue val=st;
    int dim = 0;
    if (dimension != 0) {
        val = dimension->Evaluate();
        dim = val.u.ivalue;
    }
    if(type != INTEGER && type != REAL && type != BOOLEAN)
        val.type = type;
    
    DeclItemList::iterator it = ids.begin();
    while (it != ids.end()) {
        DeclItem *id = *it;
        
        if (sTable.find(id->id) != sTable.end()) {
            printf("Variable %s ya ha sido declarada\n", id->id.c_str());
            exit(0);
        }
        if (dim  > 0) {
            val.type=type;
            val.isArray = true;
            val.ArraySize = dim;
            if(val.type==INTEGER){
                val.u.a_ivalue=new int[dim];
                for(int i=0;i<dim;i++){
                    val.setIntArrayValue(i,0);
                }
            }else{
                val.u.a_bvalue=new bool[dim];
                for(int i=0;i<dim;i++){
                    val.setBoolArrayValue(i,false);
                }
            }
        }
        sTable[id->id] = val;
        it++;
    }
}

void DeclarationRegister::Execute() {
    VValue val=st;
    val.type = REGISTER;

      DeclList::iterator it = declarations.begin();
    while (it != declarations.end()) {
         Declaration *dec = *it;
        if(dec->type != INTEGER && dec->type != REAL && dec->type != BOOLEAN)
         val.type = dec->type;
        DeclItemList::iterator it1 = dec->ids.begin();
        while (it1 != dec->ids.end()) {
                DeclItem *idItem = *it1;
             val.u.registro = new Registro();
             val.u.registro->variables[idItem->id] =  val;
              it1++;
        }
        it++;
    }
    sTable[id] = val;

}
void PrintStatement::Execute() {
    VValue result;
    list<Expression*>::iterator it = expr.begin();
    while (it != expr.end()) {
        Expression *e = *it;
        result = e->Evaluate();
        
        if(result.type == REGISTER){

        }
        if (result.type == INTEGER) {
            if(result.isArray==1){
                printf("%d", result.getIntArrayValue());
            }else{
                printf("%d", result.IntValue());
            }
        } else if (result.type == STRINGS) {
            string s = result.StringValue();
            for (int i = 0; i < s.size(); i++) {

                if (s.at(i) == '\\' && i + 1 < s.size() && s.at(i + 1) == 'n') {
                    printf("\n");
                    i++;
                } else if (s.at(i) == '\\' && i + 1 < s.size() && s.at(i + 1) == 't') {
                    printf("\t");
                    i++;
                } else if (s.at(i) == '\\' && i + 1 < s.size() && s.at(i + 1) == 'r') {
                    printf("\r");
                    i++;
                } else if (s.at(i) == '\\' && i + 1 < s.size() && s.at(i + 1) == 'a') {
                    printf("\a");
                    i++;
                } else if (s.at(i) == '\\' && i + 1 < s.size() && s.at(i + 1) == '\\') {
                    printf("\\");
                    i++;
                } else if(s.at(i) == '\\' && i + 1 < s.size()){
                    printf("Caracter de escape desconocido. \n");
                    exit(0);
                }else{
                    printf("%c", s.at(i));
                }

            }
        } else if (result.type == BOOLEAN) {
            if(result.isArray==1){
                printf("%s", result.getBoolArrayValue()?"Verdadero":"Falso");
            }else{
                printf("%s", result.BoolValue()?"Verdadero":"Falso");
            }
        } else if (result.type == CHARACTER) {
            printf("%c", result.CharValue());
        }
        it++;
    }
}

void ReadStatement::Execute() {
    bool method=false;
    DeclItemList::iterator it = ids.begin();
    while (it != ids.end()) {
        DeclItem *id = *it;
        int input =0;
        if (sTable.find(id->id) == sTable.end()) {
            if (mTable.find(MethodName) != mTable.end()) {
                if (mTable[MethodName]->LTable.find(id->id) != mTable[MethodName]->LTable.end()) {
                    method=true;
                }else{
                    printf("Variable %s no ha sido declarada\n", id->id.c_str());
                    exit(0);
                }
            }else{
                printf("Variable %s no ha sido declarada\n", id->id.c_str());
                exit(0);
            }
        }
        
        if(method){
            char *p;
            fflush(stdin);
            
        }else{
            sscanf("V: ","%d", &input);
        }
        
        it++;
    }
}


void AssignStatement::Execute() {
    
    VValue var=st;
    bool method=false;
    if (sTable.find(id) == sTable.end()) {
        if (mTable.find(MethodName) != mTable.end()) {
            if (mTable[MethodName]->LTable.find(id) != mTable[MethodName]->LTable.end()) {
                var= mTable[MethodName]->LTable[id];
                //method=true;
            }else{
                printf("Variable %s no ha sido declarada\n", id.c_str());
                exit(0);
            }
        }else{
            printf("Variable %s no ha sido declarada\n", id.c_str());
            exit(0);
        }
    }else{
        var=sTable[id];
        
    }
    
    VValue result = expr->Evaluate();


    result.isArray=false;
    
    if(var.isArray==1){
         
        if(dim==0){
            printf("%s es un Arreglo",id.c_str());
            exit(0);
        }
        int d=dim->Evaluate().IntValue();
        if(var.type==INTEGER){
            var.setIntArrayValue(d,result.IntValue());
        }else{
            var.setBoolArrayValue(d,result.BoolValue());
        }
        if(method){
            mTable[MethodName]->LTable[id]=var;
        }else{
            sTable[id] = var;
        }
    }else{
        if(var.type!=result.type){
            printf("Tipos incompatibles.\n");
            exit(0);
        }
              
        if(method){
            mTable[MethodName]->LTable[id]=result;
        }else{
             
            sTable[id] = result;
        }
    }
}


void AssignRegisterStatement::Execute() {
 
     if (sTable.find(id) == sTable.end()) {
        printf("Variable %s no ha sido declarada\n", id.c_str());
     }else{
        VValue result = expr->Evaluate();
         (sTable[id]).u.registro->variables[registerVar] = result;
     }
}

void IfStatement::Execute() {
    
    int result = cond->Evaluate().BoolValue();
    if (result) {
        StatementList::iterator it = trueBlock.begin();
        while (it != trueBlock.end()) {
            Statement* s = *it;
            s->Execute();
            it++;
        }
    } else if (falseBlock.size() != 0) {
        StatementList::iterator it2 = falseBlock.begin();
        while (it2 != falseBlock.end()) {
            Statement* s = *it2;
            s->Execute();
            it2++;
        }
    }
    
    
}

void CaseStatement::Execute() {
    VValue result = variable->Evaluate();
    bool executed=false;
    StatementList::iterator it = stList.begin();
    while (it != stList.end()) {
        CaseLineStatement *s = (CaseLineStatement*)*it;
        if (result.type == INTEGER) {
             list<Expression* >::iterator itExp = s->literals.begin();
                 while (itExp != s->literals.end()) {
                     Expression *e = *itExp;
                     if(e->Evaluate().IntValue() == result.IntValue()){
                         s->Execute();
                         executed=true;
                         break;
                     }
                     itExp++;
                 }
        }
        else if (result.type == REAL) {
             list<Expression* >::iterator itExp = s->literals.begin();
                 while (itExp != s->literals.end()) {
                     Expression *e = *itExp;
                     if(e->Evaluate().RealValue() == result.RealValue()){
                         s->Execute();
                         executed=true;
                         break;
                     }
                     itExp++;
                 }
        }
        else if (result.type == CHARACTER) {
             list<Expression* >::iterator itExp = s->literals.begin();
                 while (itExp != s->literals.end()) {
                     Expression *e = *itExp;
                     if(e->Evaluate().CharValue() == result.CharValue()){
                         s->Execute();
                         executed=true;
                         break;
                     }
                     itExp++;
                 }
        }
        else if (result.type == STRINGS) {
             list<Expression* >::iterator itExp = s->literals.begin();
                 while (itExp != s->literals.end()) {
                     Expression *e = *itExp;
                     if(e->Evaluate().StringValue() == result.StringValue()){
                         s->Execute();
                         executed=true;
                         break;
                     }
                     itExp++;
                 }
        }

        it++;
    }
    if(executed == false)
        defaultCase->Execute();
}
void CaseLineStatement::Execute() {
    StatementList::iterator it = stList.begin();
    while (it != stList.end()) {
        Statement* s = *it;
        s->Execute();
        it++;
    }
}

void WhileStatement::Execute() {
    while (cond->Evaluate().BoolValue()) {
        StatementList::iterator it = statementBlock.begin();
        while (it != statementBlock.end()) {
            Statement* s = *it;
            s->Execute();
            it++;
        }
    }
}

void DoWhileStatement::Execute() {

    do{
        StatementList::iterator it = statementBlock.begin();
        while (it != statementBlock.end()) {
            Statement* s = *it;
            s->Execute();
            it++;
        }
    }
    while (cond->Evaluate().BoolValue());
}

void ForStatement::Execute() {
    assignStatement->Execute();
    VValue first = sTable[((AssignStatement*)assignStatement)->id];
    VValue lastVal = last->Evaluate();
    if(first.type == INTEGER){
        while(sTable[((AssignStatement*)assignStatement)->id].IntValue() <= lastVal.IntValue()) {
            StatementList::iterator it3 = statementBlock.begin();
            while (it3 != statementBlock.end()) {
                Statement* s = *it3;
                s->Execute();
                it3++;
            }
            sTable[((AssignStatement*)assignStatement)->id].u.ivalue++;
        }
    }else{
        cout << "Soporte solo valores enteros" << endl;
    }
}

void ReturnStatement::Execute() {
    VValue v=st;
    if (expr == 0) {
        v.isArray=false;
        returnValue = v;
    } else {
        returnValue = this->expr->Evaluate();
        returnValue.isArray=false;
    }
}


void MethodStatement::Execute() {
    string previous=MethodName;
    
    if (mTable.find(id) == mTable.end()) {
        printf("\nNo hay un metodo \"%s\"\n", id.c_str());
        exit(0);
    }
    
    Method* m = mTable[id];
    map<string, VValue> vars;
    vars=m->LTable;
    
    list<Expression*>::iterator it5=exprs.begin();
    ActualVariables.clear();
    while(it5!=exprs.end())
    {
        VValue v=(*it5)->Evaluate();
        ActualVariables.push_back(v);
        it5++;
    }
    
    
    if (exprs.size() != m->params.size()) {
        printf("Expected %lu parameters.\n", m->params.size());
        exit(0);
    }

    list<VValue>::iterator it2 = ActualVariables.begin();
    ParamList::iterator it3 = m->params.begin();
    while (it2 != ActualVariables.end()) {
        
        VValue v = (*it2);
        Param * p = *it3;
        if (v.type != p->type) {
            printf("Se esperaba tipo %s.\n", getTextForEnum(p->type).c_str());
            exit(0);
        }
        m->LTable[p->id] = v;

        it2++;
        it3++;
    }
    ActualVariables.clear();
    MethodName = this->id;
    //mTable[id]=m;
    
    StatementList::iterator it = m->statementBlock.begin();
    while (it != m->statementBlock.end()) {
        Statement* s = *it;
        s->Execute();
        if(s->getKind()==RETURN_STATEMENT){
            MethodName = previous;
        }
        it++;
    }
    MethodName = previous;
    m->LTable=vars;
}

void Method::Execute() {
    if (mTable.find(id) != mTable.end()) {
        printf("Ya Existe un metodo llamado \"%s\"\n", id.c_str());
        exit(0);
    }
    if(id.find(string("main"))!=string::npos && params.size()>0){
        printf("metodo main no puede tener parametros.\n");
        exit(0);
    }
    
    ParamList::iterator it3 = this->params.begin();

    while (it3 != this->params.end()) {
        Param *s = *it3;
        if ( sTable.find(s->id) != sTable.end() || LTable.find(s->id) != LTable.end()) {
                printf("Variable \"%s\" ya existe.\n", s->id.c_str());
                exit(0);
        }
        VValue v;
        v.type = s->type;
        v.u.bvalue = false;
        v.u.cvalue = '\0';
        v.u.ivalue = 0;
        //v.u.svalue = '\0';
        v.isArray=false;
        v.ArraySize=0;
        this->LTable[s->id] = v;
        it3++;
    }

    DeclList::iterator it = this->declare.begin();
    while (it != this->declare.end()) {
        Declaration *d = *it;
        DeclItemList::iterator it2 = d->ids.begin();
        while (it2 != d->ids.end()) {
            DeclItem *s = *it2;
            if ( sTable.find(s->id) != sTable.end() || LTable.find(s->id) != LTable.end()) {
                printf("Variable \"%s\" ya existe.\n", s->id.c_str());
                exit(0);
            }
            VValue v;
            v.type = d->type;
            v.u.bvalue = false;
            v.u.cvalue = '\0';
            v.u.ivalue = 0;
            //v.u.svalue = '\0';
            v.isArray=false;
            v.ArraySize=0;
            this->LTable[s->id] = v;
            
            it2++;
        }
        it++;
    }
    mTable[this->id] = this;
}