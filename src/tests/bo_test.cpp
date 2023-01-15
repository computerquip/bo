#include <bo/endian.hpp>
#include <cstdint>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Any 8-bit endian conversion returns same value", "[8bit]")
{
    std::uint8_t v = 0x01;

    bo::swap_inplace(v);
    REQUIRE(v == 0x01);
    REQUIRE(bo::swap(v) == 0x01);
    REQUIRE(bo::hton(v) == 0x01);
    REQUIRE(bo::ntoh(v) == 0x01);
    REQUIRE(bo::betoh(v) == 0x01);
    REQUIRE(bo::htobe(v) == 0x01);
    REQUIRE(bo::htole(v) == 0x01);
    REQUIRE(bo::letoh(v) == 0x01);
    REQUIRE(bo::native_to_be(v) == 0x01);
    REQUIRE(bo::be_to_native(v) == 0x01);
    REQUIRE(bo::native_to_le(v) == 0x01);
    REQUIRE(bo::le_to_native(v) == 0x01);
    REQUIRE(bo::be_to_le(v) == 0x01);
    REQUIRE(bo::le_to_be(v) == 0x01);
}

TEST_CASE("8-bit in-place makes no modification", "[8bit]")
{
    std::uint8_t v = 0x01;

    bo::le_to_native_inplace(v);
    REQUIRE(v == 0x01);

    bo::native_to_le_inplace(v);
    REQUIRE(v == 0x01);

    bo::be_to_native_inplace(v);
    REQUIRE(v == 0x01);

    bo::native_to_be_inplace(v);
    REQUIRE(v == 0x01);
}

TEST_CASE("8-bit buffer returns same values", "[8bit]")
{
    std::uint8_t v = 0x01;

    SECTION("little-endian works") {
        auto buf = bo::make_le_buf(v);
        REQUIRE(buf.size() == 1);
        REQUIRE(buf.value() == 0x01);
        REQUIRE(buf.data()[0] == 0x01);
        REQUIRE(buf[0] == 0x01);
    }

    SECTION("big-endian works") {
        auto buf = bo::make_be_buf(v);
        REQUIRE(buf.size() == 1);
        REQUIRE(buf.value() == 0x01);
        REQUIRE(buf[0] == 0x01);
    }
}

/* Keep in mind that when you call something like
 * "native_to_le", it's assuming that the integer
 * you're providing is in native endianess. It can't
 * actually tell that it is. So functions like
 * native_to_be and be_to_native do literally the
 * exact same thing, just with different names. */
TEST_CASE("16-bit integer endian conversion works", "[16bit]")
{
    std::uint16_t v = 0x0102;

    SECTION("16-bit endian swap works") {
        REQUIRE(bo::swap(v) == 0x0201);
    }

    SECTION("16-bit endian in-place swap works") {
        bo::swap_inplace(v);
        REQUIRE(v == 0x0201);
    }

    if (bo::order::native == bo::order::little) {
        SECTION("16-bit UNIX-like works") {
            REQUIRE(bo::hton(v) == 0x0201);
            REQUIRE(bo::ntoh(v) == 0x0201);
        }

        SECTION("16-bit BSD-like interface works") {
            REQUIRE(bo::htobe(v) == 0x0201);
            REQUIRE(bo::betoh(v) == 0x0201);
            REQUIRE(bo::htole(v) == 0x0102);
            REQUIRE(bo::letoh(v) == 0x0102);
        }

        SECTION("16-bit bo interface work") {
            REQUIRE(bo::native_to_le(v) == 0x0102);
            REQUIRE(bo::le_to_native(v) == 0x0102);
            REQUIRE(bo::native_to_be(v) == 0x0201);
            REQUIRE(bo::be_to_native(v) == 0x0201);
        }
    }

    if (bo::order::native == bo::order::big) {
        SECTION("16-bit UNIX-like works") {
            REQUIRE(bo::hton(v) == 0x0102);
            REQUIRE(bo::ntoh(v) == 0x0102);
        }

        SECTION("16-bit BSD-like interface works") {
            REQUIRE(bo::htobe(v) == 0x0102);
            REQUIRE(bo::betoh(v) == 0x0102);
            REQUIRE(bo::htole(v) == 0x0201);
            REQUIRE(bo::letoh(v) == 0x0201);
        }

        SECTION("16-bit bo interface work") {
            REQUIRE(bo::native_to_le(v) == 0x0201);
            REQUIRE(bo::le_to_native(v) == 0x0201);
            REQUIRE(bo::native_to_be(v) == 0x0102);
            REQUIRE(bo::be_to_native(v) == 0x0102);
        }
    }
}

TEST_CASE("16-bit integer endian in-place conversion works", "[16bit]")
{
    std::uint16_t v = 0x0102;

    SECTION("16-bit endian swap works") {
        bo::swap_inplace(v);
        REQUIRE(v == 0x0201);
    }

    if (bo::order::native == bo::order::little) {
        SECTION("16-bit bo interface work") {
            bo::native_to_le_inplace(v);
            REQUIRE(v == 0x0102);

            bo::le_to_native_inplace(v);
            REQUIRE(v == 0x0102);

            bo::native_to_be_inplace(v);
            REQUIRE(v == 0x0201);

            bo::be_to_native_inplace(v);
            REQUIRE(v == 0x0102);
        }
    }

    if (bo::order::native == bo::order::big) {
        SECTION("16-bit bo interface work") {
            bo::native_to_le_inplace(v);
            REQUIRE(v == 0x0201);

            bo::le_to_native_inplace(v);
            REQUIRE(v == 0x0102);

            bo::native_to_be_inplace(v);
            REQUIRE(v == 0x0102);

            bo::be_to_native_inplace(v);
            REQUIRE(v == 0x0102);
        }
    }
}

TEST_CASE("16-bit buffer types work", "[16bit]")
{
    std::uint16_t v = 0x0102;

    SECTION("little-endian works") {
        auto buf = bo::make_le_buf(v);

        REQUIRE(buf.size() == 2);
        REQUIRE(buf.data()[0] == 0x02);
        REQUIRE(buf.data()[1] == 0x01);
        REQUIRE(buf[0] == 0x02);
        REQUIRE(buf[1] == 0x01);
        REQUIRE(buf.value() == 0x0102);
    }

    SECTION("little-endian works") {
        auto buf = bo::make_be_buf(v);

        REQUIRE(buf.size() == 2);
        REQUIRE(buf.data()[0] == 0x01);
        REQUIRE(buf.data()[1] == 0x02);
        REQUIRE(buf[0] == 0x01);
        REQUIRE(buf[1] == 0x02);
        REQUIRE(buf.value() == 0x0102);
    }
}

TEST_CASE("32-bit integer endian conversion works", "[32bit]")
{
    std::uint32_t v = 0x01020304;

    SECTION("32-bit endian swap works") {
        REQUIRE(bo::swap(v) == 0x04030201);
    }

    if (bo::order::native == bo::order::little) {
        SECTION("16-bit UNIX-like works") {
            REQUIRE(bo::hton(v) == 0x04030201);
            REQUIRE(bo::ntoh(v) == 0x04030201);
        }

        SECTION("16-bit BSD-like interface works") {
            REQUIRE(bo::htobe(v) == 0x04030201);
            REQUIRE(bo::betoh(v) == 0x04030201);
            REQUIRE(bo::htole(v) == 0x01020304);
            REQUIRE(bo::letoh(v) == 0x01020304);
        }

        SECTION("16-bit bo interface work") {
            REQUIRE(bo::native_to_le(v) == 0x01020304);
            REQUIRE(bo::le_to_native(v) == 0x01020304);
            REQUIRE(bo::native_to_be(v) == 0x04030201);
            REQUIRE(bo::be_to_native(v) == 0x04030201);
        }
    }

    if (bo::order::native == bo::order::big) {
        SECTION("16-bit UNIX-like works") {
            REQUIRE(bo::hton(v) == 0x01020304);
            REQUIRE(bo::ntoh(v) == 0x01020304);
        }

        SECTION("16-bit BSD-like interface works") {
            REQUIRE(bo::htobe(v) == 0x01020304);
            REQUIRE(bo::betoh(v) == 0x01020304);
            REQUIRE(bo::htole(v) == 0x04030201);
            REQUIRE(bo::letoh(v) == 0x04030201);
        }

        SECTION("16-bit bo interface work") {
            REQUIRE(bo::native_to_le(v) == 0x04030201);
            REQUIRE(bo::le_to_native(v) == 0x04030201);
            REQUIRE(bo::native_to_be(v) == 0x01020304);
            REQUIRE(bo::be_to_native(v) == 0x01020304);
        }
    }
}

TEST_CASE("32-bit integer endian in-place conversion works", "[32bit]")
{
    std::uint32_t v = 0x01020304;

    SECTION("32-bit endian swap works") {
        bo::swap_inplace(v);
        REQUIRE(v == 0x04030201);
    }

    if (bo::order::native == bo::order::little) {
        SECTION("32-bit bo interface work") {
            bo::native_to_le_inplace(v);
            REQUIRE(v == 0x01020304);

            bo::le_to_native_inplace(v);
            REQUIRE(v == 0x01020304);

            bo::native_to_be_inplace(v);
            REQUIRE(v == 0x04030201);

            bo::be_to_native_inplace(v);
            REQUIRE(v == 0x01020304);
        }
    }

    if (bo::order::native == bo::order::big) {
        SECTION("32-bit bo interface work") {
            bo::native_to_le_inplace(v);
            REQUIRE(v == 0x04030201);

            bo::le_to_native_inplace(v);
            REQUIRE(v == 0x01020304);

            bo::native_to_be_inplace(v);
            REQUIRE(v == 0x01020304);

            bo::be_to_native_inplace(v);
            REQUIRE(v == 0x01020304);
        }
    }
}

TEST_CASE("32-bit buffer types work", "[32bit]")
{
    std::uint32_t v = 0x01020304;

    SECTION("little-endian works") {
        auto buf = bo::make_le_buf(v);

        REQUIRE(buf.size() == 4);
        REQUIRE(buf.data()[0] == 0x04);
        REQUIRE(buf.data()[1] == 0x03);
        REQUIRE(buf.data()[2] == 0x02);
        REQUIRE(buf.data()[3] == 0x01);
        REQUIRE(buf[0] ==  0x04);
        REQUIRE(buf[1] ==  0x03);
        REQUIRE(buf[2] ==  0x02);
        REQUIRE(buf[3] ==  0x01);
        REQUIRE(buf.value() == 0x01020304);
    }

    SECTION("little-endian works") {
        auto buf = bo::make_be_buf(v);

        REQUIRE(buf.size() == 4);
        REQUIRE(buf.data()[0] == 0x01);
        REQUIRE(buf.data()[1] == 0x02);
        REQUIRE(buf.data()[2] == 0x03);
        REQUIRE(buf.data()[3] == 0x04);
        REQUIRE(buf[0] == 0x01);
        REQUIRE(buf[1] == 0x02);
        REQUIRE(buf[2] == 0x03);
        REQUIRE(buf[3] == 0x04);
        REQUIRE(buf.value() == 0x01020304);
    }
}

TEST_CASE("64-bit integer endian conversion works", "[64bit]")
{
    std::uint64_t v = 0x0102030405060708;

    SECTION("32-bit endian swap works") {
        REQUIRE(bo::swap(v) == 0x0807060504030201);
    }

    if (bo::order::native == bo::order::little) {
        SECTION("16-bit UNIX-like works") {
            REQUIRE(bo::hton(v) == 0x0807060504030201);
            REQUIRE(bo::ntoh(v) == 0x0807060504030201);
        }

        SECTION("16-bit BSD-like interface works") {
            REQUIRE(bo::htobe(v) == 0x0807060504030201);
            REQUIRE(bo::betoh(v) == 0x0807060504030201);
            REQUIRE(bo::htole(v) == 0x0102030405060708);
            REQUIRE(bo::letoh(v) == 0x0102030405060708);
        }

        SECTION("16-bit bo interface work") {
            REQUIRE(bo::native_to_le(v) == 0x0102030405060708);
            REQUIRE(bo::le_to_native(v) == 0x0102030405060708);
            REQUIRE(bo::native_to_be(v) == 0x0807060504030201);
            REQUIRE(bo::be_to_native(v) == 0x0807060504030201);
        }
    }

    if (bo::order::native == bo::order::big) {
        SECTION("16-bit UNIX-like works") {
            REQUIRE(bo::hton(v) == 0x0102030405060708);
            REQUIRE(bo::ntoh(v) == 0x0102030405060708);
        }

        SECTION("16-bit BSD-like interface works") {
            REQUIRE(bo::htobe(v) == 0x0102030405060708);
            REQUIRE(bo::betoh(v) == 0x0102030405060708);
            REQUIRE(bo::htole(v) == 0x0807060504030201);
            REQUIRE(bo::letoh(v) == 0x0807060504030201);
        }

        SECTION("16-bit bo interface work") {
            REQUIRE(bo::native_to_le(v) == 0x0807060504030201);
            REQUIRE(bo::le_to_native(v) == 0x0807060504030201);
            REQUIRE(bo::native_to_be(v) == 0x0102030405060708);
            REQUIRE(bo::be_to_native(v) == 0x0102030405060708);
        }
    }
}

TEST_CASE("64-bit integer endian in-place conversion works", "[64bit]")
{
    std::uint64_t v = 0x0102030405060708;

    SECTION("64-bit endian swap works") {
        bo::swap_inplace(v);
        REQUIRE(v == 0x0807060504030201);
    }

    if (bo::order::native == bo::order::little) {
        SECTION("32-bit bo interface work") {
            bo::native_to_le_inplace(v);
            REQUIRE(v == 0x0102030405060708);

            bo::le_to_native_inplace(v);
            REQUIRE(v == 0x0102030405060708);

            bo::native_to_be_inplace(v);
            REQUIRE(v == 0x0807060504030201);

            bo::be_to_native_inplace(v);
            REQUIRE(v == 0x0102030405060708);
        }
    }

    if (bo::order::native == bo::order::big) {
        SECTION("32-bit bo interface work") {
            bo::native_to_le_inplace(v);
            REQUIRE(v == 0x0807060504030201);

            bo::le_to_native_inplace(v);
            REQUIRE(v == 0x0102030405060708);

            bo::native_to_be_inplace(v);
            REQUIRE(v == 0x0102030405060708);

            bo::be_to_native_inplace(v);
            REQUIRE(v == 0x0102030405060708);
        }
    }
}

TEST_CASE("64-bit buffer types work", "[64bit]")
{
    std::uint64_t v = 0x0102030405060708;

    SECTION("little-endian works") {
        auto buf = bo::make_le_buf(v);

        REQUIRE(buf.size() == 8);
        REQUIRE(buf.data()[0] == 0x08);
        REQUIRE(buf.data()[1] == 0x07);
        REQUIRE(buf.data()[2] == 0x06);
        REQUIRE(buf.data()[3] == 0x05);
        REQUIRE(buf.data()[4] == 0x04);
        REQUIRE(buf.data()[5] == 0x03);
        REQUIRE(buf.data()[6] == 0x02);
        REQUIRE(buf.data()[7] == 0x01);
        REQUIRE(buf[0] == 0x08);
        REQUIRE(buf[1] == 0x07);
        REQUIRE(buf[2] == 0x06);
        REQUIRE(buf[3] == 0x05);
        REQUIRE(buf[4] == 0x04);
        REQUIRE(buf[5] == 0x03);
        REQUIRE(buf[6] == 0x02);
        REQUIRE(buf[7] == 0x01);
        REQUIRE(buf.value() == 0x0102030405060708);
    }

    SECTION("little-endian works") {
        auto buf = bo::make_be_buf(v);

        REQUIRE(buf.size() == 8);
        REQUIRE(buf.data()[0] == 0x01);
        REQUIRE(buf.data()[1] == 0x02);
        REQUIRE(buf.data()[2] == 0x03);
        REQUIRE(buf.data()[3] == 0x04);
        REQUIRE(buf.data()[4] == 0x05);
        REQUIRE(buf.data()[5] == 0x06);
        REQUIRE(buf.data()[6] == 0x07);
        REQUIRE(buf.data()[7] == 0x08);
        REQUIRE(buf[0] == 0x01);
        REQUIRE(buf[1] == 0x02);
        REQUIRE(buf[2] == 0x03);
        REQUIRE(buf[3] == 0x04);
        REQUIRE(buf[4] == 0x05);
        REQUIRE(buf[5] == 0x06);
        REQUIRE(buf[6] == 0x07);
        REQUIRE(buf[7] == 0x08);
        REQUIRE(buf.value() == 0x0102030405060708);
    }
}