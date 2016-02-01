#include <QDebug>
#include <QtTest/QtTest>

#include "kernel/errorinfo.h"
#include "test_db.h"
#include "db/metadata/object/view_object.h"
#include "db/metadata/object/constraint_object.h"
namespace corelibtest{
namespace db{

using sn::corelib::ErrorInfo;
using sn::corelib::db::metadata::ViewObject;
using sn::corelib::db::metadata::ConstraintObject;
void TestDb::testMetadata()
{
   try{
      //qDebug() << m_metadata->getSchemas();
      //qDebug() << m_metadata->getTableNames("cloudcontroller_devel");
      //m_metadata->getTable("app_sys_user_base_info");
      //m_metadata->getColumn("sexTypes", "app_sys_user_base_info");
//      qDebug() << m_metadata->getColumns("app_sys_user_base_info");
      //qDebug() << m_metadata->getColumnNames("app_sys_user_base_info");
     QSharedPointer<ConstraintObject> constraint = m_metadata->getConstraint("_zf_app_sys_user_base_info_loginErrorTimes_2", "app_sys_user_base_info");
     //qDebug() << view->getName();
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

}//db
}//corelibtest