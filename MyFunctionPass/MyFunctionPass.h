#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Metadata.h>
#include <llvm/IR/DebugInfo.h>
#include <llvm/IR/DebugLoc.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <string>
#include <set>
using namespace llvm;

namespace {
	/* Call graph is a class using a C++ pair to keep the relationship of function callee and caller */
	class CallGraph {
	public:
		CallGraph(Function *mainFunc) : mMain(mainFunc) {}
		~CallGraph() {}
		void AddEdge(Function * f, Function * t) {
			m_map[f].insert(t);
		}

		void print() const;

		void dump() const;
		/* check if input function is in mapping */
		bool hasFunction(Function *func) {
			std::map<Function*, std::set<Function*> >::iterator iter = m_map.find(func);
			if (iter == m_map.end())
				return false;
			return true;
		}
		/* C++ pair, with first being target function, and second being its following calling functions */
		std::map<Function*, std::set<Function*> > m_map;
		// std::list<Function*> valueList;
	private:
		Function* mMain;
		unsigned int edge_count = 0;
	};


	class CGPass : public ModulePass {
	public:
		static char ID;
		CGPass() : ModulePass(ID){}
		/* according to the [document](http://llvm.org/docs/WritingAnLLVMPass.html#the-runonmodule-method), class which inherit ModulePass should implement runOnModule */
		virtual bool runOnModule(Module &M) override;
		
	private:
		CallGraph *G;
	};
	char CGPass::ID = 0;
}