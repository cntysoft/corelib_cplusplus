#include "test_sql.h"
#include <QtTest/QtTest>
#include "db/sql/sql.h"
#include <QDebug>

namespace corelibtest{
namespace db{

using sn::corelib::db::sql::Sql;

TestSql::TestSql()
   : m_engine(Engine::QMYSQL, {
{"host", "127.0.0.1"},
{"username","root"},
{"password", "cntysoft"},
{"dbname", "meta_info_devel"}
})
{
   
}

void TestSql::initTestCase()
{
   
}

void TestSql::testSqlTableName()
{
   Sql sql(m_engine, "meta_info_devel");
   QCOMPARE(sql.getTableName(), QString("meta_info_devel"));
}



}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestSql)