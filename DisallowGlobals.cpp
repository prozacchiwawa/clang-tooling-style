#include "DisallowGlobals.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace {
// We want to catch:
// 1) Global on the left side of equals
// 2) Non-const pointer or reference to global
auto bannedDecl = 
    ignoringParenImpCasts
    (declRefExpr
     (to
      (varDecl
       (hasGlobalStorage(),
        unless(hasType(isConstQualified()))))));

auto globalDecl =
    ignoringParenImpCasts
    (declRefExpr
     (to
      (varDecl
       (hasGlobalStorage()))));

auto GlobalMutationMatcherSubscript =
    arraySubscriptExpr
    (hasBase(bannedDecl)).bind("global");

auto bannedUnary(const std::string &name, const std::string &binding) -> 
    decltype(unaryOperator
             (hasOperatorName(name),
              hasUnaryOperand
              (bannedDecl)).bind(binding)) {
    return unaryOperator
        (hasOperatorName(name),
         hasUnaryOperand
         (bannedDecl)).bind(binding);
}

auto bannedBinary(const std::string &name, const std::string &binding) ->
    decltype(binaryOperator
             (hasOperatorName(name), 
              hasLHS
              (bannedDecl)).bind(binding)) {
    return binaryOperator
        (hasOperatorName(name), 
         hasLHS
         (bannedDecl)).bind(binding);
}

}

void DisallowGlobals::SetupMatches(MatchFinder &finder)
{
  finder.addMatcher(GlobalMutationMatcherSubscript, this);
  for (auto unop : std::vector<std::string> { "&", "--", "++" }) {
      finder.addMatcher(bannedUnary(unop, "global"), this);
  }
  for (auto binop : std::vector<std::string> 
      { "=", "+=", "-=", "/=", "%=", "^=", "&=", "*=", "|=", "<<=", ">>=" }) {
      finder.addMatcher(bannedBinary(binop, "global"), this);
  }
}

std::string DisallowGlobals::evaluateRule(const clang::Stmt *S)
{
    return "Mutated global";
}
