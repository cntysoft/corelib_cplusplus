#include <QDebug>
#include <QtTest/QtTest>

#include "kernel/errorinfo.h"
#include "test_db.h"

namespace corelibtest{
namespace db{

using sn::corelib::ErrorInfo;


void TestDb::testGetSchemas()
{
   try{
      qDebug() << m_metadata->getSchemas().toStringList();
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

}//db
}//corelibtest