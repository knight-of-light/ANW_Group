#include "symbol_table.h"

//***********************************************************************
//					Symbol
//***********************************************************************
Sym::Sym(string name, int kind, int type, int arr_level)
{

	this->name = name;
	this->kind = kind;
	this->type = type;
	this->arr_level = arr_level;
	this->constructor = NULL;
	this->method = NULL;
	this->argTypes = NULL;
	this->location = 0;
}

Sym::Sym(string n, int kind, int type, Args *ps, Constructor *constr)
{
	this->name = n;
	this->kind = kind;
	this->type = type;
	this->arr_level = 0;
	this->constructor = constr;
	this->method = NULL;
	this->argTypes = new vector<int>;

	for(int i=0; i< ps->args->size(); i++)
		this->argTypes->push_back(ps->args->at(i)->type->type);
	
}

Sym::Sym(string n, int kind, int type, int arr_level, Args *ps, int returnType, Function *meth)
{
	this->name = n;
	this->kind = kind;
	this->type = type;
	this->arr_level = arr_level;
	this->constructor = NULL;
	this->method = meth;
	this->returnType = returnType;
	this->argTypes = new vector<int>;

	for(int i = 0; i < ps->args->size(); i++)
		this->argTypes->push_back(ps->args->at(i)->type->type);
}

//***********************************************************************
//					Scope
//***********************************************************************
Scope::Scope()
{
	this->father = NULL;
	this->hashTab = new HashTab;
	this->children = new vector<Scope *>;
}

void
Scope::AddChild()
{
	Scope *child = new Scope();
	this->children->push_back(child);
	child->father = this;
}

//***********************************************************************
//					SymTab
//***********************************************************************
SymTab::SymTab(Errors *errors)
{
	this->errors = errors;
	types[0] = "null";
	types[1] = "int";
	types[2] = "double";
	types[3] = "bool";
	types[4] = "void";
	types[5] = "object";
	types[6] = "ident";

	kinds[0] = "";//NuN
	kinds[1] = "c";//class
	kinds[2] = "f";//function
	kinds[3] = "con";//constractur
	kinds[4] = "g";//global variable
	kinds[5] = "l";//local variable
	kinds[6] = "l";//Argument

	this->current = new Scope();
	
	this->current->hashTab->AddKey("f@Write@int", new Sym("Write", -1, -1, 0));
	this->current->hashTab->AddKey("f@Write@double", new Sym("Write", -1, -1, 0));
	this->current->hashTab->AddKey("f@Write@bool", new Sym("Write", -1, -1, 0));
	this->current->hashTab->AddKey("f@Read", new Sym("Read", -1, -1, 0));
}

Sym *
SymTab::Lookup(std::string name)
{
	Scope *scope = this->current;
	Sym  *sym = NULL;
	while(scope != NULL)
	{
		sym = scope->hashTab->GetMember(name);
		if(sym != NULL)
			return sym;
		scope = scope->father;
	}

	return NULL;
}

bool
SymTab::IsDeclared(Ident *id)
{
	string key = "l@"+id->name;
	Sym *sym = this->Lookup(key);
	if(sym != NULL)
	{
		id->symbol = sym;
		return true;
	}
	else
	{
		key = "g@"+id->name;
		sym = this->Lookup(key);
		if(sym != NULL)
		{
			id->symbol = sym;
			return true;
		}
		else
		{
			this->errors->AddError("Undeclared Identifier '" + id->name + "'", id->line, id->column);
			return false;
		}
	}
}

bool
SymTab::IsDeclared(Ident *id, int kind)
{
	string key = this->kinds[kind]+"@"+id->name;
	Sym *sym = this->Lookup(key);
	if(sym != NULL)
	{
		id->symbol = sym;
		return true;
	}
	else
	{
		this->errors->AddError("Undeclared Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}
}

/*
	SymTab::IsDeclared(Ident *id, ExprList *el)
	this function is for the case of invoking 
	a function like func(3+2, 6 * x), there are expresions list (el)
*/
bool
SymTab::IsDeclared(Ident *id, int kind, ExprList *el)
{
	string key = this->kinds[kind]+"@"+id->name;
	for(int i=0; i < el->exprList->size(); i++)
	{
		int t = el->exprList->at(i)->type;
		key += "@" + types[t];
	}
	Sym *sym = this->Lookup(key);
	if(sym != NULL)
	{
		id->symbol = sym;
		return true;
	}
	else
	{
		this->errors->AddError("Undeclared Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}
}

bool
SymTab::IsDeclared(Ident *id, Deffered *def)
{
	//string key = this->kinds[kind]+"@"+id->name;
	Sym *sym = this->Lookup(id->name);
	if(sym != NULL)
	{
		id->symbol = sym;
		return true;
	}
	else
	{
		def->AddIdent(id);
		return false;
	}
}

bool
SymTab::AddSym(Ident *id, int kind, int type, int arr_level)
{
	string key = this->kinds[kind]+"@"+id->name;
	if(this->Lookup(key) == NULL)
	{
		Sym *sym = new Sym(id->name, kind, type, arr_level);
		this->current->hashTab->AddKey(key, sym);
		id->symbol = sym;
		return true;
	}
	else
	{
		this->errors->AddError("Redefinition of Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}
}

bool
SymTab::AddSym(Ident *id, int kind, int type, Args *ps, Constructor *constr)
{
	string key = this->kinds[kind]+"@"+id->name;
	for(int i=0; i < ps->args->size(); i++)
	{
		int t = ps->args->at(i)->type->type;
		key += "@" + types[t];
	}

	if(this->Lookup(key) == NULL)
	{
		Sym *sym = new Sym(id->name, kind, type, ps, constr);
		this->current->hashTab->AddKey(key, sym);
		id->symbol = sym;
		return true;
	}
	else
	{
		this->errors->AddError("Redefinition of Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}
}

bool
SymTab::AddSym(Ident *id, int kind, int type, int arr_level, Args *ps, int returnType, Function *meth)
{
	string key = this->kinds[kind]+"@"+id->name;
	for(int i=0; i < ps->args->size(); i++)
	{
		int t = ps->args->at(i)->type->type;
		key += "@" + types[t];
	}

	if(this->Lookup(key) == NULL)
	{
		Sym *sym = new Sym(id->name, kind, type, arr_level, ps, returnType, meth);
		this->current->hashTab->AddKey(key, sym);
		id->symbol = sym;
		return true;
	}
	else
	{
		this->errors->AddError("Redefinition of Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}
}

void
SymTab::AddNewScope()
{
	this->current->AddChild();
	this->current = this->current->children->at(this->current->children->size() - 1);
}

void
SymTab::OutScope()
{
	this->current = this->current->father;
}
//
//void 
//SymTab::AddVars(Variables *v, Type *et)
//{
//	for(int i=0; i < v->variables->size(); i++)
//		this->AddSym(v->variables->at(i)->name, 4, et->type);
//}


//***********************************************************************
//					Deffered
//***********************************************************************
Deffered::Deffered()
{
	this->ids = new vector<Ident *>;
}

void
Deffered::AddIdent(Ident *id)
{
	this->ids->push_back(id);
}

void
Deffered::CheckAll(SymTab *symtab)
{
	for(int i=0; i < this->ids->size(); i++)
	{
		Sym *sym = symtab->current->hashTab->GetMember(this->ids->at(i)->name);
		if(sym != NULL)
			this->ids->at(i)->symbol = sym;
		else
			symtab->errors->AddError("Undeclared Identifier '" + this->ids->at(i)->name + "'", this->ids->at(i)->line, this->ids->at(i)->column);
	}

}

//***********************************************************************
//					Error
//***********************************************************************
Error::Error(string message, int line, int column)
{
	this->message = message;
	this->line = line;
	this->column = column;
}

Errors::Errors()
{
	this->messages = new vector<Error *>;
}

void
Errors::AddError(std::string message, int line, int column)
{
	Error *error = new Error(message, line, column);
	this->messages->push_back(error);
}

void
Errors::Print()
{
	int size = this->messages->size();
	if(size == 0)
		cout << "Semntic analysis was done successfully! " << endl;
	else
	{
		cout << "There are " << size << " semantic errors: " << endl;
		for(int i = 0; i<size; i++)
		{
			cout << this->messages->at(i)->message << " at line: "
				<< this->messages->at(i)->line << " , column: "
				<< this->messages->at(i)->column << endl;
		}
	}
}