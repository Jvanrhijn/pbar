#include <iostream>
#include <iterator>

#ifndef __PBAR_H
#define __PBAR_H

namespace pbar {

template<class It>
class ProgressBar {
public:
  ProgressBar(It& it, int width, const char symbol='=')
      : width_(width), 
        symbol_(symbol), 
        pos_(0), 
        iter_(it.begin()),
        container_(it) 
  {}

  typedef typename It::iterator::value_type value_type;
  typedef typename It::iterator::reference reference;

  class iterator 
    : public std::iterator<typename It::iterator::iterator_category,
                           value_type,
                           typename It::iterator::difference_type,
                           typename It::iterator::pointer,
                           reference>
  {
  private:
    value_type val_ = *iter_;
    ProgressBar<It> *parent_;

  public:
    explicit iterator(ProgressBar<It> *parent, value_type start) 
      : val_(start), parent_(parent) {}

    iterator& operator++() { 
      ++(parent_->iter_); 
      val_ = *(parent_->iter_); 
      double fraction = static_cast<double>(std::distance(parent_->container_.begin(), 
            parent_->iter_))/parent_->container_.size();
      parent_->pos_ = parent_->width_*fraction;
      std::cout << *parent_;
      return *this; 
    }

    iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

    bool operator==(iterator other) { return val_ == other.val_; }
    bool operator!=(iterator other) { return !(*this == other); }
    reference operator*() { return val_; }

  };

  iterator begin() { return iterator(this, *container_.begin()); }
  iterator end() { return iterator(this, *container_.end()); }

  template<class I>
  friend std::ostream& operator<<(std::ostream &steam, const ProgressBar<I> &pbar);

private:
  int pos_;
  int width_;
  char symbol_;
  char left_delim_{'['};
  char right_delim_{']'};
  char pointer_{'>'};

  typename It::iterator iter_;
  It& container_;
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
