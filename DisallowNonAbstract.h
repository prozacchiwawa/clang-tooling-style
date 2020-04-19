#ifndef _DISALLOW_NON_ABSTRACT_H_
#define _DISALLOW_NON_ABSTRACT_H_

#include <string>
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "RuleChecker.h"

class DisallowNonAbstract : public RuleChecker<clang::Decl>
{
public:
    DisallowNonAbstract(const std::vector<std::string> &namespaces);
    void SetupMatches(clang::ast_matchers::MatchFinder &finder) override;

    std::string getRuleName() const override { return "abstract-namespace"; }
    std::string evaluateRule(const clang::Decl *D) override;

private:
    std::vector<std::string> disallowed_namespaces;
};

#endif//_DISALLOW_NON_ABSTRACT_H_
