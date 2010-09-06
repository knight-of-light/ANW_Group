
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