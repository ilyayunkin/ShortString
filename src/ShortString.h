#ifndef SHORTSTRING_H
#define SHORTSTRING_H
/**
 * Ilya Yunkin 2020
*/
#include <cstring>
#include <iterator>
#include <limits>
#include <assert.h>

// len is the result of sizeof().
// Maximal string lingth is len - 1
template <int len>
class BasicShortString
{
public:
    typedef char value_type;
    typedef typename std::size_t size_type;
    typedef char*  iterator;
    typedef const char* const_iterator;
    typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
    typedef std::reverse_iterator<iterator>		reverse_iterator;

    // Vital because the last private aaray's element is used as the length counter
    static_assert (len < std::numeric_limits<value_type>::max(), "Too big size of a ShortString instance");
    static_assert (std::numeric_limits<value_type>::is_integer, "value_type must be integer");

    constexpr BasicShortString() noexcept : buf{'\0'}{
        buf[len - 1] = len - 1;
    }
    constexpr BasicShortString(const char *const s) noexcept : buf{'\0'}{
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
    constexpr BasicShortString &operator=(const char *const s) noexcept{
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

    [[nodiscard]]inline constexpr operator char*() noexcept{return buf;}
    [[nodiscard]]inline constexpr operator const char*()const noexcept{return buf;}

    //Returns a reference to the first element in the container.
    //Calling front on an empty container is undefined behaviour.
    [[nodiscard]]inline constexpr auto front()const noexcept{return *begin();}
    [[nodiscard]]inline constexpr auto front() noexcept{return *begin();}
    //Returns a reference to the last element in the container.
    //Calling back on an empty container causes undefined behavior.
    [[nodiscard]]inline constexpr auto back()const noexcept{return *std::prev(end());}
    [[nodiscard]]inline constexpr auto back() noexcept{return *std::prev(end());}

    BasicShortString &operator+=(const char *const s) noexcept{
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
    BasicShortString &operator+=(const char c) noexcept{
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
    inline void push_back(const char *const s) noexcept{operator+=(s);}
    inline void push_back(const char c) noexcept{operator+=(c);}
    inline BasicShortString &append(const char *const s) noexcept{return operator+=(s);}
    [[nodiscard]]constexpr BasicShortString operator+(const BasicShortString &sr)const noexcept{
        if(full()){
            return *this;
        }else{
            BasicShortString ret(*this);
            const auto addSize = std::min(freeSpace(), sr.size());
            for(size_type i = 0; i < addSize; ++i){
                ret.buf[size() + i] = sr.buf[i];
            }
            const auto newSize = size() + addSize;
            assert(newSize < len);
            ret.buf[newSize] = '\0';
            ret.buf[len - 1] = capacity() - newSize;
            return ret;
        }
    }
private:
    char buf[len];
};
template <int len>
[[nodiscard]]inline constexpr bool operator ==(const BasicShortString<len> &sl, const char *sr) noexcept{
    return !std::strcmp(sl,  sr);
}
template <int lenL, int lenR>
[[nodiscard]]inline constexpr bool operator ==(const BasicShortString<lenL> &sl, const BasicShortString<lenR> &sr) noexcept{
    return !std::strcmp(sl,  sr);
}
template <int lenL, int lenR>
[[nodiscard]]inline constexpr bool operator <(const BasicShortString<lenL> &sl, const BasicShortString<lenR> &sr) noexcept{
    return std::strcmp(sl,  sr) == -1;
}

typedef BasicShortString<8> ShortString;
typedef BasicShortString<16> ShortString16;
static_assert (std::is_trivially_copyable<ShortString>::value);
static_assert (sizeof(ShortString) == 8);
static_assert (sizeof(ShortString16) == 16);

#endif // SHORTSTRING_H
