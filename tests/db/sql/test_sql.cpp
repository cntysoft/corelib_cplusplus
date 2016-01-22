#include "test_sql.h"
#include <QtTest/QtTest>
#include "db/sql/sql.h"
#include "db/sql/delete.h"
#include "db/sql/table_identifier.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/predicate/predicate.h"
#include "db/sql/predicate/simple_predicate.h"
#include "db/sql/expression.h"
#include "kernel/errorinfo.h"

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
using sn::corelib::db::sql::predicate::PredicateSet;
using sn::corelib::instanceof;
using sn::corelib::db::sql::predicate::Where;
using sn::corelib::db::sql::AbstractExpression;
using sn::corelib::ErrorInfo;

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
//   Sql sql(m_engine, "userinfo");
//   {
//      QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
//      TableIdentifier& table = deleteSql->getTable();
//      QCOMPARE(table.getTable(), QString("userinfo"));
//      deleteSql->where("`name` = `xiuxiu`");
//      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu`"));
//      deleteSql->where("`age` = 123", PredicateSet::OP_AND);
//      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu` AND `age` = 123"));
//   }
//   {
//      QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
//      deleteSql->where("`name` = `xiuxiu`");
//      deleteSql->where("`age` = 123", PredicateSet::OP_OR);
//      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu` OR `age` = 123"));
//   }
}

void TestSql::testWherePredicate()
{
   Sql sql(m_engine, "userinfo");
   {
      try{
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         deleteSql->where(where);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo`"));
         where->equalTo("name", "xiuxiu", AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
         where->equalTo("age", 12, AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
         where->setOrCombination();
         where->equalTo("depth", 1, AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = 'xiuxiu' AND `age` = 12 OR `depth` = 1"));
      }catch(ErrorInfo exp){
         qDebug() << exp.toString();
      }
   }
}

}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestSql)