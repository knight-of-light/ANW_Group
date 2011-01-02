%{
	#include "stdio.h"
	
	#include "symbol_table.h"
	#include "ast.h"
	
	#define YYDEBUG 1
	
	extern int yylex();
	extern FILE *yyin;
	extern int yyerror(const char *);
	extern int lin;
	extern int col;
	bool IsError = false;
	
	extern SymTab *symtab;
	Root *file;
	extern Deffered *def;
%}

%union{
		Root		*tRoot;
		Class		*tClass;
		Members		*tMembers;
		Member		*tMember;
		Global		*tGlobal;
		Constructor	*tConstructor;
		Function	*tFunction;
		
		Variables	*tVariables;
		Variable	*tVariable;
		Args		*tArgs;
		Arg			*tArg;
		
		AccessModif	*tAccessModif;
		Type		*tType;
		NoArrayType	*tNoArrayType;
		ArrayType	*tArrayType;
		Expr		*tExpr;
		ExprList	*tExprList;
		
		ArrayIndex	*tArrayIndex;
		
		Stats		*tStats;
		Stat		*tStat;
		Variables_e	*tVariables_e;
		
		Integer		*tInteger;
		Real		*tReal;
		Ident		*tIdent;
	}
%type	<tRoot>			root
%type	<tClass>		class
%type	<tMembers>		members
%type	<tMember>		member
%type	<tGlobal>		global
%type	<tConstructor>	constructor
%type	<tFunction>		function

%type	<tVariables>	variables
%type	<tVariable>		variable
%type	<tArgs>			args	args_e
%type	<tArg>			arg

%type	<tAccessModif>	accessmodif
%type	<tType>			type
%type	<tNoArrayType>	noarraytype	basictype
%type	<tArrayType>	arraytype
%type	<tExpr>			expression	expr_e
%type	<tExprList>		expr_list	expr_list_e

%type	<tArrayIndex>	arrayindex

%type	<tStats>		statements
%type	<tStat>			statement
%type	<tVariables_e>	variables_e

%token	<tIdent>		IDENT
%token	<tInteger>		INTEGER
%token	<tReal>			REAL

%token CLASS BOOL DOUBLE INT IS OBJECT
%token IF ELSE WHILE FOR
%token FALSE TRUE
%token NEW
%token THIS NUL PRIVATE PUBLIC STATIC
%token VOID RETURN
%token LSBracket RSBracket

%nonassoc IF_PREC
%nonassoc ELSE

%right '='
%right CAST
%left OR
%left AND
%left EQ NE
%left '>' '<' LE SE IS
%left '+' '-'
%left '*'  '/'  '%'
%right '!' INCREMENT DECREMENT UNARY_OP
%left '.'
%left '('

%%
root:			/* empty */
					{
						$$ = new Root(lin, col);
						symtab->AddNewScope();
						file = $$;
					}
				| root class
					{
						$1->AddClass($2);
						$$ = $1;
						file = $$;
					}
;

class:			  CLASS IDENT '{'
					{
						symtab->AddNewScope();
					}
					 members '}'
					{
						$$ = new Class($2, $5, lin, col);
						symtab->OutScope();
						symtab->AddSym($2, 1, $$);

						// Add all Globals Idnet to vector in AST.Class
						for(int i=0; i < $5->members->size(); i++)
						{
							Global *G = dynamic_cast<Global*>($5->members->at(i));
							if(G != NULL)
							{
								for(int j=0; j < G->variables->variables->size(); j++)
								{
									if(G->accessModif->acctype != 3 && G->accessModif->acctype !=4)
										$$->AddGlobal(G->variables->variables->at(j)->name);
									else
										$$->AddStatic(G->variables->variables->at(j)->name);
								}
							}
						}
					}
				| CLASS IDENT ':' IDENT '{'
					{
						symtab->AddNewScope();
					}
					 members '}'
					{
						$$ = new ClassInher($2, $4, $7, lin, col);
						symtab->OutScope();
						symtab->AddSym($2, 1, $$);
						symtab->IsDeclared($4, 1, def);

						// Add all Globals Idnet to vector in AST.ClassInher
						for(int i=0; i < $7->members->size(); i++)
						{
							Global *G = dynamic_cast<Global*>($7->members->at(i));
							Function *F = dynamic_cast<Function*>($7->members->at(i));
							if(G != NULL)
							{
								for(int j=0; j < G->variables->variables->size(); j++)
								{
									if(G->accessModif->acctype != 3 && G->accessModif->acctype !=4)
										$$->AddGlobal(G->variables->variables->at(j)->name);
									else
										$$->AddStatic(G->variables->variables->at(j)->name);
								}
							}
							if(F != NULL)
							{
								$$->AddFunc(F->name);
							}
						}
					}
;

members:		  /* empty */
					{
						$$ = new Members(lin, col);
					}
				| members member
					{
						$1->AddMember($2);
						$$ = $1;
					}
;

member:			  global
					{$$ = $1;}
				| constructor
					{$$ = $1;}
				| function
					{$$ = $1;}
;

global:			  accessmodif type variables ';'
					{
						$$ = new Global($1, $2, $3, lin, col);
						for(int i=0; i < $3->variables->size(); i++)
							symtab->AddSym($3->variables->at(i)->name, 4, $1->acctype, $2);
					}
;

constructor:	  accessmodif IDENT '('
					{
						symtab->AddNewScope();
					}
					 args_e ')' '{' statements '}'
					{
						$$ = new Constructor($1, $2, $5, $8, lin, col);
						symtab->OutScope();
						symtab->AddSym($2, 3, $1->acctype, $5, $$);
					}
;

function:		  accessmodif type IDENT '('
					{
						symtab->AddNewScope();
					}
					 args_e ')' '{' statements '}'
					{
						$$ = new Function($1, $2, $3, $6, $9, lin, col);
						symtab->OutScope();
						symtab->AddSym($3, 2, $1->acctype, $2, $6, $$);
					}
				| accessmodif VOID IDENT '('
					 {
						 symtab->AddNewScope();
					 }
					 args_e ')' '{' statements '}'
					{
						$$ = new Function($1, $3, $6, $9, lin, col);
						symtab->OutScope();
						symtab->AddSym($3, 2, $1->acctype, NULL, $6, $$);
					}
;

arg :			  type IDENT
					{
						$$ = new Arg($1, $2, lin, col);
						symtab->AddSym($2, 6, 0, $1);
					}
;

args:			  arg
					{
						$$ = new Args($1, lin, col);
					}
				| args ',' arg
					{
						$1->AddArg($3);
						$$ = $1;
					}
;

args_e:			  /* empty */
					{
						$$ = new Args(lin, col);
					}
				| args
					{
						$$ = $1;
					}
;

variables:		  variable
					{
						$$ = new Variables($1, lin, col);
					}
				| variables ',' variable
					{
						$1->AddVariable($3);
						$$ = $1;
					}
;

variable:		  IDENT
					{
						$$ = new Variable($1, lin, col);
					}
				| IDENT '=' expression
					{
						$$ = new Variable($1, $3, lin, col);
					}
;

accessmodif:	  /* empty */
					{
						$$ = new AccessModif(2, lin, col);
					}
				| PUBLIC
					{
						$$ = new AccessModif(1, lin, col);
					}
				| PRIVATE
					{
						$$ = new AccessModif(2, lin, col);
					}
				| PUBLIC STATIC
					{
						$$ = new AccessModif(3, lin, col);
					}
				| STATIC
					{
						$$ = new AccessModif(4, lin, col);
					}
				| PRIVATE STATIC
					{
						$$ = new AccessModif(4, lin, col);
					}
;

type:			  noarraytype
					{$$ = $1;}
				| arraytype
					{$$ = $1;}
;

noarraytype:	  IDENT
					{
						$$ = new NoArrayType($1, lin, col);
						symtab->IsDeclared($1, 1, def);
					}
				| basictype
					{$$ = $1;}
;

basictype:		  INT
					{
						$$ = new NoArrayType(2, lin, col);
					}
				| DOUBLE
					{
						$$ = new NoArrayType(3, lin, col);
					}
				| BOOL
					{
						$$ = new NoArrayType(4, lin, col);
					}
				| OBJECT
					{
						$$ = new NoArrayType(5, lin, col);
					}
;

arraytype:		  IDENT LSBracket RSBracket
					{
						$$ = new ArrayType(1, $1, lin, col);
						symtab->IsDeclared($1, 1, def);
					}
				| IDENT LSBracket ',' RSBracket
					{
						$$ = new ArrayType(2, $1, lin, col);
						symtab->IsDeclared($1, 1, def);
					}
				| IDENT LSBracket ',' ',' RSBracket
					{
						$$ = new ArrayType(3, $1, lin, col);
						symtab->IsDeclared($1, 1, def);
					}
				| basictype LSBracket RSBracket
					{
						$$ = new ArrayType($1->type, 1, lin, col);
					}
				| basictype LSBracket ',' RSBracket
					{
						$$ = new ArrayType($1->type, 2, lin, col);
					}
				| basictype LSBracket ',' ',' RSBracket
					{
						$$ = new ArrayType($1->type, 3, lin, col);
					}
;

expression:		  INCREMENT IDENT
					{
						$$ = new Incr($2, true, lin, col);
						symtab->IsDeclared($2, def);
					}
				| DECREMENT IDENT
					{
						$$ = new Decr($2, true, lin, col);
						symtab->IsDeclared($2, def);
					}
				| IDENT INCREMENT
					{
						$$ = new Incr($1, false, lin, col);
						symtab->IsDeclared($1, def);
					}
				| IDENT DECREMENT
					{
						$$ = new Decr($1, false, lin, col);
						symtab->IsDeclared($1, def);
					}
				| '!' expression
					{
						$$ = new Not($2, lin, col);
					}
				| '-' expression %prec UNARY_OP
					{
						$$ = new Minus($2, lin, col);
					}
				| '+' expression %prec UNARY_OP
					{
						$$ = new Plus($2, lin, col);
					}
				| '(' expression ')'
					{
						$$ = new Paren($2, lin, col);
					}
				| IDENT
					{
						$$ = new IdentExpr($1, lin, col);
						//symtab->IsDeclared($1, def);
					}
				| IDENT arrayindex
					{
						$$ = new IdentArr($1, $2, lin, col);
						symtab->IsDeclared($1, def);
					}
				| expression '=' expression
					{
						$$ = new Assign($1, $3, lin, col);
						//symtab->IsDeclared($1, def);
					}
				| IDENT '(' expr_list_e ')'
					{
						$$ = new Invoke($1, $3, lin, col);
						//symtab->AddInvokeScopeNum();
						//symtab->IsDeclared($1, 2, $3, def); // Used in Type Checking.
					}
				| NEW IDENT '(' expr_list_e ')'
					{
						$$ = new NewObject($2, $4, lin, col);
						//symtab->AddInvokeScopeNum();
						//symtab->IsDeclared($2, 3, $4, def); // Used in Type Checking.
					}
				| NEW noarraytype arrayindex
					{
						$$ = new NewArray($2, $3, lin, col);
					}
				| IDENT '.' expression
					{
						$$ = new IdentCall($1, $3, lin, col);
						//symtab->IsDeclared($1, def);
					}
				| IDENT arrayindex '.' expression
					{
						$$ = new IdentArrCall($1, $2, $4, lin, col);
						symtab->IsDeclared($1, def);
					}
				| THIS '.' expression
					{
						$$ = new ThisCall($3, lin, col);
					}
				| expression EQ expression
					{
						$$ = new Equal($1, $3, lin, col);
					}
				| expression NE expression
					{
						$$ = new NotEq($1, $3, lin, col);
					}
				| expression '<' expression
					{
						$$ = new Smaller($1, $3, lin, col);
					}
				| expression SE expression
					{
						$$ = new SmallerEq($1, $3, lin, col);
					}
				| expression '>' expression
					{
						$$ = new Larger($1, $3, lin, col);
					}
				| expression LE expression
					{
						$$ = new LargerEq($1, $3, lin, col);
					}
				| expression '+' expression
					{
						$$ = new Add($1, $3, lin, col);
					}
				| expression '-' expression
					{
						$$ = new Sub($1, $3, lin, col);
					}
				| expression '*' expression
					{
						$$ = new Mult($1, $3, lin, col);
					}
				| expression '/' expression
					{
						$$ = new Div($1, $3, lin, col);
					}
				| expression '%' expression
					{
						$$ = new Mod($1, $3, lin, col);
					}
				| expression AND expression
					{
						$$ = new And($1, $3, lin, col);
					}
				| expression OR expression
					{
						$$ = new Or($1, $3, lin, col);
					}
				| expression IS type
					{
						$$ = new Is($1, $3, lin, col);
					}
				| '(' basictype ')' expression %prec CAST
					{
						$$ = new Cast($2, $4, lin, col);
					}
				| INTEGER
					{$$ = $1;}
				| REAL
					{$$ = $1;}
				| TRUE
					{
						$$ = new True(lin, col);
					}
				| FALSE 
					{
						$$ = new False(lin, col);
					}
				| THIS
					{
						$$ = new This(lin, col);
					}
				| NUL
					{
						$$ = new Null(lin, col);
					}
;

arrayindex:		  LSBracket expression RSBracket
					{
						$$ = new ArrayIndex_1($2, lin, col);
					}
				| LSBracket expression ',' expression RSBracket
					{
						$$ = new ArrayIndex_2($2, $4, lin, col);
					}
				| LSBracket expression ',' expression ',' expression RSBracket
					{
						$$ = new ArrayIndex_3($2, $4, $6, lin, col);
					}
;

expr_list:		  expression
					{
						$$ = new ExprList($1, lin, col);
					}
				| expr_list ',' expression
					{
						$1->AddExpr($3);
						$$ = $1;
					}
;

expr_list_e:	  /* empty */
					{
						$$ = new ExprList(lin, col);
					}
				| expr_list
					{
						$$ = $1;
					}
;

statement:		  IF '(' expression ')' statement %prec IF_PREC
					{
						$$ = new If($3, $5, lin, col);
					}
				| IF '(' expression ')' statement ELSE statement
					{
						$$ = new IfElse($3, $5, $7, lin, col);
					}
				| WHILE '(' expression ')' statement
					{
						$$ = new While($3, $5, lin, col);
					}
				| FOR '(' variables_e ';' expr_e ';' expr_e ')' statement
					{
						$$ = new For($3, $5, $7, $9, lin, col);
					}
				| expression ';'
					{
						$$ = new ExprStat($1, lin, col);
					}
				| type variables ';'
					{
						$$ = new VariablesStat($1, $2, lin, col);
						for(int i = 0; i < $2->variables->size(); i++)
							symtab->AddSym($2->variables->at(i)->name, 5, 0, $1);
					}
				| ';'
					{
						$$ = new Semi(lin, col);
					}
				| '{'
					{
						symtab->AddNewScope();
					}
					 statements '}'
					{
						$$ = new Block($3, lin, col);
						symtab->OutScope();
					}
				| RETURN expr_e ';'
					{
						$$ = new Return($2, lin, col);
					}
;

statements:			  /* Empty */
					{
						$$ = new Stats(lin, col);
					}
				|statements statement
					{
						$1->AddStat($2);
						$$ = $1;
					}
;

variables_e:		  /* Empty */
					{
						$$ = NULL;
					}
				| type variables
					{
						$$ = new Variables_e($1, $2, lin, col);
						for(int i = 0; i < $2->variables->size(); i++)
							symtab->AddSym($2->variables->at(i)->name, 6, 0, $1);
					}
				| variables
					{
						$$ = new Variables_e($1, lin, col);
						for(int i = 0; i < $1->variables->size(); i++)
							symtab->IsDeclared($1->variables->at(i)->name, def);
					}
;

expr_e:			  /* Empty */
					{
						$$ = new Expr(lin, col);
					}
				| expression
					{
						$$ = $1;
					}
;

%%
int yyerror(const char *s)
{
	IsError = true;
	printf("parser error at line %d and column %d \n", lin, col);
	return 1;
}