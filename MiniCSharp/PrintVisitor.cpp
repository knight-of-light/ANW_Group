#include "visitors.h"

PrintVisitor::PrintVisitor(Root *file, bool debug)
{
	if(debug)
	{
		this->acctype[0] = " \\public\\";
		this->acctype[1] = " \\private\\";
		this->acctype[2] = " \\static\\";
		this->acctype[3] = " \\private static\\";

		this->types[0] = " \\NULL\\";
		this->types[1] = " \\int\\";
		this->types[2] = " \\double\\";
		this->types[3] = " \\bool\\";
		this->types[4] = " \\object\\";
		this->types[5] = " \\Ident\\";
		this->types[6] = " \\void\\";

		this->arr_level[0] = "";
		this->arr_level[1] = "[]";
		this->arr_level[2] = "[,]";
		this->arr_level[3] = "[,,]";

		this->count = 0;
		this->dashes = 0;
		this->spaces = 0;
		file->accept(this);
		cout << endl;
	}
}

string
PrintVisitor::PrintId(string s)
{
	string print = " \\" + s + "\\";
	return print;
}

void
PrintVisitor::PrintDashes()
{
	for(int i = 1; i <= dashes; i++)
		cout << "_";
}

void
PrintVisitor::PrintSpaces()
{
	for(int i = 1; i <= spaces; i++)
		cout << " ";
}

void
PrintVisitor::PrintBar()
{
	cout << "  |";
}

void
PrintVisitor::PrintBars(int count)
{
	if(count == 1)
	{
		PrintBar();
		PrintDashes();
		return;
	}
	spaces = 3;
	for(int i = 0; i<count; i++)
	{
		PrintBar();
		if(i + 1 < count)
			PrintSpaces();
	}
	dashes = 3;
	PrintDashes();
}

void
PrintVisitor::Visit(Root *n)
{
	cout << "<Root>" << endl;
	count++;
	for(int i=0; i<n->classes->size(); i++)
	{
		PrintBars(count);
		n->classes->at(i)->accept(this);
	}
	count--;
}

void
PrintVisitor::Visit(Class *n)
{
	cout << "<Class>" << PrintId(n->name->name) << endl;

	count++;

	PrintBars(count);
	n->name->accept(this);

	PrintBars(count);
	n->members->accept(this);

	count--;
}

void
PrintVisitor::Visit(ClassInher *n)
{
	cout << "<Class>" << PrintId(n->name->name) << ":" << PrintId(n->basic->name)  << endl;
	count++;

	PrintBars(count);
	n->name->accept(this);
	
	PrintBars(count);
	n->basic->accept(this);
	
	PrintBars(count);
	n->members->accept(this);

	count--;
}

void
PrintVisitor::Visit(Members *n)
{
	cout << "<Members>" << endl;

	count++;
	for(int i=0; i<n->members->size(); i++)
	{
		PrintBars(count);
		n->members->at(i)->accept(this);
	}
	count--;
}

void
PrintVisitor::Visit(Member  *n)
{
	// empty
}

void
PrintVisitor::Visit(Global  *n)
{
	cout << "<Global>" << endl;

	count++;

	PrintBars(count);
	n->accessModif->accept(this);

	PrintBars(count);
	n->type->accept(this);
	
	PrintBars(count);
	n->variables->accept(this);

	count--;
}

void
PrintVisitor::Visit(Constructor *n)
{
	cout << "<Constructor>" << PrintId(n->name->name) << endl;

	count++;

	PrintBars(count);
	n->accessModif->accept(this);
	
	PrintBars(count);
	n->name->accept(this);
	
	PrintBars(count);
	n->args->accept(this);
	
	PrintBars(count);
	n->stats->accept(this);

	count--;
}

void
PrintVisitor::Visit(Function *n)
{
	cout << "<Function>" << PrintId(n->name->name) << endl;

	count++;

	PrintBars(count);
	n->accessModif->accept(this);
	
	if(n->type != NULL)
	{
		PrintBars(count);
		n->type->accept(this);
	}
	
	PrintBars(count);
	n->name->accept(this);
	
	PrintBars(count);
	n->args->accept(this);
	
	PrintBars(count);
	n->stats->accept(this);

	count--;
}

void
PrintVisitor::Visit(Arg  *n)
{
	cout << "Arg" << endl;

	count++;

	PrintBars(count);
	n->type->accept(this);
	
	PrintBars(count);
	n->name->accept(this);

	count--;
}

void
PrintVisitor::Visit(Args *n)
{
	cout << "Args" << endl;
	count++;

	for(int i=0; i<n->args->size(); i++)
	{
		PrintBars(count);
		n->args->at(i)->accept(this);
	}
	count--;
}

void
PrintVisitor::Visit(Variables *n)
{
	cout << "Variables" << endl;
	count++;
	
	for(int i=0; i<n->variables->size(); i++)
	{
		PrintBars(count);
		n->variables->at(i)->accept(this);
	}
	count--;
}

void
PrintVisitor::Visit(Variable  *n)
{
	cout << "Variable" << endl;
	count++;
	
	PrintBars(count);
	n->name->accept(this);
	if(n->expr != NULL)
	{
		PrintBars(count);
		n->expr->accept(this);
	}
}

void
PrintVisitor::Visit(AccessModif *n)
{
	cout << "AccessModif" << this->acctype[n->acctype] << endl;
}

void
PrintVisitor::Visit(Type *n)
{
	if(n->name != NULL)
		cout << "Type" << PrintId(n->name->name) << endl;
	else
		cout << "Type" << types[n->type] << endl;
}

void
PrintVisitor::Visit(NoArrayType *n)
{
	if(n->name != NULL)
		cout << "Type" << PrintId(n->name->name) << endl;
	else
		cout << "Type" << types[n->type] << endl;
}

void
PrintVisitor::Visit(ArrayType *n)
{
	if(n->name != NULL)
		cout << "Type" << " \\" << n->name->name << this->arr_level[n->arr_level] << "\\" << endl;
	else
		cout << "Type" << types[n->type] << this->arr_level[n->arr_level] << endl;
}

void
PrintVisitor::Visit(Ident *n)
{
	cout << "Ident" << PrintId(n->name) << endl;
}

void
PrintVisitor::Visit(Expr *n)
{
	// empty
}

void
PrintVisitor::Visit(Incr *n)
{
	// check if Prev of not.
	if(n->isPrev)
		cout << "Prev-Incr" << endl;
	else
		cout << "Incr" << endl;
	
	count++;

	PrintBars(count);
	n->name->accept(this);

	count--;
}

void
PrintVisitor::Visit(Decr *n)
{
	// check if Prev of not.
	if(n->isPrev)
		cout << "Prev-Decr" << endl;
	else
		cout << "Decr" << endl;
	
	count++;

	PrintBars(count);
	n->name->accept(this);

	count--;
}

void
PrintVisitor::Visit(Not *n)
{
	cout << "Not" << endl;
	
	count++;

	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(Minus *n)
{
	cout << "Minus" << endl;
	
	count++;

	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(Plus *n)
{
	cout << "Plus" << endl;
	
	count++;

	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(Paren  *n)
{
	cout << "Paren" << endl;
	
	count++;

	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(IdentExpr *n)
{
	cout << "IdentExpr" << endl;
	
	count++;

	PrintBars(count);
	n->ident->accept(this);

	count--;
}

void
PrintVisitor::Visit(IdentArr *n)
{
	cout << "IdentArr" << endl;
	
	count++;

	PrintBars(count);
	n->ident->accept(this);
	
	PrintBars(count);
	n->arrayIndex->accept(this);

	count--;
}

void
PrintVisitor::Visit(Assign *n)
{
	cout << "Assign" << endl;

	count++;
	
	PrintBars(count);
	n->ident->accept(this);
	
	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(ArrAssign *n)
{
	cout << "ArrAssign" << endl;

	count++;
	
	PrintBars(count);
	n->ident->accept(this);
	
	PrintBars(count);
	n->arrayIndex->accept(this);
	
	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(Invoke *n)
{
	cout << "Invoke" << endl;
	
	count++;

	PrintBars(count);
	n->ident->accept(this);
	
	PrintBars(count);
	n->exprList->accept(this);

	count--;
}

void
PrintVisitor::Visit(NewObject *n)
{
	cout << "NewObject" << endl;

	count++;
	
	PrintBars(count);
	n->ident->accept(this);
	
	PrintBars(count);
	n->exprList->accept(this);

	count--;
}

void
PrintVisitor::Visit(NewArray *n)
{
}

void
PrintVisitor::Visit(IdentCall *n)
{
}

void
PrintVisitor::Visit(IdentArrCall *n)
{
}

void
PrintVisitor::Visit(Equal *n)
{
	cout << "Equal" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);

	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(NotEq *n)
{
	cout << "NotEq" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Smaller *n)
{
	cout << "Smaller" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(SmallerEq *n)
{
	cout << "SmallerEq" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);
	
	count--;
}

void
PrintVisitor::Visit(Larger *n)
{
	cout << "Larger" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(LargerEq *n)
{
	cout << "LargerEq" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);

	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Add *n)
{
	cout << "Add" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);


	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Sub *n)
{
	cout << "Sub" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Mult *n)
{
	cout << "Mult" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Div *n)
{
	cout << "Div" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Mod *n)
{
	cout << "Mod" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(And *n)
{
	cout << "And" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Or *n)
{
	cout << "Or" << endl;

	count++;
	
	PrintBars(count);
	n->left->accept(this);
	
	PrintBars(count);
	n->right->accept(this);

	count--;
}

void
PrintVisitor::Visit(Is *n)
{
	cout << "Is" << endl;
	
	count++;

	PrintBars(count);
	n->expr->accept(this);
	
	PrintBars(count);
	n->typ->accept(this);

	count--;
}

void
PrintVisitor::Visit(Cast *n)
{
	cout << "Cast" << endl;

	count++;
	
	PrintBars(count);
	n->typ->accept(this);

	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(Integer *n)
{
	cout << "Integer" << " \\" << n->value << "\\" << endl;
}

void
PrintVisitor::Visit(Real *n)
{
	cout << "Real" << " \\" << n->value << "\\" << endl;
}

void
PrintVisitor::Visit(True *n)
{
	cout << "True" << " \\1\\" << endl;
}

void
PrintVisitor::Visit(False *n)
{
	cout << "False" << " \\0\\" << endl;
}

void
PrintVisitor::Visit(This *n)
{
	cout << "This" << endl;
}

void
PrintVisitor::Visit(Null *n)
{
	cout << "Null" << endl;
}

void
PrintVisitor::Visit(ArrayIndex *n)
{
	// empty
}

void
PrintVisitor::Visit(ArrayIndex_1 *n)
{
	cout << "ArrayIndex_1" << endl;

	count++;
	
	PrintBars(count);
	n->expr1->accept(this);

	count--;
}

void
PrintVisitor::Visit(ArrayIndex_2 *n)
{
	cout << "ArrayIndex_2" << endl;
	
	count++;

	PrintBars(count);
	n->expr1->accept(this);
	
	PrintBars(count);
	n->expr2->accept(this);

	count--;
}

void
PrintVisitor::Visit(ArrayIndex_3 *n)
{
	cout << "ArrayIndex_3" << endl;

	count++;

	PrintBars(count);
	n->expr1->accept(this);
	
	PrintBars(count);
	n->expr2->accept(this);
	
	PrintBars(count);
	n->expr3->accept(this);

	count--;
}

void
PrintVisitor::Visit(ExprList *n)
{
	cout << "ExprList" << endl;
	
	count++;

	for(int i=0; i<n->exprList->size(); i++)
	{
		PrintBars(count);
		n->exprList->at(i)->accept(this);
	}
	count--;
}

void
PrintVisitor::Visit(Stat *n)
{
	// empty
}

void
PrintVisitor::Visit(Stats *n)
{
	cout << "Stats" << endl;

	count++;

	for(int i=0; i<n->stats->size(); i++)
	{
		PrintBars(count);
		n->stats->at(i)->accept(this);
	}
	count--;
}

void
PrintVisitor::Visit(If *n)
{
	cout << "If" << endl;

	count++;
	
	PrintBars(count);
	n->expr->accept(this);
	
	PrintBars(count);
	n->stat->accept(this);

	count--;
}

void
PrintVisitor::Visit(IfElse *n)
{
	cout << "IfElse" << endl;

	count++;
	
	PrintBars(count);
	n->expr->accept(this);
	
	PrintBars(count);
	n->stat->accept(this);

	PrintBars(count);
	n->elseStat->accept(this);

	count--;
}

void
PrintVisitor::Visit(While *n)
{
	cout << "While" << endl;

	count++;
	
	PrintBars(count);
	n->expr->accept(this);
	
	PrintBars(count);
	n->stat->accept(this);

	count--;
}

void
PrintVisitor::Visit(For *n)
{
	cout << "For" << endl;

	count++;
	
	PrintBars(count);
	n->variables_e->accept(this);

	PrintBars(count);
	n->exprCond->accept(this);
	
	PrintBars(count);
	n->exprCount->accept(this);
	
	PrintBars(count);
	n->stat->accept(this);

	count--;
}

void
PrintVisitor::Visit(ExprStat *n)
{
	cout << "ExprStat" << endl;
	
	count++;

	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(VariablesStat *n)
{
	cout << "VariablesStat" << endl;

	count++;
	
	PrintBars(count);
	n->type->accept(this);

	PrintBars(count);
	n->variables->accept(this);

	count--;
}

void
PrintVisitor::Visit(Semi *n)
{
	cout << "Semi" << endl;
}

void
PrintVisitor::Visit(Block *n)
{
	cout << "Block" << endl;

	count++;
	
	PrintBars(count);
	n->stats->accept(this);

	count--;
}

void
PrintVisitor::Visit(Return *n)
{
	cout << "Return" << endl;

	count++;
	
	PrintBars(count);
	n->expr->accept(this);

	count--;
}

void
PrintVisitor::Visit(Variables_e *n)
{
	cout << "Variables_e" << endl;

	count++;

	if(n->type != NULL)
	{
		PrintBars(count);
		n->type->accept(this);
	}
	
	PrintBars(count);
	n->variables->accept(this);

	count--;
}