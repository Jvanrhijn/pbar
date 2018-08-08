# Pbar

A tiny header-only progress bar library for C++.

**Usage**

~~~cpp
#include <iostream>
#include <unistd.h>
#include "pbar.h"

using namespace pbar;

int main() {
  int N = 100;
  int us = 100000;
  ProgressBar pbar(50, '=');
  for (int i=0; i<N; i++) {
    std::cout << pbar.Update(double{i}/(N-1));
    usleep(us);
  }
}
~~~

**Credits**

[This](https://stackoverflow.com/a/14539953) StackOverflow answer, where I got
the implementation from.
