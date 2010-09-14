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
	
	extern SymTab *symtab;
	Root *file;	
	extern Deffered *def;
%}

%union{
		Root		*tRoot;
		ClassDef	*tClassDef;
		Members		*tMembers;
		Member		*tMember;
		Function	*tFunction;
		Global		*tGlobal;
		
		Variables	*tVariables;
		Variable	*tVariable;
		Args		*tArgs;
		Arg			*tArg;
		
		Type		*tType;
		NoArrayType	*tNoArrayType;
		ArrayType	*tArrayType;
		Expr		*tExpr;
		ExprList	*tExprList;
		
		Stats		*tStats;
		Stat		*tStat;
		Variables_e	*tVariables_e;
		
		Integer		*tInteger;
		Real		*tReal;		
		Ident		*tIdent;				
	}
	
%type	<tRoot>			root
%type	<tClassDef>		class
%type	<tMembers>		members
%type	<tMember>		member
%type	<tFunction>		function
%type	<tGlobal>		global

%type	<tVariables>	variables
%type	<tVariable>		variable
%type	<tArgs>			args	args_e
%type	<tArg>			arg

%type	<tType>			type
%type	<tNoArrayType>	noarraytype	basictype
%type	<tArrayType>	arraytype
%type	<tExpr>			expr	expr_e
%type	<tExprList>		expr_list	expr_list_e

%type	<tStats>		statements
%type	<tStat>			statement
%type	<tVariables_e>	variables_e

%token	<tIdent>		IDENT	
%token	<tInteger>		INTEGER 
%token	<tReal>			REAL

%token CLASS BOOLEAN DOUBLE INT IS
%token IF ELSE WHILE FOR 
%token FALSE TRUE EXTENDS
%token INSTANCEOF NEW 
%token THIS NUL PRIVATE STATIC 
%token VOID RETURN 

%nonassoc IF_PREC
%nonassoc ELSE

%right '='
%left OR
%left AND
%left EQ NE IS
%left '>' '<' LE SE
%nonassoc INSTANCEOF
%left '+' '-'
%left '*'  '/'  '%'
%left '!' INCREMENT DECREMENT UNARY_OP
%nonassoc change_type
%left '.'
%left '(' 

%%
root:			  /* empty */
					{
						$$ = new Root(lin, col);
						file = $$;
					}
				| root class
					{
						$1->AddClass($2);
						$$ = $1;
						file = $$;
					}
;

class:			CLASS IDENT '{' members '}' 
					{
						$$ = new ClassDef($2, $4, lin, col);						
						symtab->AddSym($2, 1, -1);
										
					}
				| CLASS IDENT ':' IDENT '{' members '}'
					{
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
					{/*$$ = $1;*/}
				| function
					{$$ = $1;}
;

global:			  accessmodif type variables ';'
					{
						$$ = new Global($2, $3, lin, col);
						symtab->AddVars($3, $2);
					}
;

constructor:	  accessmodif IDENT '(' args_e ')' '{' statements '}'
					{
					}
;


function:		  accessmodif type IDENT '('
					{
						symtab->AddNewScope();
					}				 
				   args_e ')' '{'statements '}' 
					{
						$$ = new Function($2, $3, $6, $9, lin, col); 
						symtab->OutScope();	
						symtab->AddSym($3, 2, -1, $6, $2->type, $$);
					}
				| accessmodif VOID IDENT '(' 
					 {
						 symtab->AddNewScope();
					 }				 
				   args_e ')' '{'statements '}' 
					{
						$$ = new Function($3, $6, $9, lin, col); 
						symtab->OutScope();
						symtab->AddSym($3, 2, -1, $6, 4, $$);
					}				
;

arg :			  type IDENT
					{
						$$ = new Arg($1, $2, lin, col);
						symtab->AddSym($2, 6, $1->type);
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
				| IDENT '=' expr
					{
						$$ = new Variable($1, $3, lin, col);
					}
;

accessmodif:	  /*empty*/
				| PRIVATE
				| STATIC
				| PRIVATE STATIC
;

type:			  noarraytype
					{
						$$ = $1;
					}
				| arraytype
					{
						$$ = $1;
					}
;

noarraytype:	  IDENT
					{
						$$ = new NoArrayType($1, lin, col);
					}
				| basictype
					{
						$$ = $1;
					}
;

basictype:		  BOOLEAN
					{
						$$ = new NoArrayType(3, lin, col);
					}
				| INT
					{
						$$ = new NoArrayType(1, lin, col);
					}
				| DOUBLE
					{
						$$ = new NoArrayType(2, lin, col);
					}
;

arraytype:		  IDENT '[' ']'
					{
						$$ = new ArrayType(2, $1, lin, col);
					}
				| IDENT '[' ',' ']'
					{
						$$ = new ArrayType(3, $1, lin, col);
					}
				| IDENT '[' ',' ',' ']'
					{
						$$ = new ArrayType(4, $1, lin, col);
					}
				| basictype '[' ']'
					{
						$$ = new ArrayType($1->type*10+2, lin, col);
					}
				| basictype '[' ',' ']'
					{
						$$ = new ArrayType($1->type*10+3, lin, col);
					}
				| basictype '[' ',' ',' ']'
					{
						$$ = new ArrayType($1->type*10+4, lin, col);
					}
;

expr:			  INCREMENT IDENT
					{
						// $$ = new Incr($2, true, lin, col);
						// symtab->IsDeclared($2);
					} 
				| DECREMENT IDENT  
					{
						// $$ = new Decr($2, true, lin, col);
						// symtab->IsDeclared($2);
					} 
				| IDENT INCREMENT  
					{
						// $$ = new Incr($1, false, lin, col);
						// symtab->IsDeclared($1);
					} 
				| IDENT DECREMENT 
					{
						// $$ = new Decr($1, false, lin, col);
						// symtab->IsDeclared($1);
					}
				| '!' expr  
					{
						$$ = new Not($2, lin, col);
					}
				| '-' expr %prec UNARY_OP 
					{
						$$ = new Minus($2, lin, col);
					} 
				| '+' expr %prec UNARY_OP 
					{
						$$ = new Plus($2, lin, col);
					} 
				| '(' expr ')' 
					{
						$$ = new Paren($2, lin, col);
					} 
				| qualnameorarray 
					{
						// $$ = new IdentExpr($1, lin, col);
						// symtab->IsDeclared($1, def);
					}  
				| qualnameorarray '=' expr  
					{
						// $$ = new Assign($1, $3, lin, col);
						// symtab->IsDeclared($1);
					} 
				| qualifiedname '(' expr_list_e ')'	 
					{
						// $$ = new Invoke($1, $3, lin, col);
						// //symtab->IsDeclared($1, $3);
					}
				| qualifiedname '(' expr_list_e ')' arrayindex
					{
					}
				| NEW IDENT '(' expr_list_e ')'
					{
					}
				| NEW IDENT arrayindex
					{
					}
				| expr EQ expr 
					{
						$$ = new Equal($1, $3, lin, col);
					} 
				| expr NE expr 
					{
						$$ = new NotEq($1, $3, lin, col);
					} 
				| expr '<' expr 
					{
						$$ = new Smaller($1, $3, lin, col);
					} 
				| expr SE expr 
					{
						$$ = new LargserEq($1, $3, lin, col);
					}
				| expr '>' expr 
					{
						$$ = new Largser($1, $3, lin, col);
					} 
				| expr LE expr 
					{
						$$ = new SmallerEq($1, $3, lin, col);
					} 
				| expr '+' expr 
					{
						$$ = new Add($1, $3, lin, col);
					} 
				| expr '-' expr 
					{
						$$ = new Sub($1, $3, lin, col);
					} 
				| expr '*' expr 
					{
						$$ = new Mult($1, $3, lin, col);
					} 
				| expr '/' expr 
					{
						$$ = new Div($1, $3, lin, col);
					} 
				| expr '%' expr 
					{
						$$ = new Mod($1, $3, lin, col);
					} 
				| expr AND expr 
					{
						$$ = new And($1, $3, lin, col);
					} 
				| expr OR expr 
					{
						$$ = new Or($1, $3, lin, col);
					}
				| expr IS type
					{
					}
				| '(' basictype ')' expr %prec change_type
					{
					}
				| INTEGER 
					{
						$$ = $1;
					} 
				| REAL 
					{
						$$ = $1;
					} 
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
					}
				| NUL 
					{
						$$ = new Null(lin, col);
					}			
;

arrayindex:		  '[' expr ']'
					{
					}
				| '[' expr ',' expr ']'
					{
					}
				| '[' expr ',' expr ',' expr ']'
					{
					}
;

qualifiedname:		  IDENT
					{
					}
				| expr '.' IDENT
					{
					}
;

qualnameorarray:		  IDENT
					{
					}
				| IDENT arrayindex
					{
					}
				| expr '.' IDENT
					{
					}
				| expr '.' IDENT arrayindex
					{
					}
;

expr_list:		  expr  
					{
						$$ = new ExprList($1, lin, col);
					} 
				| expr_list ',' expr 
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

statement:			  IF '(' expr ')' statement %prec IF_PREC
					{
						$$ = new If($3, $5, lin, col);
					} 
				| IF '(' expr ')' statement ELSE statement
					{
						$$ = new IfElse($3, $5, $7, lin, col);
					}	
				| WHILE '(' expr ')'  statement
					{
						$$ = new While($3, $5, lin, col);
					} 
				| FOR '(' variables_e ';' expr_e ';' expr_e ')' statement
					{
						$$ = new For($3, $5, $7, $9, lin, col);
					}  
				| expr ';'
					{
						$$ = new ExprStat($1, lin, col);
					}  					
				| type variables ';'
					{
						$$ = new VariablesStat($1, $2, lin, col);
						for(int i = 0; i < $2->varDecls->size(); i++)
							symtab->AddSym($2->varDecls->at(i)->name, 3, $1->type);
					} 
				|   '{'
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
					} 
;

expr_e:			  /* Empty */
					{
						$$ = new Expr(lin, col);
					} 
				| expr
					{
						$$ = $1;
					} 
;

%%
int yyerror(const char *s)
{
	printf("parser error at line %d and column %d \n", lin, col);
	return 0;
}






