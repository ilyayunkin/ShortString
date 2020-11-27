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
    void sizeAndLengthReturnTheSame();
    void assignable();
    void assignedStringIsNotEmpty();
    void emptyAssignedStringIsEmpty();
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
    void compareBenchmark();
    void compareBenchmarkString();
    void compareBenchmarkQString();
    void initBenchmark();
    void initBenchmarkString();
    void initBenchmarkQString();
    void pushBenchmark();
    void pushBenchmarkString();
    void pushBenchmarkQString();
    void appendBenchmark();
    void appendBenchmarkString();
    void appendBenchmarkQString();
    void sortBenchmark();
    void sortBenchmarkString();
    void sortBenchmarkQString();
    void copyBenchmark();
    void copyBenchmarkString();
    void copyBenchmarkQString();
    void moveBenchmark();
    void moveBenchmarkString();
    void moveBenchmarkQString();
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
        ShortString s;
        QCOMPARE(QString(s), QString(""));
        QVERIFY(s.empty());
    }
    {
        ShortString16 s;
        QCOMPARE(QString(s), QString(""));
        QVERIFY(s.empty());
    }
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

void ShortStringTest::sizeAndLengthReturnTheSame()
{
    {
        ShortString s{"1234567"};
        QCOMPARE(s.size(), 7u);
        QCOMPARE(s.size(), s.length());
    }
    {
        ShortString s{"123"};
        QCOMPARE(s.size(), 3u);
        QCOMPARE(s.size(), s.length());
    }
    {
        ShortString s;
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
        ShortString s{"1234567"};
        QVERIFY(s == c);
    }
    {
        ShortString s = "1234567";
        ShortString s1{"1234567"};
        QVERIFY(s == s1);
    }
    {
        ShortString s = "1234567";
        ShortString16 s1{"1234567"};
        QVERIFY(s == s1);
    }
    {
        constexpr auto c = "1234567";
        ShortString s{"123456"};
        QVERIFY(s != c);
    }
    {
        ShortString s = "1234567";
        ShortString s1{"123456"};
        QVERIFY(s != s1);
    }
    {
        ShortString s = "1234567";
        ShortString16 s1{"12345678"};
        QVERIFY(s != s1);
    }
}

void ShortStringTest::castableToStdString()
{
    constexpr auto c = "1234567";
    ShortString s{"1234567"};
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
        const ShortString s{"1234567"};
        QCOMPARE(s.front(), c);
    }
    {
        ShortString s{"1234"};
        QCOMPARE(s.front(), c);
    }
    {
        const ShortString s{"1234"};
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
            const ShortString s{"1234567"};
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
            const ShortString s{"1234"};
            QCOMPARE(s.back(), c);
        }
    }
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

void ShortStringTest::stdSwapable()
{
    ShortString s1{"Hello"};
    ShortString s2{"World"};
    std::swap(s1, s2);
    QCOMPARE(QString("Hello"), QString(s2));
    QCOMPARE(QString("World"), QString(s1));
}

template <typename C>
void compareBench(){
    constexpr auto count = 8192;
    for(int j = 0; j < count; ++j){
        C s1 = "1234567";
        C s2 = "1234567";
        s1 == s2;
    }
}
void ShortStringTest::compareBenchmark(){
    QBENCHMARK{ compareBench<ShortString>(); }
}
void ShortStringTest::compareBenchmarkString(){
    QBENCHMARK{ compareBench<std::string>(); }
}
void ShortStringTest::compareBenchmarkQString(){
    QBENCHMARK{ compareBench<QString>(); }
}
template <typename C>
void initBench(){
    constexpr auto count = 8192;
    for(int j = 0; j < count; ++j){
        C s = "1234567";
        Q_UNUSED(s);
    }
}
void ShortStringTest::initBenchmark(){
    QBENCHMARK{ initBench<ShortString>(); }
}
void ShortStringTest::initBenchmarkString(){
    QBENCHMARK{ initBench<std::string>(); }
}
void ShortStringTest::initBenchmarkQString(){
    QBENCHMARK{
        initBench<QString>();
    }
}
template <typename C>
void pushBench(){
    constexpr auto count = 8192;
    C v[count];
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < count; ++j){
            v[j].push_back('1');
        }
    }
}
void ShortStringTest::pushBenchmark(){
    QBENCHMARK{ pushBench<ShortString>(); }
}
void ShortStringTest::pushBenchmarkString(){
    QBENCHMARK{ pushBench<std::string>(); }
}
void ShortStringTest::pushBenchmarkQString(){
    QBENCHMARK{ pushBench<QString>(); }
}
template <typename C>
void appendBench(){
    constexpr auto count = 8192;
    C v[count];
    QBENCHMARK{
        for(int j = 0; j < count; ++j){
            v[j].append("1234");
            v[j].append("567");
        }
    }
}
void ShortStringTest::appendBenchmark(){
    appendBench<ShortString>();
}
void ShortStringTest::appendBenchmarkString(){
    appendBench<std::string>();
}
void ShortStringTest::appendBenchmarkQString(){
    appendBench<QString>();
}
template <typename C>
void sortBench(){
    constexpr auto count = 8192;
    C v[count];
    for(int j = 0; j < count; ++j){
        v[j] = QString::number(j).toLatin1().data();
    }
    QBENCHMARK{
        std::sort(std::begin(v), std::end(v));
    }
}
void ShortStringTest::sortBenchmark(){
    sortBench<ShortString>();
}
void ShortStringTest::sortBenchmarkString(){
    sortBench<std::string>();
}
void ShortStringTest::sortBenchmarkQString(){
    sortBench<QString>();
}
template <typename C>
void copyBench(){
    constexpr auto count = 8192;
    for(int j = 0; j < count; ++j){
        C s("1234567");
        C s1(s);
    }
}
void ShortStringTest::copyBenchmark(){
    QBENCHMARK{ copyBench<ShortString>(); }
}
void ShortStringTest::copyBenchmarkString(){
    QBENCHMARK{ copyBench<std::string>(); }
}
void ShortStringTest::copyBenchmarkQString(){
    QBENCHMARK{ copyBench<QString>(); }
}
template <typename C>
void moveBench(){
    constexpr auto count = 8192;
    std::vector<C> v;
    v.reserve(count);
    QBENCHMARK{
        for(int j = 0; j < count; ++j){
            v.push_back(C("1234567"));
        }
    }
}
void ShortStringTest::moveBenchmark(){
    moveBench<ShortString>();
}
void ShortStringTest::moveBenchmarkString(){
    moveBench<std::string>();
}
void ShortStringTest::moveBenchmarkQString(){
    moveBench<QString>();
}

QTEST_APPLESS_MAIN(ShortStringTest)

#include "tst_shortstringtest.moc"
