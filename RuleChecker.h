#ifndef _RULE_CHECKER_H_
#define _RULE_CHECKER_H_

#include <functional>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/CommandLine.h"

class RuleChecker : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
    typedef 
    std::function<void(const clang::SourceManager &, const clang::Stmt *, const std::string &)>
        printer_t;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result);
    void setAnalyzePaths(const std::vector<std::string> &paths);

    virtual void SetupMatches(clang::ast_matchers::MatchFinder &finder) = 0;
    virtual std::string getRuleName() const = 0;
    virtual std::string evaluateRule(const clang::Stmt *) = 0;

    void setPrinter(printer_t printer) {
        this->printer = printer;
    }

protected:
    void ReportWarning(const clang::SourceManager &, const clang::Stmt *, const std::string &);

private:
    printer_t printer;
    std::vector<std::string> paths;
};

#endif//_RULE_CHECKER_H_
