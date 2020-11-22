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
    constexpr BasicShortString() : buf({'\0'}){}
    constexpr BasicShortString(const char *const s){
        auto outp = s;
        auto inp = buf.begin();
        while(*outp != 0 && (inp - buf.begin())
              < static_cast<int>(buf.size() - 1)){
            *inp = *outp;
            ++inp;
            ++outp;
            assert(inp < buf.end());
        }
        *inp = '\0';
    };

    constexpr auto capacity()const{return len - 1;}
    constexpr auto size()const{return std::strlen(buf.data());}
    constexpr auto length()const{return std::strlen(buf.data());}
    constexpr auto empty()const{return buf[0] == '\0';}

    constexpr auto begin()const{return buf.begin();}
    constexpr auto end()const{return buf.begin() + size();}
    constexpr auto begin(){return buf.begin();}
    constexpr auto end(){return buf.begin() + size();}

    constexpr auto cbegin()const{return buf.cbegin();}
    constexpr auto cend()const{return buf.cbegin() + size();}

    constexpr auto rbegin()const{return buf.rend() - size();}
    constexpr auto rend()const{return buf.rend();}
    constexpr auto rbegin(){return buf.rend() - size();}
    constexpr auto rend(){return buf.rend();}

    constexpr auto crbegin()const{return buf.crend() - size();}
    constexpr auto crend()const{return buf.crend();}

    constexpr operator char*(){return buf.data();}
    constexpr operator const char*()const{return buf.data();}

    //Returns a reference to the first element in the container.
    //Calling front on an empty container is undefined behaviour.
    constexpr auto front()const{return *begin();}
    constexpr auto front(){return *begin();}
    //Returns a reference to the last element in the container.
    //Calling back on an empty container causes undefined behavior.
    constexpr auto back()const{return *std::prev(end());}
    constexpr auto back(){return *std::prev(end());}

    void operator=(const char *const s){
        auto outp = s;
        auto inp = buf.begin();
        while(*outp != 0 && (inp - buf.begin())
              < static_cast<int>(buf.size() - 1)){
            *inp = *outp;
            ++inp;
            ++outp;
        }
        *inp = '\0';
        assert(inp < buf.end());
    };
    void operator+=(const char *const s){
        auto outp = s;
        auto inp = end();
        while(*outp != 0 && (inp - buf.begin())
              < static_cast<int>(buf.size() - 1)){
            *inp = *outp;
            ++inp;
            ++outp;
        }
        *inp = '\0';
        assert(inp < buf.end());
    };
    void push_back(const char *const s){operator+=(s);}
};
template <int len>
constexpr bool operator ==(const BasicShortString<len> &sl, const char *sr)
{
    return !std::strcmp(sl,  sr);
}

typedef BasicShortString<8> ShortString;
static_assert (std::is_trivially_copyable<ShortString>::value);

#endif // SHORTSTRING_H
