//Teammates: Sourin Chakrabarti, Ujwal Pratap Krishna Koluvakolanu Thyagarajan
#include "Instrument.hpp"

bool Instrument::doInitialization(Module &M __attribute__((unused))) {
    return false;
}


bool Instrument::runOnModule(Module &M) {
    for (Module::iterator f = M.begin(), fe = M.end(); f != fe; f++) {
        if (!f->isDeclaration()) {
            runOnFunction(M, *f);
        }
    }
    return true;
}


bool Instrument::runOnFunction(Module &M, Function &F) {

    for (Function::iterator b = F.begin(), be = F.end(); b != be; b++) {
        for (BasicBlock::iterator i = b->begin(), ie = b->end(); i != ie; i++) {
            Instruction *inst = (Instruction *) i;
            BasicBlock *basicBlock = (BasicBlock *) b;

            /***
            ** STEP-1:
            ** Reimplement the following if-condition statement to check
            ** for the instructions of interest (array accesses).
            ***/
            if (GetElementPtrInst * gep = dyn_cast<GetElementPtrInst>(inst)) {
                if (gep->getNumIndices() > 0) {
                    Value *array = gep->getPointerOperand();
                    Type *arrayType = array->getType()->getPointerElementType();
                    if (arrayType->isArrayTy() || arrayType->isPointerTy()) {
                        /***
	                    ** STEP-2: Retrieve the size of the array
	                    **/
                        unsigned int arraySize;
                        if (arrayType->isArrayTy()) {
                            arraySize = arrayType->getArrayNumElements();
                        } else {
                            arraySize = 0; //dynamic array
                        }


                        if (gep->getNumIndices() >= 2) {
                            /***
                            ** STEP-3: Retrieve the index of the array element being accessed
                            **/
                            Value *index = gep->getOperand(2);
                            /***
	                        ** STEP-4: Retrieve the source information of the instruction
	                        **/
                            DebugLoc loc = inst->getDebugLoc();
                            std::string filename = loc->getFilename();

                            /***
	                        ** STEP-5: Create and store the arguments of function check_bounds
	                        ** in the vector args.
	                        **/
                            std::vector < Value * > args;
                            IRBuilder<> B(basicBlock);
                            args.push_back(index);
                            args.push_back(ConstantInt::get(Type::getInt64Ty(M.getContext()), arraySize));
                            args.push_back(B.CreateGlobalStringPtr(filename));
                            args.push_back(ConstantInt::get(Type::getInt32Ty(M.getContext()), loc.getLine()));

                            /***
	                        ** The code below creates and inserts the call before inst. Modify as needed.
	                        **/
                            Function *callee = M.getFunction("check_bounds");
                            if (callee) {
                                CallInst::Create(callee, args, "", inst);
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}


bool Instrument::doFinalization(Module &M __attribute__((unused))) {
    return false;
}

void Instrument::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
}

char Instrument::ID = 0;

static const RegisterPass <Instrument> registration("instrument", "Instrument array accesses");
