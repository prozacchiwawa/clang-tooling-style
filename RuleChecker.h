#ifndef _RULE_CHECKER_H_
#define _RULE_CHECKER_H_

#include <functional>
#include <regex>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/CommandLine.h"
#include "ResultPrinter.h"

class RuleCheckerBase {
public:
    virtual ~RuleCheckerBase()
    {
    }

    void setAnalyzePaths(const std::vector<std::string> &paths)
    {
        this->paths = paths;
    }

    virtual void SetupMatches(clang::ast_matchers::MatchFinder &finder) = 0;
    virtual std::string getRuleName() const = 0;

    virtual ResultPrinterBase &getPrinter() = 0;

protected:
    std::vector<std::string> paths;
    bool path_match(const std::string &filename, const std::string &pattern)
    {
        std::regex patmatch(pattern);
        std::smatch result;
        auto search_res = std::regex_search(filename, result, patmatch);
        return search_res && result.position() == 0 && result.length() != 0;
    }
};

template <class RuleTarget>
class RuleChecker : public clang::ast_matchers::MatchFinder::MatchCallback, public RuleCheckerBase {
public:
    typedef RuleTarget target_t;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result)
    {
        if (const RuleTarget *S = result.Nodes.getNodeAs<RuleTarget>(getRuleName())) {
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

    ResultPrinterBase &getPrinter()
    {
        return printer;
    }

    virtual std::string evaluateRule(const RuleTarget *) = 0;

protected:
    void ReportWarning(const clang::SourceManager &sm, const RuleTarget *S, const std::string &msg)
    {
        printer.PrintWarning(sm, S, msg);
    }

private:
    ResultPrinter<RuleTarget> printer;
};

#endif//_RULE_CHECKER_H_
