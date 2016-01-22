/*
	get the tokens from the lex 
	parse it as grammar write in the middle part of this file
	and in each grammar create a node for it
	thus, we could get a abstract syntax tree
	use a dfs func to print the abstract syntax tree
*/
%{
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "codegen.h"
class CGContext;
extern int yylineno;
long long dep = 0;
extern char *yytext;
extern Node* Groot;
extern int yyparse();
extern int yylex();
void createCoreFunctions(CGContext& context);

void yyerror(char *str){
        fprintf(stderr, "%d : %s %s\n", yylineno, str, yytext);
}

int yywrap(){
	return 1;
}

main(){
        yyparse();
	printf("Abstract Syntax Tree: \n");
	print_ast(Groot, 0);	
	//LLVMInitializeNativeTarget();
	//InitializeNativeTargetAsmPrinter();
	//InitializeNativeTargetAsmParser();
	CGContext context;
//	createCoreFunctions(context);
	context.genCode(Groot);
//	context.runCode();
}


%}

%union{
	struct Node *node;
	char *string;
}
%token <string> TYPE ID INT RETURN CONT FOR IF BREAK ASSIGNOP BINARYOP UNBINARYOP
%token <string> SEMI COMMA DOT UNARYOP LP RP
%token <string> LB RB LC RC STRUCT ELSE
%type <node> PROGRAM EXTDEFS EXTDEF EXTVARS SPEC STSPEC OPTTAG VAR FUNC PARA PARAS STMTBLOCK STMTS STMT ESTMT DEFS DEF DECS DEC INIT EXP ARRS ARGS EXTEXP
%start PROGRAM

%%
PROGRAM:	EXTDEFS{Groot = $$ = new_node(yylineno, "PROGRAM", "program", 1, $1);}
		;

EXTDEFS:	EXTDEF EXTDEFS{$$ = new_node(yylineno, "EXTDEFS", "extdefs", 2, $1, $2);}
		|{$$ = NULL;}
		;

EXTDEF:		SPEC EXTVARS SEMI{$$ = new_node(yylineno, "EXTDEF", "extdef", 2, $1, $2);}
		|SPEC FUNC STMTBLOCK{$$ = new_node(yylineno, "EXTDEF", "extdef", 3, $1, $2, $3);}
		;

EXTVARS:	DEC{$$ = new_node(yylineno, "EXTVARS", "extvars", 1, $1);}
		|DEC COMMA EXTVARS{$$ = new_node(yylineno, "EXTVARS", "extvars", 2, $1, $3);}
		|{$$ = NULL;}		
		;

SPEC:		TYPE{$$ = new_node(yylineno, "SPEC", "spec", 1, new_node(yylineno, "TYPE", $1, 0));}
		|STSPEC{$$ = new_node(yylineno, "SPEC", "spec", 1, $1);}
		;

STSPEC:		STRUCT OPTTAG LC DEFS RC{$$ = new_node(yylineno, "STSPEC", "stspec", 2, $2, $4);}
		|STRUCT ID{$$ = new_node(yylineno, "STSPEC", "stspec", 1, new_node(yylineno, "ID", $2, 0));}
		;

OPTTAG:		ID{$$ = new_node(yylineno, "OPTTAG", "opttag", 1, new_node(yylineno, "ID", $1, 0));}
		|{$$ = NULL;}
		;

VAR:		ID{$$ = new_node(yylineno, "VAR", "var", 1, new_node(yylineno, "ID", $1, 0));}
		|VAR LB INT RB{$$ = new_node(yylineno, "VAR", "var", 2, $1, new_node(yylineno, "INT", $3, 0));}
		;

FUNC:		ID LP PARAS RP{$$ = new_node(yylineno, "FUNC", "func", 2, new_node(yylineno, "ID", $1, 0), $3);}
		;

PARA:		SPEC VAR{$$ = new_node(yylineno, "PARA", "para", 2, $1, $2);}
		;

PARAS:		PARA COMMA PARAS{$$ = new_node(yylineno, "PARAS", "paras", 2, $1, $3);}
		|PARA{$$ = new_node(yylineno, "PARAS", "paras", 1, $1);}
		|{$$ = NULL;}
		;

STMTBLOCK:	LC DEFS STMTS RC{$$ = new_node(yylineno, "STMTBLOCK", "stmtblock", 2, $2, $3);}
		;

STMTS:		STMT STMTS{$$ = new_node(yylineno, "STMTS", "stmts", 2, $1, $2);}
		|{$$ = NULL;}
		;

STMT:		EXP SEMI{$$ = new_node(yylineno, "STMT", "stmt", 1, $1);}
		|STMTBLOCK{$$ = new_node(yylineno, "STMT", "stmt", 1, $1);}
		|RETURN EXP SEMI{$$ = new_node(yylineno, "STMT", "stmt", 2, new_node(yylineno, "RETURN", $1, 0),$2);}
		|IF LP EXP RP STMT ESTMT{$$ = new_node(yylineno, "STMT", "stmt", 4, new_node(yylineno, "IF", $1, 0), $3, $5, $6);}
		|FOR LP EXTEXP SEMI EXTEXP SEMI EXTEXP RP STMT{$$ = new_node(yylineno, "STMT", "stmt", 5, new_node(yylineno, "FOR", $1, 0), $3, $5, $7, $9);}
		|CONT SEMI{$$ = new_node(yylineno, "STMT", "stmt", 1, $1);}
		|BREAK SEMI{$$ = new_node(yylineno, "STMT", "stmt", 1, new_node(yylineno, "BREAK", $1, 0));}
		;

ESTMT:		ELSE STMT{$$ = new_node(yylineno, "ESTMT", "estmt", 2, new_node(yylineno, "ELSE", $1, 0), $2);}
		|{$$ = NULL;}
		;

DEFS:		DEF DEFS{$$ = new_node(yylineno, "DEFS", "defs", 2, $1, $2);}
		|{$$ = NULL;}
		;

DEF:		SPEC DECS SEMI{$$ = new_node(yylineno, "DEF", "def", 2, $1, $2);}
		;

DECS:		DEC COMMA DECS{$$ = new_node(yylineno, "DECS", "decs", 2, $1, $3);}
		|DEC{$$ = new_node(yylineno, "DECS", "decs", 1, $1);}
		;

DEC:		VAR{$$ = new_node(yylineno, "DEC", "dec", 1, $1);}
		|VAR ASSIGNOP INIT{$$ = new_node(yylineno, "DEC", "dec", 3, $1, new_node(yylineno, "ASSIGNOP", $2, 0), $3);}
		;

INIT:		EXP{$$ = new_node(yylineno, "INIT", "init", 1, $1);}
		|LC ARGS RC{$$ = new_node(yylineno, "INIT", "init", 1, $2);}
		;

EXP:		EXP UNBINARYOP EXP{$$ = new_node(yylineno, "EXP", "exp", 3, $1, new_node(yylineno, "UNBINARYOP", $2, 0), $3);}
		|EXP BINARYOP EXP{$$ = new_node(yylineno, "EXP", "exp", 3, $1, new_node(yylineno, "BINARYOP", $2, 0), $3);}
		|UNARYOP EXP{$$ = new_node(yylineno, "EXP", "exp", 2, new_node(yylineno, "UNARYOP", $1, 0), $2);}
		|UNBINARYOP EXP{$$ = new_node(yylineno, "EXP", "exp", 2, new_node(yylineno, "UNBINARYOP", $1, 0), $2);}
		|LP EXP RP{$$ = new_node(yylineno, "EXP", "exp", 1, $2);}
		|ID LP ARGS RP{$$ = new_node(yylineno, "EXP", "exp", 2, new_node(yylineno, "ID", $1, 0), $3);}
		|ID ARRS{$$ = new_node(yylineno, "EXP", "exp", 2, new_node(yylineno, "ID", $1, 0), $2);}
		|EXP DOT ID{$$ = new_node(yylineno, "EXP", "exp", 3, $1, new_node(yylineno, "DOT", $2, 0), new_node(yylineno, "ID", $3, 0));}
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

