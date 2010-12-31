#include <iostream>
#include "ast.h"
#include "visitors.h"
#include "symbol_table.h"
#include <exception>
using namespace std;

extern int yylex();
extern FILE *yyin;
extern int yyerror(const char *);

extern int yydebug;
extern int yyparse();

extern Root *file;

extern int lineno;
extern int columnno;
extern bool IsError;

SymTab *symtab;
Errors *errors;
Deffered *def;

int main(int argsc, char **argsv)
{
	/*************************************************************************************
							ARGUMENTS for MiniCSharp
							************************
	  [input_path] [ -parse-only | [-type-only] [-ast-debug] ] [-parse-debug]
	 *************************************************************************************/
	++argsv, --argsc;

	bool parse_only		= false;
	bool type_only		= false;
	bool parse_debug	= false;
	bool ast_debug		= false;
	
	bool RunProgram = true;
	int NumInputFile = 0;

	for(int i=0; i<argsc; i++)
	{
		string argument = argsv[i];

		if(argument == "-parse-only")
			parse_only = true;
		else if(argument == "-type-only")
			type_only = true;
		else if(argument == "-ast-debug")
			ast_debug = true;
		else if(argument == "-parse-debug")
			parse_debug = true;
		else if(argument == "-help")
		{
			RunProgram = false;
			cout << " MiniCSharp Compiler [version 1.0] " << endl;
			cout << " Copyright (c) 2010 SVU CMPP S10. All rights reserved." << endl;
			cout << endl;
			cout << "  filename.txt  Redirecting standard input to a file name." << endl;
			cout << "                The file must be txt file and must be exists." << endl;
			cout << "                you can just input one file, if you don't input file name the standard input will not change." << endl;
			cout << " -ast-debug     Drow Abstract Syntax Tree." << endl;
			cout << " -help          View All argument and what each one do." << endl;
			cout << " -parse-debug   Run yydebug that debuger for parser." << endl;
			cout << " -parse-only    Run only Lexical and Syntactical Analysis." << endl; 
			cout << " -type-only     Run Semantic Analysis ,but not Code Generation." << endl;
			cout << "                [input File name] [ -parse-only | [-type-only][-ast-debug] ] [-parse-debug]" << endl;
			RunProgram = false; // if -help don't run the program just view the help
			break;
		}
		else if( argument.substr(argument.length()-4) == ".txt" )
		{
			if(NumInputFile == 0)
			{
				if(fopen(argsv[i], "r") == NULL)
				{
					RunProgram = false;
					cout << "Arguments error: the file \"" << argsv[i] << "\" not exists" << endl;
					cout << "Use \"-help\" for more information about arguments" << endl;
				}
				else
					freopen(argsv[i], "r", stdin);
			}
			else
			{
				RunProgram = false;
				cout << "Arguments error: you cannot enter more than one input file" << endl;
				cout << "Use \"-help\" for more information about arguments" << endl;
			}
			NumInputFile++;
		}
		else
		{
			RunProgram = false;
			cout << "Arguments error: unknown argument" << argument << endl;
			cout << "Use \"-help\" for more information about arguments" << endl;
		}
	}
	if( parse_only && (type_only || ast_debug) )
	{
		RunProgram = false;
		cout << "Arguments error: cannot use -parse-only with -type-only or with -ast-debug" << endl;
		cout << "Use \"-help\" for more information about arguments" << endl;
	}

	yyin = stdin;

	/*************************************************************************************
	 *************************************************************************************/


	if(RunProgram)
	{
		// Declaration
		errors = new Errors();
		symtab = new SymTab(errors);
		def = new Deffered();
		///

		// Lexical & Syntactical Analysis
		yydebug = 0;//parse_debug;// Allow parser Debug (0: false, 1: true)
		yyparse();
		if(!IsError)
			cout << "Syntactical analysis was done successfully! " << endl;
		///
		
		if(!parse_only)
		{
			if(ast_debug)
				PrintVisitor *pv = new PrintVisitor(file);

			def->CheckClass(symtab);

			if(errors->messages->size() != 0)
				IsError = true;
			else
			{
				symtab->FillingRelations(file);
				def->CheckCircular(symtab);
				def->CheckAll(symtab);
				if(errors->messages->size() == 0)
				{
					TypeVisitor *tv = new TypeVisitor(file, symtab);

					if(errors->messages->size() != 0)
						IsError = true;
					else
					{
						cout << "Semantic analysis was done successfully! " << endl;
						if(!type_only)
						{
							CodeVisitor *cv = new CodeVisitor(file, symtab, tv->mainFunc);
							cout << "Code Generation was done successfully! " << endl;
						}
					}
				}
				else
					IsError = true;
			}
		}
		if(IsError)
		{
			errors->Print();
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}