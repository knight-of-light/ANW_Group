#include "visitors.h"

// The code inside the functions of this visitor is not complete yyou must complete it
//void 
//TypeVisitor::Visit(ClassDef *n) 
//{
//	types[0] = "null";
//	types[1] = "int";
//	types[2] = "double";
//	types[3] = "boolean";
//	types[4] = "void";
//	n->members->accept(this);
//}

void
TypeVisitor::Visit(Members *n)
{
	for(int i = 0; i < n->members->size(); i++)
		n->members->at(i)->accept(this);
}

void
TypeVisitor::Visit(Member  *n)
{
	
}

void
TypeVisitor::Visit(Global  *n)
{
	n->variables->accept(this);		
}

void
TypeVisitor::Visit(Function *n)
{
	n->stats->accept(this);
}

void
TypeVisitor::Visit(Variables *n)
{
	for(int i = 0; i < n->variables->size(); i++)
		n->variables->at(i)->accept(this);
}

void
TypeVisitor::Visit(Variable  *n)
{
	if(n->expr != NULL)
		{
			n->expr->accept(this);
			int left = n->name->symbol->type;// Getting the type from the symbol table
			int right = n->expr->type;
			
			bool mismatch = false;

			switch(left)
			{
			case 1:
				if(right != 1)
					mismatch = true;
				break;
			case 2:
				if(!((right == 1)||(right == 2)))
				{
					mismatch = true;
					n->expr->type = 2;
				}

				break;
			case 3:
				if(right != 3)
					mismatch = true;
				break;
			default:
				mismatch = true;
			}
			if(mismatch)
				symtab->errors->AddError("Type mismatch between \'" + types[left] + "\' and \'" + types[right] + "\'",
				 n->name->line,							
				 n->column);
			}
}


void
TypeVisitor::Visit(Args *n)
{
	
}

void
TypeVisitor::Visit(Arg  *n)
{
	
}

void
TypeVisitor::Visit(Type *n)
{
	
}

void
TypeVisitor::Visit(Ident *n)
{
	
}

void
TypeVisitor::Visit(Expr *n)
{
	
}

void
TypeVisitor::Visit(Integer *n)
{
	n->type = 1;
}

void
TypeVisitor::Visit(Real *n)
{
	n->type = 2;
}

void
TypeVisitor::Visit(True *n)
{
	n->type = 3;
}

void
TypeVisitor::Visit(False *n)
{
	n->type = 3;
}

void
TypeVisitor::Visit(Paren  *n)
{
	
}

void
TypeVisitor::Visit(QualNArrExp *n)
{
	n->type = n->qualNArray->ident->symbol->type;
}

void
TypeVisitor::Visit(Assign *n)
{
			n->expr->accept(this);
			int left = n->qualNArray->ident->symbol->type;// Getting the type from the symbol table
			int right = n->expr->type;
			
			bool mismatch = false;

			switch(left)
			{
			case 1:
				if(right != 1)
					mismatch = true;
				break;
			case 2:
				if(!((right == 1)||(right == 2)))
				{
					mismatch = true;
					n->expr->type = 2;
				}

				break;
			case 3:
				if(right != 3)
					mismatch = true;
				break;
			default:
				mismatch = true;
			}
			if(mismatch)
				symtab->errors->AddError("Type mismatch between \'" + types[left] + "\' and \'" + types[right] + "\'",
				 n->qualNArray->ident->line,							
				 n->column);
}


void
TypeVisitor::Visit(Invoke *n)
{
	n->exprList->accept(this);
	this->symtab->IsDeclared(n->qualName->ident, n->exprList);
}

void
TypeVisitor::Visit(ExprList *n)
{
	
}

void
TypeVisitor::Visit(Not *n)
{
	
}

void
TypeVisitor::Visit(Minus *n)
{
	
}

void
TypeVisitor::Visit(Incr *n)
{
	
}
 
void
TypeVisitor::Visit(Add *n)
{
	// The code in this function is not complete you must complete it to discuss all cases
	n->left->accept(this);
	n->right->accept(this);
	int left = n->left->type;
	int right = n->right->type;

	bool mismatch = false;

	switch(left)
	{
	case 1:
		if(right == 1)
			n->type = 1;
		else if(right == 2)
		{
			n->left->type = 2;
			n->type = 2;			
		}
		else
			mismatch = true;
		break;
	case 2:
		if(right == 1)
		{
			n->right->type = 2;
			n->type = 2;
		}
		else if(right == 2)
			n->type = 2;
		else
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
	{
		symtab->errors->AddError("Type mismatch between \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->line,							
		 n->column);
		n->type = 0;
	}
}

void
TypeVisitor::Visit(Mult *n)
{
	
}

void
TypeVisitor::Visit(Larger *n)
{
	
}

void
TypeVisitor::Visit(LargerEq *n)
{
	
}

void
TypeVisitor::Visit(Stat *n)
{
	
}

void
TypeVisitor::Visit(Stats *n)
{
	for(int i = 0; i < n->stats->size(); i++)
		n->stats->at(i)->accept(this);
}

void
TypeVisitor::Visit(ExprStat *n)
{
	n->expr->accept(this);	
}

void
TypeVisitor::Visit(VariablesStat *n)
{
	n->variables->accept(this);
}

void
TypeVisitor::Visit(If *n)
{
	
}

void
TypeVisitor::Visit(IfElse *n)
{
	
}

void
TypeVisitor::Visit(While *n)
{
	
}

void
TypeVisitor::Visit(Block *n)
{
	
}

void
TypeVisitor::Visit(Variables_e *n)
{
	n->variables->accept(this);	
}

void
TypeVisitor::Visit(Return *n)
{
	/*n->expr->accept(this);
	int left = ((Function *)(n->father))->exprType->type;
	int right = n->expr->type;
	
	bool mismatch = false;

	switch(left)
	{
	case 1:
		if(right != 1)
			mismatch = true;
		break;
	case 2:
		if(!((right == 1)||(right == 2)))
		{
			mismatch = true;
			n->expr->type = 2;
		}

		break;
	case 3:
		if(right != 3)
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Type mismatch in function return type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->expr->line,							
		 n->column);*/
}


void
TypeVisitor::Visit(Null *n)
{
	n->type = 0;
}

void
TypeVisitor::Visit(Plus *n)
{
	
}

void
TypeVisitor::Visit(Decr *n)
{
	
}

void
TypeVisitor::Visit(Sub *n)
{
	
}

void
TypeVisitor::Visit(Div *n)
{
	
}

void
TypeVisitor::Visit(Mod *n)
{
	
}

void
TypeVisitor::Visit(Smaller *n)
{
	
}

void
TypeVisitor::Visit(SmallerEq *n)
{
	
}

void
TypeVisitor::Visit(Equal *n){
	
}

void
TypeVisitor::Visit(NotEq *n){
	
}

void
TypeVisitor::Visit(Or *n){
	
}

void
TypeVisitor::Visit(And *n){
	
}

void
TypeVisitor::Visit(For *n){
	
}


void
TypeVisitor::Visit(ArrayIndex *n){
	
}
void
TypeVisitor::Visit(ArrayIndex_1 *n){
	
}
void
TypeVisitor::Visit(ArrayIndex_2 *n){
	
}
void
TypeVisitor::Visit(ArrayIndex_3 *n){
	
}
void
TypeVisitor::Visit(QualName *n){

}
void
TypeVisitor::Visit(QualName_ID *n){
	
}
void
TypeVisitor::Visit(QualName_Exp *n){
	
}
void
TypeVisitor::Visit(QualNArray *n){
	
}
void
TypeVisitor::Visit(QualNArray_ID_Index *n){
	
}
void
TypeVisitor::Visit(QualNArray_Exp_Index *n){
	
}