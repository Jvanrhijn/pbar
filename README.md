# Pbar

A tiny header-only progress bar library for C++.

**Usage**

~~~cpp
#include <iostream>
#include <vector>
#include <unistd.h>
#include "pbar.h"

using namespace pbar;

int main() {
  int N = 100;
  int us = 100000;
  std::vector<int> v = {1, 2, 3, 4, 5};
  ProgressBar<std::vector<int>> pbar(v, 50);
  for (auto&& i : pbar) {
    // do something with i
    usleep(us);
  }
}
~~~

**Credits**

[This](https://stackoverflow.com/a/14539953) StackOverflow answer, where I got part of
the implementation from.
