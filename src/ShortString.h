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
class BasicShortString
{
public:
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

    inline constexpr auto capacity()const noexcept{return len - 1;}
    inline constexpr auto size()const noexcept{return capacity() - buf[len - 1];}
    inline constexpr auto length()const noexcept{return size();}
    inline constexpr auto empty()const noexcept{return buf[0] == '\0';}

    inline constexpr auto begin()const noexcept{return std::begin(buf);}
    inline constexpr auto end()const noexcept{return std::begin(buf) + size();}
    inline constexpr auto begin() noexcept{return std::begin(buf);}
    inline constexpr auto end() noexcept{return std::begin(buf) + size();}

    inline constexpr auto cbegin()const noexcept{return std::cbegin(buf);}
    inline constexpr auto cend()const noexcept{return std::cbegin(buf) + size();}

    inline constexpr auto rbegin()const noexcept{return std::rend(buf) - size();}
    inline constexpr auto rend()const noexcept{return std::rend(buf);}
    inline constexpr auto rbegin() noexcept{return std::rend(buf) - size();}
    inline constexpr auto rend() noexcept{return std::rend(buf);}

    inline constexpr auto crbegin()const noexcept{return std::crend(buf) - size();}
    inline constexpr auto crend()const noexcept{return std::crend(buf);}

    inline constexpr operator char*() noexcept{return buf;}
    inline constexpr operator const char*()const noexcept{return buf;}

    //Returns a reference to the first element in the container.
    //Calling front on an empty container is undefined behaviour.
    inline constexpr auto front()const noexcept{return *begin();}
    inline constexpr auto front() noexcept{return *begin();}
    //Returns a reference to the last element in the container.
    //Calling back on an empty container causes undefined behavior.
    inline constexpr auto back()const noexcept{return *std::prev(end());}
    inline constexpr auto back() noexcept{return *std::prev(end());}

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
    inline void push_back(const char *const s) noexcept{operator+=(s);}
    inline void append(const char *const s) noexcept{operator+=(s);}
    inline void push_back(const char c) noexcept{operator+=(c);}
private:
    char buf[len];
};
template <int len>
inline constexpr bool operator ==(const BasicShortString<len> &sl, const char *sr) noexcept{
    return !std::strcmp(sl,  sr);
}
template <int lenL, int lenR>
inline constexpr bool operator ==(const BasicShortString<lenL> &sl, const BasicShortString<lenR> &sr) noexcept{
    return !std::strcmp(sl,  sr);
}
template <int lenL, int lenR>
inline constexpr bool operator <(const BasicShortString<lenL> &sl, const BasicShortString<lenR> &sr) noexcept{
    return std::strcmp(sl,  sr) == -1;
}

typedef BasicShortString<8> ShortString;
typedef BasicShortString<16> ShortString16;
static_assert (std::is_trivially_copyable<ShortString>::value);
static_assert (sizeof(ShortString) == 8);
static_assert (sizeof(ShortString16) == 16);

#endif // SHORTSTRING_H
