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
	File *file;
	extern Deffered *def;
%}

%union{
		File		*tFile;
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
		QualName	*tQualName;
		QualNArray	*tQualNArray;
		
		Stats		*tStats;
		Stat		*tStat;
		Variables_e	*tVariables_e;
		
		Integer		*tInteger;
		Real		*tReal;
		Ident		*tIdent;
	}
%type	<tFile>			file
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
%type	<tExpr>			expr	expression	expr_e
%type	<tExprList>		expr_list	expr_list_e

%type	<tArrayIndex>	arrayindex
%type	<tQualName>		qualifiedname
%type	<tQualNArray>	qualnameorarray	qnora_without_id

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
%token THIS NUL PRIVATE STATIC
%token VOID RETURN

%nonassoc EXPRESSION

%nonassoc IF_PREC
%nonassoc ELSE

%right '='
%left OR
%left AND
%left EQ NE
%left '>' '<' LE SE IS
%right CAST
%left '+' '-'
%left '*'  '/'  '%'
%right '!' INCREMENT DECREMENT UNARY_OP
%left '.'
%left '('

%%
file:			  root
					{
						$$ = new File(lin, col);
						symtab->OutScope();
						file = $$;
					}
;

root:			/* empty */
					{
						$$ = new Root(lin, col);
						symtab->AddNewScope();
					}
				| root class
					{
						$1->AddClass($2);
						$$ = $1;
					}
;

class:			  CLASS IDENT '{'
					{
						symtab->AddSym($2, 0, -1);
						symtab->AddNewScope();
					}
					 members '}'
					{
						$$ = new Class($2, $5, lin, col);
						symtab->OutScope();
					}
				| CLASS IDENT ':' IDENT '{'
					{
						symtab->AddSym($2, 0, -1);
						symtab->IsDeclared($4, 0, -1);
						symtab->AddNewScope();
					}
					 members '}'
					{
						$$ = new ClassInher($2, $4, $7, lin, col);
						$$->AddParent($4);
						symtab->OutScope();
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
						symtab->AddVars($3, $2);
					}
;

constructor:	  accessmodif IDENT '(' args_e ')' '{' statements '}'
					{
						$$ = new Constructor($1, $2, $4, $7, lin, col);
						symtab->AddSym($2, 1, -1);
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
						symtab->AddSym($3, 2, -1, $6, $2->type, $$);
					}
				| accessmodif VOID IDENT '('
					 {
						 symtab->AddSym($3, 1, -1);
						 symtab->AddNewScope();
					 }
					 args_e ')' '{' statements '}'
					{
						$$ = new Function($1, $3, $6, $9, lin, col);
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
				| IDENT '=' expression
					{
						$$ = new Variable($1, $3, lin, col);
					}
;

accessmodif:	  /* empty */
					{
						$$ = new AccessModif(1, lin, col);
					}
				| PRIVATE
					{
						$$ = new AccessModif(2, lin, col);
					}
				| STATIC
					{
						$$ = new AccessModif(3, lin, col);
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
						symtab->IsDeclared($1);
					}
				| basictype
					{$$ = $1;}
;

basictype:		  OBJECT
					{
						$$ = new NoArrayType(5, lin, col);
					}
				| BOOL
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
						$$ = new ArrayType(1, $1, lin, col);
						symtab->IsDeclared($1);
					}
				| IDENT '[' ',' ']'
					{
						$$ = new ArrayType(2, $1, lin, col);
						symtab->IsDeclared($1);
					}
				| IDENT '[' ',' ',' ']'
					{
						$$ = new ArrayType(3, $1, lin, col);
						symtab->IsDeclared($1);
					}
				| basictype '[' ']'
					{
						$$ = new ArrayType($1->type, 1, lin, col);
					}
				| basictype '[' ',' ']'
					{
						$$ = new ArrayType($1->type, 2, lin, col);
					}
				| basictype '[' ',' ',' ']'
					{
						$$ = new ArrayType($1->type, 3, lin, col);
					}
;

expression:		  expr %prec EXPRESSION
					{$$ = $1;}
				| qualnameorarray %prec EXPRESSION
					{$$ = $1;}
;

expr:			  INCREMENT qualnameorarray
					{
						$$ = new Incr($2, true, lin, col);
					}
				| DECREMENT qualnameorarray
					{
						$$ = new Decr($2, true, lin, col);
					}
				| qualnameorarray INCREMENT
					{
						$$ = new Incr($1, false, lin, col);
					}
				| qualnameorarray DECREMENT
					{
						$$ = new Decr($1, false, lin, col);
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
				| '(' expr ')'
					{
						$$ = new Paren($2, lin, col);
					}
				| '(' IDENT ')' %prec CAST
					{
						$$ = new Paren($2, lin, col);
						symtab->IsDeclared($2);
					}
				| '(' qnora_without_id ')'
					{
						$$ = new Paren($2, lin, col);
					}
				| qualnameorarray '=' expression
					{
						$$ = new Assign($1, $3, lin, col);
					}
				| qualifiedname '(' expr_list_e ')'
					{
						$$ = new Invoke($1, $3, lin, col);
					}
				| qualifiedname '(' expr_list_e ')' arrayindex
					{
						$$ = new InvokeArr($1, $3, $5, lin, col);
					}
				| NEW IDENT '(' expr_list_e ')'
					{
						$$ = new NewObject($2, $4, lin, col);
						symtab->AddSym($2, 1, -1);
					}
				| NEW IDENT arrayindex
					{
						$$ = new NewArr($2, $3, lin, col);
						symtab->AddSym($2, 1, -1);
					}
				| expr EQ expression
					{
						$$ = new Equal($1, $3, lin, col);
					}
				| qualnameorarray EQ expression
					{
						$$ = new Equal($1, $3, lin, col);
					}
				| expr NE expression
					{
						$$ = new NotEq($1, $3, lin, col);
					}
				| qualnameorarray NE expression
					{
						$$ = new NotEq($1, $3, lin, col);
					}
				| expr '<' expression
					{
						$$ = new Smaller($1, $3, lin, col);
					}
				| qualnameorarray '<' expression
					{
						$$ = new Smaller($1, $3, lin, col);
					}
				| expr SE expression
					{
						$$ = new SmallerEq($1, $3, lin, col);
					}
				| qualnameorarray SE expression
					{
						$$ = new SmallerEq($1, $3, lin, col);
					}
				| expr '>' expression
					{
						$$ = new Larger($1, $3, lin, col);
					}
				| qualnameorarray '>' expression
					{
						$$ = new Larger($1, $3, lin, col);
					}
				| expr LE expression
					{
						$$ = new LargerEq($1, $3, lin, col);
					}
				| qualnameorarray LE expression
					{
						$$ = new LargerEq($1, $3, lin, col);
					}
				| expr '+' expression
					{
						$$ = new Add($1, $3, lin, col);
					}
				| qualnameorarray '+' expression
					{
						$$ = new Add($1, $3, lin, col);
					}
				| expr '-' expression
					{
						$$ = new Sub($1, $3, lin, col);
					}
				| qualnameorarray '-' expression
					{
						$$ = new Sub($1, $3, lin, col);
					}
				| expr '*' expression
					{
						$$ = new Mult($1, $3, lin, col);
					}
				| qualnameorarray '*' expression
					{
						$$ = new Mult($1, $3, lin, col);
					}
				| expr '/' expression
					{
						$$ = new Div($1, $3, lin, col);
					}
				| qualnameorarray '/' expression
					{
						$$ = new Div($1, $3, lin, col);
					}
				| expr '%' expression
					{
						$$ = new Mod($1, $3, lin, col);
					}
				| qualnameorarray '%' expression
					{
						$$ = new Mod($1, $3, lin, col);
					}
				| expr AND expression
					{
						$$ = new And($1, $3, lin, col);
					}
				| qualnameorarray AND expression
					{
						$$ = new And($1, $3, lin, col);
					}
				| expr OR expression
					{
						$$ = new Or($1, $3, lin, col);
					}
				| qualnameorarray OR expression
					{
						$$ = new Or($1, $3, lin, col);
					}
				| expr IS type
					{
						$$ = new Is($1, $3, lin, col);
					}
				| qualnameorarray IS type
					{
						$$ = new Is($1, $3, lin, col);
					}
				| '(' basictype ')' expression
					{
						$$ = new Cast($2, $4, lin, col);
					}
				| '(' IDENT ')' expression
					{
						$$ = new Cast($2, $4, lin, col);
						symtab->IsDeclared($2);
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

arrayindex:		  '[' expression ']'
					{
						$$ = new ArrayIndex_1($2, lin, col);
					}
				| '[' expression ',' expression ']'
					{
						$$ = new ArrayIndex_2($2, $4, lin, col);
					}
				| '[' expression ',' expression ',' expression ']'
					{
						$$ = new ArrayIndex_3($2, $4, $6, lin, col);
					}
;

qualifiedname:	  IDENT
					{
						$$ = new QualName_ID($1, lin, col);
						symtab->IsDeclared($1);
					}
				| expr '.' IDENT
					{
						$$ = new QualName_Exp($3, $1, lin, col);
						symtab->IsDeclared($3);
					}
				| qualnameorarray '.' IDENT
					{
						$$ = new QualName_Exp($3, $1, lin, col);
						symtab->IsDeclared($3);
					}
;

qualnameorarray:  IDENT
					{
						$$ = new QualName_ID($1, lin, col);
						symtab->IsDeclared($1);
					}
				| IDENT arrayindex
					{
						$$ = new QualNArray_ID_Index($1, $2, lin, col);
						symtab->IsDeclared($1);
					}
				| expr '.' IDENT
					{
						$$ = new QualName_Exp($3, $1, lin, col);
						symtab->IsDeclared($3);
					}
				| qualnameorarray '.' IDENT
					{
						$$ = new QualName_Exp($3, $1, lin, col);
						symtab->IsDeclared($3);
					}
				| expr '.' IDENT arrayindex
					{
						$$ = new QualNArray_Exp_Index($3, $1, $4, lin, col);
						symtab->IsDeclared($3);
					}
				| qualnameorarray '.' IDENT arrayindex
					{
						$$ = new QualNArray_Exp_Index($3, $1, $4, lin, col);
						symtab->IsDeclared($3);
					}
;

qnora_without_id: IDENT arrayindex
					{
						$$ = new QualNArray_ID_Index($1, $2, lin, col);
						symtab->IsDeclared($1);
					}
				| expr '.' IDENT
					{
						$$ = new QualName_Exp($3, $1, lin, col);
						symtab->IsDeclared($3);
					}
				| qualnameorarray '.' IDENT
					{
						$$ = new QualName_Exp($3, $1, lin, col);
						symtab->IsDeclared($3);
					}
				| expr '.' IDENT arrayindex
					{
						$$ = new QualNArray_Exp_Index($3, $1, $4, lin, col);
						symtab->IsDeclared($3);
					}
				| qualnameorarray '.' IDENT arrayindex
					{
						$$ = new QualNArray_Exp_Index($3, $1, $4, lin, col);
						symtab->IsDeclared($3);
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
				| WHILE '(' expression ')'  statement
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
							symtab->AddSym($2->variables->at(i)->name, 3, $1->type);
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
					}
				| variable
					{
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