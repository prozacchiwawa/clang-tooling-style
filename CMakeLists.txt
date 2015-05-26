set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(noglob
  DisallowGlobals.cpp
  DisallowNewDelete.cpp
  DisallowNonAbstract.cpp
  DisallowCoupling.cpp
  tool.cpp
  )
target_link_libraries(noglob
  clangTooling
  clangBasic
  clangASTMatchers
  )
