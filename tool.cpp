#include <memory>
#include <functional>
#include <algorithm>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "DisallowGlobals.h"
#include "DisallowNewDelete.h"
#include "DisallowNonAbstract.h"
#include "ResultPrinter.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;
using namespace std::placeholders;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory NoGlobalStyleCategory("noglob options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp
("\nDetect\n"
 "- Direct mutation of globals from checked code\n"
 "- Uses of bare new and delete operators\n"
 "- Non-abstract class or struct types in abstract-only namespaces\n"
 "- Coupling to non-abstract classes and structs in coupling-disallwed\n"
 "  namespaces\n");

static cl::opt<bool> Debug
("debug", 
 cl::desc("Enable diagnostic output for the tool itself"), 
 cl::cat(NoGlobalStyleCategory));
static cl::opt<bool> Werror
("Werror",
 cl::desc("Promote warnings to errors"),
 cl::cat(NoGlobalStyleCategory));
static cl::list<std::string> analyze_paths
("analyze-path",
 cl::desc("Path of files to analyze.  Files not matching one of these prefixes will be ignored."),
 cl::cat(NoGlobalStyleCategory));

static cl::list<std::string> abstract_namespaces
("abstract-namespace",
 cl::desc("Namespace designated to contain only abstract classes and structs."),
 cl::cat(NoGlobalStyleCategory));

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, NoGlobalStyleCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    
    MatchFinder finder;

    // Snarf abstract-only namespaces from the environment.
    std::vector<std::string> abstract_namespaces_v;
    std::copy(abstract_namespaces.begin(), abstract_namespaces.end(), std::back_inserter(abstract_namespaces_v));
    std::unique_ptr<RuleCheckerBase> rules[] = {
        make_unique<DisallowNew>(),
        make_unique<DisallowDelete>(),
        make_unique<DisallowGlobals>(),
        make_unique<DisallowNonAbstract>(abstract_namespaces_v)
    };
    size_t rules_size = sizeof(rules) / sizeof(rules[0]);
    auto rules_begin = &rules[0];
    auto rules_end = &rules[rules_size];

    std::vector<std::string> analyze_paths_v;
    std::copy(analyze_paths.begin(), analyze_paths.end(), std::back_inserter(analyze_paths_v));

    for (size_t i = 0; i < sizeof(rules) / sizeof(rules[0]); ++i) {
        auto &rule = rules[i];
        rule->setAnalyzePaths(analyze_paths_v);
        rule->SetupMatches(finder);
        rule->getPrinter().setDebug(Debug.getValue());
    }
    
#ifndef NDEBUG
    llvm::DebugFlag = Debug.getValue();
#endif

    return Tool.run(newFrontendActionFactory(&finder).get()) || 
        (Werror.getValue() && 
         std::any_of
         (rules_begin, rules_end, 
          [] (const std::unique_ptr<RuleCheckerBase> &rule) {
             return rule->getPrinter().getWarnings();
         }));
}
