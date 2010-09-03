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
	ClassDef *classDef;	
	extern Deffered *def;
%}

%union{
		ClassDef	*tClassDef;
		Fields		*tFields;
		Field		*tField;
		Method		*tMethod;
		Variable	*tVariable;
		
		VarDecls	*tVarDecls;
		VarDecl		*tVarDecl;
		Params		*tParams;
		Param		*tParam;
		
		ExprType	*tExprType;
		Expr		*tExpr;
		ExprList	*tExprList;
		
		Insts		*tInsts;
		Inst		*tInst;
		VarsDecl	*tVarsDecl;
		
		Integer		*tInteger;
		Real		*tReal;		
		Ident		*tIdent;				
	}
	
%type	<tClassDef>	class
%type	<tFields>	member
%type	<tField>	field
%type	<tMethod>	function
%type	<tVariable>	global

%type	<tVarDecls>	variables
%type	<tVarDecl>	variable
%type	<tParams>	arg	arg_e
%type	<tParam>	args

%type	<tExprType>	type
%type	<tExpr>		expression	expr_e
%type	<tExprList>	expr_list	expr_list_e

%type	<tInsts>	statements
%type	<tInst>	statement
%type	<tVarsDecl>	vars_decl_e

%token	<tIdent>	IDENT	
%token	<tInteger>	INTEGER 
%token	<tReal>		REAL

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
				| root class
;

class:		CLASS IDENT '{' member '}' 
					{
						$$ = new ClassDef($2, $4, lin, col);						
						symtab->AddSym($2, 1, -1);	
						classDef = $$;
										
					}
				| CLASS IDENT ':' IDENT '{' member '}'
					{
					}
;

member:			  /* empty */
					{$$ = new Fields(lin, col);}
				| member field 
					{
						$1->AddField($2);
						$$ = $1;
					}
;
field:			  global
					{$$ = $1;}
				| constructor
					{}
				| function
					{$$ = $1;}
;

global:		  accessmodif type variables ';'
					{
					}
				| type variables ';'
					{
						$$ = new Variable($1, $2, lin, col);						
						symtab->AddVars($2, $1);
					}
;

constructor:	  IDENT '(' arg_e ')' '{' statements '}'
					{
					}
				| accessmodif IDENT '(' arg_e ')' '{' statements '}'
					{
					}
;


function:			  accessmodif type IDENT '('
					{
					}				 
				   arg_e ')' '{'statements '}' 
					{
					}
				| accessmodif VOID IDENT '(' 
					 {
					 }				 
				   arg_e ')' '{'statements '}' 
					{ 
					}
				| type IDENT '('
					{
						symtab->AddNewScope();
					}				 
				   arg_e ')' '{'statements '}' 
					{ 
						$$ = new Method($1, $2, $5, $8, lin, col); 
						symtab->OutScope();	
						symtab->AddSym($2, 2, -1, $5, $1->type, $$);					
					}
				| VOID IDENT '(' 
					 {
						symtab->AddNewScope();
					 }				 
				   arg_e ')' '{'statements '}' 
					{ 
						$$ = new Method($2, $5, $8, lin, col); 
						symtab->OutScope();
						symtab->AddSym($2, 2, -1, $5, 4, $$);
					}
				
;

args :			  type IDENT
					{
						$$ = new Param($1, $2, lin, col);
						symtab->AddSym($2, 6, $1->type);
					}
;

arg:			  args
					{
						$$ = new Params($1, lin, col);
					}
				| arg ',' args 
					{
						$1->AddParam($3);
						$$ = $1;
					}
;

arg_e:		  /* empty */
					{
						$$ = new Params(lin, col);
					}
				| arg
					{
						$$ = $1;						
					}
;

variables:		  variable 
					{
						$$ = new VarDecls($1, lin, col);
					}
				| variables ',' variable
					{
						$1->AddVarDecl($3);
						$$ = $1;
					}
;
variable:		  IDENT 
					{
						$$ = new VarDecl($1, lin, col);
					}
				| IDENT '=' expression
					{
						$$ = new VarDecl($1, $3, lin, col);
					}
;

accessmodif:		  PRIVATE
				| STATIC
				| PRIVATE STATIC
;

type:		  noarraytype
					{
					}
				| arr_type
					{
					}
;

noarraytype:	  IDENT
					{
					}
				| arraytype
					{
					}
;

arraytype:	  BOOLEAN
					{
						//$$ = new ExprType(3, lin, col);
					}
				| INT
					{
						//$$ = new ExprType(1, lin, col);
					}
				| DOUBLE
					{
						//$$ = new ExprType(2, lin, col);
					}
;

arr_type:		  IDENT '[' ']'
					{
					}
				| IDENT '[' ',' ']'
					{
					}
				| IDENT '[' ',' ',' ']'
					{
					}
				| arraytype '[' ']'
					{
					}
				| arraytype '[' ',' ']'
					{
					}
				| arraytype '[' ',' ',' ']'
					{
					}
;

expression:			  INCREMENT IDENT
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
				| qualnameorarray 
					{
						// $$ = new IdentExpr($1, lin, col);
						// symtab->IsDeclared($1, def);
					}  
				| qualnameorarray '=' expression  
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
						$$ = new LargerEq($1, $3, lin, col);
					}
				| expression '>' expression 
					{
						$$ = new Larger($1, $3, lin, col);
					} 
				| expression LE expression 
					{
						$$ = new SmallerEq($1, $3, lin, col);
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
					}
				| '(' arraytype ')' expression %prec change_type
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

arrayindex:		  '[' expression ']'
					{
					}
				| '[' expression ',' expression ']'
					{
					}
				| '[' expression ',' expression ',' expression ']'
					{
					}
;

qualifiedname:		  IDENT
					{
					}
				| expression '.' IDENT
					{
					}
;

qualnameorarray:		  IDENT
					{
					}
				| IDENT arrayindex
					{
					}
				| expression '.' IDENT
					{
					}
				| expression '.' IDENT arrayindex
					{
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

statement:			  IF '(' expression ')' statement %prec IF_PREC
					{
						$$ = new If($3, $5, lin, col);
					} 
				| IF '(' expression ')' statement ELSE statement
					{
						$$ = new IfElse($3, $5, $7, lin, col);
					}	
				| WHILE '(' expression ')'  statement
					{
						$$ = new While($3, $5, lin, col);
					} 
				| FOR '(' vars_decl_e ';' expr_e ';' expr_e ')' statement
					{
						$$ = new For($3, $5, $7, $9, lin, col);
					}  
				| expression ';'
					{
						$$ = new ExprInst($1, lin, col);
					}  					
				| type variables ';'
					{
						$$ = new VarDeclsInst($1, $2, lin, col);
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
						$$ = new Insts(lin, col);
					} 
				|statements statement
					{
						$1->AddInst($2);
						$$ = $1;
					} 
;

vars_decl_e:		  /* Empty */
					{
						$$ = NULL;
					} 
				| type variables
					{
						$$ = new VarsDecl($1, $2, lin, col);
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
	printf("parser error at line %d and column %d \n", lin, col);
	return 0;
}






