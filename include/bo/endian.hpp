/****************************************************************
 * Copyright 2000 Darin Adler                                   *
 * Copyright 2006, 2009, 2014 Beman Dawes                       *
 * Copyright 2019 Peter Dimov                                   *
 * Copyright 2023 Zachary Lund <admin@computerquip.com>         *
 *                                                              *
 * Distributed under the Boost Software License, Version 1.0.   *
 * http://www.boost.org/LICENSE_1_0.txt                         *
 ****************************************************************/

#include <stdint.h>
#include <string.h>

namespace bo {

enum class order
{
#ifdef _WIN32
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

#ifdef UINT8_MAX
constexpr uint8_t swap(uint8_t x) noexcept
{
    return x;
}
#endif

#ifdef UINT16_MAX
constexpr uint16_t swap(uint16_t x) noexcept
{
    return (x << 8) | (x >> 8);
}
#endif

#ifdef UINT32_MAX
constexpr uint32_t swap(uint32_t x) noexcept
{
    uint32_t step16 = x << 16 | x >> 16;
    return ((step16 << 8) & 0xff00ff00) | ((step16 >> 8) & 0x00ff00ff);
}
#endif

#ifdef UINT64_MAX
constexpr uint64_t swap(uint64_t x) noexcept
{
    const uint64_t step32 = x << 32 | x >> 32;
    const uint64_t step16 =
        (step32 & 0x0000ffff0000ffffull) << 16 |
        (step32 & 0xffff0000ffff0000ull) >> 16;
    return (step16 & 0x00ff00ff00ff00ffull) << 8 | (step16 & 0xff00ff00ff00ff00ull) >> 8;
}
#endif

template <class E>
constexpr void swap_inplace(E &e) noexcept
{
    e = swap(e);
}

template<order From, order To, class E>
constexpr E convert(E x) noexcept
{
    if (From != To) {
        return swap(x);
    } else {
        return x;
    }
}

template<order From, order To, class E>
constexpr void convert_inplace(E &x) noexcept
{
    if (From != To) {
        x = swap(x);
    }
}

template<order From, order To, class E>
constexpr void store(unsigned char *buf, E x) noexcept
{
    convert_inplace<From, To>(x);
    ::memcpy(buf, reinterpret_cast<unsigned char*>(&x), sizeof x);
}

template<order From, order To, class E>
constexpr E load(const unsigned char *buf) noexcept
{
    E x = 0;
    ::memcpy(reinterpret_cast<unsigned char*>(&x), buf, sizeof x);
    return convert<From, To>(x);
}

template<class E>
constexpr auto native_to_le(E e)
{
    return convert<order::native, order::little, E>(e);
}


template<class E>
constexpr E native_to_be(E e)
{
    return convert<order::native, order::big, E>(e);
}

template<class E>
constexpr E le_to_native(E e)
{
    return convert<order::little, order::native, E>(e);
}

template<class E>
constexpr E be_to_native(E e)
{
    return convert<order::big, order::native, E>(e);
}

template<class E>
constexpr E be_to_le(E e)
{
    return convert<order::big, order::little, E>(e);
}

template<class E>
constexpr E le_to_be(E e)
{
    return convert<order::little, order::big, E>(e);
}


template<class E>
constexpr void native_to_le_inplace(E &e)
{
    convert_inplace<order::native, order::little, E>(e);
}

template<class E>
constexpr void native_to_be_inplace(E &e)
{
    convert_inplace<order::native, order::big, E>(e);
}

template<class E>
constexpr void le_to_native_inplace(E &e)
{
    convert_inplace<order::little, order::native, E>(e);
}

template<class E>
constexpr void be_to_native_inplace(E &e)
{
    convert_inplace<order::big, order::native, E>(e);
}

template<class E>
constexpr void be_to_le_inplace(E &e)
{
    convert_inplace<order::big, order::little, E>(e);
}

template<class E>
constexpr void le_to_be_inplace(E &e)
{
    convert_inplace<order::little, order::big, E>(e);
}

template<class E>
constexpr E hton(E e)
{
    return native_to_be<E>(e);
}

template<class E>
constexpr E ntoh(E e)
{
    return be_to_native<E>(e);
}

template<class E>
constexpr E htobe(E e)
{
    return hton<E>(e);
}

template<class E>
constexpr E htole(E e)
{
    return native_to_le<E>(e);
}

template<class E>
constexpr E betoh(E e)
{
    return ntoh<E>(e);
}

template<class E>
constexpr E letoh(E e)
{
    return le_to_native<E>(e);
}

template <order ValueOrder, order StoreOrder, typename ValueType>
class buf
{
public:
    using value_type = ValueType;

    constexpr buf() noexcept = default;
    constexpr explicit buf(ValueType value) noexcept
    {
        store<ValueOrder, StoreOrder>(m_buf, value);
    }

    constexpr buf(buf &&) = default;
    constexpr buf(buf &)  = default;
    constexpr buf &operator=(buf &&) = default;
    constexpr buf &operator=(buf &)  = default;
    ~buf() = default;

    constexpr buf &operator=(value_type value)
    {
        store<ValueOrder, StoreOrder>(m_buf, value);
        return *this;
    }

    constexpr value_type value() const noexcept
    {
        return load<StoreOrder, ValueOrder, value_type>(m_buf);
    }

    constexpr unsigned char *data() noexcept
    {
        return m_buf;
    }

    constexpr unsigned char const *data() const noexcept
    {
        return m_buf;
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(value_type);
    }

    constexpr unsigned char &operator[](size_t idx)
    {
        return m_buf[idx];
    }

    constexpr unsigned char &operator[](size_t idx) const
    {
        return m_buf[idx];
    }

private:
    unsigned char m_buf[sizeof(value_type)];
};

template <typename E>
using le_buf = buf<order::native, order::little, E>;

template <typename E>
using be_buf = buf<order::native, order::big, E>;

template<class E>
constexpr le_buf<E> make_le_buf(E e)
{
    return le_buf<E>(e);
}

template<class E>
constexpr be_buf<E> make_be_buf(E e)
{
    return be_buf<E>(e);
}

}
