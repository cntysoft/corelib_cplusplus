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
      //qDebug() << m_metadata->getTableNames("cloudcontroller_devel");
      //m_metadata->getTable("app_sys_user_base_info");
      //m_metadata->getColumn("sexTypes", "app_sys_user_base_info");
      qDebug() << m_metadata->getColumns("app_sys_user_base_info");
      //qDebug() << m_metadata->getColumnNames("app_sys_user_base_info");
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

}//db
}//corelibtest