#include <QtTest>

#include "ShortString.h"
// add necessary includes here

class ShortStringBenchmark : public QObject
{
    Q_OBJECT

public:
    ShortStringBenchmark();
    ~ShortStringBenchmark();

private slots:
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

ShortStringBenchmark::ShortStringBenchmark()
{

}

ShortStringBenchmark::~ShortStringBenchmark()
{

}

template <typename C>
void compareBench(){
    constexpr auto count = 8192;
    for(int j = 0; j < count; ++j){
        C s1 = "1234567";
        C s2 = "1234567";
        [[maybe_unused]]auto equal = s1 == s2;
    }
}
void ShortStringBenchmark::compareBenchmark(){
    QBENCHMARK{ compareBench<ShortString>(); }
}
void ShortStringBenchmark::compareBenchmarkString(){
    QBENCHMARK{ compareBench<std::string>(); }
}
void ShortStringBenchmark::compareBenchmarkQString(){
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
void ShortStringBenchmark::initBenchmark(){
    QBENCHMARK{ initBench<ShortString>(); }
}
void ShortStringBenchmark::initBenchmarkString(){
    QBENCHMARK{ initBench<std::string>(); }
}
void ShortStringBenchmark::initBenchmarkQString(){
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
void ShortStringBenchmark::pushBenchmark(){
    QBENCHMARK{ pushBench<ShortString>(); }
}
void ShortStringBenchmark::pushBenchmarkString(){
    QBENCHMARK{ pushBench<std::string>(); }
}
void ShortStringBenchmark::pushBenchmarkQString(){
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
void ShortStringBenchmark::appendBenchmark(){
    appendBench<ShortString>();
}
void ShortStringBenchmark::appendBenchmarkString(){
    appendBench<std::string>();
}
void ShortStringBenchmark::appendBenchmarkQString(){
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
void ShortStringBenchmark::sortBenchmark(){
    sortBench<ShortString>();
}
void ShortStringBenchmark::sortBenchmarkString(){
    sortBench<std::string>();
}
void ShortStringBenchmark::sortBenchmarkQString(){
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
void ShortStringBenchmark::copyBenchmark(){
    QBENCHMARK{ copyBench<ShortString>(); }
}
void ShortStringBenchmark::copyBenchmarkString(){
    QBENCHMARK{ copyBench<std::string>(); }
}
void ShortStringBenchmark::copyBenchmarkQString(){
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
void ShortStringBenchmark::moveBenchmark(){
    moveBench<ShortString>();
}
void ShortStringBenchmark::moveBenchmarkString(){
    moveBench<std::string>();
}
void ShortStringBenchmark::moveBenchmarkQString(){
    moveBench<QString>();
}

QTEST_APPLESS_MAIN(ShortStringBenchmark)

#include "tst_shortstringbenchmark.moc"
