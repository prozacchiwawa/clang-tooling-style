#ifndef _RESULT_RECEIVER_H_
#define _RESULT_RECEIVER_H_

#include <map>
#include <string>
#include <functional>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

class ResultPrinterBase {
public:
    ResultPrinterBase() : warnings_(), debug_() { }
    virtual ~ResultPrinterBase() { }
    
    bool getDebug() const { return debug_; }
    void setDebug(bool debug) { debug_ = debug; }

    int getWarnings() const { return warnings_; }

protected:
    int warnings_;
    bool debug_;
};

template <class RuleTarget>
class ResultPrinter : public ResultPrinterBase {
public:
    void PrintWarning(const clang::SourceManager &sm, const RuleTarget *S, const std::string &msg)
    {
        auto loc = S->getLocStart();
        llvm::outs() << "(" << loc.printToString(sm) << ") " << msg << "\n";
        warnings_++;
        if (debug_) {
            S->dump();
        }
    }
};

#endif//_RESULT_RECEIVER_H_
