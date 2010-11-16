#include "symbol_table.h"

//***********************************************************************
//					Symbol
//***********************************************************************

Sym::Sym(string name, int kind, int acctype, Type *type)
{
	this->name = name;
	this->kind = kind;
	this->acctype = acctype;
	this->type = type;
	this->location = 0;
	this->clas = NULL;
	this->constructor = NULL;
	this->method = NULL;
	this->args = NULL;
}

Sym::Sym(string name, int kind, Class *clas)
{
	this->name = name;
	this->kind = kind;
	this->acctype = 0;
	this->type = NULL;//0
	/*this->arr_level = arr_level;*/
	this->location = 0;
	this->clas = clas;
	this->constructor = NULL;
	this->method = NULL;
	this->args = NULL;
}

Sym::Sym(string n, int kind, int acctype, Args *args, Constructor *constr)
{
	this->name = n;
	this->kind = kind;
	this->acctype = acctype;
	this->type = NULL;//0
	/*this->arr_level = 0;*/
	this->location = 0;
	this->clas = NULL;
	this->constructor = constr;
	this->method = NULL;
	this->args = args;
}

Sym::Sym(string n, int kind, int acctype, Type *type, Args *args, Function *meth)
{
	this->name = n;
	this->kind = kind;
	this->acctype = acctype;
	this->type = type;
	this->location = 0;
	this->clas = NULL;
	this->constructor = NULL;
	this->method = meth;
	this->args = args;
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
	types[0] = "no type";
	types[1] = "null";
	types[2] = "int";
	types[3] = "double";
	types[4] = "bool";
	types[5] = "object";
	types[6] = "ident";
	types[7] = "void";

	kinds[0] = "";//NuN
	kinds[1] = "c";//class
	kinds[2] = "f";//function
	kinds[3] = "con";//constractur
	kinds[4] = "g";//global variable
	kinds[5] = "l";//local variable
	kinds[6] = "l";//Argument
	
	this->classType = new vector<string>;
	this->current = new Scope();
	this->errors = errors;

	this->current->hashTab->AddKey("f@Write@int", new Sym("Write", 2, 0, 0));
	this->current->hashTab->AddKey("f@Write@double", new Sym("Write", 2, 0, 0));
	this->current->hashTab->AddKey("f@Write@bool", new Sym("Write", 2, 0, 0));
	this->current->hashTab->AddKey("f@Read", new Sym("Read", 2, 0, 0));
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

/********************************************
  ********  IsDeclared Functions  ***********
  ******************************************* */
bool
SymTab::IsDeclared(Ident *id, Deffered *def)
{
	// this function is for: local Ident and global Ident.
	// How: check if the Ident is local, if not check if Ident is global
	//		if not add to deffared as global.
	// used by: (Increment Ident), (Ident Increment),
	//			(Decrement Ident), (Ident Decrement),
	//			(IdentExpr), (IdentArr), (Assign), (ArrAssign),
	//			(IdentCall), (IdentArrCall), (Variables_e variables).

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
			def->AddIdent(id, 4, NULL);
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
SymTab::IsDeclared(Ident *id, int kind, ExprList *el, Deffered *def)
{
	// this function is for: Function Ident and Constructor Ident.
	// How: check if Ident is Declared as Function or as Constructor,
	//      if not add to deffared.
	// used by: (Invoke),
	//			(NewObject).
	string key = this->kinds[kind]+"@"+id->name;
	for(int i=0; i < el->exprList->size(); i++)
	{
		int t = el->exprList->at(i)->type; // We must use this function in TypeVisitor for this line (expr->type).
		if(t == 6)
			key += "@" + types[t];
		else
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
		def->AddIdent(id, kind, el);
		//this->errors->AddError("this is Undeclared Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}
}

bool
SymTab::IsDeclared(Ident *id, int kind, Deffered *def)
{
	// this function for: Specific kind of Ident.
	// How: check if Ident Declared,
	//      if not add to deffared.
	// used by: (ClassInher Ident) ,
	//			(NoArrayType Ident),
	//			(ArrayType Ident[], Ident[,], Ident[,,]),

	string key = this->kinds[kind]+"@"+id->name;
	Sym *sym = this->Lookup(key);
	if(sym != NULL)
	{
		id->symbol = sym;
		return true;
	}
	else
	{
		def->AddIdent(id, kind, NULL);
		return false;
	}
}

/********************************************
  **********  AddSym Functions  *************
  ******************************************* */
bool
SymTab::AddSym(Ident *id, int kind, int acctype, Type *type)
{
	// Add any Ident with its.

	string key = this->kinds[kind]+"@"+id->name;
	if(this->Lookup(key) == NULL)
	{
		Sym *sym = new Sym(id->name, kind, acctype, type);
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
SymTab::AddSym(Ident *id, int kind, Class *clas)
{
	// Add Class Symbol to hash table.

	string key = this->kinds[kind]+"@"+id->name;
	if(this->Lookup(key) == NULL)
	{
		Sym *sym = new Sym(id->name, kind, clas);
		this->current->hashTab->AddKey(key, sym);
		id->symbol = sym;
		this->classType->push_back(id->name);
		return true;
	}
	else
	{
		this->errors->AddError("Redefinition of Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}
}

bool
SymTab::AddSym(Ident *id, int kind, int acctype, Args *args, Constructor *constr)
{
	// Add Constructor Symbol to hash table.

	string key = this->kinds[kind]+"@"+id->name;
	for(int i=0; i < args->args->size(); i++)
	{
		int t = args->args->at(i)->type->type;
		// if type of arg is Ident then write to key the name of Ident.
		if(t == 6)
			key += "@" + args->args->at(i)->type->name->name;
		else
			key += "@" + types[t];
	}

	if(this->Lookup(key) == NULL)
	{
		Sym *sym = new Sym(id->name, kind, acctype, args, constr);
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
SymTab::AddSym(Ident *id, int kind, int acctype, Type *type, Args *args, Function *meth)
{
	// Add Function Symbol to hash table.

	string key = this->kinds[kind]+"@"+id->name;
	for(int i=0; i < args->args->size(); i++)
	{
		int t = args->args->at(i)->type->type;
		if(t == 6)
			key += "@" + args->args->at(i)->type->name->name;
		else
			key += "@" + types[t];
	}

	if(this->Lookup(key) == NULL)
	{
		Sym *sym = new Sym(id->name, kind, acctype, type, args, meth);
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
	types[0] = "null";
	types[1] = "int";
	types[2] = "double";
	types[3] = "bool";
	types[4] = "object";
	types[5] = "ident";
	types[6] = "void";
	
	kindArr[0] = "";//NuN
	kindArr[1] = "c";//class
	kindArr[2] = "f";//function
	kindArr[3] = "con";//constractur
	kindArr[4] = "g";//global variable
	kindArr[5] = "l";//local variable
	kindArr[6] = "l";//Argument

	this->ids = new vector<Ident *>;
	this->kinds = new vector<int>;
	this->exprLists = new vector<ExprList *>;
}

void
Deffered::AddIdent(Ident *id, int kind, ExprList *el)
{
	this->ids->push_back(id);
	this->kinds->push_back(kind);
	this->exprLists->push_back(el);
}

void
Deffered::CheckAll(SymTab *symtab)
{
	Scope *scope = symtab->current;
	for(int i=0; i < this->ids->size(); i++)
	{
		string key = this->kindArr[this->kinds->at(i)]+"@"+this->ids->at(i)->name;

		if(this->exprLists->at(i) != NULL)
			for(int j=0; j<this->exprLists->size(); j++)
			{
				int t = this->exprLists->at(i)->exprList->at(j)->type; // We must use this function in TypeVisitor for this line (expr->type).
				if(t == 6)
					key += "@" + types[t];
				else
					key += "@" + types[t];
			}

		// Add ExprList types to key if this id is function or constructor.
		/*if(this->exprLists->at(i) != NULL)
		{
			for(int j=0; j<this->exprLists->at(i)->exprList->size(); j++)
			{
				int ty = this->exprLists->at(i)->exprList->at(j)->type;
				key += "@" + this->types[ty];
			}
		}*/

		Sym *sym = NULL;

		// if Ident is a Class Ident.
		if(this->kinds->at(i) == 1)
			sym = scope->hashTab->GetMember(key);
		
		// if Ident is a global Ident.
		else if(this->kinds->at(i) == 4)
			for(int j=0; j<scope->children->size(); j++)
			{
				sym = scope->children->at(j)->hashTab->GetMember(key);
				if(sym != NULL)
				{
					this->ids->at(i)->symbol = sym;
					break;
				}
			}

		if(sym == NULL)
			symtab->errors->AddError("Undeclared Identifier '" + this->ids->at(i)->name + "'", this->ids->at(i)->line, this->ids->at(i)->column);

		/*Sym  *sym = NULL;
		while(scope != NULL)
		{
			sym = scope->hashTab->GetMember(name);
			if(sym != NULL)
				return sym;
			scope = scope->father;
		}

		return NULL;*/


		/*Sym *sym = symtab->Lookup(key);
		if(sym != NULL)
			this->ids->at(i)->symbol = sym;
		else
			symtab->errors->AddError("Undeclared Identifier '" + this->ids->at(i)->name + "'", this->ids->at(i)->line, this->ids->at(i)->column);*/
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