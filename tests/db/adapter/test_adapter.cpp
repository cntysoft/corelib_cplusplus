#include <QDebug>
#include <QtTest/QtTest>

#include "test_adapter.h"

namespace corelibtest{
namespace db{

void TestAdapter::initTestCase()
{
   QVERIFY2(false, "test data not found");
}

}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestAdapter)