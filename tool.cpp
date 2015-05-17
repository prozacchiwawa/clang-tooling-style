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
static cl::extrahelp MoreHelp("\nDetect direct mutation of globals from checked code.");

static cl::opt<bool> Debug
("debug", 
 cl::desc("Enable diagnostic output for the tool itself"), 
 cl::cat(NoGlobalStyleCategory));
static cl::opt<bool> Werror
("Werror",
 cl::desc("Promote warnings to errors"),
 cl::cat(NoGlobalStyleCategory));

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, NoGlobalStyleCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  MatchFinder finder;
  DisallowGlobals disallowGlobals;
  ResultPrinter resultPrinter;

  disallowGlobals.SetupMatches(finder);
  RuleChecker::printer_t printer(std::bind(&ResultPrinter::PrintWarning, &resultPrinter, _1, _2, _3));
  disallowGlobals.setPrinter(printer);

#ifndef NDEBUG
  llvm::DebugFlag = Debug.getValue();
#endif
  resultPrinter.setDebug(Debug.getValue());

  return Tool.run(newFrontendActionFactory(&finder).get()) || (Werror.getValue() && resultPrinter.getWarnings());
}
