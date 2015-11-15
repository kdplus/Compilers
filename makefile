LEX = flex
YACC = yacc
CC = cc

exp1: lex.yy.c y.tab.c
	$(CC) lex.yy.c y.tab.c -o exp1 

y.tab.c y.tab.h: exp1.y node.h
	$(YACC) exp1.y -d

lex.yy.c: exp1.l node.h
	$(LEX) exp1.l

clean:
	rm -f *.o *.c	
