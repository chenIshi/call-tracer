
#include "MyFunctionPass.h"
#include <iostream>
#include <fstream>
#include <deque>

void CallGraph::dump() const {

	for (auto pairs : m_map) {
		for (Function* second : pairs.second) {
			outs() << "Function " << pairs.first->getName() << " -> " << "Function " << second->getName() << ";\n";
		}
	}
}


/* ModulePass get in as a module's entry, which is mostly likely to be main in cpp */
bool CGPass::runOnModule(Module &M) {

	Function *main = M.getFunction("main");

	G = new CallGraph(main);
	if (!main) return false;

	/* list of functions await to be process/ traverse */
	std::deque<Function*> list;
	list.push_back(main);

	while (!list.empty()) {
		Function* func = list.front();
		list.pop_front();

		/* function iterator iterates through basic blocks in the module */
		for (Function::iterator iter = func->begin(); iter != func->end(); ++iter) {
			/* basicblock iterator iterates through instructions in a block */
			for (BasicBlock::iterator Biter = iter->begin(); Biter != iter->end(); ++Biter)	{
				Instruction *I = &*Biter;
				if (CallInst *inst = dyn_cast<CallInst>(I))	{
					Function* called = inst->getCalledFunction();

					/* add the function call to call graph */
					if (called)	{
						G->AddEdge(func, called);
						/* add the function called to the waiting list for latter processing only if no duplicated */
						if (!G->hasFunction(called)) {
							list.push_back(called);
						}
					}
				}

				/* invokeInst is basically like a callInst, but it can end with a exception, while callInst is all end via return */
				/* the handler is then exactly the same though */
				if (InvokeInst *inst = dyn_cast<InvokeInst>(I))	{
					Function* called = inst->getCalledFunction();
					errs() << "hello\n";

					if (called)	{
						G->AddEdge(func, called);
						if (!G->hasFunction(called)) {
							list.push_back(called);
						}
					}
				}
			}
		}
	}

	G->dump();
}

static RegisterPass<CGPass> X("CGPass", "Call Graph Pass", false, false);