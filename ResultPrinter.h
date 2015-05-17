#ifndef _RESULT_RECEIVER_H_
#define _RESULT_RECEIVER_H_

#include <map>
#include <string>
#include <functional>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

class ResultPrinter {
public:
    ResultPrinter();
    
    bool getDebug() const { return debug_; }
    void setDebug(bool debug) { debug_ = debug; }
    int getWarnings() const { return warnings_; }

    void PrintWarning(const clang::SourceManager &sm, const clang::Stmt *S, const std::string &msg);

private:
    int warnings_;
    bool debug_;
};

#endif//_RESULT_RECEIVER_H_
