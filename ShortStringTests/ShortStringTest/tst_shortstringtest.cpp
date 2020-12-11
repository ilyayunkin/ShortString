#include <QtTest>
#include "ShortString.h"
#include <algorithm>
// add necessary includes here

class ShortStringTest : public QObject
{
    Q_OBJECT

public:
    ShortStringTest();
    ~ShortStringTest();

private slots:
    void sizeof8();
    void defaultCtorMakesEmptyString();
    void ctorGets1Byte();
    void initializedStringIsNotEmpty();
    void full();
    void freeSpace();
    void ctorFits7Bytes();
    void assignCuts8Byte();
    void sizeAndLengthReturnTheSame();
    void assignable();
    void assignedStringIsNotEmpty();
    void emptyAssignedStringIsEmpty();
    void addition_data();
    void addition();
    void additionConstexpr();
    void appendsWithPlusEqual();
    void appendsWithPushBack();
    void pushBackCutsExcessiveData();
    void comparable();
    void castableToStdString();
    void frontReturnsFirstChar();
    void backReturnsLastChar();
    void stdCopyableDirect();
    void stdCopyableReverse();
    void stdSwapable();
    void canBeReadByIndex();
    void canBeModifiedByIndex();
    void providesApplyableIteratorTypes();
    void providesApplyableConstIteratorTypes();
};

ShortStringTest::ShortStringTest()
{

}

ShortStringTest::~ShortStringTest()
{

}

void ShortStringTest::sizeof8()
{
    QCOMPARE(sizeof (ShortString), 8u);
}

void ShortStringTest::defaultCtorMakesEmptyString()
{
    {
        constexpr ShortString s;
        QCOMPARE(QString(s), QString(""));
        QVERIFY(s.empty());
        QVERIFY(!s.full());
    }
    {
        constexpr ShortString16 s;
        QCOMPARE(QString(s), QString(""));
        QVERIFY(s.empty());
        QVERIFY(!s.full());
    }
}

void ShortStringTest::initializedStringIsNotEmpty()
{
    constexpr ShortString s{"1"};
    QVERIFY(!s.empty());
}

void ShortStringTest::full()
{
    {
        constexpr ShortString s{"123456"};
        QVERIFY(!s.full());
    }
    {
        constexpr ShortString s{"1234567"};
        QVERIFY(s.full());
    }
}

void ShortStringTest::freeSpace()
{
    {
        constexpr ShortString s;
        QCOMPARE(s.freeSpace(), s.capacity());
    }
    {
        constexpr ShortString s{"1"};
        QCOMPARE(s.freeSpace(), 6u);
    }
    {
        constexpr ShortString s{"123456"};
        QCOMPARE(s.freeSpace(), 1u);
    }
    {
        constexpr ShortString s{"1234567"};
        QCOMPARE(s.freeSpace(), 0u);
    }
}

void ShortStringTest::ctorGets1Byte()
{
    constexpr auto c = "1";
    constexpr ShortString s{"1"};
    QCOMPARE(QString(s), QString(c));
}

void ShortStringTest::ctorFits7Bytes()
{
    constexpr auto c = "1234567";
    constexpr ShortString s{"1234567"};
    QCOMPARE(QString(s), QString(c));
}

void ShortStringTest::assignCuts8Byte()
{
    constexpr auto c = "1234567";
    ShortString s{"1"};
    s = "12345678";
    QCOMPARE(QString(s), QString(c));
}

void ShortStringTest::sizeAndLengthReturnTheSame()
{
    {
        constexpr ShortString s{"1234567"};
        QCOMPARE(s.size(), 7u);
        QCOMPARE(s.size(), s.length());
    }
    {
        constexpr ShortString s{"123"};
        QCOMPARE(s.size(), 3u);
        QCOMPARE(s.size(), s.length());
    }
    {
        constexpr ShortString s;
        QCOMPARE(s.size(), 0u);
        QCOMPARE(s.size(), s.length());
    }
}

void ShortStringTest::assignable()
{
    {
        ShortString s;
        s = "1234567";
        QCOMPARE(QString(s), QString("1234567"));
    }
    {
        ShortString16 s;
        s = "1234567ABCDEFG";
        QCOMPARE(QString(s), QString("1234567ABCDEFG"));
    }
    {
        ShortString s;
        ShortString s2("1234567");
        s = s2;
        QCOMPARE(QString(s), QString("1234567"));
    }
    {
        ShortString16 s;
        ShortString s2("1234567");
        s = s2;
        QCOMPARE(QString(s), QString("1234567"));
    }
}

void ShortStringTest::assignedStringIsNotEmpty()
{
    ShortString s;
    QVERIFY(s.empty());
    s = "12345678";
    QVERIFY(!s.empty());
}

void ShortStringTest::emptyAssignedStringIsEmpty()
{
    ShortString s;
    QVERIFY(s.empty());
    s = "12345678";
    QVERIFY(!s.empty());
    s = "";
    QVERIFY(s.empty());
}

void ShortStringTest::addition_data()
{
    QTest::addColumn<QByteArray>("left");
    QTest::addColumn<QByteArray>("right");
    QTest::addColumn<QByteArray>("expected");

    QTest::newRow("123 + 45 = 12345") << QByteArray("123") << QByteArray("45") << QByteArray("12345");
    QTest::newRow("null + 12345 = 12345") << QByteArray("") << QByteArray("12345") << QByteArray("12345");
    QTest::newRow("null + null = null") << QByteArray("") << QByteArray("") << QByteArray("");
    QTest::newRow("1234567 + null = 1234567") << QByteArray("1234567") << QByteArray("") << QByteArray("1234567");
    QTest::newRow("null + 1234567 = 1234567") << QByteArray("") << QByteArray("1234567") << QByteArray("1234567");
    QTest::newRow("1234567 + 1234567 = 1234567") << QByteArray("1234567") << QByteArray("1234567") << QByteArray("1234567");
}

void ShortStringTest::addition()
{
    QFETCH(QByteArray, left);
    QFETCH(QByteArray, right);
    QFETCH(QByteArray, expected);

    ShortString s1(left.data());
    ShortString s2(right.data());
    auto s12 = s1 + s2;
    QCOMPARE(QString(s12), QString(expected.data()));
}

void ShortStringTest::additionConstexpr()
{
    constexpr char etalon[] = "12345";
    constexpr ShortString s1("123");
    constexpr ShortString s2("45");
    constexpr auto s12 = s1 + s2;
    QCOMPARE(s12.size(), strlen(etalon));
    QCOMPARE(QString(s12), etalon);
}

void ShortStringTest::appendsWithPlusEqual()
{
    {
        constexpr auto c = "1234567";
        {
            ShortString s;
            s+= "12345678";
            QCOMPARE(QString(s), QString(c));
        }
        {
            ShortString s{"1"};
            s += "2345678";
            QCOMPARE(QString(s), QString(c));
        }
    }
    {
        constexpr auto c = "12345678";
        ShortString16 s{"1"};
        s += "2345678";
        QCOMPARE(QString(s), QString(c));
    }
    {
        constexpr auto c = "1234567ABCDEFG";
        ShortString16 s;
        s+= "1234567";
        s+= "ABCDEFG";
        QCOMPARE(QString(s), QString(c));
    }
}

void ShortStringTest::appendsWithPushBack()
{
    constexpr auto c = "1234567";
    {
        ShortString s;
        s.push_back("1234567");
        QCOMPARE(QString(s), QString(c));
    }
    {
        ShortString s{"1"};
        s.push_back("234567");
        QCOMPARE(QString(s), QString(c));
    }
    {
        ShortString s("123456");
        s.push_back('7');
        QCOMPARE(QString(s), QString(c));
    }
    {
        ShortString s;
        s.push_back('1');
        QCOMPARE(QString(s), QString("1"));
    }
}

void ShortStringTest::pushBackCutsExcessiveData()
{
    constexpr auto c = "1234567";
    {
        ShortString s;
        s.push_back("123456789");
        QCOMPARE(QString(s), QString(c));
    }
    {
        ShortString s("12345");
        s.push_back("6789");
        QCOMPARE(QString(s), QString(c));
    }
    {
        ShortString s("1234567");
        s.push_back('8');
        QCOMPARE(QString(s), QString(c));
    }
}

void ShortStringTest::comparable()
{
    {
        constexpr auto c = "1234567";
        constexpr ShortString s{"1234567"};
        QVERIFY(s == c);
    }
    {
        constexpr ShortString s = "1234567";
        constexpr ShortString s1{"1234567"};
        QVERIFY(s == s1);
    }
    {
        constexpr ShortString s = "1234567";
        constexpr ShortString16 s1{"1234567"};
        QVERIFY(s == s1);
    }
    {
        constexpr auto c = "1234567";
        constexpr ShortString s{"123456"};
        QVERIFY(s != c);
    }
    {
        constexpr ShortString s = "1234567";
        constexpr ShortString s1{"123456"};
        QVERIFY(s != s1);
    }
    {
        constexpr ShortString s = "1234567";
        constexpr ShortString16 s1{"12345678"};
        QVERIFY(s != s1);
    }
}

void ShortStringTest::castableToStdString()
{
    constexpr auto c = "1234567";
    constexpr ShortString s{"1234567"};
    std::string stdS(s);
    QCOMPARE(stdS, c);
}

void ShortStringTest::frontReturnsFirstChar()
{
    constexpr auto c = '1';
    {
        ShortString s{"1234567"};
        QCOMPARE(s.front(), c);
    }
    {
        constexpr ShortString s{"1234567"};
        QCOMPARE(s.front(), c);
    }
    {
        ShortString s{"1234"};
        QCOMPARE(s.front(), c);
    }
    {
        constexpr ShortString s{"1234"};
        QCOMPARE(s.front(), c);
    }
}

void ShortStringTest::backReturnsLastChar()
{
    {
        constexpr auto c = '7';
        {
            ShortString s{"1234567"};
            QCOMPARE(s.back(), c);
        }
        {
            constexpr ShortString s{"1234567"};
            QCOMPARE(s.back(), c);
        }
    }
    {
        constexpr auto c = '4';
        {
            ShortString s{"1234"};
            QCOMPARE(s.back(), c);
        }
        {
            constexpr ShortString s{"1234"};
            QCOMPARE(s.back(), c);
        }
    }
}

void ShortStringTest::stdCopyableDirect()
{
    constexpr auto c = "1234567";
    {
        constexpr ShortString s{"1234567"};
        QString qstr;
        std::copy(s.begin(), s.end(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
    {
        ShortString s{"1234567"};
        QString qstr;
        std::copy(s.begin(), s.end(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
    {
        constexpr ShortString s{"1234567"};
        QString qstr;
        std::copy(s.cbegin(), s.cend(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
    {
        ShortString s{"1234567"};
        QString qstr;
        std::copy(s.cbegin(), s.cend(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
}

void ShortStringTest::stdCopyableReverse()
{
    constexpr auto c = "7654321";
    {
        constexpr ShortString s{"1234567"};
        QString qstr;
        std::copy(s.rbegin(), s.rend(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
    {
        ShortString s{"1234567"};
        QString qstr;
        std::copy(s.rbegin(), s.rend(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
    {
        constexpr ShortString s{"1234567"};
        QString qstr;
        std::copy(s.crbegin(), s.crend(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
    {
        ShortString s{"1234567"};
        QString qstr;
        std::copy(s.crbegin(), s.crend(), std::back_inserter(qstr));
        QCOMPARE(qstr, QString(c));
    }
}

void ShortStringTest::stdSwapable()
{
    ShortString s1{"Hello"};
    ShortString s2{"World"};
    std::swap(s1, s2);
    QCOMPARE(QString("Hello"), QString(s2));
    QCOMPARE(QString("World"), QString(s1));
}

void ShortStringTest::canBeReadByIndex()
{
    {
        constexpr char string[] = "1234567";
        ShortString s(string);
        for(unsigned i = 0; i < sizeof(string); ++i){
            QCOMPARE(s[i], string[i]);
        }
    }
    {
        constexpr char string[] = "1234567";
        constexpr ShortString s(string);
        for(unsigned i = 0; i < sizeof(string); ++i){
            QCOMPARE(s[i], string[i]);
        }
    }
}

void ShortStringTest::canBeModifiedByIndex()
{
    std::string string = "1234567";
    ShortString s("0000000");
    for(unsigned i = 0; i < string.size(); ++i){
        s[i] = string[string.size() - i - 1];
    }

    std::reverse(std::begin(string), std::end(string));

    QCOMPARE(QString(string.data()), QString(s));
}

void ShortStringTest::providesApplyableIteratorTypes()
{
    {
        std::string string = "1234567";
        ShortString s("0000000");
        auto it = string.rbegin();
        ShortString::iterator sIt = s.begin();
        while(it != string.rend()){
            *sIt = *it;
            ++it;
            ++sIt;
        }

        std::reverse(std::begin(string), std::end(string));

        QCOMPARE(QString(string.data()), QString(s));
    }
    {
        std::string string = "1234567";
        ShortString s("0000000");
        auto it = string.begin();
        ShortString::reverse_iterator sIt = s.rbegin();
        while(it != string.end()){
            *sIt = *it;
            ++it;
            ++sIt;
        }

        std::reverse(std::begin(string), std::end(string));

        QCOMPARE(QString(string.data()), QString(s));
    }
}

void ShortStringTest::providesApplyableConstIteratorTypes()
{
    {
        constexpr char string[] = "1234567";
        constexpr ShortString s(string);
        int i = 0;
        for(ShortString::const_iterator it = s.cbegin(); it != s.cend(); ++it){
            QCOMPARE(*it, string[i]);
            ++i;
        }
    }
    {
        constexpr char string[] = "1234567";
        constexpr ShortString s(string);
        int i = strlen(string) - 1;
        for(ShortString::const_reverse_iterator it = s.crbegin(); it != s.crend(); ++it){
            QCOMPARE(*it, string[i]);
            --i;
        }
    }
}

QTEST_APPLESS_MAIN(ShortStringTest)

#include "tst_shortstringtest.moc"
