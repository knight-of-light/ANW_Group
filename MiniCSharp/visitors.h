#ifndef	VISITORS_H
#define	VISITORS_H

#include "ast.h"
#include "symbol_table.h"

//***********************************************************************
//					Visitor
//***********************************************************************
class Visitor
{
public:
	virtual void Visit(Root *) = 0;
	virtual void Visit(Class *) = 0;
	virtual void Visit(ClassInher *) = 0;
	virtual void Visit(Members *) = 0;
	virtual void Visit(Member  *) = 0;
	virtual void Visit(Global  *) = 0;
	virtual void Visit(Constructor *) = 0;
	virtual void Visit(Function *) = 0;
	virtual void Visit(Arg  *) = 0;
	virtual void Visit(Args *) = 0;
	virtual void Visit(Variables *) = 0;
	virtual void Visit(Variable  *) = 0;
	virtual void Visit(AccessModif *) = 0;
	virtual void Visit(Type *) = 0;
	virtual void Visit(NoArrayType *) = 0;
	virtual void Visit(ArrayType *) = 0;
	virtual void Visit(Ident *) = 0;
	virtual void Visit(Expr *) = 0;
	virtual void Visit(Incr *) = 0;
	virtual void Visit(Decr *) = 0;
	virtual void Visit(Not *) = 0;
	virtual void Visit(Minus *) = 0;
	virtual void Visit(Plus *) = 0;
	virtual void Visit(Paren  *) = 0;
	virtual void Visit(IdentExpr *) = 0;
	virtual void Visit(IdentArr *) = 0;
	virtual void Visit(Assign *) = 0;
	virtual void Visit(ArrAssign *) = 0;
	virtual void Visit(Invoke *) = 0;
	virtual void Visit(NewObject *) = 0;
	virtual void Visit(Equal *) = 0;
	virtual void Visit(NotEq *) = 0;
	virtual void Visit(Smaller *) = 0;
	virtual void Visit(SmallerEq *) = 0;
	virtual void Visit(Larger *) = 0;
	virtual void Visit(LargerEq *) = 0;
	virtual void Visit(Add *) = 0;
	virtual void Visit(Sub *) = 0;
	virtual void Visit(Mult *) = 0;
	virtual void Visit(Div *) = 0;
	virtual void Visit(Mod *) = 0;
	virtual void Visit(And *) = 0;
	virtual void Visit(Or *) = 0;
	virtual void Visit(Is *) = 0;
	virtual void Visit(Cast *) = 0;
	virtual void Visit(Integer *) = 0;
	virtual void Visit(Real *) = 0;
	virtual void Visit(True *) = 0;
	virtual void Visit(False *) = 0;
	virtual void Visit(This *) = 0;
	virtual void Visit(Null *) = 0;
	virtual void Visit(ArrayIndex *) = 0;
	virtual void Visit(ArrayIndex_1 *) = 0;
	virtual void Visit(ArrayIndex_2 *) = 0;
	virtual void Visit(ArrayIndex_3 *) = 0;
	virtual void Visit(ExprList *) = 0;
	virtual void Visit(Stat *) = 0;
	virtual void Visit(Stats *) = 0;
	virtual void Visit(If *) = 0;
	virtual void Visit(IfElse *) = 0;
	virtual void Visit(While *) = 0;
	virtual void Visit(For *) = 0;
	virtual void Visit(ExprStat *) = 0;
	virtual void Visit(VariablesStat *) = 0;
	virtual void Visit(Semi *) = 0;
	virtual void Visit(Block *) = 0;
	virtual void Visit(Return *) = 0;
	virtual void Visit(Variables_e *) = 0;
};

//***********************************************************************
//					TypeVisitor
//***********************************************************************
class TypeVisitor : public Visitor
{
public:
	TypeVisitor();
	virtual void Visit(Root *);
	virtual void Visit(Class *);
	virtual void Visit(ClassInher *);
	virtual void Visit(Members *);
	virtual void Visit(Member  *);
	virtual void Visit(Global  *);
	virtual void Visit(Constructor *);
	virtual void Visit(Function *);
	virtual void Visit(Arg  *);
	virtual void Visit(Args *);
	virtual void Visit(Variables *);
	virtual void Visit(Variable  *);
	virtual void Visit(AccessModif *);
	virtual void Visit(Type *);
	virtual void Visit(NoArrayType *);
	virtual void Visit(ArrayType *);
	virtual void Visit(Ident *);
	virtual void Visit(Expr *);
	virtual void Visit(Incr *);
	virtual void Visit(Decr *);
	virtual void Visit(Not *);
	virtual void Visit(Minus *);
	virtual void Visit(Plus *);
	virtual void Visit(Paren  *);
	virtual void Visit(IdentExpr *);
	virtual void Visit(IdentArr *);
	virtual void Visit(Assign *);
	virtual void Visit(ArrAssign *);
	virtual void Visit(Invoke *);
	virtual void Visit(NewObject *);
	virtual void Visit(Equal *);
	virtual void Visit(NotEq *);
	virtual void Visit(Smaller *);
	virtual void Visit(SmallerEq *);
	virtual void Visit(Larger *);
	virtual void Visit(LargerEq *);
	virtual void Visit(Add *);
	virtual void Visit(Sub *);
	virtual void Visit(Mult *);
	virtual void Visit(Div *);
	virtual void Visit(Mod *);
	virtual void Visit(And *);
	virtual void Visit(Or *);
	virtual void Visit(Is *);
	virtual void Visit(Cast *);
	virtual void Visit(Integer *);
	virtual void Visit(Real *);
	virtual void Visit(True *);
	virtual void Visit(False *);
	virtual void Visit(This *);
	virtual void Visit(Null *);
	virtual void Visit(ArrayIndex *);
	virtual void Visit(ArrayIndex_1 *);
	virtual void Visit(ArrayIndex_2 *);
	virtual void Visit(ArrayIndex_3 *);
	virtual void Visit(ExprList *);
	virtual void Visit(Stat *);
	virtual void Visit(Stats *);
	virtual void Visit(If *);
	virtual void Visit(IfElse *);
	virtual void Visit(While *);
	virtual void Visit(For *);
	virtual void Visit(ExprStat *);
	virtual void Visit(VariablesStat *);
	virtual void Visit(Semi *);
	virtual void Visit(Block *);
	virtual void Visit(Return *);
	virtual void Visit(Variables_e *);
	SymTab *symtab;
	string types [5];
	Function *mainFunc;
};

//***********************************************************************
//					CodeVisitor
//***********************************************************************
class CodeVisitor : public Visitor
{
public:
	CodeVisitor(Root *, SymTab *st, Function *mainFunc);
	virtual void Visit(Root *);
	virtual void Visit(Class *);
	virtual void Visit(ClassInher *);
	virtual void Visit(Members *);
	virtual void Visit(Member  *);
	virtual void Visit(Global  *);
	virtual void Visit(Constructor *);
	virtual void Visit(Function *);
	virtual void Visit(Arg  *);
	virtual void Visit(Args *);
	virtual void Visit(Variables *);
	virtual void Visit(Variable  *);
	virtual void Visit(AccessModif *);
	virtual void Visit(Type *);
	virtual void Visit(NoArrayType *);
	virtual void Visit(ArrayType *);
	virtual void Visit(Ident *);
	virtual void Visit(Expr *);
	virtual void Visit(Incr *);
	virtual void Visit(Decr *);
	virtual void Visit(Not *);
	virtual void Visit(Minus *);
	virtual void Visit(Plus *);
	virtual void Visit(Paren  *);
	virtual void Visit(IdentExpr *);
	virtual void Visit(IdentArr *);
	virtual void Visit(Assign *);
	virtual void Visit(ArrAssign *);
	virtual void Visit(Invoke *);
	virtual void Visit(NewObject *);
	virtual void Visit(Equal *);
	virtual void Visit(NotEq *);
	virtual void Visit(Smaller *);
	virtual void Visit(SmallerEq *);
	virtual void Visit(Larger *);
	virtual void Visit(LargerEq *);
	virtual void Visit(Add *);
	virtual void Visit(Sub *);
	virtual void Visit(Mult *);
	virtual void Visit(Div *);
	virtual void Visit(Mod *);
	virtual void Visit(And *);
	virtual void Visit(Or *);
	virtual void Visit(Is *);
	virtual void Visit(Cast *);
	virtual void Visit(Integer *);
	virtual void Visit(Real *);
	virtual void Visit(True *);
	virtual void Visit(False *);
	virtual void Visit(This *);
	virtual void Visit(Null *);
	virtual void Visit(ArrayIndex *);
	virtual void Visit(ArrayIndex_1 *);
	virtual void Visit(ArrayIndex_2 *);
	virtual void Visit(ArrayIndex_3 *);
	virtual void Visit(ExprList *);
	virtual void Visit(Stat *);
	virtual void Visit(Stats *);
	virtual void Visit(If *);
	virtual void Visit(IfElse *);
	virtual void Visit(While *);
	virtual void Visit(For *);
	virtual void Visit(ExprStat *);
	virtual void Visit(VariablesStat *);
	virtual void Visit(Semi *);
	virtual void Visit(Block *);
	virtual void Visit(Return *);
	virtual void Visit(Variables_e *);

	Function *mainFunc;
	Root *root;
	SymTab *symtab;
	string types [5];
	int gp ;
	int lp;
	int ifno;
	int forno;
	int whileno;
	vector<int> lps;
	bool isGlobal;
};

#endif
