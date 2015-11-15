/*
	Use this lex file match the tokens in the source code
	and get its string val return to yacc	

	note : add minus as a UNBINARYOP 
*/
%{
#include "stdio.h"
#include "y.tab.h"
//extern YYSTYPE  yylineno;
extern char *yytext;
%}
%x minus
%%
[ \t\v\f]+				;
[\n]                  			{yylineno++;}
([0-9]*|0[xX][0-9a-fA-F]+)		{yylval.string = strdup(yytext); return INT;}
"("					{yylval.string = strdup(yytext); return LP;}
")"					{yylval.string = strdup(yytext); return RP;}
"["					{yylval.string = strdup(yytext); return LB;}
"]"					{yylval.string = strdup(yytext); return RB;}
"{"					{yylval.string = strdup(yytext); return LC;}
"}"					{yylval.string = strdup(yytext); return RC;}
"int"					{yylval.string = strdup(yytext); return TYPE;}
"struct"				{yylval.string = strdup(yytext); return STRUCT;}
"return"				{yylval.string = strdup(yytext); return RETURN;}
"if"					{yylval.string = strdup(yytext); return IF;}
"else"					{yylval.string = strdup(yytext); return ELSE;}
"break"					{yylval.string = strdup(yytext); return BREAK;}
"continue"				{yylval.string = strdup(yytext); return CONT;}
"for"					{yylval.string = strdup(yytext); return FOR;}
";"					{yylval.string = strdup(yytext); return SEMI;}
","					{yylval.string = strdup(yytext); return COMMA;}
"."					{yylval.string = strdup(yytext); return DOT;}
"="					{yylval.string = strdup(yytext); return ASSIGNOP;}
"&"					{yylval.string = strdup(yytext); return BINARYOP;}
"!"					{yylval.string = strdup(yytext); return UNARYOP;}
"~"					{yylval.string = strdup(yytext); return UNARYOP;}
"*"					{yylval.string = strdup(yytext); return BINARYOP;}
"/"					{yylval.string = strdup(yytext); return BINARYOP;}
"%"					{yylval.string = strdup(yytext); return BINARYOP;}
"+"					{yylval.string = strdup(yytext); return BINARYOP;}
"-"					{yylval.string = strdup(yytext); return UNBINARYOP;}
"<<"					{yylval.string = strdup(yytext); return BINARYOP;}
">>"					{yylval.string = strdup(yytext); return BINARYOP;}
"|"					{yylval.string = strdup(yytext); return BINARYOP;}
"++"					{yylval.string = strdup(yytext); return UNARYOP;}
"--"					{yylval.string = strdup(yytext); return UNARYOP;}
">"					{yylval.string = strdup(yytext); return BINARYOP;}
">="					{yylval.string = strdup(yytext); return BINARYOP;}
"<"					{yylval.string = strdup(yytext); return BINARYOP;}
"<="					{yylval.string = strdup(yytext); return BINARYOP;}
"=="					{yylval.string = strdup(yytext); return BINARYOP;}
"!="					{yylval.string = strdup(yytext); return BINARYOP;}
"^"					{yylval.string = strdup(yytext); return BINARYOP;}
"&&"					{yylval.string = strdup(yytext); return BINARYOP;}
"||"					{yylval.string = strdup(yytext); return BINARYOP;}
"+="					{yylval.string = strdup(yytext); return BINARYOP;}
"-="					{yylval.string = strdup(yytext); return BINARYOP;}
"*="					{yylval.string = strdup(yytext); return BINARYOP;}
"/="					{yylval.string = strdup(yytext); return BINARYOP;}
"&="					{yylval.string = strdup(yytext); return BINARYOP;}
"^="					{yylval.string = strdup(yytext); return BINARYOP;}
"|="					{yylval.string = strdup(yytext); return BINARYOP;}
"<<="					{yylval.string = strdup(yytext); return BINARYOP;}
">>="					{yylval.string = strdup(yytext); return BINARYOP;}
[a-zA-Z][a-zA-Z0-9]*                    {yylval.string = strdup(yytext); return ID;}
.					{yyerror("Error!");}
%%
