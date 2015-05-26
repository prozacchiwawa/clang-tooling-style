#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <vector>
#include <utility>

int *x;

int main() {
    x = new int();
    auto y = (nullptr, new int());
    delete x;
}
