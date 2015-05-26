#ifndef _DISALLOW_COUPLING_H_
#define _DISALLOW_COUPLING_H_

#include <string>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "RuleChecker.h"

class DisallowCoupling : public RuleChecker<clang::Stmt>
{
public:
    DisallowCoupling(const std::vector<std::string> &namespaces);
    void SetupMatches(clang::ast_matchers::MatchFinder &finder) override;

    std::string getRuleName() const override { return "banned-namespace"; }
    std::string evaluateRule(const clang::Stmt *S) override;

private:
    std::vector<std::string> disallowed_namespaces;
};

#endif//_DISALLOW_COUPLING_H_
