//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory
//
// Created by David Beckingsale, david@llnl.gov
// LLNL-CODE-747640
//
// All rights reserved.
//
// This file is part of Umpire.
//
// For details, see https://github.com/LLNL/Umpire
// Please also see the LICENSE file for MIT license.
//////////////////////////////////////////////////////////////////////////////

#include "umpire/util/Macros.hpp"
#include "umpire/strategy/MixedPool.hpp"

namespace umpire {
namespace strategy {

namespace {

template<int FirstFixed, int Current, int LastFixed, int Increment>
struct make_fixed_pool_array {
  template<typename Array>
  static void eval(Array& fixed_pool, Allocator& allocator) {
    const int index = Current - FirstFixed;
    const std::size_t size = 1 << Current;
    std::stringstream ss{"internal_fixed_"};
    ss << size;
    fixed_pool[index] = std::make_shared<FixedPool<unsigned char[size]> >(ss.str(), -1, allocator);
    make_fixed_pool_array<FirstFixed, Current+Increment, LastFixed, Increment>::eval(fixed_pool, allocator);
  }
};

template<int FirstFixed, int LastFixed, int Increment>
struct make_fixed_pool_array<FirstFixed,LastFixed,LastFixed,Increment> {
  template<typename Array>
  static void eval(Array& fixed_pool, Allocator& allocator) {
    const int index = LastFixed - FirstFixed;
    const std::size_t size = 1 << LastFixed;
    std::stringstream ss{"internal_fixed_"};
    ss << size;
    fixed_pool[index] = std::make_shared<FixedPool<unsigned char[size]> >(ss.str(), -1, allocator);
  }
};

} // anonymous namespace

template<int FirstFixed, int Increment, int LastFixed>
size_t MixedPoolImpl<FirstFixed,Increment,LastFixed>::nextPower2(unsigned int n) {
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n++;
  return n;
}

template<int FirstFixed, int Increment, int LastFixed>
MixedPoolImpl<FirstFixed,Increment,LastFixed>::MixedPoolImpl(
    const std::string& name,
    int id,
    Allocator allocator) noexcept
:
  AllocationStrategy(name, id),
  m_allocator(allocator.getAllocationStrategy())

{
  m_dynamic_pool = std::make_shared<DynamicPool>(
      "internal_dynamic_pool",
      -1,
      allocator);

  make_fixed_pool_array<FirstFixed,FirstFixed,LastFixed,Increment>::eval(m_fixed_pool, allocator);
}

union AlignedSize {
  int index;
  std::max_align_t a;
};

template<int FirstFixed, int Increment, int LastFixed>
void* MixedPoolImpl<FirstFixed,Increment,LastFixed>::allocate(size_t bytes)
{
  const size_t bytes_with_index = bytes + sizeof(unsigned int);
  AlignedSize* s = nullptr;
  if (bytes <= (1 << LastFixed)) {
    // Find next power of 2
    const size_t alloc_size = nextPower2(bytes_with_index);

    // Convert that to bit index
    size_t bit = 0;
    {
      size_t size = alloc_size;
      while (size) { size >>= 1; ++bit; }
    }
    // Find index such that alloc_size is equal to size[index] (or, bytes < size[index] and bytes > size[index-1] if that exists)
    int index = 0;
    size_t nearest_bytes = 1 << FirstFixed;
    while (alloc_size > nearest_bytes) { index++; nearest_bytes <<= 1; }

    // Allocate
    s = static_cast<AlignedSize*>(m_fixed_pool[index]->allocate(alloc_size));
    s->index = index;
  }
  else {
    s = static_cast<AlignedSize*>(m_dynamic_pool->allocate(bytes_with_index));
    s->index = -1;
  }
  return ++s;
}

template<int FirstFixed, int Increment, int LastFixed>
void MixedPoolImpl<FirstFixed,Increment,LastFixed>::deallocate(void* ptr)
{
  AlignedSize* s = static_cast<AlignedSize*>(ptr);
  s--;
  const int index = s->index;
  if (index < 0) {
    m_dynamic_pool->deallocate((void *) s);
  }
  else {
    m_fixed_pool[index]->deallocate((void *) s);
  }
}

template<int FirstFixed, int Increment, int LastFixed>
void MixedPoolImpl<FirstFixed,Increment,LastFixed>::release()
{
}

template<int FirstFixed, int Increment, int LastFixed>
long MixedPoolImpl<FirstFixed,Increment,LastFixed>::getCurrentSize() const noexcept
{
  return 0;
}

template<int FirstFixed, int Increment, int LastFixed>
long MixedPoolImpl<FirstFixed,Increment,LastFixed>::getActualSize() const noexcept
{
  return 0;
}

template<int FirstFixed, int Increment, int LastFixed>
long MixedPoolImpl<FirstFixed,Increment,LastFixed>::getHighWatermark() const noexcept
{
  return 0;
}

template<int FirstFixed, int Increment, int LastFixed>
Platform MixedPoolImpl<FirstFixed,Increment,LastFixed>::getPlatform() noexcept
{
  return m_allocator->getPlatform();
}

}
}