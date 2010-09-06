#ifndef	VISITORS_H
#define	VISITORS_H

#include "ast.h"


class TypeVisitor : public Visitor
{
public:	
	virtual void Visit( ClassDef *);
	virtual void Visit( Members *);
	virtual void Visit( Member  *);
	virtual void Visit( Global  *);
	virtual void Visit( Function *);
	virtual void Visit( Variables *);
	virtual void Visit( Variable  *);
	virtual void Visit( Args *);
	virtual void Visit( Arg  *);
	virtual void Visit( Type *);
	virtual void Visit( Ident *);
	virtual void Visit( Expr *);
	virtual void Visit( Integer *);
	virtual void Visit( Real *);
	virtual void Visit( True *);
	virtual void Visit( False *);
	virtual void Visit( Paren  *);
	virtual void Visit( IdentExpr *);
	virtual void Visit( Assign *);
	virtual void Visit( Invoke *);
	virtual void Visit( ExprList *);
	virtual void Visit( Not *);
	virtual void Visit( Minus *);
	virtual void Visit( Incr *);
	virtual void Visit( Add *);
	virtual void Visit( Mult *);
	virtual void Visit( Largser *);
	virtual void Visit( LargserEq *);
	virtual void Visit( Stat *);
	virtual void Visit( Stats *);
	virtual void Visit( ExprStat *);
	virtual void Visit( VariablesStat *);
	virtual void Visit( If *);
	virtual void Visit( IfElse *);
	virtual void Visit( While *);
	virtual void Visit( Block *);
	virtual void Visit( Return *);
	virtual void Visit(Variables_e *);

	virtual void Visit(Null *);
	virtual void Visit(Plus *);
	virtual void Visit(Decr *);
	virtual void Visit(Sub *);
	virtual void Visit(Div *);
	virtual void Visit(Mod *);
	virtual void Visit(Smaller *);
	virtual void Visit(SmallerEq *);
	virtual void Visit(Equal *);
	virtual void Visit(NotEq *);
	virtual void Visit(Or *);
	virtual void Visit(And *);
	virtual void Visit(For *);

	SymTab *symtab;
	string types [5];
};

#endif
