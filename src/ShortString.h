#ifndef SHORTSTRING_H
#define SHORTSTRING_H
/**
 * Ilya Yunkin 2020
*/
#include <cstring>
#include <iterator>
#include <assert.h>

// len is the result of sizeof().
// Maximal string lingth is len - 1
template <int len>
struct BasicShortString
{
    char buf[len];
    constexpr BasicShortString() noexcept : buf({'\0'}){
        buf[len - 1] = len - 1;
    }
    constexpr BasicShortString(const char *const s) noexcept{
        buf[len - 1] = len - 1;
        auto outp = s;
        auto inp = std::begin(buf);
        while(*outp != 0 && buf[len - 1]){
            *inp = *outp;
            ++inp;
            ++outp;
            --(buf[len - 1]);
            assert(inp < std::end(buf));
        }
        *inp = '\0';
    };

    constexpr auto capacity()const noexcept{return len - 1;}
    constexpr auto size()const noexcept{return capacity() - buf[len - 1];}
    constexpr auto length()const noexcept{return size();}
    constexpr auto empty()const noexcept{return buf[0] == '\0';}

    constexpr auto begin()const noexcept{return std::begin(buf);}
    constexpr auto end()const noexcept{return std::begin(buf) + size();}
    constexpr auto begin() noexcept{return std::begin(buf);}
    constexpr auto end() noexcept{return std::begin(buf) + size();}

    constexpr auto cbegin()const noexcept{return std::cbegin(buf);}
    constexpr auto cend()const noexcept{return std::cbegin(buf) + size();}

    constexpr auto rbegin()const noexcept{return std::rend(buf) - size();}
    constexpr auto rend()const noexcept{return std::rend(buf);}
    constexpr auto rbegin() noexcept{return std::rend(buf) - size();}
    constexpr auto rend() noexcept{return std::rend(buf);}

    constexpr auto crbegin()const noexcept{return std::crend(buf) - size();}
    constexpr auto crend()const noexcept{return std::crend(buf);}

    constexpr operator char*() noexcept{return buf;}
    constexpr operator const char*()const noexcept{return buf;}

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
        auto inp = std::begin(buf);
        while(*outp != 0 && buf[len - 1]){
            *inp = *outp;
            ++inp;
            ++outp;
            --(buf[len - 1]);
        }
        *inp = '\0';
        assert(inp < std::end(buf));
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
        assert(inp < std::end(buf));
    };
    void operator+=(const char c) noexcept{
        auto inp = end();
        if(buf[len - 1]){
            *inp = c;
            ++inp;
            --(buf[len - 1]);
        }
        *inp = '\0';
        assert(inp < std::end(buf));
    };
    void push_back(const char *const s) noexcept{operator+=(s);}
    void append(const char *const s) noexcept{operator+=(s);}
    void push_back(const char c) noexcept{operator+=(c);}
    constexpr bool operator ==(const BasicShortString &sr) noexcept
    {
        return !std::strcmp(buf,  sr.buf);
    }
    constexpr bool operator <(const BasicShortString &sr) noexcept
    {
        return std::strcmp(buf,  sr.buf) == -1;
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
