#include "test_sql.h"
#include <QtTest/QtTest>
#include "db/sql/sql.h"
#include "db/sql/delete.h"
#include "db/sql/table_identifier.h"
#include "db/sql/abstract_sql.h"

#include <QSharedPointer>
#include <QDebug>
#include "global/common_funcs.h"

namespace corelibtest{
namespace db{

using sn::corelib::db::sql::Sql;
using sn::corelib::db::sql::Delete;
using sn::corelib::db::sql::TableIdentifier;
using sn::corelib::db::sql::AbstractSql;
using sn::corelib::db::sql::AbstractPreparableSql;
using sn::corelib::instanceof;

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
   Sql sql(m_engine, "userinfo");
   QCOMPARE(sql.getTableName(), QString("userinfo"));
}

void TestSql::testDeleteSql()
{
   Sql sql(m_engine, "userinfo");
   QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
   TableIdentifier& table = deleteSql->getTable();
   QCOMPARE(table.getTable(), QString("userinfo"));
   deleteSql->where("`name` = `adasdasd`");
   qDebug() << sql.buildSqlString(deleteSql);
}

}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestSql)