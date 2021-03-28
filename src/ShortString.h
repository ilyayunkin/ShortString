#ifndef SHORTSTRING_H
#define SHORTSTRING_H
/**
 * Ilya Yunkin 2020
*/
#include <cstring>
#include <iterator>
#include <limits>
#include <assert.h>
#include <string>

// len is the result of sizeof().
// Maximal string lingth is len - 1
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
class BasicShortString
{
public:
    typedef _CharT value_type;
    typedef typename std::size_t size_type;
    typedef _CharT*  iterator;
    typedef const _CharT* const_iterator;
    typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
    typedef std::reverse_iterator<iterator>		reverse_iterator;

    // Vital because the last private aaray's element is used as the length counter
    static_assert (len < std::numeric_limits<value_type>::max(), "Too big size of a ShortString instance");
    static_assert (std::numeric_limits<value_type>::is_integer, "value_type must be integer");

    constexpr BasicShortString() noexcept : buf{'\0'}{
        buf[len - 1] = len - 1;
    }
    constexpr BasicShortString(const _CharT *const s) noexcept : buf{'\0'}{
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
    constexpr BasicShortString(const BasicShortString &) = default;
    constexpr BasicShortString(BasicShortString &&) = default;
    constexpr BasicShortString &operator=(const BasicShortString &) = default;
    constexpr BasicShortString &operator=(BasicShortString &&) = default;
    constexpr BasicShortString &operator=(const _CharT *const s) noexcept{
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
        return *this;
    };
    [[nodiscard]]inline constexpr size_type capacity()const noexcept{return len - 1;}
    [[nodiscard]]inline constexpr size_type size()const noexcept{return capacity() - freeSpace();}
    [[nodiscard]]inline constexpr size_type length()const noexcept{return size();}
    [[nodiscard]]inline constexpr size_type freeSpace()const noexcept{return buf[len - 1];}
    [[nodiscard]]inline constexpr auto empty()const noexcept{return buf[0] == '\0';}
    [[nodiscard]]inline constexpr auto full()const noexcept{return size() == capacity();}

    [[nodiscard]]inline constexpr auto begin()const noexcept{return std::begin(buf);}
    [[nodiscard]]inline constexpr auto end()const noexcept{return std::begin(buf) + size();}
    [[nodiscard]]inline constexpr auto begin() noexcept{return std::begin(buf);}
    [[nodiscard]]inline constexpr auto end() noexcept{return std::begin(buf) + size();}

    [[nodiscard]]inline constexpr auto cbegin()const noexcept{return std::cbegin(buf);}
    [[nodiscard]]inline constexpr auto cend()const noexcept{return std::cbegin(buf) + size();}

    [[nodiscard]]inline constexpr auto rbegin()const noexcept{return std::rend(buf) - size();}
    [[nodiscard]]inline constexpr auto rend()const noexcept{return std::rend(buf);}
    [[nodiscard]]inline constexpr auto rbegin() noexcept{return std::rend(buf) - size();}
    [[nodiscard]]inline constexpr auto rend() noexcept{return std::rend(buf);}

    [[nodiscard]]inline constexpr auto crbegin()const noexcept{return std::crend(buf) - size();}
    [[nodiscard]]inline constexpr auto crend()const noexcept{return std::crend(buf);}

    [[nodiscard]]inline constexpr operator _CharT*() noexcept{return buf;}
    [[nodiscard]]inline constexpr operator const _CharT*()const noexcept{return buf;}

    //Returns a reference to the first element in the container.
    //Calling front on an empty container is undefined behaviour.
    [[nodiscard]]inline constexpr auto front()const noexcept{return *begin();}
    [[nodiscard]]inline constexpr auto front() noexcept{return *begin();}
    //Returns a reference to the last element in the container.
    //Calling back on an empty container causes undefined behavior.
    [[nodiscard]]inline constexpr auto back()const noexcept{return *std::prev(end());}
    [[nodiscard]]inline constexpr auto back() noexcept{return *std::prev(end());}

    constexpr BasicShortString &operator+=(const _CharT *const s) noexcept{
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
        return *this;
    };
    constexpr BasicShortString &operator+=(const _CharT c) noexcept{
        auto inp = end();
        if(buf[len - 1]){
            *inp = c;
            ++inp;
            --(buf[len - 1]);
        }
        *inp = '\0';
        assert(inp < std::end(buf));
        return *this;
    };
    inline void push_back(const _CharT *const s) noexcept{operator+=(s);}
    inline void push_back(const _CharT c) noexcept{operator+=(c);}
    inline BasicShortString &append(const _CharT *const s) noexcept{return operator+=(s);}
private:
    _CharT buf[len];
};
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]constexpr const BasicShortString<len, _CharT, _Traits> operator+(const BasicShortString<len, _CharT, _Traits> &sl, const BasicShortString<len, _CharT, _Traits> &sr) noexcept{
    BasicShortString<len, _CharT, _Traits> temp(sl);
    temp+= sr;
    return temp;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]constexpr const BasicShortString<len, _CharT, _Traits> operator+(const BasicShortString<len, _CharT, _Traits> &sl, const _CharT *const sr) noexcept{
    BasicShortString<len, _CharT, _Traits> temp(sl);
    temp+= sr;
    return temp;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator ==(const BasicShortString<len, _CharT, _Traits> &sl, const _CharT *sr) noexcept{
    auto leftIt = sl.cbegin();
    auto rightIt = sr;
    while((*leftIt != '\0') && (*rightIt != '\0')){
        if(!_Traits::eq(*leftIt, *rightIt)){
            return false;
        }
        ++leftIt;
        ++rightIt;
    }
    return (*rightIt == '\0') && (*leftIt == '\0');
}
template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator ==(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                               const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    auto leftIt = sl.cbegin();
    auto rightIt = sr.cbegin();
    while((*leftIt != '\0') && (*rightIt != '\0')){
        if(!_Traits::eq(*leftIt, *rightIt)){
            return false;
        }
        ++leftIt;
        ++rightIt;
    }
    return *rightIt == '\0' && *leftIt == '\0';
}
template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator <(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                              const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    auto leftIt = sl.cbegin();
    auto rightIt = sr.cbegin();
    while((*leftIt != '\0') && (*rightIt != '\0')){
        if (_Traits::lt(*leftIt, *rightIt)){
            return true;
        }
        if (_Traits::lt(*rightIt, *leftIt)){
            return false;
        }
        ++leftIt;
        ++rightIt;
    }
    if(*leftIt == '\0' && *rightIt != '\0'){
        return true;
    }

    return false;
}

typedef BasicShortString<8> ShortString;
typedef BasicShortString<16> ShortString16;
static_assert (std::is_trivially_copyable<ShortString>::value);
static_assert (sizeof(ShortString) == 8);
static_assert (sizeof(ShortString16) == 16);
static_assert (sizeof(BasicShortString<8, wchar_t>) == 8 * sizeof(wchar_t));

static_assert (ShortString{""}.empty());
static_assert (!ShortString{"1"}.empty());
static_assert (!ShortString{"123456"}.full());

static_assert (ShortString{"1234567"}.full());
static_assert ([]{constexpr ShortString s; return s.freeSpace() == s.capacity();}());
static_assert ([]{constexpr ShortString s; return s.freeSpace() == static_cast<ShortString::size_type>(7);}());
static_assert ([]{constexpr ShortString s{"1"}; return s.freeSpace() == static_cast<ShortString::size_type>(6);}());
static_assert ([]{constexpr ShortString s{"123456"}; return s.freeSpace() == static_cast<ShortString::size_type>(1);}());
static_assert ([]{constexpr ShortString s{"1234567"}; return s.freeSpace() == static_cast<ShortString::size_type>(0);}());

static_assert (ShortString{"1234567"}.size() == static_cast<ShortString::size_type>(7));
static_assert (ShortString{"1234567"}.length() == static_cast<ShortString::size_type>(7));
static_assert (ShortString{"123"}.size() == static_cast<ShortString::size_type>(3));
static_assert (ShortString{"123"}.length() == static_cast<ShortString::size_type>(3));
static_assert (ShortString{""}.size() == static_cast<ShortString::size_type>(0));
static_assert (ShortString{""}.length() == static_cast<ShortString::size_type>(0));
static_assert (ShortString{}.size() == static_cast<ShortString::size_type>(0));
static_assert (ShortString{}.length() == static_cast<ShortString::size_type>(0));
static_assert ([]{constexpr ShortString s{"1234567"}; return s.back() == '7';}());
static_assert ([]{constexpr ShortString s{"1234"}; return s.back() == '4';}());
static_assert ([]{constexpr ShortString s{"1234567"}; return s.front() == '1';}());
static_assert ([]{constexpr ShortString s{"1234"}; return s.front() == '1';}());

static_assert (ShortString{"1234567"} == "1234567",         "String comparizon should work");
static_assert (ShortString{"1234567"} == ShortString{"1234567"}, "String comparizon should work");
static_assert (ShortString{"1234"} == "1234",               "String comparizon should work");
static_assert (ShortString{"1234"} == ShortString{"1234"},  "String comparizon should work");
static_assert (ShortString{""} == "",                       "String comparizon should work");
static_assert (ShortString{""} == ShortString{""},          "String comparizon should work");

static_assert (ShortString{"1234"} < ShortString{"12345"},  "String comparizon with < should work");
static_assert (ShortString{"1234"} < ShortString{"1235"},   "String comparizon with < should work");
static_assert (ShortString{""} < ShortString{"1"},          "String comparizon with < should work");
static_assert (ShortString{"a"} < ShortString{"b"},         "String comparizon with < should work");
static_assert (!(ShortString{"12345"} < ShortString{"1234"}),  "String comparizon with < should work");
static_assert (!(ShortString{"1235"} < ShortString{"1234"}),   "String comparizon with < should work");
static_assert (!(ShortString{"1"} < ShortString{""}),          "String comparizon with < should work");
static_assert (!(ShortString{"b"} < ShortString{"a"}),         "String comparizon with < should work");
static_assert (!(ShortString{"b"} < ShortString{"b"}),         "String comparizon with < should work");

static_assert (ShortString{"1234"} + ShortString{"567"} == "1234567", "String catenation should work");
static_assert (ShortString{"1234"} + "567" == "1234567",    "String catenation should work");
static_assert (ShortString{} + "1234567" == "1234567",      "String catenation should work");

static_assert (ShortString{} + "12345678901" == "1234567",  "Strings should be cut to fit the internal buf");
static_assert (ShortString{"12345678901"} == "1234567",     "Strings should be cut to fit the internal buf");

static_assert ([]
{
constexpr char string[] = "1234567";
constexpr ShortString s(string);
int i = 0;
for(ShortString::const_iterator it = s.cbegin(); it != s.cend(); ++it){
    if(*it != string[i])
        return false;
    ++i;
}
return true;
}(),                                                        "const_iterator test failed");

//static_assert (ShortString{"1"} == "1"); // strcmp is not a constexpr function


#endif // SHORTSTRING_H
