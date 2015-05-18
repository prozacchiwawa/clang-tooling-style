#ifndef _DISALLOW_GLOBALS_H_
#define _DISALLOW_GLOBALS_H_

#include <string>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "RuleChecker.h"

class DisallowGlobals : public RuleChecker
{
public:
    void SetupMatches(clang::ast_matchers::MatchFinder &finder) override;

    std::string getRuleName() const override { return "global"; }
    std::string evaluateRule(const clang::Stmt *) override;
};

#endif//_DISALLOW_GLOBALS_H_
