#include "test_version.h"
#include "utils/version.h"
#include <QtTest/QtTest>
#include <QDebug>
namespace corelibtest{
namespace utils{

using sn::corelib::utils::Version;

void TestVersion::testVersionInfo()
{
   Version version1("1.0.2alpha");
   Version version2("1.0.2beta");
   qDebug() << version1.toVersion();
}

void TestVersion::initTestCase()
{}

}//utils
}//corelibtest
QTEST_MAIN(corelibtest::utils::TestVersion)