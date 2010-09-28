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
	virtual void Visit(File *) = 0;
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
	virtual void Visit(Assign *) = 0;
	virtual void Visit(Invoke *) = 0;
	virtual void Visit(InvokeArr *) = 0;
	virtual void Visit(NewObject *) = 0;
	virtual void Visit(NewArr *) = 0;
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
	virtual void Visit(QualNArray *) = 0;
	virtual void Visit(QualNArray_ID_Index *) = 0;
	virtual void Visit(QualNArray_Exp_Index *) = 0;
	virtual void Visit(QualName *) = 0;
	virtual void Visit(QualName_ID *) = 0;
	virtual void Visit(QualName_Exp *) = 0;
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
	virtual void Visit(File *);
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
	virtual void Visit(Assign *);
	virtual void Visit(Invoke *);
	virtual void Visit(InvokeArr *);
	virtual void Visit(NewObject *);
	virtual void Visit(NewArr *);
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
	virtual void Visit(QualNArray *);
	virtual void Visit(QualNArray_ID_Index *);
	virtual void Visit(QualNArray_Exp_Index *);
	virtual void Visit(QualName *);
	virtual void Visit(QualName_ID *);
	virtual void Visit(QualName_Exp *);
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
};

//class TypeVisitor : public Visitor
//{
//public:	
//	virtual void Visit(File *) ;
//	virtual void Visit(Root *) ;
//	virtual void Visit(Class *) ;
//	virtual void Visit(ClassInher *);
//	virtual void Visit(Members *) ;
//	virtual void Visit(Member  *) ;
//	virtual void Visit( Global  *);
//	virtual void Visit(Constructor *);
//	virtual void Visit( Function *);
//	virtual void Visit( Variables *);
//	virtual void Visit( Variable  *);
//	virtual void Visit( Args *);
//	virtual void Visit( Arg  *);
//	virtual void Visit(AccessModif *) ;
//	virtual void Visit( Type *);
//	virtual void Visit( Ident *);
//	virtual void Visit( Expr *);
//	virtual void Visit( Cast *);
//	virtual void Visit( Integer *);
//	virtual void Visit( Real *);
//	virtual void Visit( True *);
//	virtual void Visit( False *);
//	virtual void Visit( Paren  *);
//	virtual void Visit( QualNArrExp *);
//	virtual void Visit( Assign *);
//	virtual void Visit( Invoke *);
//	virtual void Visit( ExprList *);
//	virtual void Visit( Not *);
//	virtual void Visit( Minus *);
//	virtual void Visit( Incr *);
//	virtual void Visit( Add *);
//	virtual void Visit( Mult *);
//	virtual void Visit( Larger *);
//	virtual void Visit( LargerEq *);
//	virtual void Visit( Stat *);
//	virtual void Visit( Stats *);
//	virtual void Visit( ExprStat *);
//	virtual void Visit( VariablesStat *);
//	virtual void Visit( If *);
//	virtual void Visit( IfElse *);
//	virtual void Visit( While *);
//	virtual void Visit(Semi *);
//	virtual void Visit( Block *);
//	virtual void Visit( Return *);
//	virtual void Visit(Variables_e *);
//	virtual void Visit(Null *);
//	virtual void Visit(Plus *);
//	virtual void Visit(Decr *);
//	virtual void Visit(Sub *);
//	virtual void Visit(Div *);
//	virtual void Visit(Mod *);
//	virtual void Visit(Smaller *);
//	virtual void Visit(SmallerEq *);
//	virtual void Visit(Equal *);
//	virtual void Visit(NotEq *);
//	virtual void Visit(Or *);
//	virtual void Visit(And *);
//	virtual void Visit(For *);
//	virtual void Visit(ArrayIndex *);
//	virtual void Visit(ArrayIndex_1 *);
//	virtual void Visit(ArrayIndex_2 *);
//	virtual void Visit(ArrayIndex_3 *);
//	virtual void Visit(QualName *);
//	virtual void Visit(QualName_ID *);
//	virtual void Visit(QualName_Exp *);
//	virtual void Visit(QualNArray *);
//	virtual void Visit(QualNArray_ID_Index *);
//	virtual void Visit(QualNArray_Exp_Index *);
//	SymTab *symtab;
//	string types [5];
//};

#endif
