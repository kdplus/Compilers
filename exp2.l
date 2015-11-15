/*
    This source file defines the tokens needed in the grammar.
    Tokens are represented as regular expressions and correctly
    returned when encountered.
*/

%{

#include <stdio.h>
#include "node.h"
#include "y.tab.h"
extern FILE *yyin;
extern char *yytext;

%}

INT         0|[1-9][0-9]*|0[xX][0-9a-fA-F]+|0[0-7]+
SEMI        \;
COMMA       \,
DOT         \.
BINARYOP3   [\*\/\%]
BINARYOP4   [\+\-]
BINARYOP5   \<\<|\>\>
BINARYOP6   [\>\<]|\>\=|\<\=
BINARYOP7   \=\=|\!\=
BINARYOP8   \&
BINARYOP9   \^
BINARYOP10  \|
BINARYOP11  \&\&
BINARYOP12  \|\|
UNARYOP     [\-\+\~\!]|\+\+|\-\-
ASSIGNOP    [\=]|\+\=|\-\=|\*\=|\/\=|\&\=|\^\=|\|\=|\>\>\=|\<\<\=
TYPE        int
LP          \(
RP          \)
LB          \[
RB          \]
LC          \{
RC          \}
STRUCT      struct
RETURN      return
IF          if
ELSE        else
BREAK       break
CONT        continue
FOR         for
ID          [a-zA-Z][a-zA-Z]*

%%
{INT}       {/* When the number is hexadecimal, convert it into decimal form. */
             if(yytext[0]=='0' && (yytext[1]=='x' || yytext[1]=='X')) yylval.iValue = strtol(yytext,NULL,16);
             /* When the number is octal, convert it into decimal form. */
             else if(yytext[0]=='0') yylval.iValue = strtol(yytext,NULL,8);
             /* Return directly if the number is decimal */
             else yylval.iValue = atoi(yytext); return INT;}
{SEMI}      {return SEMI;}
{COMMA}     {return COMMA;}
{DOT}       {return DOT;}
{BINARYOP3}     {yylval.oPerator = yytext; return BINARYOP3;}
{BINARYOP4}     {yylval.oPerator = yytext; return BINARYOP4;}
{BINARYOP5}     {yylval.oPerator = yytext; return BINARYOP5;}
{BINARYOP6}     {yylval.oPerator = yytext; return BINARYOP6;}
{BINARYOP7}     {yylval.oPerator = yytext; return BINARYOP7;}
{BINARYOP8}     {yylval.oPerator = yytext; return BINARYOP8;}
{BINARYOP9}     {yylval.oPerator = yytext; return BINARYOP9;}
{BINARYOP10}    {yylval.oPerator = yytext; return BINARYOP10;}
{BINARYOP11}    {yylval.oPerator = yytext; return BINARYOP11;}
{BINARYOP12}    {yylval.oPerator = yytext; return BINARYOP12;}
{UNARYOP}   {yylval.oPerator = yytext; return UNARYOP;}
{ASSIGNOP}  {yylval.oPerator = yytext; return ASSIGNOP;}
{TYPE}      {return TYPE;}
{LP}        {return LP;}
{RP}        {return RP;}
{LB}        {return LB;}
{RB}        {return RB;}
{LC}        {return LC;}
{RC}        {return RC;}
{STRUCT}    {return STRUCT;}
{RETURN}    {return RETURN;}
{IF}        {return IF;}
{ELSE}      {return ELSE;}
{BREAK}     {return BREAK;}
{CONT}      {return CONT;}
{FOR}       {return FOR;}
\n          /* ignore */
[ \t]+      /* ignore whitespace */
{ID}        {yylval.sIndex = yytext; return ID;}

%%

int yywrap(void){
    return 1;
}
