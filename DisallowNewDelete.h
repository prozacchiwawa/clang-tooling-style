#ifndef _DISALLOW_NEW_DELETE_H_
#define _DISALLOW_NEW_DELETE_H_

#include <string>
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "RuleChecker.h"

class DisallowNew : public RuleChecker<clang::Stmt>
{
public:
    void SetupMatches(clang::ast_matchers::MatchFinder &finder) override;

    std::string getRuleName() const override { return "new"; }
    std::string evaluateRule(const clang::Stmt *) override;
};

class DisallowDelete : public RuleChecker<clang::Stmt>
{
public:
    void SetupMatches(clang::ast_matchers::MatchFinder &finder) override;

    std::string getRuleName() const override { return "delete"; }
    std::string evaluateRule(const clang::Stmt *) override;
};

#endif//_DISALLOW_NEW_DELETE_H_
