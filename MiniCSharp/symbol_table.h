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
	//1: class, 2: func, 3: constructor, 4: global Global, 5: Local Global
	int kind;
	string name;
	//-1: no type, 0 = Null, 1 = int , 2 = double , 3 = boolean, 4: Object, 5: Ident
	int type;
	// 0: no array, 1: [], 2: [,], 3: [,,]
	int arr_level;
	// to spicific number in memory
	int location;

	vector<int> *argTypes;
	Class *clas;
	Constructor *constructor;
	Function *method;

	Sym(string n, int kind, int type, int arr_level);
	Sym(string n, int kind, int arr_level, Class *clas);
	Sym(std::string , int kind, Args *ps, Constructor *constr);
	Sym(std::string , int kind, int returnType, int arr_level, Args *ps, Function *meth);	
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
	string types[7];
	string kinds[7];
public:
	Scope *current;
	Errors *errors;

	SymTab(Errors *errors);
	Sym *Lookup(string name);
	bool IsDeclared(Ident *id);
	bool IsDeclared(Ident *id, int kind);
	bool IsDeclared(Ident *id, int kind, ExprList *el);
	bool IsDeclared(Ident *id, int kind, Deffered *def);
	bool AddSym(Ident *id, int kind, int type, int arr_level);
	bool AddSym(Ident *id, int kind, int arr_level, Class *clas);
	bool AddSym(Ident *id, int kind, Args *ps, Constructor *constr);
	bool AddSym(Ident *id, int kind, int returnType, int arr_level, Args *ps, Function *meth);	

	void AddNewScope();
	void OutScope();

	//void AddVars(Variables *v, Type *et);
};

//***********************************************************************
//					Deffered
//***********************************************************************

class Deffered
{
public:
	string kindArr[7];
	vector<Ident *> *ids;
	vector<int> *kinds;
	//vector<Sym *> *syms;
	Deffered();
	void AddIdent(Ident *, int);
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

#endif