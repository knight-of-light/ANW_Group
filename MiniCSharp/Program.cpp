#include <iostream>
#include "ast.h"
#include "visitors.h"
#include "symbol_table.h"

extern int yylex();
extern FILE *yyin;
extern int yyerror(const char *);

extern int yydebug;
extern int yyparse();

extern Root *file;

extern int lineno;
extern int columnno;

SymTab *symtab;
Errors *errors;
Deffered *def;

int main(int argsc, char **argsv)
{
	++argsv, --argsc;
    if(argsc > 0)
            yyin = fopen(argsv[0], "r");
    else
            yyin = stdin;

	errors = new Errors();
	symtab = new SymTab(errors);
	def = new Deffered();
	
	bool pvdebug = 1;
	bool tvdebug = 1;
	bool cvdebug = 0;

	yydebug = 0;
    yyparse();
	cout << "Syntactical analysis was done successfully! " << endl;
	
	PrintVisitor *pv = new PrintVisitor(file, pvdebug);

	def->CheckAll(symtab);

	if(errors->messages->size() != 0){
		errors->Print();
	}
	else
	{
		TypeVisitor *tv = new TypeVisitor(file, symtab, tvdebug);
		if(errors->messages->size() != 0){
			errors->Print();
		}
		else
		{
			cout << "Semantic analysis was done successfully! " << endl;
			CodeVisitor *cv = new CodeVisitor(file, symtab, tv->mainFunc, cvdebug);
		}
	}

    return 0;
}