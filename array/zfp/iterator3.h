// forward const iterator that visits 3D array or view block by block; this class is nested within container_type
class const_iterator : public const_handle {
public:
  // typedefs for STL compatibility
  typedef container_type::value_type value_type;
  typedef ptrdiff_t difference_type;
  typedef container_type::reference reference;
  typedef container_type::pointer pointer;
  typedef std::forward_iterator_tag iterator_category;

  typedef container_type::const_reference const_reference;
  typedef container_type::const_pointer const_pointer;

  // default constructor
  const_iterator() : const_handle(0, 0, 0, 0) {}

  // constructor
  explicit const_iterator(container_type* container, uint i, uint j, uint k) : const_handle(container, i, j, k) {}

  // dereference iterator
  const_reference operator*() const { return const_reference(container, i(), j(), k()); }

  // equality operators
  bool operator==(const const_iterator& it) const { return container == it.container && i() == it.i() && j() == it.j() && k() == it.k(); }
  bool operator!=(const const_iterator& it) const { return !operator==(it); }

  // increment
  const_iterator& operator++() { increment(); return *this; }
  const_iterator operator++(int) { const_iterator it = *this; increment(); return it; }

  // container indices of value referenced by iterator
  uint i() const { return const_handle::i; }
  uint j() const { return const_handle::j; }
  uint k() const { return const_handle::k; }

protected:
  void increment()
  {
    ++const_handle::i;
    if (!(const_handle::i & 3u) || const_handle::i == container->size_x()) {
      const_handle::i = (const_handle::i - 1) & ~3u;
      ++const_handle::j;
      if (!(const_handle::j & 3u) || const_handle::j == container->size_y()) {
        const_handle::j = (const_handle::j - 1) & ~3u;
        ++const_handle::k;
        if (!(const_handle::k & 3u) || const_handle::k == container->size_z()) {
          const_handle::k = (const_handle::k - 1) & ~3u;
          // done with block; advance to next
          const_handle::i += 4;
          if (const_handle::i >= container->size_x()) {
            const_handle::i = 0;
            const_handle::j += 4;
            if (const_handle::j >= container->size_y()) {
              const_handle::j = 0;
              const_handle::k += 4;
              if (const_handle::k >= container->size_z())
                const_handle::k = container->size_z();
            }
          }
        }
      }
    }
  }

  using const_handle::container;
};

// forward iterator that visits 3D array or view block by block; this class is nested within container_type
class iterator : public const_iterator {
public:
  // typedefs for STL compatibility
  typedef container_type::value_type value_type;
  typedef ptrdiff_t difference_type;
  typedef container_type::reference reference;
  typedef container_type::pointer pointer;
  typedef std::forward_iterator_tag iterator_category;

  // default constructor
  iterator() : const_iterator(0, 0, 0, 0) {}

  // constructor
  explicit iterator(container_type* container, uint i, uint j, uint k) : const_iterator(container, i, j, k) {}

  // dereference iterator
  reference operator*() const { return reference(container, i(), j(), k()); }

  // equality operators
  bool operator==(const iterator& it) const { return container == it.container && i() == it.i() && j() == it.j() && k() == it.k(); }
  bool operator!=(const iterator& it) const { return !operator==(it); }

  // increment
  iterator& operator++() { increment(); return *this; }
  iterator operator++(int) { iterator it = *this; increment(); return it; }

  using const_iterator::i;
  using const_iterator::j;
  using const_iterator::k;

protected:
  using const_iterator::increment;
  using const_iterator::container;
};
