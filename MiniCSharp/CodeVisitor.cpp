#include "visitors.h"

CodeVisitor::CodeVisitor(Root *ro, SymTab *st, Function *mainF)
{
	// accept mainFunc
	this->root = ro;
	this->symtab = st;
	this->mainFunc = mainF;
	
	ifno = -1;
	forno = -1;
	whileno = 0;
	this->gp = 999;
	this->lp = -1; // 1
	this->NullLoc = this->gp; // Null location
	this->Functions = new vector<Function *>;
	this->IsLocation = false;
	this->IsCall = false;
	this->ObjectClass = NULL;
	this->callNum = 0;
	
	types[0] = "no type";
	types[1] = "null";
	types[2] = "int";
	types[3] = "double";
	types[4] = "boolean";
	types[5] = "object";
	types[6] = "Ident";
	types[7] = "void";

	// Revers all Globals vector for all classes.
	for(int i=0; i<ro->classes->size(); i++)
	{
		ro->classes->at(i)->Reverse(ro->classes->at(i)->Globals);// Reverse Class->Globals Idents.
		ro->classes->at(i)->GiveNum(ro->classes->at(i)->Globals);// Add num to global_location for each Ident in vector.
	}

	// alloc memory for NULL.
	vout << "alloc 1" << endl;
	vout << "storeg " << this->NullLoc << endl;


	// Add gp as location for each ident in Static vector.
	for(int i=0; i<ro->classes->size(); i++)
	{
		Class *C = ro->classes->at(i);
		for(int j=0; j<C->Statics->size(); j++)
		{
			this->gp -= 1;
			C->Statics->at(j)->symbol->location = this->gp;
		}
	}

	vout << "start" << endl;

	// vout << "alloc " << mainF->clas->globals +1 << endl;
	// vout << "storel 0" << endl;
	this->mainFunc->accept(this);
	vout << "stop" << endl;

	vout << endl;
	vout << "Write_bool:" << endl;
	vout << "pushg " << this->gp << endl;
	this->gp += 1;
	vout << "jz If_False" << endl;
	vout << "pushs \"true\"" << endl;
	vout << "writes" << endl;
	vout << "jump END_Write_bool" << endl;
	vout << "If_False:" << endl;
	vout << "pushs \"false\"" << endl;
	vout << "writes" << endl;
	vout << "END_Write_bool:" << endl;
	vout << "return" << endl;
	vout << endl;

	for(int i=0; i<this->Functions->size(); i++)
	{
		Functions->at(i)->accept(this);
		vout << endl;
	}

}

void
CodeVisitor::Visit(Root *n)
{
}

void
CodeVisitor::Visit(Class *n)
{
	n->members->accept(this);
}

void
CodeVisitor::Visit(ClassInher *n)
{
}

void
CodeVisitor::Visit(Members *n)
{
	for(int i=0; i < n->members->size(); i++)
		n->members->at(i)->accept(this);
}

void
CodeVisitor::Visit(Member  *n)
{
}

void
CodeVisitor::Visit(Global  *n)
{
	n->variables->accept(this);
}

void
CodeVisitor::Visit(Constructor *n)
{
}

void
CodeVisitor::Visit(Function *n)
{
	// if the Function is not main 
	if( !((n->name->name == "Main") && (n->type == NULL) && (n->args->args->size() == 0)) )
	{
		this->lps.push_back(this->lp);
		this->lp = -1;
		
		vout << "jump END_" << this->Rename(n->name) << endl;
		vout << this->Rename(n->name) << ":" << endl;
		for(int i = 0; i < n->args->args->size(); i++)
		{
			vout << "pushg " << this->gp << endl;
			this->gp += 1;
			this->lp += 1;
			n->args->args->at(i)->name->symbol->location = this->lp;
			
			vout << "storel " << this->lp << endl;
			//vout << "pushl " << this->lp << endl;
		}
		for(int i = 0; i < n->stats->stats->size(); i++)
		{
			n->stats->stats->at(i)->accept(this);
		}

		vout << "return" << endl;
		vout << "END_" << this->Rename(n->name) << ":" <<endl;
		this->lp = this->lps.at(lps.size() - 1);
		lps.pop_back();
	}
	// if the Function is main
	else
	{
		for(int i = 0; i < n->stats->stats->size(); i++)
		{
			n->stats->stats->at(i)->accept(this);
		}
	}
}

void
CodeVisitor::Visit(Arg  *n)
{
}

void
CodeVisitor::Visit(Args *n)
{
}

void
CodeVisitor::Visit(Variables *n)
{
	for(int i = 0; i < n->variables->size(); i++)
		n->variables->at(i)->accept(this);
}

void
CodeVisitor::Visit(Variable  *n)
{
	//this code only for the real number type should to continue the other Type
	//for flout number we write pushf

	this->lp += 1;
	n->name->symbol->location = this->lp;
	
	int type = n->name->symbol->type->type;
	switch(type)
	{
	case 2:
		if(n->expr == NULL)
		{
			vout << "pushi 0" << endl; // default value is 0.
			vout << "storel " << this->lp << endl;
		}
		else
		{
			n->expr->accept(this);
			vout << "storel " << this->lp << endl;
		}
		break;
	case 3:
		if(n->expr == NULL)
		{
			vout << "pushf 0" << endl; // default value is 0.
			vout << "storel " << this->lp << endl;
		}
		else
		{
			n->expr->accept(this);
			vout << "storel " << this->lp << endl;
		}
		break;
	case 4:
		if(n->expr == NULL)
		{
			vout << "pushi 0" << endl; // default value is false.
			vout << "storel " << this->lp << endl;
		}
		else
		{
			n->expr->accept(this);
			vout << "storel " << this->lp << endl;
		}
		break;
	case 6:
		this->lp -= 1; // change back lp.
		this->gp -= 1;
		n->name->symbol->location = this->gp;
		if(n->expr == NULL)
		{
			vout << "pushg " << this->NullLoc << endl;
			vout << "storeg " << this->gp << endl;
		}
		else
		{
			n->expr->accept(this);
			vout << "storeg " << this->gp << endl;
		}
		break;
	}
}

void
CodeVisitor::Visit(AccessModif *n)
{
}

void
CodeVisitor::Visit(Type *n)
{
}

void
CodeVisitor::Visit(NoArrayType *n)
{
}

void
CodeVisitor::Visit(ArrayType *n)
{
}

void
CodeVisitor::Visit(Ident *n)
{
	if(this->callNum != 0) // if Call.
		vout << "pushi " << n->symbol->global_location << endl; // write global_location num.
	else // if not called.
		if(!this->IsLocation) // if need to value (not location).
			vout << "pushl " << n->symbol->location << endl; // Read value from location on lp and write to stack.
}

void
CodeVisitor::Visit(Expr *n)
{
}

void
CodeVisitor::Visit(Incr *n)
{
	if(n->name->symbol->type->type == 2)
	{
		// ++ IDENT
		if(n->isPrev)
		{
			n->name->accept(this);
			vout << "pushi 1" << endl;
			vout << "add" << endl; 
			vout << "storel " << n->name->symbol->location << endl;
			vout << "pushl " << n->name->symbol->location << endl;
		}
		// IDENT ++
		else
		{
			n->name->accept(this);
			this->gp -= 1;
			vout << "storeg " << this->gp << endl;
			vout << "pushg " << this->gp << endl;
			vout << "pushi 1" << endl;
			vout << "add" << endl;
			vout << "storel " << n->name->symbol->location << endl;
			vout << "pushg " << this->gp << endl;
			this->gp += 1;
		}
	}
	else if(n->name->symbol->type->type == 3)
	{
		// ++ IDENT
		if(n->isPrev)
		{
			n->name->accept(this);
			vout << "pushf 1" << endl;
			vout << "fadd" << endl;
			vout << "storel " << n->name->symbol->location << endl;
			vout << "pushl " << n->name->symbol->location << endl;
		}
		// IDENT ++
		else
		{
			n->name->accept(this);
			this->gp -= 1;
			vout << "storeg " << this->gp << endl;
			vout << "pushg " << this->gp << endl;
			vout << "pushf 1" << endl;
			vout << "fadd" << endl;
			vout << "storel " << n->name->symbol->location << endl;
			vout << "pushg " << this->gp << endl;
			this->gp += 1;
		}
	}
}

void
CodeVisitor::Visit(Decr *n)
{
}

void
CodeVisitor::Visit(Not *n)
{
	// ! expression, expression type is bool.
	n->expr->accept(this);
	vout << "not" << endl; 
}

void
CodeVisitor::Visit(Minus *n)
{
	// - expression, expression type is integer or double.
	n->expr->accept(this);
	// if expression is Integer
	if(n->expr->type->type == 2)
	{
		vout << "pushi -1" << endl;
		vout << "mul" << endl;
	}
	// if expression is Real
	/*else if(n->expr->type->type == 3)
	{
		vout << "pushf -1" << endl;
		vout << "fmul" << endl;
	}*/
}

void
CodeVisitor::Visit(Plus *n)
{
	// + expression, expression type is integer or double.
	n->expr->accept(this);
}

void
CodeVisitor::Visit(Paren  *n)
{
	// '(' expression ')' type is the same of expression type.
	n->expr->accept(this);
}

void
CodeVisitor::Visit(IdentExpr *n)
{
	// IdentExpr type is the same of Ident type.
	//vout << "pushl " << n->ident->symbol->location << endl;
	n->ident->accept(this);
}

void
CodeVisitor::Visit(IdentArr *n)
{
	//X IDENT arrayIndex , check if arrayIndex the same array_level of IDENT array_level.
}

void
CodeVisitor::Visit(Assign *n)
{
	// expression = expression.

	IdentExpr *ident = dynamic_cast<IdentExpr*>(n->left);
	if(ident == NULL)
	{
		this->IsLocation = true;
		n->left->accept(this);
		this->IsLocation = false;
	}

	n->right->accept(this);

	if(ident != NULL)
	{
		if(ident->type->type == 6) // if ident type is class.
			vout << "storeg " << ident->ident->symbol->location << endl;
		else
			vout << "storel " << ident->ident->symbol->location << endl;
	}
	else
		vout << "storen" << endl;
}

void
CodeVisitor::Visit(Invoke *n)
{
	// check if function is Write
	if(  (n->ident->name == "Write") && (n->exprList->exprList->size() == 1) && 
		 ( // check if type of exprList at 0 is int, double or bool
			(n->exprList->exprList->at(0)->type->type == 2) || 
			(n->exprList->exprList->at(0)->type->type == 3) || 
			(n->exprList->exprList->at(0)->type->type == 4)    )  )
	{
		n->exprList->exprList->at(0)->accept(this);
		if(n->exprList->exprList->at(0)->type->type == 2)
			vout << "writei" << endl;
		else if(n->exprList->exprList->at(0)->type->type == 3)
			vout << "writef" << endl;
		else if(n->exprList->exprList->at(0)->type->type == 4)
		{
			this->gp -= 1;
			vout << "storeg " << this->gp << endl;
			vout << "pusha Write_bool" << endl;
			vout << "call" << endl;
		}
	}
	// check if function is Read
	else if( (n->ident->name == "Read") && (n->exprList->exprList->size() == 0) )
	{
		vout << "Read" << endl;
	}
	// Else
	else
	{
		Class *clas = this->ObjectClass; // save ObjectClass in clas, because maybe it change.
		this->ObjectClass = NULL; // put NULL in ObjectClass.
		int mainloc = 0;
		bool IsCalled = false;
		if(this->callNum != 0)
		{
			IsCalled = true;
			this->callNum = 0;
		}

		// if this Invoke invoking by call from object then save all object elements in gp to use it.
		// passed the Global variable in object.
		if(IsCalled)
		{
			this->gp -= 1;
			mainloc = this->gp; // save this gp.
			vout << "storeg " << mainloc << endl;
			if(clas != NULL) // check if not NULL, that can cuse Stop program.
				for(int i = clas->Globals->size()-1; i>=0; i--) // for each Global in class and perants save it's values to use in function.
				{
					this->gp -= 1;
					clas->Globals->at(i)->symbol->location = this->gp;
					vout << "pushg " << mainloc << endl;
					vout << "pushi " << clas->Globals->at(i)->symbol->global_location << endl;
					vout << "loadn" << endl;
					vout << "storeg " << clas->Globals->at(i)->symbol->location << endl;
				}
		}

		// passed the arguments.
		for(int i = n->exprList->exprList->size() - 1; i >= 0 ; i--)
		{
			this->gp -= 1;
			n->exprList->exprList->at(i)->accept(this); // retrun value of expr
			vout << "storeg " << this->gp << endl; // store value in gp
		}

		// Call Function.
		vout << "pusha " << this->Rename(n->ident) << endl;
		vout << "call" << endl;
		
		//n->ident->symbol->method->accept(this);
		
		this->Functions->push_back(n->ident->symbol->method);

		// if this Invoke invoking by call from object then load all object elements from gp to stok memory.
		// return the news values of Global variables to object memory location.
		if(IsCalled)
		{
			if(clas != NULL)
				for(int i=0; i<clas->Globals->size(); i++)
				{
					vout << "pushg " << mainloc << endl;
					vout << "pushi " << clas->Globals->at(i)->symbol->global_location << endl;
					vout << "pushg " << clas->Globals->at(i)->symbol->location << endl;
					vout << "storen" << endl;
					this->gp += 1;
					//clas->Globals->at(i)->symbol->location = 0;
				}
			this->gp += 1;
			mainloc = 0;
		}
		
		// write the return value on stack.
		if( n->ident->symbol->method->type != NULL )
		{
			vout << "pushg " << this->gp << endl;
			this->gp += 1;
		}
	}
}

void
CodeVisitor::Visit(NewObject *n)
{
	// NEW IDENT '(' expr_list ')'.
	Class *clas = n->ident->symbol->clas;
	vout << "alloc " << clas->Globals->size() << endl;
	this->gp -= 1;
	vout << "storeg " << this->gp << endl;
	for(int i=0; i<clas->Globals->size(); i++)
	{
		vout << "pushg " << this->gp << endl;
		vout << "pushi " << clas->Globals->at(i)->symbol->global_location << endl;
		switch(clas->Globals->at(i)->symbol->type->type)
		{
		case 2:
			vout << "pushi 0" << endl;
			break;
		case 3:
			vout << "pushf 0" << endl;
			break;
		case 4:
			vout << "pushi 0" << endl;
			break;
		case 6:
			vout << "pushg " << this->NullLoc << endl;
			break;
		}
		vout << "storen" << endl;
	}
	vout << "pushg " << this->gp << endl;
	this->gp += 1;
}

void
CodeVisitor::Visit(NewArray *n)
{
	// NEW noarraytype arrayindex.
}

void
CodeVisitor::Visit(IdentCall *n)
{
	// Ident '.' expression.
	
	int num = this->callNum;
	IdentCall *call = dynamic_cast<IdentCall*>(n->expr);
	IdentExpr *ident = dynamic_cast<IdentExpr*>(n->expr);
	Invoke *invoke = dynamic_cast<Invoke*>(n->expr);
	
	if(this->callNum == 0) // if this call is First call											// [_this_]<--gp
		vout << "pushg " << n->ident->symbol->location << endl; // return memory location of object	// [______]
																									// [______]
	if(call != NULL) // if expression is a IdentCall.												//  STACK
	{
		vout << "pushi " << call->ident->symbol->global_location << endl; // return sub memory location of object
		vout << "loadn" << endl; // return value of memory location at(sub memory location)
/*									  ______						  ______
        [__gp__]---location--\		2[object]---sub-memory---\		2[object]
		[______]			 |		1[_value]				 |		1[_value]
		[______]			 \----->0[______]				 \----->0[______]
		 STACK						  ALLOC
*/
	}
	
	if(invoke != NULL) // if expr is Innvoke put this type class in ObjectClass.
		this->ObjectClass = n->ident->symbol->type->name->symbol->clas;

	this->callNum += 1;
	n->expr->accept(this);

	if(ident != NULL) // if expr is Ident.
		if(!IsLocation) // if need to value (not location).
			vout << "loadn" << endl;

	if(num == 0)
		this->callNum = 0;
}

void
CodeVisitor::Visit(IdentArrCall *n)
{
	// Ident arrayindex '.' expression.
}

void
CodeVisitor::Visit(ThisCall *n)
{
	// THIS '.' expression.
}

void
CodeVisitor::Visit(Equal *n)
{
	//expression == expression
	n->left->accept(this);
	n->right->accept(this);
	vout << "equal" << endl;
}

void
CodeVisitor::Visit(NotEq *n)
{
	//expression != expression
	n->left->accept(this);
	n->right->accept(this);
	vout << "equal" << endl;
	vout << "not" << endl;
}

void
CodeVisitor::Visit(Smaller *n)
{
	//expression < expression
	n->left->accept(this);
	n->right->accept(this);
	if(n->left->type->type == 2)
		vout << "inf" << endl;
	else if(n->left->type->type == 3)
		vout << "finf" << endl;
}

void
CodeVisitor::Visit(SmallerEq *n)
{
	//expression <= expression
	n->left->accept(this);
	n->right->accept(this);
	if(n->left->type->type == 2)
		vout << "infeq" << endl;
	else if(n->left->type->type == 3)
		vout << "finfeq" << endl;
}

void
CodeVisitor::Visit(Larger *n)
{
	//expression > expression
	n->left->accept(this);
	n->right->accept(this);
	if(n->left->type->type == 2)
		vout << "sup" << endl;
	else if(n->left->type->type == 3)
		vout << "fsup" << endl;
}

void
CodeVisitor::Visit(LargerEq *n)
{
	//expression >= expression
	n->left->accept(this);
	n->right->accept(this);
	if(n->left->type->type == 2)
		vout << "supeq" << endl;
	else if(n->left->type->type == 3)
		vout << "fsupeq" << endl;
}

void
CodeVisitor::Visit(Add *n)
{
	//expression + expression
	int left = n->left->type->type;
	int right = n->right->type->type;

	// int + int
	if((left == 2) && (right == 2) )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "add" << endl;
	}

	// int + double 
	if((left == 2) && (right == 3) )
	{
		n->left->accept(this);
		vout << "itof" << endl;
		n->right->accept(this);
		vout << "fadd" << endl;
	}

	// double + int
	if((left == 3) && (right == 2) )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "itof" << endl;
		vout << "fadd" << endl;
	}

	// double + double
	else if(left == 3 && right == 3 )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "fadd" << endl;
	}
}

void
CodeVisitor::Visit(Sub *n)
{
	//expression - expression
	int left = n->left->type->type;
	int right = n->right->type->type;
	
	// int - int
	if((left == 2) && (right == 2) )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "sub" << endl;
	}

	// int - double 
	if((left == 2) && (right == 3) )
	{
		n->left->accept(this);
		vout << "itof" << endl;
		n->right->accept(this);
		vout << "fsub" << endl;
	}

	// double - int
	if((left == 3) && (right == 2) )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "itof" << endl;
		vout << "fsub" << endl;
	}

	// double - double
	else if(left == 3 && right == 3 )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "fsub" << endl;
	}
}

void
CodeVisitor::Visit(Mult *n)
{
	//expression * expression
	int left = n->left->type->type;
	int right = n->right->type->type;
	
	// int * int
	if((left == 2) && (right == 2) )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "mul" << endl;
	}

	// int * double 
	if((left == 2) && (right == 3) )
	{
		n->left->accept(this);
		vout << "itof" << endl;
		n->right->accept(this);
		vout << "fmul" << endl;
	}

	// double * int
	if((left == 3) && (right == 2) )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "itof" << endl;
		vout << "fmul" << endl;
	}

	// double * double
	else if(left == 3 && right == 3 )
	{
		n->left->accept(this);
		n->right->accept(this);
		vout << "fmul" << endl;
	}
}

void
CodeVisitor::Visit(Div *n)
{
	//expression / expression
	n->left->accept(this);
	n->right->accept(this);
	if(n->left->type->type == 2)
	{
		n->left->accept(this);
		vout << "itof" << endl;
		n->right->accept(this);
		if(n->right->type->type == 2)
			vout << "itof" << endl;
		vout << "fdiv" << endl;
	}
	else if(n->left->type->type == 3)
	{
		if(n->right->type->type == 2)
			vout << "itof" << endl;
		vout << "fdiv" << endl;
	}
}

void
CodeVisitor::Visit(Mod *n)
{
	//expression % expression
	n->left->accept(this);
	n->right->accept(this);
	vout << "mod" << endl;
}

void
CodeVisitor::Visit(And *n)
{
	// expression And expression
	n->left->accept(this);
	n->right->accept(this);
	vout << "mul" << endl;
}

void
CodeVisitor::Visit(Or *n)
{
	// expression Or expression
	n->left->accept(this);
	n->right->accept(this);
	vout << "add" << endl;
}

void
CodeVisitor::Visit(Is *n)
{
	//X expression IS type
}

void
CodeVisitor::Visit(Cast *n)
{
	// ( basictype ) expression
	/*n->expr->accept(this);
	if((n->typ->type == 2) && (n->expr->type == 3))
		vout << "ftoi" << endl;
	else if(n->typ->type == 3 && (n->expr->type == 2))
		vout << "itof" << endl;*/
}

void
CodeVisitor::Visit(Integer *n)
{
	vout << "pushi " << n->value << endl;
}

void
CodeVisitor::Visit(Real *n)
{
	vout << "pushf " << n->value << endl;
}

void
CodeVisitor::Visit(True *n)
{
	vout << "pushi 1" << endl;
}

void
CodeVisitor::Visit(False *n)
{
	vout << "pushi 0" << endl;
}

void
CodeVisitor::Visit(This *n)
{
}

void
CodeVisitor::Visit(Null *n)
{
	vout << "pushg " << this->NullLoc << endl;
}

void
CodeVisitor::Visit(ArrayIndex *n)
{
}

void
CodeVisitor::Visit(ArrayIndex_1 *n)
{
}

void
CodeVisitor::Visit(ArrayIndex_2 *n)
{
}

void
CodeVisitor::Visit(ArrayIndex_3 *n)
{
}

void
CodeVisitor::Visit(ExprList *n)
{
}

void
CodeVisitor::Visit(Stat *n)
{
	// empty
}

void
CodeVisitor::Visit(Stats *n)
{
	for(int i = 0; i < n->stats->size(); i++)
		n->stats->at(i)->accept(this);
}

void
CodeVisitor::Visit(If *n)
{
	// if Expression is false(0) go to End if (pc to EndIf)
	// if Expression is true(1) go into if (pc ++)
	this->ifno++;
	n->expr->accept(this);
	vout << "jz " << "ENDIF_" << this->ifno << endl;
	n->stat->accept(this);
	vout << "ENDIF_" << this->ifno << ":" << endl;
}

void
CodeVisitor::Visit(IfElse *n)
{
	// if Expression is false(0) go to ELSE (pc to ELSE)
	// if Expression is true(1) go into if (pc ++) at end of statments jump to ENDIF (pc to ENDIF)
	this->ifno++;
	n->expr->accept(this);
	vout << "jz " << "ELSE_" << this->ifno << endl;
	n->stat->accept(this);
	vout << "jump " << "ENDIF_" << this->ifno << endl;
	vout << "ELSE_" << this->ifno << ":" << endl;
	n->elseStat->accept(this);
	vout << "ENDIF_" << this->ifno << ":" << endl;

}

void
CodeVisitor::Visit(While *n)
{
	this->whileno++;
	vout << "WBegin_" << this->whileno << ":" << endl;
	n->expr->accept(this);
	vout << "jz " << "WEnd_" << this->whileno << endl;
	n->stat->accept(this);
	vout << "jump WBegin_" << this->whileno << endl;
	vout << "WEnd_" << this->whileno << ":" <<endl;
}

void
CodeVisitor::Visit(For *n)
{
	this->forno++;

	if(n->variables_e != NULL)
		n->variables_e->accept(this);
	vout << "ForBegin_" << this->forno << ":" << endl;

	if(n->exprCond->type->type == 0)
		vout << "pushi 1" << endl; // Always true;
	else
		n->exprCond->accept(this);

	vout << "jz " << "ForEnd_" << this->forno << endl;
	n->stat->accept(this);
	n->exprCount->accept(this);
	vout << "jump " << "ForBegin_" << this->forno << endl;
	vout << "ForEnd_" << this->forno << ":" << endl;
}

void
CodeVisitor::Visit(ExprStat *n)
{
	// expression ';'
	n->expr->accept(this);
}

void
CodeVisitor::Visit(VariablesStat *n)
{
	// type variables ';'
	n->variables->accept(this);
}

void
CodeVisitor::Visit(Semi *n)
{
	// ';'
	// empty.
}

void
CodeVisitor::Visit(Block *n)
{
	// '{' statements '}'
	n->stats->accept(this);
}

void
CodeVisitor::Visit(Return *n)
{
	// RETURN expr_e ';'
	n->expr->accept(this);
	this->gp -= 1;
	vout << "storeg " << this->gp << endl;
	vout << "return" << endl;
}

void
CodeVisitor::Visit(Variables_e *n)
{
}

/* - * - * - * - * - * - * - * - * - * - * - *
 *			Other Functions					 *
 * - * - * - * - * - * - * - * - * - * - * - */

string
CodeVisitor::Rename(Ident *name)
{
	// Generat Lable name for Function and Constructor.
	string final_name = "";
	final_name += name->symbol->clas->name->name;
	final_name += "_";
	final_name += name->name;
	Args *args = dynamic_cast<Args*>(name->symbol->args);
	for(int i=0; args->args->size(); i++)
	{
		final_name += "_";

		int typ = args->args->at(i)->type->type;

		// if type of arg is Ident then write to final_name the name of Ident.
		if(typ == 6)
			final_name += args->args->at(i)->type->name->name;
		else
			final_name += types[typ];
	}
	return final_name;
}