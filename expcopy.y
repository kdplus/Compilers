%{
#include <stdio.h>
#include <string.h>
#include "node.h"
extern int yylineno;
long long dep = 0;
extern char *yytext;

void yyerror(char *str){
        fprintf(stderr, "%d : %s %s\n", yylineno, str, yytext);
}

int yywrap(){
        return 1;
}

main(){
	printf("what");
        yyparse();
}

%}

%union{
	struct Node *node;
	char *string;
}
%token <string> TYPE ID INT RETURN CONT FOR IF BREAK ASSIGNOP BINARYOP
%token <string> SEMI COMMA DOT UNARYOP LP RP
%token <string> LB RB LC RC STRUCT ELSE
%type <node> PROGRAM EXTDEFS EXTDEF EXTVARS SPEC STSPEC OPTTAG VAR FUNC PARA PARAS STMTBLOCK STMTS STMT ESTMT DEFS DEF DECS DEC INIT EXP ARRS ARGS EXTEXP
%start PROGRAM

%%
PROGRAM:	EXTDEFS
		;

EXTDEFS:	EXTDEF EXTDEFS
		|
		;

EXTDEF:		SPEC EXTVARS SEMI
		|SPEC FUNC STMTBLOCK
		;

EXTVARS:	DEC
		|DEC COMMA EXTVARS
		|
		;

SPEC:		TYPE
		|STSPEC
		;

STSPEC:		STRUCT OPTTAG LC DEFS RC
		|STRUCT ID
		;

OPTTAG:		ID
		|
		;

VAR:		ID
		|VAR LB INT RB
		;

FUNC:		ID LP PARAS RP
		;

PARA:		SPEC VAR
		;

PARAS:		PARA COMMA PARAS
		|PARA
		|
		;

STMTBLOCK:	LC DEFS STMTS RC
		;

STMTS:		STMT STMTS
		|
		;

STMT:		EXP SEMI
		|STMTBLOCK
		|RETURN EXP SEMI
		|IF LP EXP RP STMT ESTMT
		|FOR LP EXTEXP SEMI EXTEXP SEMI EXTEXP RP STMT
		|CONT SEMI
		|BREAK SEMI
		;

ESTMT:		ELSE STMT
		|
		;

DEFS:		DEF DEFS
		|
		;

DEF:		SPEC DECS SEMI
		;

DECS:		DEC COMMA DECS
		|DEC
		;

DEC:		VAR
		|VAR ASSIGNOP INIT
		;

INIT:		EXP
		|LC ARGS RC
		;

EXP:		EXP BINARYOP EXP
		|UNARYOP EXP
		|LP EXP RP
		|ID LP ARGS RP
		|ID ARRS
		|EXP DOT ID
		|INT{$$ = new_node(yylineno, "EXP", "exp", 1, new_node(yylineno, "INT", $1, 0));}
		|EXP ASSIGNOP EXP{$$ = new_node(yylineno, "EXP", "exp", 3, $1, new_node(yylineno, "ASSIGNOP", $2, 0), $3);}
		;

EXTEXP:		EXP{$$ = new_node(yylineno, "EXTEXP", "extexp", 1, $1);}
		|{$$ = NULL;}
		;

ARRS:		LB EXP RB ARRS{$$ = new_node(yylineno, "ARRS", "arrs", 2, $2, $4);}
		|{$$ = NULL;}
		;

ARGS:		EXP COMMA ARGS{$$ = new_node(yylineno, "ARGS", "args", 2, $1, $3);}
		|EXP{$$ = new_node(yylineno, "ARGS", "args", 1, $1);}
		;



%%

