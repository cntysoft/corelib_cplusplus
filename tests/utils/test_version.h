#ifndef UTILS_TEST_VERSION_H
#define UTILS_TEST_VERSION_H

#include <QObject>
#include <QString>

namespace corelibtest{
namespace utils{

class TestVersion : public QObject
{
   Q_OBJECT
private slots:
   void initTestCase();
   void testVersionInfo();
};

}//utils
}//corelibtest

#endif // UTILS_TEST_VERSION_H
