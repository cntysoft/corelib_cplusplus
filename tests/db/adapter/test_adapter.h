#ifndef DB_ADAPTER_TEST_ADAPTER_H
#define DB_ADAPTER_TEST_ADAPTER_H

#include <QObject>

namespace corelibtest{
namespace db{

class TestAdapter : public QObject
{
   Q_OBJECT
private slots:
   void initTestCase();
};

}//db
}//corelibtest

#endif // DB_ADAPTER_TEST_ADAPTER_H
