#ifndef SHORTSTRING_H
#define SHORTSTRING_H
/**
 * Ilya Yunkin 2020
*/
#include <array>
#include <cstring>
#include <assert.h>

// len is the result of sizeof().
// Maximal string lingth is len - 1
template <int len>
struct BasicShortString
{
    std::array<char, len> buf;
    constexpr BasicShortString() noexcept : buf({'\0'}){
        buf[len - 1] = len - 1;
    }
    constexpr BasicShortString(const char *const s) noexcept{
        buf[len - 1] = len - 1;
        auto outp = s;
        auto inp = buf.begin();
        while(*outp != 0 && buf[len - 1]){
            *inp = *outp;
            ++inp;
            ++outp;
            --(buf[len - 1]);
            assert(inp < buf.end());
        }
        *inp = '\0';
    };

    constexpr auto capacity()const noexcept{return len - 1;}
    constexpr auto size()const noexcept{return capacity() - buf[len - 1];}
    constexpr auto length()const noexcept{return size();}
    constexpr auto empty()const noexcept{return buf[0] == '\0';}

    constexpr auto begin()const noexcept{return buf.begin();}
    constexpr auto end()const noexcept{return buf.begin() + size();}
    constexpr auto begin() noexcept{return buf.begin();}
    constexpr auto end() noexcept{return buf.begin() + size();}

    constexpr auto cbegin()const noexcept{return buf.cbegin();}
    constexpr auto cend()const noexcept{return buf.cbegin() + size();}

    constexpr auto rbegin()const noexcept{return buf.rend() - size();}
    constexpr auto rend()const noexcept{return buf.rend();}
    constexpr auto rbegin() noexcept{return buf.rend() - size();}
    constexpr auto rend() noexcept{return buf.rend();}

    constexpr auto crbegin()const noexcept{return buf.crend() - size();}
    constexpr auto crend()const noexcept{return buf.crend();}

    constexpr operator char*() noexcept{return buf.data();}
    constexpr operator const char*()const noexcept{return buf.data();}

    //Returns a reference to the first element in the container.
    //Calling front on an empty container is undefined behaviour.
    constexpr auto front()const noexcept{return *begin();}
    constexpr auto front() noexcept{return *begin();}
    //Returns a reference to the last element in the container.
    //Calling back on an empty container causes undefined behavior.
    constexpr auto back()const noexcept{return *std::prev(end());}
    constexpr auto back() noexcept{return *std::prev(end());}

    void operator=(const char *const s) noexcept{
        buf[len - 1] = len - 1;
        auto outp = s;
        auto inp = buf.begin();
        while(*outp != 0 && buf[len - 1]){
            *inp = *outp;
            ++inp;
            ++outp;
            --(buf[len - 1]);
        }
        *inp = '\0';
        assert(inp < buf.end());
    };
    void operator+=(const char *const s) noexcept{
        auto outp = s;
        auto inp = end();
        while(*outp != 0 && buf[len - 1]){
            *inp = *outp;
            ++inp;
            ++outp;
            --(buf[len - 1]);
        }
        *inp = '\0';
        assert(inp < buf.end());
    };
    void operator+=(const char c) noexcept{
        auto inp = end();
        if(buf[len - 1]){
            *inp = c;
            ++inp;
            --(buf[len - 1]);
        }
        *inp = '\0';
        assert(inp < buf.end());
    };
    void push_back(const char *const s) noexcept{operator+=(s);}
    void append(const char *const s) noexcept{operator+=(s);}
    void push_back(const char c) noexcept{operator+=(c);}
    constexpr bool operator ==(const BasicShortString &sr) noexcept
    {
        return !std::strcmp(buf.data(),  sr.buf.data());
    }
    constexpr bool operator <(const BasicShortString &sr) noexcept
    {
        return std::strcmp(buf.data(),  sr.buf.data()) == -1;
    }
};
template <int len>
constexpr bool operator ==(const BasicShortString<len> &sl, const char *sr) noexcept
{
    return !std::strcmp(sl,  sr);
}

typedef BasicShortString<8> ShortString;
static_assert (std::is_trivially_copyable<ShortString>::value);
static_assert (sizeof(ShortString) == 8);

#endif // SHORTSTRING_H
