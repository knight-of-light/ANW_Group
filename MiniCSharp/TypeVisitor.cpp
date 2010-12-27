#include "visitors.h"

TypeVisitor::TypeVisitor(Root *file, SymTab *st, Deffered *def)
{
	this->mainFunc = NULL;
	this->symtab = st;
	this->def = def;
	this->scopeNum = 0;
	this->callNum = 0;
	this->IsStatic = false;
	this->NeedReturn = false; // In default, Cannot use return.
	this->NumReturns = 0;
	file->accept(this);
	//this->def->CheckAll(this->symtab);
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

	this->scopeNum++;
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
	this->scopeNum++;
	n->members->accept(this);

	// Check if Members are Right
	for(int i=0; i<n->members->members->size(); i++)
	{
		// Check if name of constructors ARE the same as name of class.
		if(dynamic_cast<Constructor*>(n->members->members->at(i)) != NULL )
		{
			Constructor *C = dynamic_cast<Constructor*>(n->members->members->at(i));
			if(n->name->name != C->name->name)
				this->symtab->errors->AddError("Method must have a retrun type", C->name->line, C->name->column);
		}

		// Check if name of Functins ARE NOT the same as name of class.
		else if(dynamic_cast<Function*>(n->members->members->at(i)) != NULL)
		{
			Function *F = dynamic_cast<Function*>(n->members->members->at(i));
			if(n->name->name == F->name->name)
				this->symtab->errors->AddError("Member names cannot be the same as their enclosing type", F->name->line, F->name->column);
		}

		// Check if name of Global ARE NOT the same as name of class.
		else if(dynamic_cast<Global*>(n->members->members->at(i)) != NULL)
		{
			Global *G = dynamic_cast<Global*>(n->members->members->at(i));
			for(int j=0; j<G->variables->variables->size(); j++)
				if(n->name->name == G->variables->variables->at(j)->name->name)
					this->symtab->errors->AddError("Member names cannot be the same as their enclosing type", G->variables->variables->at(j)->line, G->variables->variables->at(j)->column);
		}
	}
}

void
TypeVisitor::Visit(ClassInher *n)
{
	this->scopeNum++;
	n->members->accept(this);

	// Check if Members are Right
	for(int i=0; i<n->members->members->size(); i++)
	{
		// Check if name of constructors ARE the same as name of class.
		if(dynamic_cast<Constructor*>(n->members->members->at(i)) != NULL )
		{
			Constructor *C = dynamic_cast<Constructor*>(n->members->members->at(i));
			if(n->name->name != C->name->name )
				this->symtab->errors->AddError("Method must have a retrun type", C->name->line, C->name->column);
		}

		// Check if name of Functins ARE NOT the same as name of class.
		else if(dynamic_cast<Function*>(n->members->members->at(i)) != NULL)
		{
			Function *F = dynamic_cast<Function*>(n->members->members->at(i));
			if(n->name->name == F->name->name)
				this->symtab->errors->AddError("Member names cannot be the same as their enclosing type", F->name->line, F->name->column);
		}

		// Check if name of Global ARE NOT the same as name of class.
		else if(dynamic_cast<Global*>(n->members->members->at(i)) != NULL)
		{
			Global *G = dynamic_cast<Global*>(n->members->members->at(i));
			for(int j=0; j<G->variables->variables->size(); j++)
				if(n->name->name == G->variables->variables->at(j)->name->name)
					this->symtab->errors->AddError("Member names cannot be the same as their enclosing type", G->variables->variables->at(j)->line, G->variables->variables->at(j)->column);
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
	this->scopeNum++;
	n->args->accept(this);
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
			this->symtab->errors->AddError("The program must have one main only in whole Program at ",
			 n->name->line,
			 n->name->column);
	}

	this->scopeNum++;
	n->args->accept(this);

	if(n->type != NULL)
	{
		this->NeedReturn = true;
		this->NumReturns = 0;
	}

	n->stats->accept(this);

	if(this->NeedReturn)
	{
		// check if Function contain return keyword.
		if(this->NumReturns == 0)
			this->symtab->errors->AddError("Not all code paths return a value", n->line, n->column);

		this->NeedReturn = false;
	}
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
		int right = n->expr->type->type;
		
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
				n->expr->type->type = 3;
			}

			break;
		case 4:
			if(right != 4)
				mismatch = true;
			break;
		case 6:
			if(right == 6)
			{
				if(n->name->symbol->type->name->name == n->expr->type->name->name) // if ident type of left == ident type of right.
					break;
				else
				{
					mismatch = true;

					// see if left is father of right.
					Class *c = n->name->symbol->type->name->symbol->clas; // class type of left.
					for(int i=0; i<c->Childrens->size(); i++)
					{
						if(c->Childrens->at(i)->name == n->expr->type->name->name)
						{
							mismatch = false;
							break;
						}
					}
				}
			}
			break;
		default:
			mismatch = true;
		}
		if(mismatch)
		{
			string typeLeft = types[left];
			string typeRight = types[right];
			if(left == 6)
				typeLeft = n->name->symbol->type->name->name;
			if(right == 6)
				typeRight = n->expr->type->name->name;
			symtab->errors->AddError("Type mismatch between \'" + typeLeft + "\' and \'" + typeRight + "\'",
			 n->name->line,
			 n->column);
		}
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
	
	n->type = n->name->symbol->type;
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

	n->type = n->name->symbol->type;
}

void
TypeVisitor::Visit(Not *n)
{
	// ! expression, check if expression type is bool.
	n->expr->accept(this);
	int ty = n->expr->type->type;
	if(ty != 4)
		symtab->errors->AddError("Operator \'!\' cannot be applied to operand of type \'" + types[ty],
			n->expr->line,
			n->column);

	n->type = n->expr->type;
}

void
TypeVisitor::Visit(Minus *n)
{
	// - expression, check if expression type is integer or double.
	n->expr->accept(this);
	int ty = n->expr->type->type;
	if( (ty != 2) && (ty != 3) )
		symtab->errors->AddError("Operator \'-\' cannot be applied to operand of type \'" + types[ty],
			 n->expr->line,
			 n->column);

	n->type = n->expr->type;
}

void
TypeVisitor::Visit(Plus *n)
{
	// + expression, check if expression type is integer or double.
	n->expr->accept(this);
	int ty = n->expr->type->type;
	if( (ty != 2) && (ty != 3) )
		symtab->errors->AddError("Operator \'+\' cannot be applied to operand of type \'" + types[ty],
			 n->expr->line,
			 n->column);

	n->type = n->expr->type;
}

void
TypeVisitor::Visit(Paren  *n)
{
	// '(' expression ')' type is the same of expression type.
	n->expr->accept(this);
	n->type = n->expr->type;
}

void
TypeVisitor::Visit(IdentExpr *n)
{
	// IdentExpr type is the Ident type.
	if(this->callNum == 0)
	{
		if(this->symtab->IsDeclared(n->ident, this->scopeNum, false, false))
			n->type = n->ident->symbol->type;
	}
	else
	{
		if(this->symtab->IsDeclared(n->ident, this->callNum, false, true))
		{
			// Check from static protection level.
			int acctype = n->ident->symbol->acctype;
			if( ( (!this->IsStatic) &&  (acctype != 3) && (acctype != 4) )	// if func must be not static and it is not static.
			 || ( ( this->IsStatic) && ((acctype == 3) || (acctype == 4)) ) )	// if func must be static and it is static.
				n->type = n->ident->symbol->type;
			else
				this->symtab->errors->AddError("Member " + n->ident->name + " cannot be accessed with an instance reference", n->ident->line, n->ident->column);
		}
	}
}

void
TypeVisitor::Visit(IdentArr *n)
{
	//X IDENT arrayIndex , check if arrayIndex the same array_level of IDENT array_level.

	n->type = n->ident->symbol->type;
}

void
TypeVisitor::Visit(Assign *n)
{
	// IDENT = expression, check if IDENT type is same of expression type.
	n->expr->accept(this);
	int left = n->ident->symbol->type->type;// Getting the type from the symbol table
	int right = n->expr->type->type;
	
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
			n->expr->type->type = 3;			// note: why change type from int to double
		}
		break;
	case 4:
		if(right != 4)
			mismatch = true;
		break;
	case 6:
		if(right == 6)
		{
			if(n->ident->symbol->type->name->name != n->expr->type->name->name) // if ident type of left != ident type of right.
			{
				mismatch = true;

				// see if left is father of right.
				Class *c = n->ident->symbol->type->name->symbol->clas; // class type of left.
				for(int i=0; i<c->Childrens->size(); i++)
				{
					if(c->Childrens->at(i)->name == n->expr->type->name->name)
					{
						mismatch = false;
						break;
					}
				}
			}
		}
		break;
	default:
		mismatch = true;
	}
	if(mismatch)
	{
		string typeLeft = types[left];
		string typeRight = types[right];
		if(left == 6)
			typeLeft = n->ident->symbol->type->name->name;
		if(right == 6)
			typeRight = n->expr->type->name->name;
		symtab->errors->AddError("Type mismatch between \'" + typeLeft + "\' and \'" + typeRight + "\'",
		 n->ident->line,
		 n->column);
	}

	n->type = n->ident->symbol->type;
}

void
TypeVisitor::Visit(ArrAssign *n)
{
	//X IDENT arrayIndex = expression , check if arrayIndex the same array_level of IDENT array_level.
	//								 , check if IDENT type is same of expression type.
	n->arrayIndex->accept(this);
	n->expr->accept(this);
	int left = n->ident->symbol->type->type;// Getting the type from the symbol table
	int right = n->expr->type->type;
	
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
			n->expr->type->type = 3;			// note: why change type from int to double
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

	n->type->type = left;
}

void
TypeVisitor::Visit(Invoke *n)
{
	// IDENT '(' expr_list_e ')'.

	// Because <Invoke exprList> if callNum still != 0, the IdentExpr will don't do right check.
	// so set callNum = 0, and after finish <exprList accept>, will return callNum to its value.
	int num = 0;
	if(this->callNum != 0)
	{
		num = this->callNum;
		this->callNum = 0;
	}
	n->exprList->accept(this);
	this->callNum = num;


	if( !((n->ident->name == "Write") || (n->ident->name == "Read")) ) // if Not Write or Read func.
	{
		if(this->callNum == 0) // if Not Called by IdentCall.
		{
			if(this->symtab->IsDeclared(n->ident, 2, n->exprList, scopeNum, false))
				n->type = n->ident->symbol->method->type;
		}
		else // if function Called by IdentCall.
			if(this->symtab->IsDeclared(n->ident, 2, n->exprList, callNum, true)) // if function was declared.
			{
				// Check from static protection level.
				int acctype = n->ident->symbol->acctype;
				if( ( (!this->IsStatic) &&  (acctype != 3) && (acctype != 4) )	// if func must be not static and it is not static.
				 || ( ( this->IsStatic) && ((acctype == 3) || (acctype == 4)) ) )	// if func must be static and it is static.
					n->type = n->ident->symbol->method->type;
				else
					this->symtab->errors->AddError("Member " + n->ident->name + " cannot be accessed with an instance reference", n->ident->line, n->ident->column);
			}
	}
	else // if Write or Read func.
	{
		if(this->callNum == 0)
			n->type->type = 7;
		else
			this->symtab->errors->AddError("Undeclared Identifier '" + n->ident->name , n->ident->line, n->ident->column);
	}
}

void
TypeVisitor::Visit(NewObject *n)
{
	// NEW IDENT '(' expr_list_e ')'.

	n->exprList->accept(this);

	if(this->symtab->IsDeclared(n->ident, 3, n->exprList, this->scopeNum))
	{
		n->type->type = 6;
		n->type->name = n->ident;
	}
	else
		n->type->type = 0;

}

void
TypeVisitor::Visit(NewArray *n)
{
	// NEW noarraytype arrayindex.

	n->typ->accept(this);
	n->arrayIndex->accept(this);
	
}

void
TypeVisitor::Visit(IdentCall *n)
{
	// Ident '.' expression.

	int num = this->callNum;

	if(this->callNum == 0)
	{
		if(this->symtab->IsDeclared(n->ident, this->scopeNum, true, false))
		{
			Class *C;
			if(n->ident->symbol->clas == NULL) // if ident not found as class name.
				C = n->ident->symbol->type->name->symbol->clas;
			else // if ident found as class name.
			{
				C = n->ident->symbol->clas;
				this->IsStatic = true;
			}
			this->callNum = this->symtab->ClassToScope(C)->number;
		}
	}
	else
	{
		if(this->symtab->IsDeclared(n->ident, this->callNum, false, true))
		{
			Class *C = n->ident->symbol->type->name->symbol->clas;
			this->callNum = this->symtab->ClassToScope(C)->number;
		}
	}
	
	n->expr->accept(this);
	n->type = n->expr->type;

	if(num == 0)
	{
		this->callNum = 0;
		this->IsStatic = false;
	}
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

	int left = n->left->type->type;
	int right = n->right->type->type;
	
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

	n->type->type = 4;
}

void
TypeVisitor::Visit(NotEq *n)
{
	//expression NotEq expression , int != (int || double) , double != (int || double) , bool != bool
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;
	
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

	n->type->type = 4;
}

void
TypeVisitor::Visit(Smaller *n)
{
	//expression Smaller expression , int < (int || double) , double < (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;
	
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

	n->type->type = 4;
}

void
TypeVisitor::Visit(SmallerEq *n)
{
	//expression SmallerEq expression , int <= (int || double) , double <= (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;
	
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

	n->type->type = 4;
}

void
TypeVisitor::Visit(Larger *n)
{
	//expression Larger expression , int > (int || double) , double > (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;
	
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

	n->type->type = 4;
}

void
TypeVisitor::Visit(LargerEq *n)
{
	//expression LargerEq expression , int >= (int || double) , double >= (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;
	
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

	n->type->type = 4;
}
 
void
TypeVisitor::Visit(Add *n)
{
	// expression Add expression , int + (int || double) , double + (int || double)
	n->left->accept(this);
	n->right->accept(this);
	int left = n->left->type->type;
	int right = n->right->type->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type->type = 2;
		else if(right == 3)
		{
			n->left->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;
		}
		else if(right == 3)
			n->type->type = 3;
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
		n->type->type = 1;
	}
}

void
TypeVisitor::Visit(Sub *n)
{
	// expression Sub expression , int - (int || double) , double - (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type->type = 2;
		else if(right == 3)
		{
			n->left->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;
		}
		else if(right == 3)
			n->type->type = 3;
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
		n->type->type = 1;
	}
}

void
TypeVisitor::Visit(Mult *n)
{
	// expression Mult expression , int * (int || double) , double * (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type->type = 2;
		else if(right == 3)
		{
			n->left->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;
		}
		else if(right == 3)
			n->type->type = 3;
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
		n->type->type = 1;
	}
}

void
TypeVisitor::Visit(Div *n)
{
	// expression Div expression , int / (int || double) , double / (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;

	bool mismatch = false;

	switch(left)
	{
	case 2:
		if(right == 2)
			n->type->type = 3;
		else if(right == 3)
		{
			n->left->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;			// note: why change type from int to double
			n->type->type = 3;
		}
		else if(right == 3)
			n->type->type = 3;
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
		n->type->type = 1;
	}
}

void
TypeVisitor::Visit(Mod *n)
{
	// expression Mod expression , int % (int || double) , double % (int || double)
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;

	if( !((left == 2) && (right == 2)) )
		symtab->errors->AddError("Operator \'%\' cannot be applied to operand of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->line,
		 n->column);

	n->type->type = 2;
}

void
TypeVisitor::Visit(And *n)
{
	// expression And expression, check if expressions have boolean type and return bool.
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;

	if( (left != 4) || (right != 4) )
		symtab->errors->AddError("Operator \'&&\' cannot be applied to operands of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type->type = 4;
}

void
TypeVisitor::Visit(Or *n)
{
	// expression Or expression, check if expressions have boolean type and return bool.
	n->left->accept(this);
	n->right->accept(this);

	int left = n->left->type->type;
	int right = n->right->type->type;

	if( (left != 4) || (right != 4) )
		symtab->errors->AddError("Operator \'||\' cannot be applied to operands of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type->type = 4;
}

void
TypeVisitor::Visit(Is *n)
{
	//X expression IS type, check if expression and type have the same type and return bool.
	
	n->type->type = 4;
}

void
TypeVisitor::Visit(Cast *n)
{
	// ( basictype ) expression, the new type is basictype type.
	/*if( ((n->typ->type == 2) || (n->typ->type == 3)) && 
		((n->expr->type->type == 2) || (n->expr->type->type == 3)) )
		n->type = n->typ->type;
	else
	{
		symtab->errors->AddError("Cannot convert from \'" + types[n->expr->type->type] + "\' to \'" + types[n->typ->type->type] + "\'",
		 n->expr->line,
		 n->column);
	}
	n->type->type = 1;*/
}

void
TypeVisitor::Visit(Integer *n)
{
	n->type->type = 2;
}

void
TypeVisitor::Visit(Real *n)
{
	n->type->type = 3;
}

void
TypeVisitor::Visit(True *n)
{
	n->type->type = 4;
}

void
TypeVisitor::Visit(False *n)
{
	n->type->type = 4;
}

void
TypeVisitor::Visit(This *n)
{
	n->type->type = 0;
}

void
TypeVisitor::Visit(Null *n)
{
	n->type->type = 1;
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
	int index_1 = n->expr1->type->type;

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

	int index_1 = n->expr1->type->type;
	int index_2 = n->expr2->type->type;

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

	int index_1 = n->expr1->type->type;
	int index_2 = n->expr2->type->type;
	int index_3 = n->expr3->type->type;

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
	int ty = n->expr->type->type;
	if(ty != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[ty] + "\' and \'bool\'",
		 n->expr->line,
		 n->column);

	
	int num_ret = this->NumReturns; // save NumReturns befor check all stats.

	n->stat->accept(this);

	// check if all code paths return a value.
	if(this->NeedReturn)
	{
		if(num_ret == this->NumReturns)
			this->symtab->errors->AddError("Not all code paths return a value", n->line, n->column);
		else
			this->NumReturns = num_ret;
	}
}

void
TypeVisitor::Visit(IfElse *n)
{
	n->expr->accept(this);
	int ty = n->expr->type->type;
	if(ty != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[ty] + "\' and \'bool\'",
		 n->expr->line,
		 n->column);

	
	int num_ret = this->NumReturns; // save NumReturns befor check all stats.

	n->stat->accept(this);

	// check if all code paths return a value.
	if(this->NeedReturn)
	{
		if(num_ret == this->NumReturns)
			this->symtab->errors->AddError("Not all code paths return a value", n->line, n->column);
		else
			this->NumReturns = num_ret;
	}
	
	n->elseStat->accept(this);

	// check if all code paths return a value.
	if(this->NeedReturn)
	{
		if(num_ret == this->NumReturns)
			this->symtab->errors->AddError("Not all code paths return a value", n->elseStat->line, n->elseStat->column);
		else
			this->NumReturns = num_ret;
	}
}

void
TypeVisitor::Visit(While *n)
{
	n->expr->accept(this);
	int ty = n->expr->type->type;
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
	this->scopeNum++;
	n->stats->accept(this);
}

void
TypeVisitor::Visit(Return *n)
{
	// RETURN IDENT ,
	if(this->NeedReturn) // Check if allow to use return.
	{
		this->NumReturns = this->NumReturns + 1;
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
	else
		if(n->expr->type->type != 0) // If there are no expr after return, there are no wrong (return;).
			this->symtab->errors->AddError("Since Function returns void, a return keyword must not be followed by an object expression", n->line, n->column);
}

void
TypeVisitor::Visit(Variables_e *n)
{
	n->type->accept(this);
	n->variables->accept(this);
}