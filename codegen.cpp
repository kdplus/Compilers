#include <iostream>
#include "node.h"
#include "codegen.h"

using namespace llvm;

//extern Node* Groot;

void CGContext::genCode(Node* root){
	//Create the Module and main func
	std::cout << "Generating code ...\n";
	std::vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), makeArrayRef(argTypes), false);
	Function *globaFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "globa", module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", globaFunction, 0);
	//builder.SetInsertPoint(entry);
	
	//push
	pushBlock(bblock);
	root->codeGen(*this);
	popBlock();

	//root->codeGen(*this);
	std::cout << "Done.\n";
	std::string s("");
	llvm::raw_string_ostream os(s);
	(*this).module->print(os, NULL);
	os.flush();
	std::cout << s << std::endl;	
}

/*
GenericValue CGContext::runCode() {
	std::cout << "Running code...\n";
	
//	ExecutionEngine *ee = EngineBuilder(module).create();
//	ee->finalizeObject();
//	std::vector<GenericValue> noargs;
//	GenericValue v = ee->runFunction(mainFunction, noargs);
	std::cout << "Code was run.\n";
	return NULL;
}*/
