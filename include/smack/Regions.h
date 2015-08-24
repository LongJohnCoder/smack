//
// This file is distributed under the MIT License. See LICENSE for details.
//
#ifndef REGION_H
#define REGION_H

#include "dsa/DSGraph.h"
#include "smack/DSAAliasAnalysis.h"

namespace smack {

class Region {
private:
  const DSNode* representative;
  unsigned offset;
  unsigned length;
  bool bytewise;

  static const DataLayout* DL;
  static DSAAliasAnalysis* DSA;
  // static DSNodeEquivs* NEQS;

  void init(const Value* V, unsigned offset, unsigned length);
  bool isIncomplete();
  bool isComplicated();
  bool isDisjoint(unsigned offset, unsigned length);

public:
  Region(const Value* V);
  Region(const Value* V, unsigned length);
  Region(const Value* V, unsigned offset, unsigned length);

  static void init(Module& M, Pass& P);

  void merge(Region& R);
  bool overlaps(Region& R);

  bool isSingleton() const;
  bool isAllocated() const;
  bool bytewiseAccess() const { return bytewise; }

  void print(raw_ostream&);

};

class Regions : public ModulePass, public InstVisitor<Regions> {

  vector<Region> regions;
  unsigned idx(Region& R);

public:
  static char ID;
  Regions() : ModulePass(ID) { }
  virtual void getAnalysisUsage(llvm::AnalysisUsage& AU) const;
  virtual bool runOnModule(llvm::Module& M);

  unsigned size() const;
  unsigned idx(const llvm::Value* v);
  unsigned idx(const llvm::Value* v, unsigned offset);
  unsigned idx(const llvm::Value* v, unsigned offset, unsigned length);
  Region& get(unsigned R);

  // void visitModule(Module& M) {
  //   for (const GlobalValue& G : M.globals())
  //     collect(&G);
  // }

  // void visitAllocaInst(AllocaInst& I) {
    // getRegion(&I);
  // }

  void visitLoadInst(LoadInst&);
  void visitStoreInst(StoreInst&);
  void visitAtomicCmpXchgInst(AtomicCmpXchgInst&);
  void visitAtomicRMWInst(AtomicRMWInst&);
  void visitMemIntrinsic(MemIntrinsic&);
  void visitCallInst(CallInst&);

};

}

#endif // REGION_H