#include <iostream>

#ifndef __PBAR_H
#define __PBAR_H

namespace pbar {

class ProgressBar {
public:
  ProgressBar(int width, const char symbol)
      : width_(width), symbol_(symbol), pos_(0) {}

  ProgressBar Update(float percentage);

  friend std::ostream& operator<<(std::ostream &steam, const ProgressBar &pbar);

private:
  int pos_;
  int width_;
  char symbol_;
  char left_delim_{'['};
  char right_delim_{']'};
  char pointer_{'>'};

}; // class ProgressBar

inline ProgressBar ProgressBar::Update(float percentage) {
      pos_ = width_*percentage;
      return *this;
  }

inline std::ostream& operator<<(std::ostream &stream, const ProgressBar &pbar) {
    stream << pbar.left_delim_;
    for (int i=0; i<pbar.width_; i++) {
        if (i < pbar.pos_)
            stream << pbar.symbol_;
        else if (i == pbar.pos_)
            stream << pbar.pointer_;
        else
            stream << " ";
    }
    stream << pbar.right_delim_ << int(float(pbar.pos_)/pbar.width_*100) << "%\r";
    stream.flush();
    return stream;
}

}; // namespace pbar

#endif // __PBAR_H
