#include "visitors.h"

TypeVisitor::TypeVisitor(Root *file, SymTab *st, Deffered *def, bool debug)
{
	this->mainFunc = NULL;
	this->symtab = st;
	this->def = def;
	if(debug)
		file->accept(this);
}

void
TypeVisitor::Visit(Root *n)
{
	types[0] = "no type";
	types[1] = "null";
	types[2] = "int";
	types[3] = "double";
	types[4] = "boolean";
	types[5] = "object";
	types[6] = "Ident";
	types[7] = "void";

	for(int i=0; i< n->classes->size(); i++)
		n->classes->at(i)->accept(this);

	if(this->mainFunc == NULL)
		this->symtab->errors->AddError("The program must have a main function ",
		 -1,
		 -1);
}

void
TypeVisitor::Visit(Class *n)
{
	n->members->accept(this);
}

void
TypeVisitor::Visit(ClassInher *n)
{
	if(n->basic->symbol != NULL)
	{
		Class *par = n->basic->symbol->clas;
		if(par != NULL)
		{
			// Add all of Parents of this->parent to this->vector of Parents
			for(int i=0; i< par->Parents->size(); i++)
				n->Parents->push_back(par->Parents->at(i));

			 //Add all of this->Childrens of this to this->parent
			/*for(int i=0; i< n->Childrens->size(); i++)
				par->Childrens->push_back(n->Childrens->at(i));*/
		}
	}
}

void
TypeVisitor::Visit(Members *n)
{
	for(int i = 0; i < n->members->size(); i++)
		n->members->at(i)->accept(this);
}

void
TypeVisitor::Visit(Member *n)
{
}

void
TypeVisitor::Visit(Global *n)
{
	n->variables->accept(this);		
}

void
TypeVisitor::Visit(Constructor *n)
{
	n->stats->accept(this);
}

void
TypeVisitor::Visit(Function *n)
{
	if( (n->name->name == "main") && (n->type == NULL) && (n->args->args->size() == 0) )
	{
		if(this->mainFunc == NULL)
			this->mainFunc = n;
		else
			this->symtab->errors->AddError("The program must have one main only int whole Program at ",
			 n->name->line,
			 n->name->column);
	}

	n->args->accept(this);
	n->stats->accept(this);
}

void
TypeVisitor::Visit(Arg  *n)
{
	n->type->accept(this);
	n->name->accept(this);
}

void
TypeVisitor::Visit(Args *n)
{
	for(int i = 0; i < n->args->size(); i++)
		n->args->at(i)->accept(this);
}

void
TypeVisitor::Visit(Variables *n)
{
	for(int i = 0; i < n->variables->size(); i++)
		n->variables->at(i)->accept(this);
}

void
TypeVisitor::Visit(Variable *n)
{
	// IDENT = expression , check if IDENT and expression have the same type.

	n->name->accept(this);

	if(n->expr != NULL)
	{
		n->expr->accept(this);
		int left = n->name->symbol->type->type;// Getting the type from the symbol table
		int right = n->expr->type;
		
		bool mismatch = false;

		switch(left)
		{
		case 2:
			if(right != 2)
				mismatch = true;
			break;
		case 3:
			if(!((right == 2)||(right == 3)))
			{
				mismatch = true;
				n->expr->type = 3;
			}

			break;
		case 4:
			if(right != 4)
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
TypeVisitor::Visit(AccessModif *n)
{
}

void
TypeVisitor::Visit(Type *n)
{
}

void
TypeVisitor::Visit(NoArrayType *n)
{
}

void
TypeVisitor::Visit(ArrayType *n)
{
}

void
TypeVisitor::Visit(Ident *n)
{
}

void
TypeVisitor::Visit(Expr *n)
{
	// empty
}

void
TypeVisitor::Visit(Incr *n)
{
	// INCREMENT IDENT, check if IDENT type is integer or double.
	int ty = n->name->symbol->type->type;
	if( (ty != 2) && (ty != 3) )
		symtab->errors->AddError("Operator \'++\' cannot be applied to operand of type \'" + types[ty],
			 n->name->line,
			 n->column);
	
	n->type = ty;
}

void
TypeVisitor::Visit(Decr *n)
{
	// DECREMENT IDENT, check if IDENT type is integer or double.
	int ty = n->name->symbol->type->type;
	if( (ty != 2) && (ty != 3) )
		symtab->errors->AddError("Operator \'--\' cannot be applied to operand of type \'" + types[ty],
			 n->name->line,
			 n->column);

	n->type = ty;
}

void
TypeVisitor::Visit(Not *n)
{
	// ! expression, check if expression type is bool.
	n->expr->accept(this);
	int ty = n->expr->type;
	if(ty != 4)
		symtab->errors->AddError("Operator \'!\' cannot be applied to operand of type \'" + types[ty],
			n->expr->line,
			n->column);

	n->type = ty;
}

void
TypeVisitor::Visit(Minus *n)
{
	// - expression, check if expression type is integer or double.
	n->expr->accept(this);
	int ty = n->expr->type;
	if( (ty != 2) && (ty != 3) )
		symtab->errors->AddError("Operator \'-\' cannot be applied to operand of type \'" + types[ty],
			 n->expr->line,
			 n->column);

	n->type = ty;
}

void
TypeVisitor::Visit(Plus *n)
{
	// - expression, check if expression type is integer or double.
	n->expr->accept(this);
	int ty = n->expr->type;
	if( (ty != 2) && (ty != 3) )
		symtab->errors->AddError("Operator \'+\' cannot be applied to operand of type \'" + types[ty],
			 n->expr->line,
			 n->column);

	n->type = ty;
}

void
TypeVisitor::Visit(Paren  *n)
{
	// '(' expression ')' type is the same of expression type.
	n->type = n->expr->type;
}

void
TypeVisitor::Visit(IdentExpr *n)
{
	// IdentExpr type is the Ident type.
	n->type = n->ident->symbol->type->type;
}

void
TypeVisitor::Visit(IdentArr *n)
{
	//X IDENT arrayIndex , check if arrayIndex the same array_level of IDENT array_level.

	n->type = n->ident->symbol->type->type;
}

void
TypeVisitor::Visit(Assign *n)
{
	// IDENT = expression, check if IDENT type is same of expression type.
	n->expr->accept(this);
	int left = n->ident->symbol->type->type;// Getting the type from the symbol table
	int right = n->expr->type;
	
	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right != 2)
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
		{
			mismatch = true;
			n->expr->type = 3;			// note: why change type from int to double
		}
		break;
	case 4:
		if(right != 4)
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Type mismatch between \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->ident->line,
		 n->column);

	n->type = left;
}

void
TypeVisitor::Visit(ArrAssign *n)
{
	//X IDENT arrayIndex = expression , check if arrayIndex the same array_level of IDENT array_level.
	//								 , check if IDENT type is same of expression type.
	n->arrayIndex->accept(this);
	n->expr->accept(this);
	int left = n->ident->symbol->type->type;// Getting the type from the symbol table
	int right = n->expr->type;
	
	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right != 2)
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
		{
			mismatch = true;
			n->expr->type = 3;			// note: why change type from int to double
		}
		break;
	case 4:
		if(right != 4)
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Type mismatch between \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->ident->line,
		 n->column);

	n->type = left;
}

void
TypeVisitor::Visit(Invoke *n)
{
	cout << "Start_Invoke" << endl;
	// IDENT '(' expr_list_e ')'.
	n->exprList->accept(this);
	bool tt = this->symtab->IsDeclared(n->ident, 2, n->exprList, def);

	if( !((n->ident->name == "Write") || (n->ident->name == "Read")) )
	{
		if(tt)
			n->type = n->ident->symbol->method->type->type;
	}
	n->type = 7;
	cout << "End_Invoke" << endl;
}

void
TypeVisitor::Visit(NewObject *n)
{
	// NEW IDENT '(' expr_list_e ')'.

	n->exprList->accept(this);
	this->symtab->IsDeclared(n->ident, 3, n->exprList, def);
	n->type = n->ident->symbol->type->type;
}

void
TypeVisitor::Visit(NewArray *n)
{
	// NEW noarraytype arrayindex.
}

void
TypeVisitor::Visit(IdentCall *n)
{
	// Ident '.' expression.
}

void
TypeVisitor::Visit(IdentArrCall *n)
{
	// Ident arrayindex '.' expression.
}

void
TypeVisitor::Visit(Equal *n)
{
	//expression EQ expression , int == (int || double) , double == (int || double) , bool == bool
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;
	
	bool mismatch = false;
	
	switch(left)
	{
	case 2:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 4:
		if(right != 4)
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Operator \'==\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}

void
TypeVisitor::Visit(NotEq *n)
{
	//expression NotEq expression , int != (int || double) , double != (int || double) , bool != bool
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;
	
	bool mismatch = false;
	
	switch(left)
	{
	case 2:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 4:
		if(right != 4)
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Operator \'!=\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}

void
TypeVisitor::Visit(Smaller *n)
{
	//expression Smaller expression , int < (int || double) , double < (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;
	
	bool mismatch = false;
	
	switch(left)
	{
	case 2:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Operator \'<\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}

void
TypeVisitor::Visit(SmallerEq *n)
{
	//expression SmallerEq expression , int <= (int || double) , double <= (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;
	
	bool mismatch = false;
	
	switch(left)
	{
	case 2:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Operator \'<=\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}

void
TypeVisitor::Visit(Larger *n)
{
	//expression Larger expression , int > (int || double) , double > (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;
	
	bool mismatch = false;
	
	switch(left)
	{
	case 2:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Operator \'>\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}

void
TypeVisitor::Visit(LargerEq *n)
{
	//expression LargerEq expression , int >= (int || double) , double >= (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;
	
	bool mismatch = false;
	
	switch(left)
	{
	case 2:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
		symtab->errors->AddError("Operator \'>=\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}
 
void
TypeVisitor::Visit(Add *n)
{
	// expression Add expression , int + (int || double) , double + (int || double)
	n->left->accept(this);
	n->right->accept(this);
	int left = n->left->type;
	int right = n->right->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type = 2;
		else if(right == 3)
		{
			n->left->type = 3;			// note: why change type from int to double
			n->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type = 3;			// note: why change type from int to double
			n->type = 3;
		}
		else if(right == 3)
			n->type = 3;
		else
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
	{
		symtab->errors->AddError("Operator \'+\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->line,
		 n->column);
		n->type = 1;
	}
}

void
TypeVisitor::Visit(Sub *n)
{
	// expression Sub expression , int - (int || double) , double - (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type = 2;
		else if(right == 3)
		{
			n->left->type = 3;			// note: why change type from int to double
			n->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type = 3;			// note: why change type from int to double
			n->type = 3;
		}
		else if(right == 3)
			n->type = 3;
		else
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
	{
		symtab->errors->AddError("Operator \'-\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->line,
		 n->column);
		n->type = 1;
	}
}

void
TypeVisitor::Visit(Mult *n)
{
	// expression Mult expression , int * (int || double) , double * (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type = 2;
		else if(right == 3)
		{
			n->left->type = 3;			// note: why change type from int to double
			n->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type = 3;			// note: why change type from int to double
			n->type = 3;
		}
		else if(right == 3)
			n->type = 3;
		else
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
	{
		symtab->errors->AddError("Operator \'*\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->line,
		 n->column);
		n->type = 1;
	}
}

void
TypeVisitor::Visit(Div *n)
{
	// expression Div expression , int / (int || double) , double / (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type = 3;
		else if(right == 3)
		{
			n->left->type = 3;			// note: why change type from int to double
			n->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type = 3;			// note: why change type from int to double
			n->type = 3;
		}
		else if(right == 3)
			n->type = 3;
		else
			mismatch = true;
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
	{
		symtab->errors->AddError("Operator \'/\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->line,
		 n->column);
		n->type = 1;
	}
}

void
TypeVisitor::Visit(Mod *n)
{
	// expression Mod expression , int % (int || double) , double % (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;

	if( !((left == 2) && (right == 3)) )
	{
		symtab->errors->AddError("Operator \'%\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->line,
		 n->column);
		n->type = 1;
	}
	n->type = 2;
}

void
TypeVisitor::Visit(And *n)
{
	// expression And expression, check if expressions have boolean type and return bool.
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;

	if( (left != 4) || (right != 4) )
		symtab->errors->AddError("Operator \'&&\' cannot be applied to operands of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}

void
TypeVisitor::Visit(Or *n)
{
	// expression Or expression, check if expressions have boolean type and return bool.
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type;
	int right = n->right->type;

	if( (left != 4) || (right != 4) )
		symtab->errors->AddError("Operator \'||\' cannot be applied to operands of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type = 4;
}

void
TypeVisitor::Visit(Is *n)
{
	//X expression IS type, check if expression and type have the same type and return bool.
	
	n->type = 4;
}

void
TypeVisitor::Visit(Cast *n)
{
	// ( basictype ) expression, the new type is basictype type.
	/*if( ((n->typ->type == 2) || (n->typ->type == 3)) && 
		((n->expr->type == 2) || (n->expr->type == 3)) )
		n->type = n->typ->type;
	else
	{
		symtab->errors->AddError("Cannot convert from \'" + types[n->expr->type] + "\' to \'" + types[n->typ->type] + "\'",
		 n->expr->line,
		 n->column);
	}
	n->type = 1;*/
}

void
TypeVisitor::Visit(Integer *n)
{
	n->type = 2;
}

void
TypeVisitor::Visit(Real *n)
{
	n->type = 3;
}

void
TypeVisitor::Visit(True *n)
{
	n->type = 4;
}

void
TypeVisitor::Visit(False *n)
{
	n->type = 4;
}

void
TypeVisitor::Visit(This *n)
{
	n->type = 0;
}

void
TypeVisitor::Visit(Null *n)
{
	n->type = 1;
}


void
TypeVisitor::Visit(ArrayIndex *n)
{
	// empty
}
void
TypeVisitor::Visit(ArrayIndex_1 *n)
{
	// ArrayIndex must be integer.
	n->expr1->accept(this);
	int index_1 = n->expr1->type;

	if(index_1 != 2)
		symtab->errors->AddError("Array index cannot be \'" + types[index_1],
			 n->expr1->line,
			 n->column);
}
void
TypeVisitor::Visit(ArrayIndex_2 *n)
{
	// ArrayIndex must be integer.
	n->expr1->accept(this);
	n->expr2->accept(this);

	int index_1 = n->expr1->type;
	int index_2 = n->expr2->type;

	if(index_1 != 2)
		symtab->errors->AddError("Array index cannot be \'" + types[index_1],
			 n->expr1->line,
			 n->column);

	if(index_2 != 2)
		symtab->errors->AddError("Array index cannot be \'" + types[index_2],
			 n->expr2->line,
			 n->column);
}
void
TypeVisitor::Visit(ArrayIndex_3 *n)
{
	// ArrayIndex must be integer.
	n->expr1->accept(this);
	n->expr2->accept(this);
	n->expr3->accept(this);

	int index_1 = n->expr1->type;
	int index_2 = n->expr2->type;
	int index_3 = n->expr3->type;

	if(index_1 != 2)
		symtab->errors->AddError("Array index cannot be \'" + types[index_1],
			 n->expr1->line,
			 n->column);

	if(index_2 != 2)
		symtab->errors->AddError("Array index cannot be \'" + types[index_2],
			 n->expr2->line,
			 n->column);

	if(index_3 != 2)
		symtab->errors->AddError("Array index cannot be \'" + types[index_3],
			 n->expr3->line,
			 n->column);
}

void
TypeVisitor::Visit(ExprList *n)
{
	for(int i=0; i < n->exprList->size(); i++)
		n->exprList->at(i)->accept(this);
}

void
TypeVisitor::Visit(Stat *n)
{
	// empty
}

void
TypeVisitor::Visit(Stats *n)
{
	for(int i = 0; i < n->stats->size(); i++)
		n->stats->at(i)->accept(this);
}

void
TypeVisitor::Visit(If *n)
{
	n->expr->accept(this);
	int ty = n->expr->type;
	if(ty != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[ty] + "\' and \'bool\'",
		 n->expr->line,
		 n->column);

	n->stat->accept(this);
}

void
TypeVisitor::Visit(IfElse *n)
{
	n->expr->accept(this);
	int ty = n->expr->type;
	if(ty != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[ty] + "\' and \'bool\'",
		 n->expr->line,
		 n->column);

	n->stat->accept(this);
	n->elseStat->accept(this);
}

void
TypeVisitor::Visit(While *n)
{
	n->expr->accept(this);
	int ty = n->expr->type;
	if(ty != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[ty] + "\' and \'bool\'",
		 n->expr->line,
		 n->column);

	n->stat->accept(this);
}

void
TypeVisitor::Visit(For *n)
{/*
	if(n->variables_e != NULL)
		n->variables_e->accept(this);

	if(n->exprCond != NULL)
		n->exprCond->accept(this);

	n->exprCount->accept(this);
	n->stat->accept(this);

	int cond = n->exprCond->type;
	int count = n->exprCount->type;
	
	if(cond != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[cond] + "\' and \'bool\'",
		 n->exprCond->line,
		 n->column);
	if(count != 2)
		symtab->errors->AddError("Type mismatch between \'" + types[count] + "\' and \'int\'",
		 n->exprCount->line,
		 n->column);
	if(count != 3)
		symtab->errors->AddError("Type mismatch between \'" + types[count] + "\' and \'double\'",
		 n->exprCount->line,
		 n->column);*/
}

void
TypeVisitor::Visit(ExprStat *n)
{
	n->expr->accept(this);	
}

void
TypeVisitor::Visit(VariablesStat *n)
{
	n->type->accept(this);
	n->variables->accept(this);
}

void
TypeVisitor::Visit(Semi *n)
{
	// empty
}

void
TypeVisitor::Visit(Block *n)
{
	n->stats->accept(this);
}

void
TypeVisitor::Visit(Return *n)
{
	// RETURN IDENT , 
	/*n->expr->accept(this);
	int left = ((Function *)(n->father))->exprType->type;
	int right = n->expr->type;
	
	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right != 2)
			mismatch = true;
		break;
	case 3:
		if(!((right == 2)||(right == 3)))
		{
			mismatch = true;
			n->expr->type = 3;
		}

		break;
	case 4:
		if(right != 4)
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
TypeVisitor::Visit(Variables_e *n)
{
	n->type->accept(this);
	n->variables->accept(this);	
}