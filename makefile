LEX = flex
YACC = yacc
CC = cc

LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS += $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs bitwriter core support)

exp1: lex.yy.c y.tab.c
	llvm-g++ lex.yy.c y.tab.c codegen.cpp node.cpp `llvm-config --cppflags --libs core --ldflags --system-libs` -std=c++11 -lpthread -ldl -o smallc -ll > error.log
y.tab.c y.tab.h: smallc.y node.h
	$(YACC) smallc.y -d > error.log

lex.yy.c: smallc.l node.h
	$(LEX) smallc.l > error.log

clean:
	rm -f *.o *.c *tab.h	
