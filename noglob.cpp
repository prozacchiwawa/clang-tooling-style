// Thanks: http://clang.llvm.org/docs/LibASTMatchersTutorial.html
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("noglob options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nDetect direct mutation of globals from checked code.");

// We want to catch:
// 1) Global on the left side of equals
// 2) Non-const pointer or reference to global
auto GlobalMutationMatcherAssignment = 
    binaryOperator
    (hasOperatorName("="), 
     hasLHS
     (ignoringParenImpCasts
      (declRefExpr
       (to
        (varDecl
         (hasGlobalStorage())))))).bind("global");
auto GlobalMutationMatcherAddressOf =
    unaryOperator
    (hasOperatorName("&"),
     hasUnaryOperand
     (declRefExpr
      (to
       (varDecl
        (hasGlobalStorage(),
         unless(hasType(isConstQualified()))))))).bind("global");

class GlobalPrinter : public MatchFinder::MatchCallback {
public :
    virtual void run(const MatchFinder::MatchResult &Result) {
        if (const clang::Stmt *S = Result.Nodes.getNodeAs<clang::Stmt>("global"))
            S->dump();
    }
};

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  MatchFinder finder;
  GlobalPrinter globalPrinter;

  finder.addMatcher(GlobalMutationMatcherAssignment, &globalPrinter);
  finder.addMatcher(GlobalMutationMatcherAddressOf, &globalPrinter);
  
  return Tool.run(newFrontendActionFactory(&finder).get());
}
