#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "RuleChecker.h"

using namespace clang;
using namespace clang::ast_matchers;

void RuleChecker::run(const MatchFinder::MatchResult &result)
{
    if (const clang::Stmt *S = result.Nodes.getNodeAs<clang::Stmt>(getRuleName())) {
        auto msg = evaluateRule(S);
        if (!msg.empty()) {
            ReportWarning(*result.SourceManager, S, msg);
        }
    }
}

void RuleChecker::ReportWarning(const SourceManager &sm, const clang::Stmt *S, const std::string &msg)
{
    if (printer) {
        printer(sm, S, msg);
    }
}
