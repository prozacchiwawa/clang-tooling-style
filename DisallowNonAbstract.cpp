#include "DisallowNonAbstract.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace clang {
    namespace ast_matchers {
        AST_MATCHER(CXXMethodDecl, isNotVirtual) {
            return !Node.isVirtual();
        }
        AST_MATCHER(CXXMethodDecl, isNotImplicit) {
            return !Node.isImplicit();
        }
    }
}

namespace {
    auto bannedRecordDecl(std::string binding, const std::string &nameMatch) -> decltype(recordDecl().bind("")) {
        return recordDecl
            (matchesName(nameMatch),
             unless(isImplicit()),
             hasMethod
             (allOf
              (isNotVirtual(),
               isNotImplicit()))).bind(binding);
    }
}

DisallowNonAbstract::DisallowNonAbstract(const std::vector<std::string> &disallowed) :
    disallowed_namespaces(disallowed)
{
}

void DisallowNonAbstract::SetupMatches(clang::ast_matchers::MatchFinder &finder)
{
    for (auto &match : disallowed_namespaces) {
        finder.addMatcher(bannedRecordDecl(getRuleName(), match), this);
    }
}

std::string DisallowNonAbstract::evaluateRule(const clang::Decl *D)
{
    return "Non-abstract class declared in banned namespace";
}
