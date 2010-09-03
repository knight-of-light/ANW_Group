
%{
	#include "stdio.h"
	
	#include <iostream>
	using std::cout;	
	using std::endl;
	using std::left;
	
	#include "ast.h"
	
	#define YY_INPUT(buf,result,max_size) \
    { \
    int c = getchar(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }\

	
	#include "parser.h"
	
	int lin = 1, col = 1, errorno = 0;
%}
%x comment
REAL1	(0|[1-9][0-9]*)"."[0-9]*([eE][-+]?(0|[1-9][0-9]*))?
REAL2	"."[0-9]+([eE][-+]?(0|[1-9][0-9]*))?
REAL3	(0|[1-9][0-9]*)([eE][-+]?(0|[1-9][0-9]*))
%%
"/*"					BEGIN(comment); col += yyleng;
<comment>[^*\n]*		col += yyleng;
<comment>"*"+[^*/\n]*	col += yyleng;
<comment>\n				lin++; col = 1;
<comment>"*"+"/"		BEGIN(0); col += yyleng;

"//".*\n				lin++; col = 1;

"class"				{
						col += yyleng;
						return CLASS;
					}
"boolean"			{						
						col += yyleng;
						return BOOLEAN;
					}
"double"			{
						col += yyleng;
						return DOUBLE;
					}
"int"				{
						col += yyleng;
						return INT;
					}
"if"				{
						col += yyleng;
						return IF;
					}
"else"				{
						col += yyleng;
						return ELSE;
					}
"while"				{
						col += yyleng;
						return WHILE;
					}
"for"				{
						col += yyleng;
						return FOR;
					}
"false"				{
						col += yyleng;
						return FALSE;
					}
"true"				{
						col += yyleng;
						return TRUE;
					}
"extends"			{
						col += yyleng;
						return EXTENDS;
					}
"instanceof"	    {
						col += yyleng;
						return INSTANCEOF;
					}
"new"				{
						col += yyleng;
						return NEW;
					}
"this"				{
						col += yyleng;
						return THIS;
					}
"null"				{
						col += yyleng;
						return NUL;
					}
"private"			{
						col += yyleng;
						return PRIVATE;
					}
"static"			{
						col += yyleng;
						return STATIC;
					}
"void"				{
						col += yyleng;
						return VOID;
					}
"return"			{
						col += yyleng;
						return RETURN;
					}
"++"				{
						col += yyleng;
						return INCREMENT;
					}
"--"				{
						col += yyleng;
						return DECREMENT;
					}
">="				{
						col += yyleng;
						return LE;
					}
"<="				{						
						col += yyleng;
						return SE;
					}
"=="				{
						col += yyleng;
						return EQ;
					}
"!="				{
						col += yyleng;
						return NE;
					}
"&&"				{
						col += yyleng;
						return AND;
					}
"||"				{
						col += yyleng;
						return OR;
					}
			
									
0|[1-9][0-9]*		{
						col += yyleng;
						yylval.tInteger = new Integer(atoi(yytext), lin, col);
						return INTEGER;
					}
					
(0|[1-9][0-9]*)"."[0-9]*([eE][-+]?(0|[1-9][0-9]*))? |
"."[0-9]+([eE][-+]?(0|[1-9][0-9]*))?				|
(0|[1-9][0-9]*)([eE][-+]?(0|[1-9][0-9]*))			{
														col += yyleng;
														yylval.tReal = new Real(atof(yytext), lin, col);
														return REAL;
													}
([a-zA-Z]|_)([a-zA-Z]|[0-9]|_)*	{
									col += yyleng;
									yylval.tIdent = new Ident(strdup(yytext), lin, col);
									return IDENT;
								}
[\t]*				col += 4 * yyleng;
[ ]*				col += yyleng;
\n					lin++; col = 1;
[+*/%<>={}().;,-<>]		return *yytext;
.					{
						col += yyleng;
						errorno++;
					}
%%
int yywrap(void)
{
	return 1;
}



   