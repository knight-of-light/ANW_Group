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
	string name;

	// 1: class, 2: func, 3: constructor, 4: global Global, 5: Local Global.
	int kind;

	// 0: no accessModef, 1: public, 2: private, 3: static, 4: private static.
	int acctype;
	
	//// 0: no type, 1: Null, 2: int , 3: double , 4: boolean, 5: Object, 6: Ident, 7: void.
	//int type;
	//
	//// 0: no array, 1: [], 2: [,], 3: [,,].
	//int arr_level;

	// to spicific number in memory use in CodeVisitor.
	int location;

	Type *type;
	Class *clas;
	//// if type is 6 (Ident)
	//string classType;

	Constructor *constructor;
	Function *method;
	Args *args;

	Sym(string n, int kind, int acctype, Type *type); // Ident
	Sym(string n, int kind, Class *clas); // Class Ident
	Sym(string n, int kind, int acctype, Args *args, Constructor *constr); // Constructor Ident
	Sym(string n, int kind, int acctype, Type *type, Args *args, Function *meth); // Function Ident
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
	string acctype[5];
	string types[8];
	string kinds[7];
public:
	vector<string> *classType;
	Scope *current;
	Errors *errors;

	SymTab(Errors *errors);
	Sym *Lookup(string name);

	bool IsDeclared(Ident *id, Deffered *def); // local and global Ident.
	bool IsDeclared(Ident *id, int kind); // Class
	bool IsDeclared(Ident *id, int kind, ExprList *el, Deffered *def);
	bool IsDeclared(Ident *id, int kind, Deffered *def);

	bool AddSym(Ident *id, int kind, int acctype, Type *type);
	bool AddSym(Ident *id, int kind, Class *clas);
	bool AddSym(Ident *id, int kind, int acctype, Args *args, Constructor *constr);
	bool AddSym(Ident *id, int kind, int acctype, Type *type, Args *args, Function *meth);	

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
	string types[7];
	string kindArr[7];
	vector<Ident *> *ids;
	vector<int> *kinds;
	vector<ExprList *> *exprLists;
	//vector<Sym *> *syms;
	Deffered();
	void AddIdent(Ident *, int, ExprList *);
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