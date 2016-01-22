/*
	create a node for every grammar
	thus we could get a syntax tree
	and print it by dfs
*/
#ifndef NODE
#define NODE
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include "codegen.h"
#include <vector>
#include <llvm/IR/Value.h>
#include <deque>
class CGContext;
struct Node;
llvm::Value* programCg(Node* now, CGContext& context);
llvm::Value* extdefsCg(Node* now, CGContext& context);
llvm::Value* extdefCg(Node* now, CGContext& context);
llvm::Value* extvarsCg(Node* now, CGContext& context);
//llvm::Value* specCg(Node* now, CGContext& context);
//llvm::Value* stspecCg(Node* now, CGContext& context);
//llvm::Value* opttagCg(Node* now, CGContext& context);
//llvm::Value* varCg(Node* now, CGContext& context);
//llvm::Value* funcCg(Node* now, CGContext& context);
//llvm::Value* parasCg(Node* now, CGContext& context);
llvm::Value* stmtblockCg(Node* now, CGContext& context);
llvm::Value* stmtsCg(Node* now, CGContext& context);
llvm::Value* stmtCg(Node* now, CGContext& context);
//llvm::Value* estmtCg(Node* now, CGContext& context);
//llvm::Value* defsCg(Node* now, CGContext& context);
//llvm::Value* defCg(Node* now, CGContext& context);
//llvm::Value* decsCg(Node* now, CGContext& context);
//llvm::Value* decCg(Node* now, CGContext& context);
//llvm::Value* initCg(Node* now, CGContext& context);
llvm::Value* expCg(Node* now, CGContext& context);
//llvm::Value* extexp(Node* now, CGContext& context);
//llvm::Value* arrs(Node* now, CGContext& context);
llvm::Value* VarDecCg(char* varType, Node* now, CGContext& context);
llvm::Value* FunDecCg(Node* now, CGContext& context);
llvm::Value* ReturnCg(Node* now, CGContext& context);


typedef struct Node{
	char *type;
	char *data;
	int cap, childCnt, lineNum;
	struct Node** childs;
	llvm::Value* codeGen(CGContext& context){
		if(strcmp(type, "PROGRAM") == 0) return programCg(this, context); 	
		else if(strcmp(type, "EXTDEFS") == 0) return extdefsCg(this, context);
		else if(strcmp(type, "EXTDEF") == 0) return extdefCg(this, context);
		else if(strcmp(type, "EXTVARS") == 0) return extvarsCg(this, context);
		//else if(strcmp(type, "SPEC") == 0) return specCg(this, context);
		//else if(strcmp(type, "STSPEC") == 0) return stspecCg(this, context);
		//else if(strcmp(type, "OPTTAG") == 0) return opttagCg(this, context);
		//else if(strcmp(type, "VAR") == 0) return varCg(this, context);
		//else if(strcmp(type, "FUNC") == 0) return funcCg(this, context);
		//else if(strcmp(type, "PARAS") == 0) return parasCg(this, context);
		else if(strcmp(type, "STMTBLOCK") == 0) return stmtblockCg(this, context);
		else if(strcmp(type, "STMTS") == 0) return stmtsCg(this, context);
		else if(strcmp(type, "STMT") == 0) return stmtCg(this, context);
		//else if(strcmp(type, "ESTMT") == 0) return estmtCg(this, context);
		//else if(strcmp(type, "DEFS") == 0) return defsCg(this, context);
		//else if(strcmp(type, "DEF") == 0) return defCg(this, context);
		//else if(strcmp(type, "DECS") == 0) return decsCg(this, context);
		//else if(strcmp(type, "DEC") == 0) return decCg(this, context);
		//else if(strcmp(type, "INIT") == 0) return initCg(this, context);
		else if(strcmp(type, "EXP") == 0) return expCg(this, context);
		//else if(strcmp(type, "EXTEXP") == 0) return extexp(this, context);
		//else if(strcmp(type, "ARRS") == 0) return arrs(this, context);
	}
} Node;






/*
class cgcontext;
class Stmt;
class Exp;
class Extdef;

typedef std::vector<Stmt*> StmtList;
typedef std::vector<Exp*> ExpList;
typedef std::vector<Extdef*> ExtdefList;

class BNode{
public:
	virtual ~BNode() {}
	virtual llvm::Value* codeGen(){return NULL;}
};

class Exp : public BNode{
};

class Stmt : public BNode{
};

class Integer : public Exp{
public:
	long long value;
	Integer(long long value) : value(value) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};
*/
/*
class Extdef;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::deque<Extdef*> ExtdefList;
typedef std::deque<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class BNode {
public:
	virtual ~BNode() {}
	virtual llvm::Value* codeGen(CodeGenContext& context) { return NULL; }
};

class NExpression : public BNode {
};

class NStatement : public BNode {
};

class Extdef : public BNode{
};
 
class Extdefs : public BNode{
public:
	ExtdefList extdefs;
	Extdefs() {}
	virtual llvm::Value* codeGen(CGContext& context);
};

		

class NInteger : public NExpression {
public:
	long long value;
	NInteger(long long value) : value(value) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};


class NIdentifier : public NExpression {
public:
	std::string name;
	NIdentifier(const std::string& name) : name(name) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NMethodCall : public NExpression {
public:
	const NIdentifier& id;
	ExpressionList arguments;
	NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
		id(id), arguments(arguments) { }
	NMethodCall(const NIdentifier& id) : id(id) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression {
public:
	int op;
	NExpression& lhs;
	NExpression& rhs;
	NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
		lhs(lhs), rhs(rhs), op(op) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class Unbinaryop : public NExpression{
public:
	int op;
	NExpression& rhs;
	Unbinaryop(int op, NExpression& rhs) : op(op), rhs(rhs) {}
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class Unaryop : public NExpression{
public:
        int op;
        NExpression& rhs;
        Unbinaryop(int op, NExpression& rhs) : op(op), rhs(rhs) {}
        virtual llvm::Value* codeGen(CodeGenContext& context);
};



class NAssignment : public NExpression {
public:
	NIdentifier& lhs;
	NExpression& rhs;
	NAssignment(NIdentifier& lhs, NExpression& rhs) : 
		lhs(lhs), rhs(rhs) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement {
public:
	NExpression& expression;
	NExpressionStatement(NExpression& expression) : 
		expression(expression) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NReturnStatement : public NStatement {
public:
	NExpression& expression;
	NReturnStatement(NExpression& expression) : 
		expression(expression) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NVariableDeclaration : public NStatement {
public:
	const NIdentifier& type;
	NIdentifier& id;
	NExpression *assignmentExpr;
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
		type(type), id(id) { assignmentExpr = NULL; }
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
		type(type), id(id), assignmentExpr(assignmentExpr) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExternDeclaration : public NStatement {
public:
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList arguments;
    NExternDeclaration(const NIdentifier& type, const NIdentifier& id,
            const VariableList& arguments) :
        type(type), id(id), arguments(arguments) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionDeclaration : public NStatement {
public:
	const NIdentifier& type;
	const NIdentifier& id;
	VariableList arguments;
	NBlock& block;
	NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, 
			const VariableList& arguments, NBlock& block) :
		type(type), id(id), arguments(arguments), block(block) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};
*/
Node* new_node(int line, char const *type, char const *data, int childCnt, ...);

void print_ast(Node *now, int dep);

#endif	
