#include <QDebug>
#include <QtTest/QtTest>

#include "test_adapter.h"

namespace corelibtest{
namespace db{

class A{
public:
   virtual ~A(){
      qDebug() << "destroy A";
   }
};

class B : public A{
   public:
   virtual ~B(){
      qDebug() << "destroy B";
   }
   virtual void s()
   {
      qDebug() << "b";
   }
};
class Q{
   public:
   virtual ~Q(){
      qDebug() << "destroy Q";
   }
};
class C:public B, public Q{
   public:
   virtual ~C(){
      qDebug() << "destroy C";
   }
   virtual void s()
   {
      qDebug() << "c";
   }
};

void TestAdapter::initTestCase()
{
   Q* b = new C;
   delete b;
}

}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestAdapter)