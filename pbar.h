#include <iostream>
#include <iterator>

#ifndef __PBAR_H
#define __PBAR_H

namespace pbar {

template<class It>
class ProgressBar {
public:
  ProgressBar(It&& it, It&& it_end, int width, const char symbol='=')
      : pos_(0), 
        width_(width), 
        symbol_(symbol), 
        iter_(it),
        iter_begin_(it),
        iter_end_(it_end) 
  {}

  using value_type = typename It::value_type;
  using reference = typename It::reference;

  class iterator 
    : public std::iterator<typename It::iterator_category,
                           value_type,
                           typename It::difference_type,
                           typename It::pointer,
                           reference>
  {
  private:
    value_type val_ = *iter_;
    ProgressBar<It> *parent_;

  public:
    iterator(ProgressBar<It> *parent, value_type start) 
      : val_(start), parent_(parent) {}

    iterator& operator++();
    iterator operator++(int);

    bool operator==(iterator other);
    bool operator!=(iterator other);
    reference operator*(); 

  };

  iterator begin();
  iterator end();

  template<class I>
  friend std::ostream& operator<<(std::ostream &steam, const ProgressBar<I> &pbar);

private:
  int pos_;
  int width_;
  char symbol_;
  char left_delim_{'['};
  char right_delim_{']'};
  char pointer_{'>'};

  It iter_;
  It iter_begin_;
  It iter_end_;

}; // class ProgressBar


template<class It>
using piter = typename ProgressBar<It>::iterator;

template<class It>
inline bool ProgressBar<It>::iterator::operator==(piter<It> other) {
  return val_ == other.val_;
}

template<class It>
inline bool ProgressBar<It>::iterator::operator!=(piter<It> other) {
  return !(*this == other);
}

template<class It>
inline typename It::reference ProgressBar<It>::iterator::operator*() {
  return val_;
}

template<class It>
inline piter<It>& ProgressBar<It>::iterator::operator++() {
  ++(parent_->iter_); 
  val_ = *(parent_->iter_); 
  auto fraction = static_cast<double>(std::distance(parent_->iter_begin_, 
        parent_->iter_))/std::distance(parent_->iter_begin_, parent_->iter_end_);
  parent_->pos_ = parent_->width_*fraction;
  std::cout << *parent_;
  return *this; 
}

template<class It>
inline piter<It> ProgressBar<It>::iterator::operator++(int) {
  auto retval = *this;
  ++(*this);
  return retval;
}

template<class It>
inline piter<It> ProgressBar<It>::begin() {
  return ProgressBar<It>::iterator(this, *iter_begin_);
}

template<class It>
inline piter<It> ProgressBar<It>::end() {
  return ProgressBar<It>::iterator(this, *iter_end_);
}


template<class It>
inline std::ostream& operator<<(std::ostream &stream, const ProgressBar<It> &pbar) {
  stream << pbar.left_delim_;
  for (int i=0; i<pbar.width_; i++) {
    if (i < pbar.pos_)
      stream << pbar.symbol_;
    else if (i == pbar.pos_)
      stream << pbar.pointer_;
    else
      stream << " ";
  }
  stream << pbar.right_delim_ << int(double(pbar.pos_)/pbar.width_*100) << "%\r";
  stream.flush();
  return stream;
}

}; // namespace pbar

#endif // __PBAR_H
