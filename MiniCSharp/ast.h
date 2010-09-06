
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
class ClassDef;
class Members;
class Member ;
class Global ;
class Function;
class VarDecls;
class VarDecl ;
class Params;
class Param ;
class ExprType;
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
class Larger;
class LargerEq;
class Inst;
class Insts;
class ExprInst;
class VarDeclsInst;
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
	ExprType	*exprType;	
	VarDecls	*varDecls;

	Global(ExprType *, VarDecls *, int , int);
	virtual void accept(Visitor *);
};

class Function : public Member
{
public:
	ExprType	*exprType;
	Ident		*name;
	Params		*params;
	Insts		*insts;

	Function(ExprType *, Ident *, Params *, Insts *, int, int);
	Function(Ident *, Params *, Insts *, int, int);
	virtual void accept(Visitor *);
};

class VarDecls	: public Node
{
public:
	vector<VarDecl *>	*varDecls;

	VarDecls(int, int);
	VarDecls(VarDecl *, int , int );
	void AddVarDecl(VarDecl *);
	virtual void accept(Visitor *);
};

class VarDecl : public Node
{
public:
	Ident	*name;
	Expr	*expr;

	VarDecl(Ident *, int, int);
	VarDecl(Ident *, Expr *, int, int);
	virtual void accept(Visitor *);
};

class Params : public Node
{
public:
	vector<Param *>	 *params;

	Params(int, int);
	Params(Param *, int, int);
	void AddParam(Param *);
	virtual void accept(Visitor *);
};

class Param : public Node
{
public:
	ExprType	*exprType;
	Ident		*name;

	Param(ExprType *, Ident *, int, int);
	virtual void accept(Visitor *);
};

class ExprType : public Node
{
public:	
	//-1: no type, 0 = Null, 1 = int , 2 = double , 3 = boolean, 4: void
	int type;

	ExprType(int, int, int);
	virtual void accept(Visitor *);
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

class Larger  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	Larger(Expr *, Expr *, int, int);
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

class LargerEq  : public Expr
{
public:
	Expr	*left;
	Expr	*right;

	LargerEq(Expr *, Expr *, int, int);
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
//					Inst
//***********************************************************************

class Inst : public Node
{
public:
	Inst(int, int);
	virtual void accept(Visitor *);
};

class Insts : public Node
{
public:
	vector<Inst	*>	*insts;

	Insts(int, int);
	Insts(Inst *, int, int);
	void AddInst(Inst *);
	virtual void accept(Visitor *);
};

class ExprInst : public Inst
{
public:
	Expr	*expr;
	ExprInst(Expr *, int, int);
	virtual void accept(Visitor *);
};

class VarDeclsInst : public Inst
{
public:
	ExprType	*exprType;
	VarDecls	*vardecls;
	VarDeclsInst(ExprType *, VarDecls *, int, int);
	virtual void accept(Visitor *);
};

class If : public Inst
{
public:
	Expr	*expr;
	Inst	*inst;
	
	If(Expr *, Inst *, int, int);
	virtual void accept(Visitor *);
};

class IfElse : public If
{
public:
	Inst		*elseInst;

	IfElse(Expr *, Inst *, Inst *, int , int );
	virtual void accept(Visitor *);
};

class While : public Inst
{
public:
	Expr	*expr;
	Inst	*inst;
	
	While(Expr *, Inst *, int, int);
	virtual void accept(Visitor *);
};

class VarsDecl : public Node
{
public:
	ExprType	*exprType;
	VarDecls	*varDecls;
	VarsDecl(ExprType *, VarDecls *, int, int);
	virtual void accept(Visitor *);
};

class For : public Inst
{
public:
	VarsDecl	*varsDecl;
	Expr	*exprCond;
	Expr	*exprCount;
	Inst	*inst;
	
	For(VarsDecl *, Expr *, Expr *, Inst *, int , int );
	virtual void accept(Visitor *);
};

class Block : public Inst
{
public:
	Insts *insts;

	Block(Insts *, int , int );
	virtual void accept(Visitor *);
};

class Return : public Inst
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
	vector<int> *argsTypes;
	int returnType;
	Function *method;
	Sym(string n, int kind, int type);
	Sym(std::string , int kind, int type, Params * ps, int returnType, Function *meth);	
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
	bool AddSym(Ident *id, int kind, int type, Params *ps, int returnType, Function *meth);	

	void AddNewScope();
	void OutScope();

	void AddVars(VarDecls *v, ExprType *et);
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
	virtual void Visit(VarDecls *) = 0;
	virtual void Visit(VarDecl  *) = 0;
	virtual void Visit(Params *) = 0;
	virtual void Visit(Param  *) = 0;
	virtual void Visit(ExprType *) = 0;
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
	virtual void Visit(Larger *) = 0;
	virtual void Visit(LargerEq *) = 0;
	virtual void Visit(Inst *) = 0;
	virtual void Visit(Insts *) = 0;
	virtual void Visit(ExprInst *) = 0;
	virtual void Visit(VarDeclsInst *) = 0;
	virtual void Visit(If *) = 0;
	virtual void Visit(IfElse *) = 0;
	virtual void Visit(While *) = 0;
	virtual void Visit(Block *) = 0;
	virtual void Visit(Return *) = 0;
	virtual void Visit(VarsDecl *) = 0;

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

