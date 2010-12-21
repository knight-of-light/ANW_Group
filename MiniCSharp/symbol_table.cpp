#include "symbol_table.h"

//***********************************************************************
//					Symbol
//***********************************************************************

Sym::Sym(string name, int kind, int acctype, Scope *scope, Type *type)
{
	this->name = name;
	this->kind = kind;
	this->acctype = acctype;
	this->type = type;
	this->location = 0;
	this->scope = scope;
	this->clas = NULL;
	this->constructor = NULL;
	this->method = NULL;
	this->args = NULL;
}

Sym::Sym(string name, int kind, Scope *scope, Class *clas)
{
	this->name = name;
	this->kind = kind;
	this->acctype = 0;
	this->type = NULL;
	this->location = 0;
	this->scope = scope;
	this->clas = clas;
	this->constructor = NULL;
	this->method = NULL;
	this->args = NULL;
}

Sym::Sym(string n, int kind, int acctype, Scope *scope, Args *args, Constructor *constr)
{
	this->name = n;
	this->kind = kind;
	this->acctype = acctype;
	this->type = NULL;
	this->location = 0;
	this->scope = scope;
	this->clas = NULL;
	this->constructor = constr;
	this->method = NULL;
	this->args = args;
}

Sym::Sym(string n, int kind, int acctype, Scope *scope, Type *type, Args *args, Function *meth)
{
	this->name = n;
	this->kind = kind;
	this->acctype = acctype;
	this->type = type;
	this->location = 0;
	this->scope = scope;
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
	this->number = 0;
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
	
	this->classType = new vector<string>; //?
	this->Invoke_scope_num = new vector<int>;

	this->current = new Scope();
	this->scope_counter = 0;
	this->errors = errors;

	this->classes = new vector<Class *>;
	this->scopes = new vector<Scope *>;

	// Add Write and Read Functions.
	this->current->hashTab->AddKey("f@Write@int", new Sym("Write", 2, 0, 0));//?
	this->current->hashTab->AddKey("f@Write@double", new Sym("Write", 2, 0, 0));//?
	this->current->hashTab->AddKey("f@Write@bool", new Sym("Write", 2, 0, 0));//?
	this->current->hashTab->AddKey("f@Read", new Sym("Read", 2, 0, 0));//?
}

Sym *
SymTab::Lookup(std::string key)
{
	Scope *scope = this->current;
	Sym  *sym = NULL;
	while(scope != NULL)
	{
		sym = scope->hashTab->GetMember(key);
		if(sym != NULL)
			return sym;
		scope = scope->father;
	}

	return NULL;
}

/********************************************
  **********  AddSym Functions  *************
  ******************************************* */
bool
SymTab::AddSym(Ident *id, int kind, int acctype, Type *type)
{
	// Add Global and Lockal(stat var, for var) Ident.

	string key = this->kinds[kind]+"@"+id->name;
	if(this->Lookup(key) == NULL) // if there is no id have same key, then OK.
	{
		Sym *sym = new Sym(id->name, kind, acctype, this->current, type);
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
	if(this->Lookup(key) == NULL) // if there is no id have same key, then OK.
	{
		Sym *sym = new Sym(id->name, kind, this->current, clas);
		this->current->hashTab->AddKey(key, sym);
		id->symbol = sym;
		this->classType->push_back(id->name);//?
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

	if( acctype != 4) // check if acctype didn't contain "private static".
	{
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

		if(this->Lookup(key) == NULL) // if there is no id have same key, then OK.
		{
			Sym *sym = new Sym(id->name, kind, acctype, this->current, args, constr);
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
	else
	{
		this->errors->AddError("Access modifiers are not allowed on static constructors", id->line, id->column);
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
		// if type of arg is Ident then write to key the name of Ident.
		if(t == 6)
			key += "@" + args->args->at(i)->type->name->name;
		else
			key += "@" + types[t];
	}

	if(this->Lookup(key) == NULL) // if there is no id have same key, then OK.
	{
		Sym *sym = new Sym(id->name, kind, acctype, this->current, type, args, meth);
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

/********************************************
  ********  IsDeclared Functions  ***********
  ******************************************* */
bool
SymTab::IsDeclared(Ident *id, Deffered *def)
{
	/* this function is for: local Ident and global Ident.
	** How: check if the Ident is local, if not check if Ident is global
	**		if not add to deffared as global.
	** used by: (Increment Ident), (Ident Increment),
	**			(Decrement Ident), (Ident Decrement),
	**			(IdentExpr), (IdentArr), (Assign), (ArrAssign),
	**			(IdentCall), (IdentArrCall), (Variables_e variables).
	*/

	// Check if Local.
	string key = "l@"+id->name;
	Sym *sym = this->Lookup(key);
	if(sym != NULL)
	{
		id->symbol = sym;
		return true;
	}
	else
	{
		// Check if Global.
		key = "g@"+id->name;
		sym = this->Lookup(key);
		if(sym != NULL)
		{
			id->symbol = sym;
			return true;
		}
		else
		{
			// Check if Global later.
			def->AddIdent(id, 4, this->current->number);
			return false;
		}
	}
}

bool
SymTab::IsDeclared(Ident *id, int kind, ExprList *el, int CurrentScope)
{
	/*	this function is for: Function Ident and Constructor Ident.
	**	How: check if Ident is Declared as Function or as Constructor,
	**		 if not add to deffared.
	**		 this function is for the case of invoking 
	**		 a function like func(3+2, 6 * x), there are expresions list (el)
	**	used by: (Invoke),
	**			 (NewObject).
	*/
	Scope *scope = this->current;
	Sym *sym = NULL;

	// know the key.
	string key = this->kinds[kind]+"@"+id->name;
	for(int i=0; i < el->exprList->size(); i++)
	{
		int t = el->exprList->at(i)->type->type; // We must use this function in TypeVisitor for this line (expr->type).
		if(t == 6)
			key += "@" + el->exprList->at(i)->type->name->name;
		else
			key += "@" + types[t];
	}

	sym = this->IntToScope(CurrentScope)->hashTab->GetMember(key);
	if(sym == NULL)
	{
		Class *clas = this->ScopeToClass(this->IntToScope(CurrentScope));
		for(int i=0; i<clas->Parents->size(); i++)
		{
			sym = this->ClassToScope(clas->Parents->at(i)->symbol->clas)->hashTab->GetMember(key);
			if(sym != NULL)
			{
				if(sym->acctype == 1 || sym->acctype == 3)
				{
					id->symbol = sym;
					return true;
				}
				else
				{
					this->errors->AddError("Cannot Access Identifier '" + id->name + "' from this scope", id->line, id->column);
					return false;
				}
			}
		}
	}
	if(sym == NULL)
	{
		this->errors->AddError("Undeclared Identifier '" + id->name + "' from this scope", id->line, id->column);
		return false;
	}

	/*for(int i=0; i<scope->children->size(); i++)
	{
		sym = scope->children->at(i)->hashTab->GetMember(key);
		if(sym != NULL)
		{
			if( this->CheckScopes(sym->scope->number,CurrentScope) )
			{
				id->symbol = sym;
				return true;
			}
			else
			{
				this->errors->AddError("Cannot Access this Identifier '" + id->name + "' from this scope", id->line, id->column);
				return false;
			}
		}
	}

	if(sym == NULL)
	{
		this->errors->AddError("Undeclared Identifier '" + id->name + "'", id->line, id->column);
		return false;
	}*/
}

bool
SymTab::IsDeclared(Ident *id, int kind, Deffered *def)
{
	/* this function for: Specific kind of Ident.
	** How: check if Ident Declared,
	**      if not add to deffared.
	** used by: (ClassInher Ident) ,
	**			(NoArrayType Ident),
	**			(ArrayType Ident[], Ident[,], Ident[,,]),
	*/
	string key = this->kinds[kind]+"@"+id->name;
	Sym *sym = this->Lookup(key);
	if(sym != NULL)
	{
		id->symbol = sym;
		return true;
	}
	else
	{
		def->AddIdent(id, kind, this->current->number);
		return false;
	}
}

/********************************************
  ***********  Other Functions  *************
  ******************************************* */

void
SymTab::AddNewScope()
{
	this->scope_counter = this->scope_counter + 1;
	this->current->AddChild();
	this->current = this->current->children->at(this->current->children->size() - 1);
	this->current->number = this->scope_counter;
}

void
SymTab::OutScope()
{
	this->current = this->current->father;
}

void
SymTab::AddInvokeScopeNum()
{
	// to know the number of scope where Invoke called or used.
	this->Invoke_scope_num->push_back(this->current->number);
}

void
SymTab::FillingRelations(Root *file)
{
	// Filling All Parents of classes and Childrens of classes for all classes in AST.
	// And filling SymTab->classes and SymTab->scope_of_classes.
	Class *father;
	Class *son;
	for(int i=0; i<file->classes->size(); i++)
	{
		// Go to all classes in AST.
		for(int j=0; j<file->classes->size(); j++)
		{
			// Put in son this class.
			son = file->classes->at(j);
			// If this class is Inher class <OK>.
			if(son->basic != NULL)
			{
				// Put in father the basic class of this class.
				father = son->basic->symbol->clas;

				// Add son to Childrens vector of father.
				father->AddChild(son->name);
				// Add father to Parents vector of son.
				son->AddParent(father->name);

				// Add all of son Children to father Children.
				for(int k=0; k<son->Childrens->size(); k++)
					father->AddChild(son->Childrens->at(k));
				
				// Add all of father Parents to son Parents.
				for(int k=0; k<father->Parents->size(); k++)
					son->AddParent(father->Parents->at(k));
			}
		}
	}
	
	// Add All classes in AST to classes in SymTab.
	for(int i=0; i<file->classes->size(); i++)
		this->classes->push_back(file->classes->at(i));

	// Add All classes scope to scope_of_classes in SymTab.
	Scope *scope = this->current;
	for(int i=0; i<scope->children->size(); i++)
		this->scopes->push_back(scope->children->at(i));
}

int
SymTab::FatherScopeNum(int NumOfSubScope)
{
	int classScopeNum = -1;
	for(int i=0; i<this->current->children->size(); i++)
	{
		if(this->current->children->at(i)->number <= NumOfSubScope)
		{
			if((i+1) < this->current->children->size())
			{
				if(this->current->children->at(i+1)->number > NumOfSubScope)
				{
					classScopeNum = this->current->children->at(i)->number;
					break;
				}
			}
			else
			{
				classScopeNum = this->current->children->at(i)->number;
				break;
			}
		}
	}
	return classScopeNum;
}

bool
SymTab::CheckScopes(int num_of_scope_of_declaration, int num_of_scope_of_used)
{
	// Check if the two number are from the same class scope.

	int declaration = -2;
	int used = -2;

	// know the number of scope of class that contain Declaration of symbol.
	declaration = FatherScopeNum(num_of_scope_of_declaration);
	
	// know the number of scope of class that contain Used of symbol.
	used = FatherScopeNum(num_of_scope_of_used);

	// Compair father and son.
	if(declaration == used)
		return true;

	return false;
}

Scope*
SymTab::IntToScope(int num)
{
	// From Father scope num to Father Scope.
	Scope *scope = NULL;
	int fatherNumber = this->FatherScopeNum(num);
	for(int i=0; i<this->scopes->size(); i++)
		if(this->scopes->at(i)->number == fatherNumber)
			scope = this->scopes->at(i);

	return scope;
}

Scope*
SymTab::ClassToScope(Class *clas)
{
	Scope *scope = NULL;
	for(int i=0; i<this->classes->size(); i++)
		if(this->classes->at(i)->name->name == clas->name->name)
			scope = this->scopes->at(i);

	return scope;
}

Class*
SymTab::ScopeToClass(Scope *scope)
{
	Class *clas = NULL;
	for(int i=0; i<this->scopes->size(); i++)
		if(this->scopes->at(i)->number == scope->number)
			clas = this->classes->at(i);

	return clas;
}

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
	this->scope_num = new vector<int>;
}

void
Deffered::AddIdent(Ident *id, int kind, int scope_num)
{
	this->ids->push_back(id);
	this->kinds->push_back(kind);
	this->scope_num->push_back(scope_num);
}

void
Deffered::CheckClass(SymTab *symtab)
{
	// Check for Classes.
	Scope *scope = symtab->current;
	for(int i=0; i<this->ids->size(); i++)
	{
		if(this->kinds->at(i) == 1) // If this id is class.
		{
			string key = this->kindArr[this->kinds->at(i)] + "@" + this->ids->at(i)->name;
			Sym *sym = NULL;
			sym = scope->hashTab->GetMember(key);
			if(sym != NULL)
				this->ids->at(i)->symbol = sym;
			else
				symtab->errors->AddError("Undeclared Identifier '" + this->ids->at(i)->name + "'", this->ids->at(i)->line, this->ids->at(i)->column);
		}
	}
}

void
Deffered::CheckCircular(SymTab *symtab)
{
	// Check if there are Circular of Classes
	bool out = false;
	for(int i=0; i<symtab->classes->size(); i++) // For each class.
	{
		for(int j=0; j<symtab->classes->at(i)->Parents->size(); j++) // See
		{
			if(out)
				continue;
			for(int k=0; k<symtab->classes->at(i)->Childrens->size(); k++)
			{
				if(symtab->classes->at(i)->Parents->at(j)->name == symtab->classes->at(i)->Childrens->at(k)->name)
				{
					symtab->errors->AddError("Circular base class dependency involving \'" +
					 symtab->classes->at(i)->name->name +
					 "\' and \'" +
					 symtab->classes->at(i)->Childrens->at(k)->name +
					 "\'",
					 symtab->classes->at(i)->name->line,
					 symtab->classes->at(i)->name->column);

					out = true;
					break;
				}
			}
		}
	}
}

void
Deffered::CheckAll(SymTab *symtab)
{
	// Check for global variables.
	Scope *scope = symtab->current;
	Sym *sym = NULL;
	for(int i=0; i < this->ids->size(); i++)
	{
		if(this->kinds->at(i) == 4) // If id is a Global.
		{
			string key = this->kindArr[this->kinds->at(i)]+"@"+this->ids->at(i)->name;

			//for(int j=0; j<scope->children->size(); j++)
			//{
			//	sym = scope->children->at(j)->hashTab->GetMember(key);
			//	if(sym != NULL) // If id is declared.
			//	{
			//		if( symtab->CheckScopes(sym->scope->number, this->scope_num->at(i)) ) // If id is declared in same scope of used.
			//			this->ids->at(i)->symbol = sym;
			//		else
			//		{
			//			Class *clas = symtab->ScopeToClass(symtab->IntToScope(this->scope_num->at(i)));
			//			for(int k=0; k<clas->Parents->size(); k++)
			//				if(symtab->ClassToScope(clas->Parents->at(k)->symbol->clas)->number == symtab->FatherScopeNum(sym->scope->number))
			//					if(sym->acctype == 1 || sym->acctype == 3)
			//						this->ids->at(i)->symbol = sym;
			//		}
			//	}
			//}
			//if(this->ids->at(i)->symbol == NULL)
			//{
			//	if(sym == NULL)
			//		symtab->errors->AddError("Undeclared Identifier '" + this->ids->at(i)->name + "'", this->ids->at(i)->line, this->ids->at(i)->column);
			//	else
			//		symtab->errors->AddError("Cannot Access Identifier '" + this->ids->at(i)->name + "' from this scope", this->ids->at(i)->line, this->ids->at(i)->column);
			//}

			sym = symtab->IntToScope(symtab->FatherScopeNum(this->scope_num->at(i)))->hashTab->GetMember(key);
			if(sym == NULL)
			{
				Class *clas = symtab->ScopeToClass(symtab->IntToScope(this->scope_num->at(i)));
				for(int j=0; j<clas->Parents->size(); j++)
				{
					sym = symtab->ClassToScope(clas->Parents->at(j)->symbol->clas)->hashTab->GetMember(key);
					if(sym != NULL)
					{
						if(sym->acctype == 1 || sym->acctype == 3)
							this->ids->at(i)->symbol = sym;
						else
							symtab->errors->AddError("Cannot Access Identifier '" + this->ids->at(i)->name + "' from this scope", this->ids->at(i)->line, this->ids->at(i)->column);
						break;
					}
				}
			}
			if(sym == NULL)
				symtab->errors->AddError("Undeclared Identifier '" + this->ids->at(i)->name + "' from this scope", this->ids->at(i)->line, this->ids->at(i)->column);
		}
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