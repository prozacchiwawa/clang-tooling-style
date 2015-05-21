#include "DisallowNewDelete.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace {
// We want to catch uses of new and delete.
    auto bannedNew = newExpr().bind("new");
    auto bannedDelete = deleteExpr().bind("delete");
}

void DisallowNew::SetupMatches(MatchFinder &finder)
{
    finder.addMatcher(bannedNew, this);
}

std::string DisallowNew::evaluateRule(const clang::Stmt *S)
{
    return "New used";
}

void DisallowDelete::SetupMatches(MatchFinder &finder)
{
    finder.addMatcher(bannedDelete, this);
}

std::string DisallowDelete::evaluateRule(const clang::Stmt *S)
{
    return "Delete used";
}
