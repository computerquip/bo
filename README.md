# What is this?
This is a small endianess library for C++14 and up. It is heavily inspired by Boost.Endian, enough so that I appended the copyright list with associated names.

# Why not just use Boost.Endian?
Boost.Endian is a fairly large library. I haven't used about 75% of it ever. It requires Boost headers meaning it's difficult to separate it from Boost itself. I actually made a [modified version of Boost.Endian that used the standard library](https://github.com/computerquip/endian) but it was still a bit much too lug around.

# When should I use Boost.Endian over this?
 - If you already rely on Boost.
 - If you need to support niche or older compilers (i.e. C++11 or even older in some cases).
 - If you have so many symbols and macros in your codebase that this library is causing symbol conflict somehow.
 - See differences for things we don't implement.

In otherwords, Boost.Endian is the crufty commercial-grade version of this library.

# Notes

## Boost.Endian is large because of C++11
C++11 was a huge leap but it had problems. For just some examples that affect Boost.Endian:
- constexpr functions were only allowed a single return statement and no other statements.
- It didn't allow `void` as a return type for constexpr meaning it had to be treated specially for in-place functions.
- `const` member functions couldn't be overloaded by non-const member functions.
- C++11 didn't allow variable template aliases.
- C++11 compilers had misleading and sometimes incorrect warnings concerning forward-compatibility with C++14 and beyond.

Simply requiring a minimum of C++14 reduces the code by a *lot*.

## Differences from Boost.Endian
 - Logic that deals with types that aren't a power of 2 aren't done here (i.e. no 24-bit buffer type).
 - `bo::buf` requires the value endianess be specified as a template argument.
 - Types for specific bit-widths won't be implemented, only the generic templatized types/functions will be done.
 - Requires at least C++14.
 - Provides convenience functions similar to what's in POSIX (e.g. bo::hton and bo::ntoh).
 - Provides convenience functions similar to what's in BSDs (e.g. bo::htobe and bo::betoh).
 - We rely on the compiler to generate decent assembly.
 - Relies only on memcpy from the CRT.

## This doesn't support middle-endian

## Minimum supported compilers
 - clang 3.5 or higher
 - gcc 5.1 or higher
 - djgpp 5.2 or higher
 - icc 17.0.0 or higher
 - Visual Studio 2017 (but earlier versions may work as well)