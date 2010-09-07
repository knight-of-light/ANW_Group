#ifndef	VISITORS_H
#define	VISITORS_H

#include "ast.h"


class TypeVisitor : public Visitor
{
public:	
	virtual void Visit( ClassDef *);
	virtual void Visit( Fields *);
	virtual void Visit( Field  *);
	virtual void Visit( Variable  *);
	virtual void Visit( Method *);
	virtual void Visit( VarDecls *);
	virtual void Visit( VarDecl  *);
	virtual void Visit( Params *);
	virtual void Visit( Param  *);
	virtual void Visit( ExprType *);
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
	virtual void Visit( Larger *);
	virtual void Visit( LargerEq *);
	virtual void Visit( Inst *);
	virtual void Visit( Insts *);
	virtual void Visit( ExprInst *);
	virtual void Visit( VarDeclsInst *);
	virtual void Visit( If *);
	virtual void Visit( IfElse *);
	virtual void Visit( While *);
	virtual void Visit( Block *);
	virtual void Visit( Return *);
	virtual void Visit(VarsDecl *);

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
	virtual void Visit(Arr_index *);
	virtual void Visit(Arr_index_1 *);
	virtual void Visit(Arr_index_2 *);
	virtual void Visit(Arr_index_3 *);
	virtual void Visit(qual_name *);
	virtual void Visit(qual_name_id *);
	virtual void Visit(qual_name_id_exp *);
	virtual void Visit(qual_name_array *);
	virtual void Visit(qual_name_array_ident *);
	virtual void Visit(qual_name_array_ident_index *);
	virtual void Visit(qual_name_array_exp_ident *);
	virtual void Visit(qual_name_array_exp_ident_index *);

	SymTab *symtab;
	string types [5];
};

#endif
