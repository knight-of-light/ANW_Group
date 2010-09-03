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
	
%type	<tClassDef>	class_def
%type	<tFields>	fields
%type	<tField>	field
%type	<tMethod>	method
%type	<tVariable>	variable

%type	<tVarDecls>	var_decls
%type	<tVarDecl>	var_decl
%type	<tParams>	params	params_e
%type	<tParam>	param

%type	<tExprType>	expr_type
%type	<tExpr>		expr	expr_e
%type	<tExprList>	expr_list	expr_list_e

%type	<tInsts>	insts
%type	<tInst>	inst
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
file:			  /* empty */
				| file class_def
;

class_def:		CLASS IDENT '{' fields '}' 
					{
						$$ = new ClassDef($2, $4, lin, col);						
						symtab->AddSym($2, 1, -1);	
						classDef = $$;
										
					}
				| CLASS IDENT ':' IDENT '{' fields '}'
					{
					}
;

fields:			  /* empty */
					{$$ = new Fields(lin, col);}
				| fields field 
					{
						$1->AddField($2);
						$$ = $1;
					}
;
field:			  variable
					{$$ = $1;}
				| constructor
					{}
				| method
					{$$ = $1;}
;

variable:		  acc_modif expr_type var_decls ';'
					{
					}
				| expr_type var_decls ';'
					{
						$$ = new Variable($1, $2, lin, col);						
						symtab->AddVars($2, $1);
					}
;

constructor:	  IDENT '(' params_e ')' '{' insts '}'
					{
					}
				| acc_modif IDENT '(' params_e ')' '{' insts '}'
					{
					}
;


method:			  acc_modif expr_type IDENT '('
					{
					}				 
				   params_e ')' '{'insts '}' 
					{
					}
				| acc_modif VOID IDENT '(' 
					 {
					 }				 
				   params_e ')' '{'insts '}' 
					{ 
					}
				| expr_type IDENT '('
					{
						symtab->AddNewScope();
					}				 
				   params_e ')' '{'insts '}' 
					{ 
						$$ = new Method($1, $2, $5, $8, lin, col); 
						symtab->OutScope();	
						symtab->AddSym($2, 2, -1, $5, $1->type, $$);					
					}
				| VOID IDENT '(' 
					 {
						symtab->AddNewScope();
					 }				 
				   params_e ')' '{'insts '}' 
					{ 
						$$ = new Method($2, $5, $8, lin, col); 
						symtab->OutScope();
						symtab->AddSym($2, 2, -1, $5, 4, $$);
					}
				
;

param :			  expr_type IDENT
					{
						$$ = new Param($1, $2, lin, col);
						symtab->AddSym($2, 6, $1->type);
					}
;

params:			  param
					{
						$$ = new Params($1, lin, col);
					}
				| params ',' param 
					{
						$1->AddParam($3);
						$$ = $1;
					}
;

params_e:		  /* empty */
					{
						$$ = new Params(lin, col);
					}
				| params
					{
						$$ = $1;						
					}
;

var_decls:		  var_decl 
					{
						$$ = new VarDecls($1, lin, col);
					}
				| var_decls ',' var_decl
					{
						$1->AddVarDecl($3);
						$$ = $1;
					}
;
var_decl:		  IDENT 
					{
						$$ = new VarDecl($1, lin, col);
					}
				| IDENT '=' expr
					{
						$$ = new VarDecl($1, $3, lin, col);
					}
;

acc_modif:		  PRIVATE
				| STATIC
				| PRIVATE STATIC
;

expr_type:		  no_arr_type
					{
					}
				| arr_type
					{
					}
;

no_arr_type:	  IDENT
					{
					}
				| no_arr_ident
					{
					}
;

no_arr_ident:	  BOOLEAN
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
				| no_arr_ident '[' ']'
					{
					}
				| no_arr_ident '[' ',' ']'
					{
					}
				| no_arr_ident '[' ',' ',' ']'
					{
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
				| q_name_arr 
					{
						// $$ = new IdentExpr($1, lin, col);
						// symtab->IsDeclared($1, def);
					}  
				| q_name_arr '=' expr  
					{
						// $$ = new Assign($1, $3, lin, col);
						// symtab->IsDeclared($1);
					} 
				| qual_name '(' expr_list_e ')'	 
					{
						// $$ = new Invoke($1, $3, lin, col);
						// //symtab->IsDeclared($1, $3);
					}
				| qual_name '(' expr_list_e ')' arr_index
					{
					}
				| NEW IDENT '(' expr_list_e ')'
					{
					}
				| NEW IDENT arr_index
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
						$$ = new LargerEq($1, $3, lin, col);
					}
				| expr '>' expr 
					{
						$$ = new Larger($1, $3, lin, col);
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
				| expr IS expr_type
					{
					}
				| '(' no_arr_ident ')' expr %prec change_type
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

arr_index:		  '[' expr ']'
					{
					}
				| '[' expr ',' expr ']'
					{
					}
				| '[' expr ',' expr ',' expr ']'
					{
					}
;

qual_name:		  IDENT
					{
					}
				| expr '.' IDENT
					{
					}
;

q_name_arr:		  IDENT
					{
					}
				| IDENT arr_index
					{
					}
				| expr '.' IDENT
					{
					}
				| expr '.' IDENT arr_index
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

inst:			  IF '(' expr ')' inst %prec IF_PREC
					{
						$$ = new If($3, $5, lin, col);
					} 
				| IF '(' expr ')' inst ELSE inst
					{
						$$ = new IfElse($3, $5, $7, lin, col);
					}	
				| WHILE '(' expr ')'  inst
					{
						$$ = new While($3, $5, lin, col);
					} 
				| FOR '(' vars_decl_e ';' expr_e ';' expr_e ')' inst
					{
						$$ = new For($3, $5, $7, $9, lin, col);
					}  
				| expr ';'
					{
						$$ = new ExprInst($1, lin, col);
					}  					
				| expr_type var_decls ';'
					{
						$$ = new VarDeclsInst($1, $2, lin, col);
						for(int i = 0; i < $2->varDecls->size(); i++)
							symtab->AddSym($2->varDecls->at(i)->name, 3, $1->type);
					} 
				|   '{'
						{
							symtab->AddNewScope();
						}
					 insts '}' 
						 {
							$$ = new Block($3, lin, col);
							symtab->OutScope();
						 }
				| RETURN expr_e ';'
					{
						$$ = new Return($2, lin, col);
					}
;

insts:			  /* Empty */
					{
						$$ = new Insts(lin, col);
					} 
				|insts inst
					{
						$1->AddInst($2);
						$$ = $1;
					} 
;

vars_decl_e:		  /* Empty */
					{
						$$ = NULL;
					} 
				| expr_type var_decls
					{
						$$ = new VarsDecl($1, $2, lin, col);
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






