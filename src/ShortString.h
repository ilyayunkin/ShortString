#ifndef SHORTSTRING_H
#define SHORTSTRING_H
/**
 * Ilya Yunkin 2020
*/
#include <array>
#include <cstring>

template <int len>
struct BasicShortString
{
    std::array<char, len> buf;
    constexpr BasicShortString() : buf({'\0'}){}
    constexpr BasicShortString(const char *const s){
        auto outp = s;
        auto inp = buf.begin();
        while(*outp != 0 && (outp - s)
              < static_cast<int>(buf.size() - 1)){
            *inp = *outp;
            ++inp;
            ++outp;
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

    void operator=(const char *const s){
        auto outp = s;
        auto inp = buf.begin();
        while(*outp != 0 && (outp - s)
              < static_cast<int>(buf.size() - 1)){
            *inp = *outp;
            ++inp;
            ++outp;
        }
        *inp = '\0';
    };
};
template <int len>
constexpr bool operator ==(const BasicShortString<len> &sl, const char *sr)
{
    return !std::strcmp(sl,  sr);
}

typedef BasicShortString<8> ShortString;
static_assert (std::is_trivially_copyable<ShortString>::value);

#endif // SHORTSTRING_H
