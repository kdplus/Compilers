#include <iostream>
#include "node.h"
Node* Groot = NULL;
static Type *typeOf(char* type) {
	if (strcmp(type, "int") == 0) {
		return Type::getInt64Ty(getGlobalContext());
	}
	return Type::getVoidTy(getGlobalContext());
}

llvm::Value* programCg(Node* now, CGContext& context){
	Node* extdefs = now->childs[0];
	std::cout << "Creating Program\n";
	return extdefs->codeGen(context);
}

llvm::Value* extdefsCg(Node* now, CGContext& context){
        //StatementList::const_iterator it;
	Value *last = NULL;
	std::cout << "Genrating code for " << now->type << " childCnt " << now->childCnt << std::endl;
	for (int i = 0; i < now->childCnt; ++i) {
		if(now->childs[i] == NULL) break;
		std::cout << "Generating code for " << now->childs[i]->type <<" " << i << std::endl;
		last = now->childs[i]->codeGen(context);
	}
	std::cout << "Creating extdefs\n";
	return last;
}

llvm::Value* extdefCg(Node* now, CGContext& context){
        Value *last = NULL;
	std::cout << "Generating code for " << now->type << std::endl;
	if(strcmp(now->childs[1]->type, "EXTVARS") == 0){
		char* varType = now->childs[0]->childs[0]->data; 
		last = VarDecCg(varType, now->childs[1], context);
	} else { 
		last = FunDecCg(now, context);    
	}
	std::cout << "Creating extdef\n";
	return last;
}

llvm::Value* extvarsCg(Node* now, CGContext& context){
        Value *last = NULL;
        for (int i = 0; i < now->childCnt; ++i) {
                std::cout << "Generating code for " << now->type << std::endl;
                last = now->childs[i]->codeGen(context);
        }
        std::cout << "Creating extdef" << std::endl;
        return last;
}

long long myAtoi(char* s){
	long long ret = 0;
	for(int i = 0; s[i]; ++i){
		ret *= 10;
		ret += s[i] - '0';
	}return ret;
}

Value* IntegerCg(Node* now, CGContext& context)
{
	if(strcmp(now->type, "INIT") == 0){
		char* intstr = now->childs[0]->childs[0]->data;
		long long value = myAtoi(intstr);
		std::cout << "Creating integer: " << value << std::endl;
		return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), value, true);
	}
	if(strcmp(now->type, "INT") == 0){
		char* intstr = now->data;
		long long value = myAtoi(intstr);
                std::cout << "Creating integer: " << value << std::endl;
                return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), value, true);
	}
}


llvm::Value* AssignCg(Node* now, CGContext& context)
{
	char * idName = now->childs[0]->childs[0]->data;
	std::cout << "Creating assignment for " << idName << std::endl;
	if (context.locals().find(idName) == context.locals().end()) {
		std::cerr << "undeclared variable " << idName << std::endl;
		return NULL;
	}
	return new StoreInst(IntegerCg(now->childs[2], context), context.locals()[idName], false, context.currentBlock());
}


llvm::Value* VarDecCg(char* varType, Node* now, CGContext& context)
{
	Node* dec = now->childs[0];
	char* idName = dec->childs[0]->childs[0]->data; 
	std::cout << "Creating variable declaration " << varType << " " << idName << std::endl;
	AllocaInst *alloc = new AllocaInst(typeOf(varType), idName, context.currentBlock());
	context.locals()[idName] = alloc;
std::cout << "test here ok" << std::endl;

	if (dec->childCnt > 1) {
		AssignCg(dec, context);
	}
	std::cout << "test here ok" << std::endl;
	//if there are more than one var should be dec..
	if(now->childCnt > 1){
		VarDecCg(varType, now->childs[1], context);
	}
	std::cout << "test ok" << std::endl;
	return alloc;
}

Value* FunDecCg(Node* now, CGContext& context){	
	char* funType = now->childs[0]->childs[0]->data; 
	char* funIdName = now->childs[1]->childs[0]->data;
	std::cout << "Generating code for func " << funType << ' ' << funIdName << std::endl;
	std::vector<Type*> argTypes;
	Node* paras = now->childs[1]->childs[1];
	std::vector<char*> idList;
	//put the there their type and id to two vectors
	std::cout << "test here 1" << std::endl;
	while(1){
		if(paras == NULL) break;
		if(paras->childCnt == 0) break;
		char* typeStr = paras->childs[0]->childs[0]->childs[0]->data;
		char* idName = paras->childs[0]->childs[1]->childs[0]->data;
		std::cout << "Generating arg var " << typeStr << ' ' << idName << std::endl;
		argTypes.push_back(typeOf(typeStr));
		idList.push_back(idName);
		if(paras->childCnt <= 1) break;
		paras = paras->childs[1];
	}
	std::cout << "test here 2" << std::endl;
	FunctionType *ftype = FunctionType::get(typeOf(funType), makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, funIdName, context.module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);
	
	context.pushBlock(bblock);
	//if this func is mainfunc 
	if(strcmp(funIdName, "main") == 0) context.mainFunction = function;		
	
	Function::arg_iterator argsValues = function->arg_begin();
    	Value* argumentValue;
	for (int i = 0; i < idList.size(); ++i) {
		//vardec
		std::cout << "Creating variable declaration " << argTypes[i] << " " << idList[i] << std::endl;
		AllocaInst *alloc = new AllocaInst(argTypes[i], idList[i], context.currentBlock());
		context.locals()[idList[i]] = alloc;
		//store
		argumentValue = argsValues++;
		argumentValue->setName(idList[i]);
		StoreInst *inst = new StoreInst(argumentValue, context.locals()[idList[i]], false, bblock);
	}
	
	now->childs[2]->codeGen(context);
	std::cout << "back to the fundec\n";
	ReturnInst::Create(getGlobalContext(), context.getCurrentReturnValue(), bblock);
	std::cout << "test here\n";
	context.popBlock();
	std::cout << "Creating function: " << funIdName << std::endl;
	return function;
}

llvm::Value* stmtblockCg(Node* now, CGContext& context){
        //StatementList::const_iterator it;
        /*
	Value *last = NULL;
        for (int i = 0; i < now->childCnt; ++i) {
                std::cout << "Generating code for " << now->type << std::endl;
                last = now->childs[i]->codeGen(now->childs[i], context);
        }*/
	std::cout << "Generating code for " << now->type << " child " << now->childCnt<< std::endl;
	if(now->childs[1] != NULL)now->childs[1]->codeGen(context);
        std::cout << "Creating stmtblock\n";
        return NULL;
}

llvm::Value* stmtsCg(Node *now, CGContext& context){
	std::cout << "Generating code for " << now->type << std::endl;
	now->childs[0]->codeGen(context);
	if(now->childs[1]) now->childs[1]->codeGen(context);
	std::cout << "Creating stmts\n";
	return NULL;
}

llvm::Value* stmtCg(Node* now, CGContext& context){
	if(strcmp(now->childs[0]->type, "RETURN") == 0) ReturnCg(now, context);
	return NULL;
}

llvm::Value* ReturnCg(Node* now, CGContext& context){
	std::cout << "Generating return code for " << now->childs[1]->type << std::endl;
	Value *returnValue = now->childs[1]->codeGen(context);
	context.setCurrentReturnValue(returnValue);
	return returnValue;
}

llvm::Value* expCg(Node* now, CGContext& context){
	std::cout << "Generating code for " << now->type << std::endl;
	if(strcmp(now->childs[0]->type, "INT") == 0) return IntegerCg(now->childs[0], context);
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), 0, true);
}

Node* new_node(int line, char const *type, char const *data, int childCnt, ...){
	Node *now = (Node*)malloc(sizeof(Node));
		now->lineNum;
	now->data = strdup(data);
	now->type = strdup(type);
	now->childCnt = now->cap = childCnt;
	now->childs = (Node**)malloc(sizeof(Node*) * childCnt);
	va_list ap;
	va_start(ap, childCnt);
	int i;
	for(i = 0; i < childCnt; ++i) now->childs[i] = va_arg(ap, Node*);
	va_end(ap);
	return now;
}

void print_ast(Node *now, int dep){
	int i = 0;
	for(i = 0; i < 2 * dep; ++i) printf("-");
	printf("|%s (%s)\n", now->type, now->data);
	for(i = 0; i < now->childCnt; ++i){
		if(now->childs[i]) print_ast(now->childs[i], dep + 1);
	}
	
	/*if(strcmp(now->type, "PROGRAM") == 0){
		programBlock = 
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} else if(strcmp(now->type, "EXTDEFS") == 0){
	} */
}
