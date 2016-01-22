#include <stack>
#include <typeinfo>
#ifndef CGC
#define CGC
#include <iostream>
#include <llvm-c/Target.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
//#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
//#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
//#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Process.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
//#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include "node.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/ADT/Triple.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Timer.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

struct Node;
class CGBlock {
public:
    BasicBlock *block;
    Value *returnValue;
    std::map<char* , Value*> locals;
};


class CGContext{
public:
	std::stack<CGBlock *> blocks;
    	Function *mainFunction;
	Module *module;
	//LLVMContext& ctt;
public:
	CGContext(){
		//ctt = llvm::getGlobalContext();
		//Module* module = new llvm::Module("top", context);
		//IRBuilder<> builder(context);
		module =  new Module("top", getGlobalContext());
/*
llvm::FunctionType *funcType = 
      llvm::FunctionType::get(builder.getInt32Ty(), false);
  llvm::Function *mainFunc = 
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
  builder.SetInsertPoint(entry);
*/
/*
llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world!\n");

  std::vector<llvm::Type *> putsArgs;
  putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

  llvm::FunctionType *putsType = 
    llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
  llvm::Constant *putsFunc = module->getOrInsertFunction("puts", putsType);

builder.CreateCall(putsFunc, helloWorld);
  builder.CreateRetVoid();

		module->dump();*/
		std::cout << "I am here!" << std::endl;
	}
	void genCode(Node *root);
	//GenericValue runCode();
	std::map<char* , Value*>& locals() { return blocks.top()->locals; }
    	BasicBlock *currentBlock() { return blocks.top()->block; }
    	void pushBlock(BasicBlock *block) { blocks.push(new CGBlock()); blocks.top()->returnValue = NULL; blocks.top()->block = block; }
   	void popBlock() { CGBlock *top = blocks.top(); blocks.pop(); delete top; }
    	void setCurrentReturnValue(Value *value) { blocks.top()->returnValue = value; }
    	Value* getCurrentReturnValue() { return blocks.top()->returnValue; }
};

#endif
