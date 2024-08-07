#include <QtTest>
#include <QTextStream>
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
    void ctorGets1Byte();
    void ctorFits7Bytes();
    void assignCuts8Byte();
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
    void stdCopyableDirect();
    void stdCopyableReverse();
    void stdSwapable();
    void canBeReadByIndex();
    void canBeModifiedByIndex();
    void providesApplyableIteratorTypes();
    void providesApplyableConstIteratorTypes();
    void canBeCleared();
    void canBeStreamed();
    void allowsInsertion();
};

ShortStringTest::ShortStringTest()
{

}

ShortStringTest::~ShortStringTest()
{

}

void ShortStringTest::ctorGets1Byte()
{
    constexpr auto c = "1";
    constexpr ShortString s{"1"};
    QCOMPARE(QString(s.c_str()), QString(c));
}

void ShortStringTest::ctorFits7Bytes()
{
    constexpr auto c = "1234567";
    constexpr ShortString s{"1234567"};
    QCOMPARE(QString(s.c_str()), QString(c));
}

void ShortStringTest::assignCuts8Byte()
{
    constexpr auto c = "1234567";
    ShortString s{"1"};
    s = "12345678";
    QCOMPARE(QString(s.c_str()), QString(c));
}

void ShortStringTest::assignable()
{
    {
        ShortString s;
        s = "1234567";
        QCOMPARE(QString(s.c_str()), QString("1234567"));
    }
    {
        ShortString16 s;
        s = "1234567ABCDEFG";
        QCOMPARE(QString(s.c_str()), QString("1234567ABCDEFG"));
    }
    {
        ShortString s;
        ShortString s2("1234567");
        s = s2;
        QCOMPARE(QString(s.c_str()), QString("1234567"));
    }
    {
        ShortString16 s;
        ShortString s2("1234567");
        s = s2.c_str();
        QCOMPARE(QString(s.c_str()), QString("1234567"));
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
    QCOMPARE(QString(s12.c_str()), QString(expected.data()));
}

void ShortStringTest::additionConstexpr()
{
    constexpr char etalon[] = "12345";
    constexpr ShortString s1("123");
    constexpr ShortString s2("45");
    constexpr auto s12 = s1 + s2;
    QCOMPARE(s12.size(), strlen(etalon));
    QCOMPARE(QString(s12.c_str()), etalon);
}

void ShortStringTest::appendsWithPlusEqual()
{
    {
        constexpr auto c = "1234567";
        {
            ShortString s;
            s+= "12345678";
            QCOMPARE(QString(s.c_str()), QString(c));
        }
        {
            ShortString s{"1"};
            s += "2345678";
            QCOMPARE(QString(s.c_str()), QString(c));
        }
    }
    {
        constexpr auto c = "12345678";
        ShortString16 s{"1"};
        s += "2345678";
        QCOMPARE(QString(s.c_str()), QString(c));
    }
    {
        constexpr auto c = "1234567ABCDEFG";
        ShortString16 s;
        s+= "1234567";
        s+= "ABCDEFG";
        QCOMPARE(QString(s.c_str()), QString(c));
    }
}

void ShortStringTest::appendsWithPushBack()
{
    constexpr auto c = "1234567";
    {
        ShortString s;
        s.push_back("1234567");
        QCOMPARE(QString(s.c_str()), QString(c));
    }
    {
        ShortString s{"1"};
        s.push_back("234567");
        QCOMPARE(QString(s.c_str()), QString(c));
    }
    {
        ShortString s("123456");
        s.push_back('7');
        QCOMPARE(QString(s.c_str()), QString(c));
    }
    {
        ShortString s;
        s.push_back('1');
        QCOMPARE(QString(s.c_str()), QString("1"));
    }
}

void ShortStringTest::pushBackCutsExcessiveData()
{
    constexpr auto c = "1234567";
    {
        ShortString s;
        s.push_back("123456789");
        QCOMPARE(QString(s.c_str()), QString(c));
    }
    {
        ShortString s("12345");
        s.push_back("6789");
        QCOMPARE(QString(s.c_str()), QString(c));
    }
    {
        ShortString s("1234567");
        s.push_back('8');
        QCOMPARE(QString(s.c_str()), QString(c));
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
        QVERIFY(s.c_str() != c);
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
    std::string stdS(s.c_str());
    QCOMPARE(stdS, c);
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
    using std::swap;
    ShortString s1{"Hello"};
    ShortString s2{"World"};
    swap(s1, s2);
    QCOMPARE(QString("Hello"), QString(s2.c_str()));
    QCOMPARE(QString("World"), QString(s1.c_str()));
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

    QCOMPARE(QString(string.data()), QString(s.c_str()));
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

        QCOMPARE(QString(string.data()), QString(s.c_str()));
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

        QCOMPARE(QString(string.data()), QString(s.c_str()));
    }
}

void ShortStringTest::providesApplyableConstIteratorTypes()
{
    {
        constexpr char string[] = "1234567";
        constexpr ShortString s(string);
        int i = strlen(string) - 1; // strlen is not a constexpr function and can't be checked by static_assert
        for(ShortString::const_reverse_iterator it = s.crbegin(); it != s.crend(); ++it){
            QCOMPARE(*it, string[i]);
            --i;
        }
    }
}

void ShortStringTest::canBeCleared()
{
    ShortString s = "7654321";
    s.clear();
    QVERIFY(s.empty());
    QCOMPARE(QString(s.c_str()), QString());
}

void ShortStringTest::canBeStreamed()
{
    ShortString s = "7654321";
    QString qs;
    QTextStream stream(&qs, QIODevice::WriteOnly);
    stream << s;
    QCOMPARE(QString(s.c_str()), qs);
}

void ShortStringTest::allowsInsertion()
{
    {
        ShortString s;
        s.insert(0, "1234567");
        QCOMPARE(QString(s.c_str()), QString("1234567"));
    }
    {
        ShortString s = "234567";
        s.insert(0, "1");
        QCOMPARE(QString(s.c_str()), QString("1234567"));
    }
    {
        ShortString s = "234567";
        s.insert(0, '1');
        QCOMPARE(QString(s.c_str()), QString("1234567"));
    }
    {
        ShortString s = "";
        s.insert(0, "1234567");
        QCOMPARE(QString(s.c_str()), QString("1234567"));
    }
    {
        ShortString s = "134";
        s.insert(1, '2');
        QCOMPARE(QString(s.c_str()), QString("1234"));
    }
}

QTEST_APPLESS_MAIN(ShortStringTest)

#include "tst_shortstringtest.moc"
