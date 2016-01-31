#include <QDebug>
#include <QtTest/QtTest>

#include "kernel/errorinfo.h"
#include "test_db.h"

namespace corelibtest{
namespace db{

using sn::corelib::ErrorInfo;


void TestDb::testMetadata()
{
   try{
      //qDebug() << m_metadata->getSchemas();
      qDebug() << m_metadata->getTableNames("cloudcontroller_devel");
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

}//db
}//corelibtest