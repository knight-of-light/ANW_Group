#ifndef	SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

#include <vector>
using std::vector;

#include <string>
using std::string;

#include "hash_table.h"

//***********************************************************************
//					Symbol
//***********************************************************************

class Sym
{
public:
	//1:class 2: func,3: constructor, 4: global Global, , 5: Local Global, 6: Argument
	int kind;
	string name;
	//-1: no type, 0 = Null, 1 = int , 2 = double , 3 = boolean, 4: void
	int type;
	vector<int> *argTypes;
	int returnType;
	Function *method;

	Sym(string n, int kind, int type);
	Sym(std::string , int kind, int type, Args * ps, int returnType, Function *meth);	
};

//*******      HashTab		*********
typedef CHashTable<Sym> HashTab;

//*******       Scope		*********
class Scope
{
public:
	Scope *father;
	vector<Scope *> *children;
	HashTab	*hashTab;

	Scope();
	void AddChild();	
};

//*******   Symbol Table	*********
class SymTab
{
private:
	string types [5];
	string kinds[5];
public:
	Scope *current;
	Errors *errors;

	SymTab(Errors *errors);
	Sym *Lookup(string name);
	bool IsDeclared(Ident *id );
	bool IsDeclared(Ident *id, ExprList *el);
	bool IsDeclared(Ident *id, Deffered *def);
	bool IsDeclared(Ident *id , int kind  ,int type );
	bool AddSym(Ident *id, int kind, int type);
	bool AddSym(Ident *id, int kind, int type, Args *ps, int returnType, Function *meth);	

	void AddNewScope();
	void OutScope();

	void AddVars(Variables *v, Type *et);
};

//***********************************************************************
//					Deffered
//***********************************************************************

class Deffered
{
public:
	vector<Ident *> *ids;
	//vector<Sym *> *syms;
	Deffered();
	void AddIdent(Ident *);
	void CheckAll(SymTab *symtab);
};


//***********************************************************************
//					Error
//***********************************************************************

class Error
{
public:
	string message;
	int line;
	int column;

	Error(string message, int line, int column);
};

class Errors
{
public:
	vector<Error *> *messages;
	Errors();
	void AddError(string message, int line, int column);
	void Print();
};
/*
#ifndef	SYMBOL_TABLE_H
#define	SYMBOL_TABLE_H

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "hash_table.h"

#include "headers.h"

class Sym;
class Scope;
class SymTab;

class Sym
{
public:
	//1: func, 2: args, 3: variable, 
	int kind;
	string name;
	int type;
	vector<int> *argTypes;
	int returnType;
	Sym(std::string , int kind, int type, Args * ps, int returnType);
	Sym(string n, int kind, int type);
	
};

typedef CHashTable<Sym> HashTab;

class Scope
{
public:
	Scope *father;
	vector<Scope *> *children;
	HashTab	*hashTab;

	Scope();
	void AddChild();

	
};

class SymTab
{
public:
	Scope *current;
	Errors *errors;

	SymTab(Errors *errors);

	void AddNewScope();
	void OutScope();

	Sym Lookup(string name);
	bool AddSym(Ident *id, int kind, int type);
	bool AddSym(Ident *id, int kind, int type, Args *ps, int returnType);	
};

#endif

*/
#endif