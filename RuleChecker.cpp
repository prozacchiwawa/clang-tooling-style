#include <regex>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "RuleChecker.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace {
    bool path_match(const std::string &filename, const std::string &pattern)
    {
        std::regex patmatch(pattern);
        std::smatch result;
        auto search_res = std::regex_search(filename, result, patmatch);
        return search_res && result.position() == 0 && result.length() != 0;
    }
}

void RuleChecker::run(const MatchFinder::MatchResult &result)
{
    if (const clang::Stmt *S = result.Nodes.getNodeAs<clang::Stmt>(getRuleName())) {
        auto msg = evaluateRule(S);
        if (!msg.empty()) {
            auto loc = S->getLocStart();
            auto file_path = result.SourceManager->getFilename(loc);
            if (std::any_of
                (paths.begin(), paths.end(), 
                 [file_path, this] (const std::string &path) {
                    return path_match(file_path, path);
                })) {
                ReportWarning(*result.SourceManager, S, msg);                
            }
        }
    }
}

void RuleChecker::setAnalyzePaths(const std::vector<std::string> &paths)
{
    this->paths = paths;
}

void RuleChecker::ReportWarning(const SourceManager &sm, const clang::Stmt *S, const std::string &msg)
{
    if (printer) {
        printer(sm, S, msg);
    }
}
