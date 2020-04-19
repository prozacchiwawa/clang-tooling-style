#include "DisallowCoupling.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::ast_matchers;
using clang::ast_matchers::internal::Matcher;

namespace clang {
    namespace ast_matchers {
        AST_MATCHER_P(FunctionDecl, functionBody, internal::Matcher<Stmt>, InnerMatcher) {
            auto body = Node.getBody();
            return (body != nullptr && InnerMatcher.matches(*body, Finder, Builder));
        }

        AST_MATCHER_P(QualType, dump, internal::Matcher<QualType>, InnerMatcher) {
            Node.dump();
            return InnerMatcher.matches(Node, Finder, Builder);
        }
    }
}

namespace {
    using clang::ast_matchers::functionBody;

#define EXPR_INNER ignoringParenImpCasts(hasType(recordDecl(matchesName(nameMatch))))

    auto namespaceMatcher(const std::string &nameMatch) -> Matcher<clang::QualType> {
        return qualType
            (hasDeclaration
             (hasDeclContext
              (namespaceDecl
               (matchesName(nameMatch)))));
    }

    auto matchingMemberExpression(const std::string &binding, const std::string &nameMatch) -> decltype(expr().bind("")) {
        return memberExpr
            (hasObjectExpression
             (anyOf
              (hasType
               (hasCanonicalType
                (pointsTo(namespaceMatcher(nameMatch)))),
               hasType
               (hasCanonicalType
                (namespaceMatcher(nameMatch)))))).bind(binding);
    }
    auto matchingExpression(const std::string &binding, const std::string &nameMatch) -> decltype(expr().bind("")) {
        return expr
            (anyOf
             (forEachDescendant
              (expr
               (hasType
                (hasCanonicalType
                 (pointsTo
                  (namespaceMatcher(nameMatch)))))),
              forEachDescendant
              (expr
               (hasType
                (hasCanonicalType
                 (namespaceMatcher(nameMatch))))),
              forEachDescendant
              (callExpr
               (hasAnyArgument
                (hasType
                 (hasCanonicalType
                  (namespaceMatcher(nameMatch)))))))).bind(binding);
    }
    auto matchingStatement(const std::string &binding, const std::string &nameMatch) -> Matcher<clang::Stmt> {
        return stmt
            (anyOf
             (forEachDescendant
              (matchingMemberExpression(binding, nameMatch)),
              forEachDescendant
              (matchingExpression(binding, nameMatch)),
              declStmt
              (containsDeclaration
               (0, varDecl
                (hasInitializer
                 (matchingExpression(binding, nameMatch)))))));

    }
    auto banned(const std::string &binding, const std::string &nameMatch) -> Matcher<clang::Decl> {
        return 
            functionDecl
            (unless(anyOf(cxxMethodDecl(ofClass(matchesName(nameMatch))),namedDecl(matchesName(nameMatch)))),
             functionBody(compoundStmt(forEach(matchingStatement(binding, nameMatch)))));
    }
}

DisallowCoupling::DisallowCoupling(const std::vector<std::string> &disallowed) :
    disallowed_namespaces(disallowed)
{
}

void DisallowCoupling::SetupMatches(clang::ast_matchers::MatchFinder &finder)
{
    for (auto &match : disallowed_namespaces) {
        finder.addMatcher(banned(getRuleName(), match), this);
    }
}

std::string DisallowCoupling::evaluateRule(const clang::Stmt *S)
{
    return "Coupling to banned namespace";
}
