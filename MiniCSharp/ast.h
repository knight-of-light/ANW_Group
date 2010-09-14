
#ifndef	AST_H
#define AST_H

#include <iostream>
using std::cout;
using std::endl;

#include <sstream>


#include <vector>
using std::vector;

#include <string>
using std::string;

#include "hash_table.h"

class Node;
class Root;
class ClassDef;
class Members;
class Member ;
class Global ;
class Function;
class Variables;
class Variable ;
class Args;
class Arg ;
class Type;
class Ident;
class Expr;
class Integer;
class Real;
class True;
class False;
class Paren ;
class IdentExpr;
class Assign;
class Invoke;
class ExprList;
class Not;
class Minus;
class Incr;
class Add;
class Mult;
class Largser;
class LargserEq;
class Stat;
class Stats;
class ExprStat;
class VariablesStat;
class If;
class IfElse;
class While;
class Block;
class Return;

class Null;
class Plus;
class Decr;
class Sub;
class Div;
class Mod;
class Smaller;
class SmallerEq;
class Equal;
class NotEq;
class Or;
class And;
class For;

class Sym;
class Scope;
class SymTab;
class Deffered;

class Errors;

class Visitor;

class Node
{
public:
	int		line;
	int		column;
	Node	*father;

	Node(int, int);
	virtual void accept(Visitor *);
};

class Root : public Node
{
public:
	vector<ClassDef *>	*classes;

	Root(int, int);
	void AddClass(ClassDef *);
	virtual void accept(Visitor *);
};

class ClassDef : public Node
{
public:
	Ident	*name;
	Members	*members;

	ClassDef(Ident *, Members *, int, int);
	virtual void accept(Visitor *);
};


class Members : public Node
{
public:
	vector<Member *>	*members;

	Members(int, int);
	void AddMember(Member *);
	virtual void accept(Visitor *);
};

class Member : public Node
{
public:
	Member(int, int);
	virtual void accept(Visitor *);
};

class Global : public Member
{
public:
	Type	*exprType;	
	Variables	*varDecls;

	Global(Type *, Variables *, int , int);
	virtual void accept(Visitor *);
};

class Function : public Member
{
public:
	Type	*exprType;
	Ident		*name;
	Args		*params;
	Stats		*insts;

	Function(Type *, Ident *, Args *, Stats *, int, int);
	Function(Ident *, Args *, Stats *, int, int);
	virtual void accept(Visitor *);
};

class Variables	: public Node
{
public:
	vector<Variable *>	*varDecls;

	Variables(int, int);
	Variables(Variable *, int , int );
	void AddVariable(Variable *);
	virtual void accept(Visitor *);
};

class Variable : public Node
{
public:
	Ident	*name;
	Expr	*expr;

	Variable(Ident *, int, int);
	Variable(Ident *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Args : public Node
{
public:
	vector<Arg *>	 *params;

	Args(int, int);
	Args(Arg *, int, int);
	void AddArg(Arg *);
	virtual void accept(Visitor *);
};

class Arg : public Node
{
public:
	Type	*exprType;
	Ident		*name;

	Arg(Type *, Ident *, int, int);
	virtual void accept(Visitor *);
};

class Type : public Node
{
public:
	//-1: no type, 0 = Null, 1 = int , 2 = double , 3 = boolean, 4: void, 5:object, 6:ident
	int type;

	Type(int, int);
	Type(int, int, int);
	virtual void accept(Visitor *);
};

class NoArrayType : public Type
{
public:
	//-1: no type, 0 = Null, 1 = int , 2 = double , 3 = boolean, 4: void, 5:object, 6:ident
	Ident *name;

	NoArrayType(int, int, int);
	NoArrayType(Ident *, int, int);
	//virtual void accept(Visitor *);
};

class ArrayType : public Type
{
public:
	Ident *name;

	ArrayType(int, int, int);
	ArrayType(int, Ident *, int, int);
	//virtual void accept(Visitor *);
};

class Ident : public Node
{
public:
	string	name;
	Sym *symbol;
	Ident(string , int , int);
	virtual void accept(Visitor *);
};

//********************************************************
//			Expr
//********************************************************

class Expr : public Node
{
public:
	//-1: no type, 0 = Null, 1 = int , 2 = double , 3 = boolean, 4: void
	int type ;
	Expr(int, int);
	virtual void accept(Visitor *);
};

class Integer : public Expr
{
public:
	int value;

	Integer(int, int, int);
	virtual void accept(Visitor *);
};

class Real : public Expr
{
public:
	double value;

	Real(int, int, int);
	virtual void accept(Visitor *);
};

class True  : public Expr
{
public:
	True(int, int);
	virtual void accept(Visitor *);
};

class False  : public Expr
{
public:
	False(int, int);
	virtual void accept(Visitor *);
};

class Null  : public Expr
{
public:
	Null(int, int);
	virtual void accept(Visitor *);
};

class Paren  : public Expr
{
public:
	Expr	*expr;

	Paren(Expr *, int, int);
	virtual void accept(Visitor *);
};

class IdentExpr  : public Expr
{
public:
	Ident	*ident;

	IdentExpr(Ident *, int, int);
	virtual void accept(Visitor *);
};

class Assign  : public Expr
{
public:
	Ident	*ident;
	Expr	*expr;

	Assign(Ident *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Invoke  : public Expr
{
public:
	Ident		*ident;
	ExprList	*exprList;

	Invoke(Ident *, ExprList *, int, int);
	virtual void accept(Visitor *);
};

class ExprList : public Node
{
public:
	vector<Expr *>	*exprList;

	ExprList(int, int);
	ExprList(Expr *, int, int);
	void AddExpr(Expr *);
	virtual void accept(Visitor *);
};

class Not  : public Expr
{
public:
	Expr	*expr;

	Not(Expr *, int, int);
	virtual void accept(Visitor *);
};

class Plus  : public Expr
{
public:
	Expr	*expr;

	Plus(Expr *, int, int);
	virtual void accept(Visitor *);
};

class Minus  : public Expr
{
public:
	Expr	*expr;

	Minus(Expr *, int, int);
	virtual void accept(Visitor *);
};

class Incr  : public Expr
{
public:
	Ident	*ident;
	bool	isPrev;

	Incr(Ident *, bool, int, int);
	virtual void accept(Visitor *);
};

class Decr  : public Expr
{
public:
	Ident	*ident;
	bool	isPrev;

	Decr(Ident *, bool, int, int);
	virtual void accept(Visitor *);
};

class Add  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	Add(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Sub  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	Sub(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Mult  : public Expr
{
	public:
	Expr	*left;
	Expr	*right;

	Mult(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Div  : public Expr
{
	public:
	Expr	*left;
	Expr	*right;

	Div(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Mod  : public Expr
{
	public:
	Expr	*left;
	Expr	*right;

	Mod(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Smaller  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	Smaller(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Largser  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	Largser(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class SmallerEq  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	SmallerEq(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class LargserEq  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	LargserEq(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Equal  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	Equal(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class NotEq  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	NotEq(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Or  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	Or(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class And  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	And(Expr *, Expr *, int, int);
	virtual void accept(Visitor *);
};

//***********************************************************************
//					Stat
//***********************************************************************

class Stat : public Node
{
public:
	Stat(int, int);
	virtual void accept(Visitor *);
};

class Stats : public Node
{
public:
	vector<Stat	*>	*insts;

	Stats(int, int);
	Stats(Stat *, int, int);
	void AddStat(Stat *);
	virtual void accept(Visitor *);
};

class ExprStat : public Stat
{
public:
	Expr	*expr;
	ExprStat(Expr *, int, int);
	virtual void accept(Visitor *);
};

class VariablesStat : public Stat
{
public:
	Type	*exprType;
	Variables	*vardecls;
	VariablesStat(Type *, Variables *, int, int);
	virtual void accept(Visitor *);
};

class If : public Stat
{
public:
	Expr	*expr;
	Stat	*inst;
	
	If(Expr *, Stat *, int, int);
	virtual void accept(Visitor *);
};

class IfElse : public If
{
public:
	Stat		*elseStat;

	IfElse(Expr *, Stat *, Stat *, int , int );
	virtual void accept(Visitor *);
};

class While : public Stat
{
public:
	Expr	*expr;
	Stat	*inst;
	
	While(Expr *, Stat *, int, int);
	virtual void accept(Visitor *);
};

class Variables_e : public Node
{
public:
	Type	*exprType;
	Variables	*varDecls;
	Variables_e(Type *, Variables *, int, int);
	virtual void accept(Visitor *);
};

class For : public Stat
{
public:
	Variables_e	*varsDecl;
	Expr	*exprCond;
	Expr	*exprCount;
	Stat	*inst;
	
	For(Variables_e *, Expr *, Expr *, Stat *, int , int );
	virtual void accept(Visitor *);
};

class Block : public Stat
{
public:
	Stats *insts;

	Block(Stats *, int , int );
	virtual void accept(Visitor *);
};

class Return : public Stat
{
public:
	Expr	*expr;
	Return(Expr *, int, int);
	virtual void accept(Visitor *);
};


//*************************************************************

class Sym
{
public:
	//1:class 2: func, 3: global Global, 4: constructor, 5: Local Global, 6: Argument
	int kind;
	string name;
	//-1: no type, 0 = Null, 1 = int , 2 = double , 3 = boolean, 4: void
	int type;
	vector<int> *argTypes;
	int returnType;
	Function *method;
	Sym(string n, int kind, int type);
	Sym(std::string , int kind, int type, Args * ps, int returnType, Function *meth);	
};

typedef CHashTable<Sym> HashTab;

class Scope
{
public:
	Scope *father;
	vector<Scope *> *children;
	HashTab	*hashTab;

	Scope();
	void AddChild();	
};

class SymTab
{
private:
	string types [5];
public:
	Scope *current;
	Errors *errors;

	SymTab(Errors *errors);

	Sym *Lookup(string name);

	bool IsDeclared(Ident *id);
	bool IsDeclared(Ident *id, ExprList *el);
	bool IsDeclared(Ident *id, Deffered *def);
	bool AddSym(Ident *id, int kind, int type);
	bool AddSym(Ident *id, int kind, int type, Args *ps, int returnType, Function *meth);	

	void AddNewScope();
	void OutScope();

	void AddVars(Variables *v, Type *et);
};
//*************************************************************
class Deffered
{
public:
	vector<Ident *> *ids;
	//vector<Sym *> *syms;
	Deffered();
	void AddIdent(Ident *);
	void CheckAll(SymTab *symtab);
};

//*************************************************************


class Error
{
public:
	string message;
	int line;
	int column;

	Error(string message, int line, int column);
};

class Errors
{
public:
	vector<Error *> *messages;
	Errors();
	void AddError(string message, int line, int column);
	void Print();
};

//*****************************************************************

class Visitor
{
public:
	virtual void Visit(ClassDef *) = 0;
	virtual void Visit(Members *) = 0;
	virtual void Visit(Member  *) = 0;
	virtual void Visit(Global  *) = 0;
	virtual void Visit(Function *) = 0;
	virtual void Visit(Variables *) = 0;
	virtual void Visit(Variable  *) = 0;
	virtual void Visit(Args *) = 0;
	virtual void Visit(Arg  *) = 0;
	virtual void Visit(Type *) = 0;
	virtual void Visit(Ident *) = 0;
	virtual void Visit(Expr *) = 0;
	virtual void Visit(Integer *) = 0;
	virtual void Visit(Real *) = 0;
	virtual void Visit(True *) = 0;
	virtual void Visit(False *) = 0;
	virtual void Visit(Paren  *) = 0;
	virtual void Visit(IdentExpr *) = 0;
	virtual void Visit(Assign *) = 0;
	virtual void Visit(Invoke *) = 0;
	virtual void Visit(ExprList *) = 0;
	virtual void Visit(Not *) = 0;
	virtual void Visit(Minus *) = 0;
	virtual void Visit(Incr *) = 0;
	virtual void Visit(Add *) = 0;
	virtual void Visit(Mult *) = 0;
	virtual void Visit(Largser *) = 0;
	virtual void Visit(LargserEq *) = 0;
	virtual void Visit(Stat *) = 0;
	virtual void Visit(Stats *) = 0;
	virtual void Visit(ExprStat *) = 0;
	virtual void Visit(VariablesStat *) = 0;
	virtual void Visit(If *) = 0;
	virtual void Visit(IfElse *) = 0;
	virtual void Visit(While *) = 0;
	virtual void Visit(Block *) = 0;
	virtual void Visit(Return *) = 0;
	virtual void Visit(Variables_e *) = 0;

	virtual void Visit(Null *) = 0;
	virtual void Visit(Plus *) = 0;
	virtual void Visit(Decr *) = 0;
	virtual void Visit(Sub *) = 0;
	virtual void Visit(Div *) = 0;
	virtual void Visit(Mod *) = 0;
	virtual void Visit(Smaller *) = 0;
	virtual void Visit(SmallerEq *) = 0;
	virtual void Visit(Equal *) = 0;
	virtual void Visit(NotEq *) = 0;
	virtual void Visit(Or *) = 0;
	virtual void Visit(And *) = 0;
	virtual void Visit(For *) = 0;
};


#endif

