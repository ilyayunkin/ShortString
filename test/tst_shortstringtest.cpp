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
    void ctorFits7Bytes();
    void assignCuts8Byte();
    void assignedStringIsNotEmpty();
    void emptyAssignedStringIsEmpty();
    void appendsWithPlusEqual();
    void appendsWithPushBack();
    void comparableWithRawStrings();
    void castableToStdString();
    void stdCopyableDirect();
    void stdCopyableReverse();
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
    ShortString s;
    QCOMPARE(QString(s), QString(""));
    QVERIFY(s.empty());
}

void ShortStringTest::initializedStringIsNotEmpty()
{
    ShortString s{"1"};
    QVERIFY(!s.empty());
}

void ShortStringTest::ctorGets1Byte()
{
    constexpr auto c = "1";
    ShortString s{"1"};
    QCOMPARE(QString(s), QString(c));
}

void ShortStringTest::ctorFits7Bytes()
{
    constexpr auto c = "1234567";
    ShortString s{"1234567"};
    QCOMPARE(QString(s), QString(c));
}

void ShortStringTest::assignCuts8Byte()
{
    constexpr auto c = "1234567";
    ShortString s{"1"};
    s = "12345678";
    QCOMPARE(QString(s), QString(c));
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

void ShortStringTest::appendsWithPlusEqual()
{
    constexpr auto c = "1234567";
    ShortString s{"1"};
    s += "2345678";
    QCOMPARE(QString(s), QString(c));
}

void ShortStringTest::appendsWithPushBack()
{
    constexpr auto c = "1234567";
    ShortString s{"1"};
    s.push_back("2345678");
    QCOMPARE(QString(s), QString(c));
}

void ShortStringTest::comparableWithRawStrings()
{
    constexpr auto c = "1234567";
    ShortString s{"1234567"};
    QVERIFY(s == c);
}

void ShortStringTest::castableToStdString()
{
    constexpr auto c = "1234567";
    ShortString s{"1234567"};
    std::string stdS(s);
    QCOMPARE(stdS, c);
}

void ShortStringTest::stdCopyableDirect()
{
    constexpr auto c = "1234567";
    {
        const ShortString s{"1234567"};
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
        const ShortString s{"1234567"};
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
        const ShortString s{"1234567"};
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
        const ShortString s{"1234567"};
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


QTEST_APPLESS_MAIN(ShortStringTest)

#include "tst_shortstringtest.moc"
