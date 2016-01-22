LEX = flex
YACC = yacc
CC = cc

LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS += $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs bitwriter core support)

exp1: lex.yy.c y.tab.c
	llvm-g++ lex.yy.c y.tab.c codegen.cpp node.cpp `llvm-config --cppflags --libs core --ldflags --system-libs` -std=c++11 -lpthread -ldl -o exp1 -ll > error.log
y.tab.c y.tab.h: exp1.y node.h
	$(YACC) exp1.y -d > error.log

lex.yy.c: exp1.l node.h
	$(LEX) exp1.l > error.log

clean:
	rm -f *.o *.c *tab.h	
