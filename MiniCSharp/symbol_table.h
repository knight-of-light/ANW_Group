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
	string name; // Ident name.

	// 1: class, 2: func, 3: constructor, 4: Global, 5: Local Global.
	int kind; // Ident kind.

	// 0: no accessModef, 1: public, 2: private, 3: static, 4: private static.
	int acctype; // if Global, Constructor or Function.

	int location; // Ident location where is stored in memory, used in CodeVisitor.
	int global_location; // Ident location where is stored in object memory, used in CodeVisitor.
	int ReturnValue;
	vector<Ident> *Globals;

	Scope *scope; // scope where the Ident Declared (not where it used).

	Type *type; // Type of Ident, contain type, array level and Ident if its type is class.

	Class *clas; // If the Ident is a Class name, pointer to its class.
	Constructor *constructor; // If the Ident is a Constructor name, pointer to its Constructor.
	Function *method; // If the Ident is a Function name, pointer to its Function.
	Args *args; // If the Ident is a Constructor or a Function, pointer to its Arguments.

	Sym(string n, int kind, int acctype, Scope *scope, Type *type); // If the Ident is not Class, Constructor and Function use this Constructor.
	Sym(string n, int kind, Scope *scope, Class *clas); // If the Ident is a Class name use this Constructor.
	Sym(string n, int kind, int acctype, Scope *scope, Args *args, Constructor *constr); // If the Ident is a Constructor name use this Constructor.
	Sym(string n, int kind, int acctype, Scope *scope, Type *type, Args *args, Function *meth); // If the Ident is a Function Ident use this Constructor.
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
	int number; // Number of Scope.

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
	int scope_counter; // counter used to add number to each scope in symbol table.
public:
	vector<string> *classType; //?

	Scope *current; // the current scope.
	Errors *errors;
	Scope *firstFather;

	vector<Class *> *classes;
	vector<Scope *> *scopes;

	vector<int> *Invoke_scope_num;	// number of scope of Ident of Invoke.

	SymTab(Errors *errors); // The Constructor.
	Sym *Lookup(string key, Scope *Current);
	
	bool AddSym(Ident *id, int kind, int acctype, Type *type); // Add Symbol to Ident and add it to hashTable, used for Ident.
	bool AddSym(Ident *id, int kind, Class *clas); // Add Symbol to Ident and add it to hashTable, used for Ident of Class.
	bool AddSym(Ident *id, int kind, int acctype, Args *args, Constructor *constr); // Add Symbol to Ident and add it to hashTable, used for Ident of Constructor.
	bool AddSym(Ident *id, int kind, int acctype, Type *type, Args *args, Function *meth); // Add Symbol to Ident and add it to hashTable, used for Ident of Function.

	bool IsDeclared(Ident *id, Deffered *def); // Check if the Ident is Declared, used for local and global Ident.
	bool IsDeclared(Ident *id, int ScopeNum, bool CheckClasses, bool IsCall); // Check if Ident is declared in spacific scope or his fathers ( and in class if CheckClassName is true).
	bool IsDeclared(Ident *id, int kind, ExprList *el, int CurrentScope, bool IsCall); // Check if the Ident is Declared, used for Invoke and NewObject Ident.
	bool IsDeclared(Ident *id, int kind, Deffered *def); // Check if the Idnet is Declared, used for ClassInhert and IdentType.

	void AddNewScope();
	void OutScope();
	
	void FillingRelations(Root *file);

	int FatherScopeNum(int NumOfSubScope); // input sub scope number, return the number of father of this scope.
	bool CheckScopes(int num_of_scope_of_declaration, int num_of_scope_of_used); // Check if the two number are from the same class scope.
	
	Scope* IntToScope(int ScopeNumber); // From scope number to scope.
	Scope* IntToScope(int ScopeNumber, Scope *Current); // From scope number in spicific scope to scope.
	Scope* ClassToScope(Class *clas); // From Class to Scope.
	Class* ScopeToClass(Scope *scope); // From Scope to Class.
};

//***********************************************************************
//					Deffered
//***********************************************************************

class Deffered
{
public:
	string types[7];
	string kindArr[7];
	vector<Ident *> *ids; // Pointer to Ident.
	vector<int> *kinds; // Add kind of Idnet.
	vector<int> *scope_num; // Add number of scope where the Ident used.

	Deffered();
	void AddIdent(Ident *id, int kind, int scope_num);
	void CheckClass(SymTab *symtab);
	void CheckCircular(SymTab *symtab);
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