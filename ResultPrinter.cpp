#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "ResultPrinter.h"

ResultPrinter::ResultPrinter() :
    warnings_(),
    debug_()
{
}

void ResultPrinter::PrintWarning
(const clang::SourceManager &sm, const clang::Stmt *S, const std::string &msg)
{
    auto loc = S->getLocStart();
    llvm::outs() << "(" << loc.printToString(sm) << ") " << msg << "\n";
    warnings_++;
    if (debug_) {
        S->dump();
    }
}
