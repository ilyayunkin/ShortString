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
        _Traits::assign(buf[len - 1], len - 1);
    }
    constexpr BasicShortString(const _CharT *const s) noexcept : buf{'\0'}{
        init(s);
    };
    constexpr BasicShortString(const BasicShortString &) = default;
    constexpr BasicShortString(BasicShortString &&) = default;
    constexpr BasicShortString &operator=(const BasicShortString &) = default;
    constexpr BasicShortString &operator=(BasicShortString &&) = default;
    constexpr BasicShortString &operator=(const _CharT *const s) noexcept{
        init(s);
        return *this;
    };
    constexpr void init(const _CharT *const s) noexcept{
        clear();
        operator+=(s);
    };
    constexpr void clear() noexcept{
        _Traits::assign(buf[0], '\0');
        _Traits::assign(buf[len - 1], len - 1);
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

    [[nodiscard]]inline constexpr _CharT* c_str() noexcept{return buf;}
    [[nodiscard]]inline constexpr const _CharT* c_str()const noexcept{return buf;}

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
        while(!_Traits::eq(*outp, '\0') && buf[len - 1]){
            _Traits::assign(*inp, *outp);
            ++inp;
            ++outp;
            --(buf[len - 1]);
        }
        _Traits::assign(*inp, '\0');
        assert(inp < std::end(buf));
        return *this;
    };
    constexpr BasicShortString &operator+=(const _CharT c) noexcept{
        auto inp = end();
        if(!_Traits::eq(buf[len - 1], '\0')){
            _Traits::assign(*inp, c);
            ++inp;
            --(buf[len - 1]);
        }
        *inp = '\0';
        assert(inp < std::end(buf));
        return *this;
    };
    template <int lenR>
    constexpr BasicShortString &operator+=(const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
        return operator+=(sr.c_str());
    }

    [[nodiscard]] constexpr _CharT& operator[](std::size_t i) noexcept{return buf[i];}
    [[nodiscard]] constexpr const _CharT& operator[](std::size_t i) const noexcept{return buf[i];}

    constexpr inline void push_back(const _CharT *const s) noexcept{operator+=(s);}
    constexpr inline void push_back(const _CharT c) noexcept{operator+=(c);}
    constexpr inline BasicShortString &append(const _CharT *const s) noexcept{return operator+=(s);}
    constexpr inline BasicShortString &append(const _CharT c) noexcept{return operator+=(c);}
    template <int lenR>
    constexpr inline BasicShortString &append(const BasicShortString<lenR, _CharT, _Traits> &s) noexcept{return operator+=(s.c_str());}
private:
    _CharT buf[len];
};

template <int lenL,
          int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]constexpr const BasicShortString<lenL, _CharT, _Traits> operator+(const BasicShortString<lenL, _CharT, _Traits> &sl, const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    BasicShortString<lenL, _CharT, _Traits> temp(sl);
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
template <typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr int sscmp(const _CharT *sl, const _CharT *sr) noexcept{
    auto leftIt = sl;
    auto rightIt = sr;
    while(!_Traits::eq(*leftIt, '\0') && !_Traits::eq(*rightIt, '\0') && _Traits::eq(*leftIt, *rightIt)){
        ++leftIt;
        ++rightIt;
    }
    if (_Traits::eq(*rightIt, '\0') && _Traits::eq(*leftIt, '\0'))
        return 0;
    if(_Traits::eq(*leftIt, '\0') || _Traits::lt(*leftIt, *rightIt))
        return -1;
    return 1;
}

template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator ==(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                               const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl.c_str(), sr.c_str()) == 0;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator ==(const BasicShortString<len, _CharT, _Traits> &sl,
                                               const _CharT *sr) noexcept{
    return sscmp(sl.c_str(), sr) == 0;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator ==(const _CharT *sl,
                                               const BasicShortString<len, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl, sr.c_str()) == 0;
}

template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator !=(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                               const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    return !operator==(sl, sr);
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator !=(const BasicShortString<len, _CharT, _Traits> &sl,
                                               const _CharT *sr) noexcept{
    return !operator==(sl, sr);
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator !=(const _CharT *sl,
                                               const BasicShortString<len, _CharT, _Traits> &sr) noexcept{
    return !operator==(sl, sr);
}

template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator <(const BasicShortString<len, _CharT, _Traits> &sl,
                                              const _CharT *sr) noexcept{

    return sscmp(sl.c_str(), sr) == -1;
}
template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator <(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                              const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl.c_str(), sr.c_str()) == -1;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator <(const _CharT *sl,
                                              const BasicShortString<len, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl, sr.c_str()) == -1;
}

template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator >(const BasicShortString<len, _CharT, _Traits> &sl,
                                              const _CharT *sr) noexcept{

    return sscmp(sl.c_str(), sr) == 1;
}
template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator >(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                              const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl.c_str(), sr.c_str()) == 1;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator >(const _CharT *sl,
                                              const BasicShortString<len, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl, sr.c_str()) == 1;
}

template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator <=(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                               const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl.c_str(), sr.c_str()) != 1;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator <=(const BasicShortString<len, _CharT, _Traits> &sl,
                                               const _CharT *sr) noexcept{
    return sscmp(sl.c_str(), sr) != 1;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator <=(const _CharT *sl,
                                               const BasicShortString<len, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl, sr.c_str()) != 1;
}

template <int lenL, int lenR,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator >=(const BasicShortString<lenL, _CharT, _Traits> &sl,
                                               const BasicShortString<lenR, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl.c_str(), sr.c_str()) != -1;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator >=(const BasicShortString<len, _CharT, _Traits> &sl,
                                               const _CharT *sr) noexcept{
    return sscmp(sl.c_str(), sr) != -1;
}
template <int len,
          typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>>
[[nodiscard]]inline constexpr bool operator >=(const _CharT *sl,
                                               const BasicShortString<len, _CharT, _Traits> &sr) noexcept{
    return sscmp(sl, sr.c_str()) != -1;
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
static_assert ([]{constexpr ShortString s;            return s.freeSpace() == s.capacity();}());
static_assert ([]{constexpr ShortString s;            return s.freeSpace() == static_cast<ShortString::size_type>(7);}());
static_assert ([]{constexpr ShortString s{"1"};       return s.freeSpace() == static_cast<ShortString::size_type>(6);}());
static_assert ([]{constexpr ShortString s{"123456"};  return s.freeSpace() == static_cast<ShortString::size_type>(1);}());
static_assert ([]{constexpr ShortString s{"1234567"}; return s.freeSpace() == static_cast<ShortString::size_type>(0);}());

static_assert (ShortString{"1234567"}.size()   == static_cast<ShortString::size_type>(7));
static_assert (ShortString{"1234567"}.length() == static_cast<ShortString::size_type>(7));
static_assert (ShortString{"123"}.size()       == static_cast<ShortString::size_type>(3));
static_assert (ShortString{"123"}.length()     == static_cast<ShortString::size_type>(3));
static_assert (ShortString{""}.size()          == static_cast<ShortString::size_type>(0));
static_assert (ShortString{""}.length()        == static_cast<ShortString::size_type>(0));
static_assert (ShortString{}.size()            == static_cast<ShortString::size_type>(0));
static_assert (ShortString{}.length()          == static_cast<ShortString::size_type>(0));

static_assert ([]{constexpr ShortString s{"1234567"}; return s.back() == '7';}());
static_assert ([]{constexpr ShortString s{"1234"};    return s.back() == '4';}());
static_assert ([]{constexpr ShortString s{"1234567"}; return s.front() == '1';}());
static_assert ([]{constexpr ShortString s{"1234"};    return s.front() == '1';}());

static_assert (ShortString{"1234567"}   == "1234567",               "String comparizon should work");
static_assert (ShortString{"1234567"}   == ShortString{"1234567"},  "String comparizon should work");
static_assert (ShortString{"1234"}      == "1234",                  "String comparizon should work");
static_assert (ShortString{"1234"}      == ShortString{"1234"},     "String comparizon should work");
static_assert (ShortString{""}          == "",                      "String comparizon should work");
static_assert (ShortString{""}          == ShortString{""},         "String comparizon should work");

static_assert (ShortString{"1234567"}   != "123456",                "String comparizon should work");
static_assert (ShortString{"1234567"}   != ShortString{"123456"},   "String comparizon should work");
static_assert (ShortString{"1234"}      != "123",                   "String comparizon should work");
static_assert (ShortString{"1234"}      != ShortString{"123"},      "String comparizon should work");
static_assert (ShortString{""}          != "1",                     "String comparizon should work");
static_assert (ShortString{""}          != ShortString{"1"},        "String comparizon should work");

static_assert (!(ShortString{"1234567"} == "123456"),               "String comparizon should work");
static_assert (!(ShortString{"1234567"} == ShortString{"123456"}),  "String comparizon should work");
static_assert (!(ShortString{"1234"}    == "123"),                  "String comparizon should work");
static_assert (!(ShortString{"1234"}    == ShortString{"123"}),     "String comparizon should work");
static_assert (!(ShortString{""}        == "1"),                    "String comparizon should work");
static_assert (!(ShortString{""}        == ShortString{"1"}),       "String comparizon should work");

static_assert (!(ShortString{"1234567"} != "1234567"),              "String comparizon should work");
static_assert (!(ShortString{"1234567"} != ShortString{"1234567"}), "String comparizon should work");
static_assert (!(ShortString{"1234"}    != "1234"),                 "String comparizon should work");
static_assert (!(ShortString{"1234"}    != ShortString{"1234"}),    "String comparizon should work");
static_assert (!(ShortString{""}        != ""),                     "String comparizon should work");
static_assert (!(ShortString{""}        != ShortString{""}),        "String comparizon should work");

static_assert ("1234567"   == ShortString{"1234567"}, "String comparizon in reverse order should work");
static_assert (!("1234567" == ShortString{"123456"}), "String comparizon in reverse order should work");

static_assert (ShortString{"1234567"} == ShortString16{"1234567"}, "String comparizon between types should work");

static_assert (ShortString{"1234"}    < "12345",               "String comparizon with < should work");
static_assert (ShortString{"1234"}    < ShortString{"12345"},  "String comparizon with < should work");
static_assert (ShortString{"1234"}    < ShortString{"1235"},   "String comparizon with < should work");
static_assert (ShortString{""}        < ShortString{"1"},      "String comparizon with < should work");
static_assert (ShortString{"a"}       < ShortString{"b"},      "String comparizon with < should work");
static_assert (!(ShortString{"12345"} < ShortString{"1234"}),  "String comparizon with < should work");
static_assert (!(ShortString{"1235"}  < ShortString{"1234"}),  "String comparizon with < should work");
static_assert (!(ShortString{"1"}     < ShortString{""}),      "String comparizon with < should work");
static_assert (!(ShortString{"b"}     < ShortString{"a"}),     "String comparizon with < should work");
static_assert (!(ShortString{"b"}     < ShortString{"b"}),     "String comparizon with < should work");
static_assert ("1234"                 < ShortString{"12345"},  "String comparizon with < should work");

static_assert (!(ShortString{"1234"}  > "12345"),              "String comparizon with < should work");
static_assert (!(ShortString{"1234"}  > ShortString{"12345"}), "String comparizon with < should work");
static_assert (!(ShortString{"1234"}  > ShortString{"1235"}),  "String comparizon with < should work");
static_assert (!(ShortString{""}      > ShortString{"1"}),     "String comparizon with < should work");
static_assert (!(ShortString{"a"}     > ShortString{"b"}),     "String comparizon with < should work");
static_assert (ShortString{"12345"}   > ShortString{"1234"},   "String comparizon with < should work");
static_assert (ShortString{"1235"}    > ShortString{"1234"},   "String comparizon with < should work");
static_assert (ShortString{"1"}       > ShortString{""},       "String comparizon with < should work");
static_assert (ShortString{"b"}       > ShortString{"a"},      "String comparizon with < should work");
static_assert (!(ShortString{"b"}     > ShortString{"b"}),     "String comparizon with < should work");

static_assert (ShortString{"1234567"}   <= "1234567",               "String comparizon should work");
static_assert (ShortString{"1234567"}   <= ShortString{"1234567"},  "String comparizon should work");
static_assert (ShortString{"1234"}      <= "1234",                  "String comparizon should work");
static_assert (ShortString{"1234"}      <= ShortString{"1234"},     "String comparizon should work");
static_assert (ShortString{""}          <= "",                      "String comparizon should work");
static_assert (ShortString{""}          <= ShortString{""},         "String comparizon should work");
static_assert (ShortString{"1234"}      <= "12345",                 "String comparizon with < should work");
static_assert (ShortString{"1234"}      <= ShortString{"12345"},    "String comparizon with < should work");
static_assert (ShortString{"1234"}      <= ShortString{"1235"},     "String comparizon with < should work");
static_assert (ShortString{""}          <= ShortString{"1"},        "String comparizon with < should work");
static_assert (ShortString{"a"}         <= ShortString{"b"},        "String comparizon with < should work");

static_assert (ShortString{"1234567"}   >= "1234567",               "String comparizon should work");
static_assert (ShortString{"1234567"}   >= ShortString{"1234567"},  "String comparizon should work");
static_assert (ShortString{"1234"}      >= "1234",                  "String comparizon should work");
static_assert (ShortString{"1234"}      >= ShortString{"1234"},     "String comparizon should work");
static_assert (ShortString{""}          >= "",                      "String comparizon should work");
static_assert (ShortString{""}          >= ShortString{""},         "String comparizon should work");
static_assert (ShortString{"12345"}     >= ShortString{"1234"},     "String comparizon with < should work");
static_assert (ShortString{"1235"}      >= ShortString{"1234"},     "String comparizon with < should work");
static_assert (ShortString{"1"}         >= ShortString{""},         "String comparizon with < should work");
static_assert (ShortString{"b"}         >= ShortString{"a"},        "String comparizon with < should work");

static_assert (ShortString{"1234"} + ShortString{"567"}   == "1234567", "String catenation should work");
static_assert (ShortString{"1234"} + "567"                == "1234567", "String catenation should work");
static_assert (ShortString{}       + "1234567"            == "1234567", "String catenation should work");
static_assert (ShortString{"1234"} + ShortString16{"567"} == "1234567", "String catenation between types should work");

static_assert ([]{ShortString s{"1234"}; s+= ShortString{"567"}        ; return s == "1234567";}(), "String catenation should work");
static_assert ([]{ShortString s{"1234"}; s+= "567"                     ; return s == "1234567";}(), "String catenation should work");
static_assert ([]{ShortString s{}      ; s+= "1234567"                 ; return s == "1234567";}(), "String catenation should work");
static_assert ([]{ShortString s{"1234"}; s+= '5'                       ; return s == "12345"  ;}(), "String catenation should work");
static_assert ([]{ShortString s{"1234"}; s+= ShortString16{"567"}      ; return s == "1234567";}(), "String catenation between types should work");
static_assert ([]{ShortString s{"1234"}; s.append(ShortString{"567"}  ); return s == "1234567";}(), "String catenation should work");
static_assert ([]{ShortString s{"1234"}; s.append("567"               ); return s == "1234567";}(), "String catenation should work");
static_assert ([]{ShortString s{}      ; s.append("1234567"           ); return s == "1234567";}(), "String catenation should work");
static_assert ([]{ShortString s{"1234"}; s.append('5'                 ); return s == "12345"  ;}(), "String catenation should work");
static_assert ([]{ShortString s{"1234"}; s.append(ShortString16{"567"}); return s == "1234567";}(), "String catenation between types should work");

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
