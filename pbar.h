#include <iostream>
#include <iterator>

#ifndef __PBAR_H
#define __PBAR_H

namespace pbar {

template<class It>
class ProgressBar {
public:
  ProgressBar(It it, It it_end, size_t width, const char symbol='=')
      : count_(0),
        width_(width), 
        symbol_(symbol), 
        iter_(std::move(it)),
        iter_begin_(iter_),
        iter_end_(std::move(it_end))
  {
    size_ = std::distance(iter_begin_, iter_end_); 
  }

  struct iterator {

    using iterator_category = typename It::iterator_category;
    using value_type = typename It::value_type;
    using difference_type = typename It::difference_type;
    using pointer = typename It::pointer;
    using reference = typename It::reference;

  public:
    iterator(ProgressBar<It>& parent, It& iter) 
      : parent_(parent), iter_(iter) {}

    iterator& operator++();
    iterator operator++(int);

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;

    reference operator*() const;

  private:
    ProgressBar<It>& parent_;
    It& iter_;

  };

  iterator begin();
  iterator end();

  template<class I>
  friend std::ostream& operator<<(std::ostream&, const ProgressBar<I>&);

private:
  size_t count_;
  size_t width_;
  size_t size_;

  char symbol_;
  char left_delim_{'['};
  char right_delim_{']'};
  char pointer_{'>'};

  It iter_;
  It iter_begin_;
  It iter_end_;

}; // class ProgressBar

template<class It>
inline bool ProgressBar<It>::iterator::operator==(
    const ProgressBar<It>::iterator& other) const {
  return iter_ == other.iter_;
}

template<class It>
inline bool ProgressBar<It>::iterator::operator!=(
    const ProgressBar<It>::iterator& other) const {
  return !(*this == other);
}

template<class It>
inline typename It::reference ProgressBar<It>::iterator::operator*() const {
  return *iter_;
}

template<class It>
inline typename ProgressBar<It>::iterator& ProgressBar<It>::iterator::operator++() {
  ++(iter_); 
  ++parent_.count_;
  std::clog << parent_;
  return *this; 
}

template<class It>
inline typename ProgressBar<It>::iterator ProgressBar<It>::iterator::operator++(int) {
  auto retval = *this;
  ++(*this);
  return retval;
}

template<class It>
inline typename ProgressBar<It>::iterator ProgressBar<It>::begin() {
  return ProgressBar<It>::iterator(*this, iter_begin_);
}

template<class It>
inline typename ProgressBar<It>::iterator ProgressBar<It>::end() {
  return ProgressBar<It>::iterator(*this, iter_end_);
}

template<class It>
inline std::ostream& operator<<(std::ostream &stream, const ProgressBar<It> &pbar) {
  size_t pos = pbar.width_*pbar.count_/pbar.size_;
  stream << pbar.left_delim_;
  for (int i=0; i<pbar.width_; i++) {
    if (i < pos)
      stream << pbar.symbol_;
    else if (i == pos)
      stream << pbar.pointer_;
    else
      stream << " ";
  }
  stream << pbar.right_delim_ << pbar.count_*100/pbar.size_ << "%\r";
  stream.flush();
  return stream;
}

}; // namespace pbar

#endif // __PBAR_H

