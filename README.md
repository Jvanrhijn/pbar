# Pbar

A tiny header-only progress bar library for C++. The progress bar is itself an iterator,
whose `operator++()` is overloaded to output a progress bar to `std::clog` on each call.

### Usage

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

### Notes

Currently, calling `ProgressBar::begin()` resets the internal counter to zero, so calling `begin()`
on the progress bar object during iteration will reset the bar. This is not ideal, so for the moment
you shouldn't call `begin()` on the ProgressBar when you need to access the first element, but
instead call it on the actual container you're iterating over.

### Credits

* [This](https://stackoverflow.com/a/14539953) StackOverflow answer, where I got part of
the implementation from.
* Toby Speight on codereview stackexchange, who helped signicantly improve the code quality.
