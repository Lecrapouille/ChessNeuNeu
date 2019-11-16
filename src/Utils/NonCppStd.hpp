//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of ChessNeuNeu.
//
// ChessNeuNeu is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef CHESSNEUNEU_NONCPPSTD_HPP
#  define CHESSNEUNEU_NONCPPSTD_HPP

#  include <cstddef>
#  include <memory>

#  if defined(__GNUC__)
#    define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#  elif defined(__clang__)
#    define CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#  endif

// *****************************************************************************
//! \brief Make the class and its derived class non copyable by deleting methods
//! such as copy constructor, operator=().
//!
//! Inspired by http://www.boost.org/doc/libs/1_66_0/boost/core/noncopyable.hpp
// *****************************************************************************
class NonCopyable
{
protected:

  constexpr NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(const NonCopyable&) = delete;
  const NonCopyable& operator=(const NonCopyable&) = delete;
};

// *****************************************************************************
//! \brief Allows to create literal values of type std::size_t In the same way
//! than U, LL or UL macros.
//!
//! Indeed size_t can be uint32_t or uint64_t depending on the architecture.
//! \code
//! size_t i = 42_z;
//! \endcode
// *****************************************************************************
constexpr std::size_t operator "" _z (unsigned long long const n)
{
  return static_cast<std::size_t>(n);
}

// *****************************************************************************
// Implementation of C++14's make_unique for C++11 compilers.
// *****************************************************************************
#if !                                                                   \
  ((defined(_MSC_VER) && (_MSC_VER >= 1800)) ||                         \
   (defined(__APPLE__) && (CLANG_VERSION >= 60000)) ||                  \
   ((!defined(__APPLE__)) && (CLANG_VERSION >= 30400)) ||               \
   (defined(__GNUC__) && (GCC_VERSION >= 40900) && (__cplusplus > 201103L)))

namespace std
{
  //! \brief Implement the C++14 std::make_unique for C++11
  template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}
#endif

// *****************************************************************************
//! \brief Return the number of elements in an array.
//! \tparam S for the size of the array.
//! \tparam T for the type of data.
// *****************************************************************************
template<size_t S, typename T>
inline size_t ARRAY_SIZE(T (&)[S])
{
  return S;
}

// *****************************************************************************
//! \brief Forward declaration of classes with shared and unique
//! pointers:
//! - Class
//! - ClassSP: shared_pointer<Class>
//! - classUP: unique_pointer<Class>
// *****************************************************************************
#define DECLARE_CLASS(TypeName)                      \
    class TypeName;                                  \
    using TypeName##_SP = std::shared_ptr<TypeName>; \
    using TypeName##_UP = std::unique_ptr<TypeName>

// *****************************************************************************
//! \brief One of the most used optimization used in Linux kernel. When
//! working with conditional code (if-else statements), we often know
//! which branch is true and which is not. If compiler knows this
//! information in advance, it can generate most optimized code.
// *****************************************************************************
#  ifndef likely
#    define likely(x)       __builtin_expect(!!(x),1)
#  endif
#  ifndef unlikely
#    define unlikely(x)     __builtin_expect(!!(x),0)
# endif

#  ifndef NORETURN
#    if __GNUC__ > 2 || defined(__INTEL_COMPILER)
#      define NORETURN __attribute__((__noreturn__))
#    else
#      define NORETURN
#    endif
#  endif

#endif // CHESSNEUNEU_NONCPPSTD_HPP
