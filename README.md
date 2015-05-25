# clang-tooling-style
An enforcer for some opinionated rules about coupling in C++

This is an exercise that is helping to refactor a large, old C++ codebase
(along with <https://github.com/prozacchiwawa/braceIt>).

Because this codebase exhibits high coupling (specifically, a god object, an all-static class,
and a few other highly coupled classes all of which contain methods or parts of methods that
belong to each-other), I am writing this to output a sort of refactoring roadmap and as a kind
of scorecard.  I have already reduced one 15kloc source file from 50 violations of these current
rules to 1, resulting in an object that can finally exist in a unit test.  

The last rule, banning coupling to specific namespaces is intended as a way of holding
coupling back once some level of decoupling has been achieved.  How well this works in practice
is a social, rather than a technological problem.  It can't be worse that what's been tried
before however.

Currently enforces the following rules:

- Expressions that generate non-const references to global storage are not allowed.
- Bare new and delete operators are not allowed.
- Namespaces may be specified where non-abstract classes are not allowed.

Planned rules:

- Expressions generating objects of record type from namespaces to which coupling is banned.

