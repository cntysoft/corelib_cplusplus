#include <QDebug>
#include <QtTest/QtTest>

#include "kernel/errorinfo.h"
#include "test_metadata.h"

namespace corelibtest{
namespace db{

using sn::corelib::ErrorInfo;


void TestMetaData::testGetTables()
{
   try{
      qDebug() << m_metadata->getSchemas().toStringList();
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

}//db
}//corelibtest