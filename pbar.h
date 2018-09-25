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

  class iterator 
    : public std::iterator<typename It::iterator_category,
                           typename It::value_type,
                           typename It::difference_type,
                           typename It::pointer,
                           typename It::reference>
  {
  private:
    typename It::value_type val_ = *iter_;
    ProgressBar<It> *parent_;

  public:
    iterator(ProgressBar<It> *parent, typename It::value_type start) 
      : val_(start), parent_(parent) {}

    iterator& operator++() { 
      ++(parent_->iter_); 
      val_ = *(parent_->iter_); 
      double fraction = static_cast<double>(std::distance(parent_->iter_begin_, 
            parent_->iter_))/std::distance(parent_->iter_begin_, parent_->iter_end_);
      parent_->pos_ = parent_->width_*fraction;
      std::cout << *parent_;
      return *this; 
    }

    iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

    bool operator==(iterator other) { return val_ == other.val_; }
    bool operator!=(iterator other) { return !(*this == other); }
    typename It::reference operator*() { return val_; }

  };

  iterator begin() { return iterator(this, *iter_); }
  iterator end() { return iterator(this, *iter_end_); }

  template<class I>
  friend std::ostream& operator<<(std::ostream &steam, const ProgressBar<I> &pbar);

private:
  int pos_;
  int width_;
  char symbol_;
  char left_delim_{'['};
  char right_delim_{']'};
  char pointer_{'>'};

  //typename It::iterator iter_;
  It iter_;
  It iter_begin_;
  It iter_end_;
}; // class ProgressBar

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
    stream << pbar.right_delim_ << int(float(pbar.pos_)/pbar.width_*100) << "%\r";
    stream.flush();
    return stream;
}

}; // namespace pbar

#endif // __PBAR_H
