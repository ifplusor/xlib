#ifndef XLIB_CONTAINER_MAP_SKIPLIST_HPP_
#define XLIB_CONTAINER_MAP_SKIPLIST_HPP_

#include <algorithm>

namespace xlib {

namespace detail {

template <typename T, typename C, typename A, typename LG, bool D>
class sl_impl;

template <typename LIST>
class sl_iterator;

template <typename LIST>
class sl_const_iterator;

}  // namespace detail

template <typename T,
          typename Compare = std::less<T>,
          typename Allocator = std::allocator<T>,
          typename LevelGenerator = detail::skip_list_level_generator<32>,
          bool AllowDuplicates = false>
class skip_list {
 protected:
  typedef typename detail::sl_impl<T, Compare, Allocator, LevelGenerator, AllowDuplicates> impl_type;
  typedef typename impl_type::node_type node_type;

  template <typename T1>
  friend class detail::sl_iterator;
  template <typename T1>
  friend class detail::sl_const_iterator;

 public:
  //======================================================================
  // types

  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename impl_type::size_type size_type;
  typedef typename allocator_type::difference_type difference_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef Compare compare;

  typedef typename detail::sl_iterator<impl_type> iterator;
  typedef typename iterator::const_iterator const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  //======================================================================
  // lifetime management

  explicit skip_list(const Allocator& alloc = Allocator());

  template <class InputIterator>
  skip_list(InputIterator first, InputIterator last, const Allocator& alloc = Allocator());

  skip_list(const skip_list& other);
  skip_list(const skip_list& other, const Allocator& alloc);

  // C++11
  // skip_list(const skip_list &&other);
  // skip_list(const skip_list &&other, const Allocator &alloc);
  // skip_list(std::initializer_list<T> init, const Allocator &alloc = Allocator());

  allocator_type get_allocator() const { return impl.get_allocator(); }

  //======================================================================
  // assignment

  skip_list& operator=(const skip_list& other);
  // C++11 skip_list& operator=(skip_list&& other);

  template <typename InputIterator>
  void assign(InputIterator first, InputIterator last);

  //======================================================================
  // element access

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  //======================================================================
  // iterators

  iterator begin() { return iterator(&impl, impl.front()); }
  const_iterator begin() const { return const_iterator(&impl, impl.front()); }
  const_iterator cbegin() const { return const_iterator(&impl, impl.front()); }

  iterator end() { return iterator(&impl, impl.one_past_end()); }
  const_iterator end() const { return const_iterator(&impl, impl.one_past_end()); }
  const_iterator cend() const { return const_iterator(&impl, impl.one_past_end()); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }

  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

  //======================================================================
  // capacity

  bool empty() const { return impl.size() == 0; }
  size_type size() const { return impl.size(); }
  size_type max_size() const { return impl.get_allocator().max_size(); }

  //======================================================================
  // modifiers

  void clear();

  typedef typename std::pair<iterator, bool> insert_by_value_result;

  insert_by_value_result insert(const value_type& value);
  iterator insert(const_iterator hint, const value_type& value);

  // C++11iterator insert(value_type &&value);
  // C++11iterator insert(const_iterator hint, const value_type &&value);

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last);

  // C++11iterator insert(std::initializer_list<value_type> ilist);
  // C++11emplace

  size_type erase(const value_type& value);
  iterator erase(const_iterator position);
  iterator erase(const_iterator first, const_iterator last);

  void swap(skip_list& other) { impl.swap(other.impl); }

  friend void swap(skip_list& lhs, skip_list& rhs) { lhs.swap(rhs); }

  //======================================================================
  // lookup

  bool contains(const value_type& value) const { return count(value) != 0; }
  size_type count(const value_type& value) const;

  iterator find(const value_type& value);
  const_iterator find(const value_type& value) const;

  //======================================================================
  // other operations

  // std::list has:
  //   * merge
  //   * splice
  //   * remove
  //   * remove_if
  //   * reverse
  //   * unique
  //   * sort

  template <typename STREAM>
  void dump(STREAM& stream) const {
    impl.dump(stream);
  }

 protected:
  impl_type impl;

  iterator to_iterator(node_type* node, const value_type& value) {
    return impl.is_valid(node) && detail::equivalent(node->value, value, impl.less) ? iterator(&impl, node) : end();
  }
  const_iterator to_iterator(const node_type* node, const value_type& value) const {
    return impl.is_valid(node) && detail::equivalent(node->value, value, impl.less) ? const_iterator(&impl, node)
                                                                                    : end();
  }
};

}  // namespace xlib

#endif  // XLIB_CONTAINER_MAP_SKIPLIST_HPP_
