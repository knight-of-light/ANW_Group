#include "visitors.h"

TypeVisitor::TypeVisitor(Root *file, SymTab *st)
{
	this->mainFunc = NULL;
	this->symtab = st;
	this->scopeNum = 0;
	this->callNum = 0;
	this->IsStatic = false;
	this->IsReturn = false; // In default, Cannot use return.
	this->NumReturns = 0;
	this->RetrurnType = NULL;

	types[0] = "no type";
	types[1] = "null";
	types[2] = "int";
	types[3] = "double";
	types[4] = "boolean";
	types[5] = "object";
	types[6] = "Ident";
	types[7] = "void";

	arrlvl[0] = "";
	arrlvl[1] = "[]";
	arrlvl[2] = "[,]";
	arrlvl[3] = "[,,]";

	file->accept(this);
}

void
TypeVisitor::Visit(Root *n)
{
	this->scopeNum++;

	for(int i=0; i< n->classes->size(); i++)
		n->classes->at(i)->accept(this);

	if(this->mainFunc == NULL)
		this->symtab->errors->AddError("The program must have a Main function",
		 -1,
		 -1);
}

void
TypeVisitor::Visit(Class *n)
{
	this->scopeNum++;
	n->members->accept(this);
	bool IsContCons = false;

	// Check if Members are Right
	for(int i=0; i<n->members->members->size(); i++)
	{
		// Check if name of constructors ARE the same as name of class.
		if(dynamic_cast<Constructor*>(n->members->members->at(i)) != NULL )
		{
			Constructor *C = dynamic_cast<Constructor*>(n->members->members->at(i));
			if(n->name->name != C->name->name)
				this->symtab->errors->AddError("Method must have a retrun type", C->name->line, C->name->column);
			C->name->symbol->clas = n;
			IsContCons = true;
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
	if(!IsContCons) // if there is no Constructor in this class.
		this->symtab->errors->AddError("Class should contain at less one Constructor", n->line, n->column);
}

void
TypeVisitor::Visit(ClassInher *n)
{
	this->scopeNum++;
	n->members->accept(this);
	bool IsContCons = false;

	// Check if Members are Right
	for(int i=0; i<n->members->members->size(); i++)
	{
		// Check if name of constructors ARE the same as name of class.
		if(dynamic_cast<Constructor*>(n->members->members->at(i)) != NULL )
		{
			Constructor *C = dynamic_cast<Constructor*>(n->members->members->at(i));
			if(n->name->name != C->name->name )
				this->symtab->errors->AddError("Method must have a retrun type", C->name->line, C->name->column);
			C->name->symbol->clas = n;
			IsContCons = true;
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
	if(!IsContCons) // if there is no Constructor in this class.
		this->symtab->errors->AddError("Class should contain at less one Constructor", n->line, n->column);
	
	// Add all Global ident in parents to this class Global.
	for(int i=0; i<n->Parents->size(); i++)
	{
		Class *C = dynamic_cast<Class*>(n->Parents->at(i)->symbol->clas);
		for(int j=0; j<C->Globals->size(); j++)
			n->AddGlobal(C->Globals->at(j));
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
	// empty
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

	// acctype of Constructor cannot be "static".
	if((n->accessModif->acctype == 3) || (n->accessModif->acctype == 4))
		this->symtab->errors->AddError("Access modifiers are not allowed on static constructors", n->name->line, n->name->column);

	n->args->accept(this);
	n->stats->accept(this);
}

void
TypeVisitor::Visit(Function *n)
{
	if( (n->name->name == "Main") && (n->type == NULL) && (n->args->args->size() == 0) && 
		( (n->accessModif->acctype == 3) || (n->accessModif->acctype == 4) )   )
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
		this->IsReturn = true;
		this->NumReturns = 0;
		this->RetrurnType = n->type;
	}

	n->stats->accept(this);

	if(this->IsReturn)
	{
		// check if Function contain return keyword.
		if(this->NumReturns == 0)
			this->symtab->errors->AddError("Not all code paths return a value", n->line, n->column);

		this->IsReturn = false;
		this->RetrurnType = NULL;
	}

	// redefinsion of father method cannot be in more secure level from its parents.
	string key = "f@" + n->name->name;
	for(int i=0; i < n->args->args->size(); i++)
	{
		int t = n->args->args->at(i)->type->type;
		// if type of arg is Ident then write to key the name of Ident.
		if(t == 6)
			key += "@" + n->args->args->at(i)->type->name->name;
		else
			key += "@" + types[t];
	}

	Class *C = symtab->ScopeToClass(symtab->IntToScope(symtab->FatherScopeNum(scopeNum)));
	Sym *sym = NULL;
	for(int i=0; i<C->Parents->size(); i++)
	{
		sym = symtab->ClassToScope(C->Parents->at(i)->symbol->clas)->hashTab->GetMember(key);
		if(sym != NULL)
		{
			switch(sym->acctype)
			{
			case 1:
				{
					if( (n->accessModif->acctype == 2) || (n->accessModif->acctype == 4) )
						symtab->errors->AddError("Function cannot be more protiction level from his father fanction", n->name->line, n->name->column);
					break;
				}
			case 3:
				{
					if( (n->accessModif->acctype == 2) || (n->accessModif->acctype == 4) )
						symtab->errors->AddError("Function cannot be more protiction level from his father fanction", n->name->line, n->name->column);
					break;
				}
			default:
				break;
			}
		}
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

					// see if left-hand type is father of right-hand type.
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
			else
				mismatch = true;
			break;
		default:
			mismatch = true;
		}

		// check from arr_level.
		int left_lvl = n->name->symbol->type->arr_level;
		int right_lvl = n->expr->type->arr_level;
		if(left_lvl != right_lvl)
			mismatch = true;

		if(mismatch)
		{
			// Print error.
			string typeLeft = types[left];
			string typeRight = types[right];
			if(left == 6)
				typeLeft = n->name->symbol->type->name->name;
			if(right == 6)
				typeRight = n->expr->type->name->name;

			symtab->errors->AddError("Type mismatch between \'" + typeLeft + arrlvl[left_lvl] + "\' and \'" + typeRight + arrlvl[right_lvl] + "\'",
			 n->name->line,
			 n->column);
		}
	}
}

void
TypeVisitor::Visit(AccessModif *n)
{
	// empty.
}

void
TypeVisitor::Visit(Type *n)
{
	// empty.
}

void
TypeVisitor::Visit(NoArrayType *n)
{
	// empty.
}

void
TypeVisitor::Visit(ArrayType *n)
{
	// empty.
}

void
TypeVisitor::Visit(Ident *n)
{
	// empty.
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
				// if IsStatic true, and func not static, write error.
				// Or if IsStatic false, and func static, wirte error.
			if( !(( (!this->IsStatic) &&  (acctype != 3) && (acctype != 4) )	// if func must be not static and it is not static.
			 || ( ( this->IsStatic) && ((acctype == 3) || (acctype == 4)) ) ))	// if func must be static and it is static.
				this->symtab->errors->AddError("Member " + n->ident->name + " cannot be accessed with an instance reference", n->ident->line, n->ident->column);
			
			n->type = n->ident->symbol->type;
		}
	}
}

void
TypeVisitor::Visit(IdentArr *n)
{
	//X IDENT arrayIndex , check if arrayIndex the same array_level of IDENT array_level.

	n->arrayIndex->accept(this);
	
	int left = n->ident->symbol->type->arr_level;
	int right = n->arrayIndex->arr_level;
	if(left != right)
		this->symtab->errors->AddError("array level of \'" + n->ident->name + "\' is " + arrlvl[left] + "\' Not \'" + arrlvl[right] + "\'",
		 n->ident->line, n->column);

	n->type = n->ident->symbol->type;
	n->type->arr_level = 0;									// I put zero in arr_level.
}

void
TypeVisitor::Visit(Assign *n)
{
	// expression = expression, check if left-hand type is same of right-hand type.

	// left-hand type must be IdentExpr, IdentArr, IdentCall, ThisCall or IdentArrCall.
	bool SyntaxError = false;

	if( (dynamic_cast<IdentExpr*>(n->left) == NULL) &&
		(dynamic_cast<IdentArr*>(n->left) == NULL)  &&
		(dynamic_cast<IdentCall*>(n->left) == NULL) &&
		(dynamic_cast<ThisCall*>(n->left) == NULL)  &&
		(dynamic_cast<IdentArrCall*>(n->left) == NULL) )
		SyntaxError = true;

	if(!SyntaxError)
	{
		n->left->accept(this);
		n->right->accept(this);
		int left = n->left->type->type;
		int right = n->right->type->type;
		
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
				n->right->type->type = 3;
			}
			break;
		case 4:
			if(right != 4)
				mismatch = true;
			break;
		case 6:
			if(right == 6)
			{
				if(n->left->type->name->name != n->right->type->name->name) // if ident type of left != ident type of right.
				{
					mismatch = true;

					// see if left is father of right.
					Class *c = n->left->type->name->symbol->clas; // class type of left.
					for(int i=0; i<c->Childrens->size(); i++)
					{
						if(c->Childrens->at(i)->name == n->right->type->name->name)
						{
							mismatch = false;
							break;
						}
					}
				}
			}
			else
				mismatch = true;
			break;
		default:
			mismatch = true;
		}

		// check from arr_level.
		int left_lvl = n->left->type->arr_level;
		int right_lvl = n->right->type->arr_level;
		if(left_lvl != right_lvl)
			mismatch = true;

		if(mismatch)
		{
			// Print error.
			string typeLeft = types[left];
			string typeRight = types[right];
			if(left == 6)
				typeLeft = n->left->type->name->name;
			if(right == 6)
				typeRight = n->right->type->name->name;
			symtab->errors->AddError("Type mismatch between \'" + typeLeft + arrlvl[left_lvl] + "\' and \'" + typeRight + arrlvl[right_lvl] + "\'",
			 n->left->line,
			 n->column);
		}
	}
	else
		this->symtab->errors->AddError("the left-hand side of an assignment must be a variable or indexer", n->left->line, n->column);
	
	
	n->type = n->left->type;
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
					// if IsStatic true, and func not static, write error.
					// Or if IsStatic false, and func static, wirte error.
				if( !(( (!this->IsStatic) &&  (acctype != 3) && (acctype != 4) )
				 || ( ( this->IsStatic) && ((acctype == 3) || (acctype == 4)) ) ))
					this->symtab->errors->AddError("Member " + n->ident->name + " cannot be accessed with an instance reference", n->ident->line, n->ident->column);

				n->type = n->ident->symbol->method->type;
			}
	}
	else // if Write or Read func.
	{
		if(this->callNum != 0)
			this->symtab->errors->AddError("Undeclared Identifier '" + n->ident->name , n->ident->line, n->ident->column);
		
		n->type->type = 7;
	}
}

void
TypeVisitor::Visit(NewObject *n)
{
	// NEW IDENT '(' expr_list_e ')'.

	n->exprList->accept(this);

	if(this->symtab->IsDeclared(n->ident, 3, n->exprList, this->scopeNum, false))
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
	
	n->type = n->typ;
	n->type->arr_level = n->arrayIndex->arr_level;
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

	n->type = n->expr->type;
}

void
TypeVisitor::Visit(IdentArrCall *n)
{
	// Ident arrayindex '.' expression.

	// empty for now.
}

void
TypeVisitor::Visit(ThisCall *n)
{
	// THIS '.' expression.

	int num = this->callNum;

	if(this->callNum == 0)
		this->callNum = this->symtab->FatherScopeNum(this->scopeNum);
	else
		this->symtab->errors->AddError("\'this\' is a keyword", n->line, n->column);
	
	n->expr->accept(this);

	if(num == 0)
		this->callNum = 0;

	n->type = n->expr->type;
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
		else if(right == 3)
			n->left->type->type = 3;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		else
			n->right->type->type = 3;
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
		else if(right == 3)
			n->left->type->type = 3;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		else
			n->right->type->type = 3;
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
		else if(right == 3)
			n->left->type->type = 3;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		else
			n->right->type->type = 3;
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
		else if(right == 3)
			n->left->type->type = 3;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		else
			n->right->type->type = 3;
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
		else if(right == 3)
			n->left->type->type = 3;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		else
			n->right->type->type = 3;
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
		else if(right == 3)
			n->left->type->type = 3;
		break;
	case 3:
		if( !((right == 2) || (right == 3)) )
			mismatch = true;
		else
			n->right->type->type = 3;
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
			n->left->type->type = 3;
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;
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
			n->left->type->type = 3;
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;
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
			n->left->type->type = 3;
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;
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
			n->left->type->type = 3;
			n->type->type = 3;			
		}
		else
			mismatch = true;
		break;
	case 3:
		if(right == 2)
		{
			n->right->type->type = 3;
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
	// expression Mod expression , (int % int) = int
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

	if( !((left == 4) && (right == 4)) )
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

	if( !((left == 4) && (right == 4)) )
		symtab->errors->AddError("Operator \'||\' cannot be applied to operands of type \'" + types[left] + "\' and \'" + types[right] + "\'",
		 n->left->line,
		 n->column);

	n->type->type = 4;
}

void
TypeVisitor::Visit(Is *n)
{
	// expression IS type, return bool.
	n->expr->accept(this);
	
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
	Class *C = this->symtab->ScopeToClass(this->symtab->IntToScope(this->symtab->FatherScopeNum(this->scopeNum)));
	n->type->type = 6;
	n->type->name = C->name;
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
	// IF '(' expression ')' statement

	// check if expression is bool.
	n->expr->accept(this);
	int ty = n->expr->type->type;
	if(ty != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[ty] + "\' and \'bool\'",
		 n->expr->line,
		 n->column);

	
	int num_ret = this->NumReturns; // save NumReturns befor check all stats.

	n->stat->accept(this);

	// check if all code paths return a value.
	if(this->IsReturn)
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
	// IF '(' expression ')' statement ELSE statement

	// check if expression is bool.
	n->expr->accept(this);
	int ty = n->expr->type->type;
	if(ty != 4)
		symtab->errors->AddError("Type mismatch between \'" + types[ty] + "\' and \'bool\'",
		 n->expr->line,
		 n->column);

	
	int num_ret = this->NumReturns; // save NumReturns befor check all stats.
	bool IfStatReturn = false; // Is (if statment) return a value.

	n->stat->accept(this);

	// check if all code paths return a value.
	if(this->IsReturn)
	{
		if(num_ret == this->NumReturns)
			this->symtab->errors->AddError("Not all code paths return a value", n->line, n->column);
		else
		{
			this->NumReturns = num_ret;
			IfStatReturn = true;
		}
	}
	
	n->elseStat->accept(this);

	// check if all code paths return a value.
	if(this->IsReturn)
	{
		if(num_ret == this->NumReturns)
			this->symtab->errors->AddError("Not all code paths return a value", n->elseStat->line, n->elseStat->column);
		else
		{
			this->NumReturns = num_ret;
			if(IfStatReturn)
				this->NumReturns++;
		}
	}
}

void
TypeVisitor::Visit(While *n)
{
	// WHILE '(' expression ')' statement
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
{
	// FOR '(' variables_e ';' expr_e ';' expr_e ')' statement
	
	if(n->variables_e != NULL)
		n->variables_e->accept(this);

	n->exprCond->accept(this);
	int cond = n->exprCond->type->type;
	if( !((cond == 4) || (cond == 0)) )
		symtab->errors->AddError("Type mismatch between \'" + types[cond] + "\' and \'bool\'",
		 n->exprCond->line,
		 n->column);
	else if(cond == 0)
		n->exprCond->type->type = 4;

	n->exprCount->accept(this);
	int count = n->exprCount->type->type;
	if( !((count == 2) || (count == 3) || (count == 0)))
		symtab->errors->AddError("Type mismatch between \'" + types[count] + "\' and \'int\' or \'double\'",
		 n->exprCount->line,
		 n->column);

	n->stat->accept(this);
}

void
TypeVisitor::Visit(ExprStat *n)
{
	// expression ';'
	n->expr->accept(this);
}

void
TypeVisitor::Visit(VariablesStat *n)
{
	// type variables ';'
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
	// '{' statements '}'
	this->scopeNum++;
	n->stats->accept(this);
}

void
TypeVisitor::Visit(Return *n)
{
	// RETURN expr_e ';'
	if(this->IsReturn) // Check if allow to use return.
	{
		this->NumReturns = this->NumReturns + 1;
		n->expr->accept(this);

		int left = this->RetrurnType->type;
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
				if(this->RetrurnType->name->name != n->expr->type->name->name) // if ident type of left != ident type of right.
				{
					mismatch = true;

					// see if left is father of right.
					Class *c = this->RetrurnType->name->symbol->clas; // class type of left.
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
			else
				mismatch = true;
			break;
		default:
			mismatch = true;
		}
		
		// check from arr_level.
		int left_lvl = this->RetrurnType->arr_level;
		int right_lvl = n->expr->type->arr_level;
		if(left_lvl != right_lvl)
			mismatch = true;

		if(mismatch)
		{
			// Print error.
			string typeLeft = types[left];
			string typeRight = types[right];
			if(left == 6)
				typeLeft = this->RetrurnType->name->name;
			if(right == 6)
				typeRight = n->expr->type->name->name;

			symtab->errors->AddError("Type mismatch in function return type \'" + typeLeft + arrlvl[left_lvl] + "\' and \'" + typeRight + arrlvl[right_lvl] + "\'",
			 n->expr->line,
			 n->column);
		}
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