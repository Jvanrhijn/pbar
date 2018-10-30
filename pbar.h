#ifndef __PBAR_H
#define __PBAR_H

#include <iostream>
#include <iterator>

namespace pbar {

template<class It>
class ProgressBar {
public:
  ProgressBar(It it, It it_end, size_t width, const char symbol='=')
      : count_(0),
        width_(width), 
        size_(std::distance(it, it_end)),
        symbol_(symbol), 
        it_begin_(*this, std::move(it)),
        it_end_(*this, std::move(it_end))
  {
    size_ = size_? size_ : 1; // to prevent division by zero in notify()
  }

  ~ProgressBar() {
    std::clog << std::endl;
  }

  struct iterator;

  iterator begin() {
    count_ = 0;
    return it_begin_;
  }

  iterator end() {
    return it_end_;
  }

private:
  void notify() const;

  size_t count_;
  size_t width_;
  size_t size_;

  char symbol_;
  char left_delim_{'['};
  char right_delim_{']'};
  char pointer_{'>'};

  iterator it_begin_;
  iterator it_end_;

}; // class ProgressBar


template<class It>
struct ProgressBar<It>::iterator {

  using iterator_category = std::forward_iterator_tag;
  using value_type = typename It::value_type;
  using difference_type = typename It::difference_type;
  using pointer = typename It::pointer;
  using reference = typename It::reference;

public:
  iterator(ProgressBar<It>& parent, It iter) 
    : parent_(parent), iter_(std::move(iter)) {}

  iterator& operator++() {
    ++(iter_); 
    ++parent_.count_;
    parent_.notify();
    return *this; 
  }

  iterator operator++(int) {
    auto retval = *this;
    ++(*this);
    return retval;
  }

  bool operator==(const iterator& other) const {
    return iter_ == other.iter_;
  }
  
  bool operator!=(const iterator& other) const {
    return !(*this == other);
  }

  reference operator*() const {
    return *iter_;
  }

private:
  ProgressBar<It>& parent_;
  It iter_;

};


template<class It>
inline void ProgressBar<It>::notify() const {
  size_t pos_old = width_*(count_-1)/size_;
  size_t pos = width_*count_/size_;
  if (pos_old == pos)
    return;
  std::clog << left_delim_;
  for (size_t i=0; i<width_; i++) {
    if (i < pos)
      std::clog << symbol_;
    else if (i == pos)
      std::clog << pointer_;
    else
      std::clog << " ";
  }
  std::clog << right_delim_ << count_*100/size_ << "%\r";
  std::clog.flush();
}

} // namespace pbar

#endif // __PBAR_H

