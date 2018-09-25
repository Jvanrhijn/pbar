# Pbar

A tiny header-only progress bar library for C++. The progress bar is itself an iterator,
whose `operator++()` is overloaded to output a progress bar to stdout on each call.

**Usage**

~~~cpp
#include <iostream>
#include <vector>
#include <unistd.h>
#include "pbar.h"

using namespace pbar;

int main() {
  int us = 100000;
  std::vector<int> v = {1, 2, 3, 4, 5};
  ProgressBar<std::vector<int>::iterator> pbar(v.begin(), v.end(), 50);
  for (auto i = pbar.begin(); i != pbar.end(); i++) {
    usleep(us);
  }

  // In C++17, the template argument may be omitted
  // The constructor allows changing the bar symbol (default '=')
  ProgressBar pbar2(v.begin(), v.end(), 50, '#');

  std::cout << "\nRange based loops also work" << std::endl;

  for (auto& i: pbar2) {
    usleep(us);
  }
}
~~~

**Credits**

[This](https://stackoverflow.com/a/14539953) StackOverflow answer, where I got part of
the implementation from.
