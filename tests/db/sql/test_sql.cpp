#include "test_sql.h"
#include <QtTest/QtTest>
#include "db/sql/sql.h"
#include "db/sql/delete.h"
#include "db/sql/insert.h"
#include "db/sql/table_identifier.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/predicate/predicate.h"
#include "db/sql/predicate/simple_predicate.h"
#include "db/sql/simple_sql.h"
#include "db/sql/expression.h"
#include "kernel/errorinfo.h"
#include "global/common_funcs.h"
#include <QSharedPointer>
#include <QDebug>
#include "global/common_funcs.h"
#include <QMap>
#include <QString>
#include <QList>

namespace corelibtest{
namespace db{

using sn::corelib::db::sql::Sql;
using sn::corelib::db::sql::Delete;
using sn::corelib::db::sql::Select;
using sn::corelib::db::sql::Insert;
using sn::corelib::db::sql::TableIdentifier;
using sn::corelib::db::sql::AbstractSql;
using sn::corelib::db::sql::AbstractPreparableSql;
using sn::corelib::db::sql::predicate::PredicateSet;
using sn::corelib::instanceof;
using sn::corelib::db::sql::Where;
using sn::corelib::db::sql::Having;
using sn::corelib::db::sql::AbstractExpression;
using sn::corelib::db::sql::Expression;
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
   qRegisterMetaType<QSharedPointer<QString>>("StrPtr1");
   qRegisterMetaType<QSharedPointer<int>>("StrPtr");
}

void TestSql::testSqlTableName()
{
   Sql sql(m_engine, "userinfo");
   QCOMPARE(sql.getTableName(), QString("userinfo"));
}

void TestSql::testDeleteSql()
{
   Sql sql(m_engine, "userinfo");
   {
      QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
      TableIdentifier& table = deleteSql->getTable();
      QCOMPARE(table.getTable(), QString("userinfo"));
      deleteSql->where("`name` = `xiuxiu`");
      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu`"));
      deleteSql->where("`age` = 123", PredicateSet::OP_AND);
      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu` AND `age` = 123"));
   }
   {
      QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
      deleteSql->where("`name` = `xiuxiu`");
      deleteSql->where("`age` = 123", PredicateSet::OP_OR);
      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu` OR `age` = 123"));
   }
}

void TestSql::testWherePredicate()
{
   try{
      Sql sql(m_engine, "userinfo");
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         deleteSql->where(where);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo`"));
         where->equalTo("name", "xiuxiu", AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
         where->equalTo("age", 12, AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
         where->setOrCombination();
         where->equalTo("depth", 1, AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = 'xiuxiu' AND `age` = 12 OR `depth` = 1"));
         where->clearPredicates();
         where->notEqualTo("name", "xiuxiu", AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` != 'xiuxiu'"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->isNull("name");
         deleteSql->where(where);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` IS NULL"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->isNotNull("name");
         deleteSql->where(where);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` IS NOT NULL"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->like("name", "mike");
         where->like("address", "bier");
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` LIKE 'mike' AND `address` LIKE 'bier'"));
         where->clearPredicates();
         where->notLike("name", "mike");
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` NOT LIKE 'mike'"));
         //qDebug() << sql.buildSqlString(deleteSql);
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->between("total", 12, 21);
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `total` BETWEEN 12 AND 21"));
         where->clearPredicates();
         where->between("range", "a", "z");
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `range` BETWEEN 'a' AND 'z'"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->expression("`name` = ? AND `age` = ?", {QVariant("xiuxiu"), QVariant(123)});
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = 'xiuxiu' AND `age` = 123"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->literal("`name` = 'xiuxiu'");
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = 'xiuxiu'"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->in("id", {1, 23, 221, "z"});
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `id` IN (1, 23, 221, 'z')"));
      }
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}



void TestSql::testSelectSql()
{
   try{
      Sql sql(m_engine, "userinfo");
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `userinfo`.* FROM `userinfo`"));
         //qDebug() << sql.buildSqlString(selectSql);
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `userinfo`.`name` AS `name_alias` FROM `userinfo`"));
         //qDebug() << sql.buildSqlString(selectSql);
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `userinfo`.`name` AS `name_alias` FROM `userinfo` WHERE `name` = 'sheneninfo'"));
         //qDebug() << sql.buildSqlString(selectSql);
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         selectSql->setTableReadOnly(false);
         selectSql->from("userinfo", "metainfo");
         //qDebug() << sql.buildSqlString(selectSql);
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         selectSql->setQuantifier("DISTINCT");
         selectSql->setTableReadOnly(false);
         selectSql->from("userinfo");
         QSharedPointer<Having> having(new Having);
         having->equalTo("goupid", 12);
         selectSql->having(having);
         //qDebug() << sql.buildSqlString(selectSql);
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT DISTINCT `userinfo`.`name` AS `name_alias` FROM `userinfo` WHERE `name` = 'sheneninfo' HAVING `goupid` = 12"));
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         selectSql->setTableReadOnly(false);
         selectSql->from("userinfo", "metainfo");
         selectSql->group("age");
         selectSql->group("height");
         //qDebug() << sql.buildSqlString(selectSql);
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `metainfo`.`userinfo`.`name` AS `name_alias` FROM `metainfo`.`userinfo` WHERE `name` = 'sheneninfo' GROUP BY `age`, `height`"));
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         selectSql->setTableReadOnly(false);
         selectSql->from("userinfo", "metainfo");
         selectSql->order("id", Select::ORDER_ASCENDING);
         selectSql->order("name", Select::ORDER_DESCENDING);
         //qDebug() << sql.buildSqlString(selectSql);
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `metainfo`.`userinfo`.`name` AS `name_alias` FROM `metainfo`.`userinfo` WHERE `name` = 'sheneninfo' ORDER BY `id` ASC, `name` DESC"));
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         selectSql->setTableReadOnly(false);
         selectSql->limit(10);
         selectSql->from("userinfo", "metainfo");
         //qDebug() << sql.buildSqlString(selectSql);
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `metainfo`.`userinfo`.`name` AS `name_alias` FROM `metainfo`.`userinfo` WHERE `name` = 'sheneninfo' LIMIT 10"));
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         selectSql->setTableReadOnly(false);
         selectSql->limit(10);
         selectSql->offset(2);
         selectSql->from("userinfo", "metainfo");
         //qDebug() << sql.buildSqlString(selectSql);
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `metainfo`.`userinfo`.`name` AS `name_alias` FROM `metainfo`.`userinfo` WHERE `name` = 'sheneninfo' LIMIT 10 OFFSET 2"));
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->addColumn(QString("name"), "name_alias");
         QSharedPointer<Where> where(new Where);
         where->equalTo("name", "sheneninfo");
         selectSql->where(where);
         selectSql->setTableReadOnly(false);
         selectSql->limit(10);
         selectSql->offset(2);
         selectSql->from("userinfo", "metainfo");
         QSharedPointer<Select> selectSql1 = sql.getSelectSql("meta");
         selectSql->combine(selectSql1);
         //qDebug() << sql.buildSqlString(selectSql);
         QCOMPARE(sql.buildSqlString(selectSql), QString("( SELECT `metainfo`.`userinfo`.`name` AS `name_alias` FROM `metainfo`.`userinfo` WHERE `name` = 'sheneninfo' LIMIT 10 OFFSET 2 ) UNION ( SELECT `meta`.* FROM `meta` )"));
      }
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         selectSql->setTableReadOnly(false);
         selectSql->from("foo");
         selectSql->join(QString("bar"), "foo.foo_id = bar.foo_id");
         //qDebug() << sql.buildSqlString(selectSql);
         QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `foo`.*, `bar`.* FROM `foo` INNER JOIN `bar` ON `foo`.`foo_id` = `bar`.`foo_id`"));
      }
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

void TestSql::testExpression()
{
   try{
      Sql sql(m_engine, "userinfo");
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         QSharedPointer<Select> selectSql1 = sql.getSelectSql("meta");
         selectSql->addColumn(QSharedPointer<AbstractExpression>(new Expression("`expr_column` = ?,`age` = ?, ?", {
                                                                                   1, 
                                                                                   QVariant::fromValue(selectSql1),
                                                                                   QVariant::fromValue(QSharedPointer<AbstractExpression>(new Expression("name = ?", {1})))
                                                                                })), "name_alias");
         qDebug() << sql.buildSqlString(selectSql);
         //QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `expr_column` = 1,`age` = (SELECT `meta`.* FROM `meta`), name = 1 AS `name_alias` FROM `userinfo`"));
      }
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

void TestSql::testSubSelect()
{
   try{
      Sql sql(m_engine, "userinfo");
      {
         QSharedPointer<Select> selectSql = sql.getSelectSql();
         QSharedPointer<Select> selectSql1 = sql.getSelectSql();
         selectSql1->setTableReadOnly(false);
         selectSql1->from("meta");
         selectSql->join(selectSql1, "subselect", "subselect.foo_id = meta.foo_id");
         qDebug() << sql.buildSqlString(selectSql);
         //QCOMPARE(sql.buildSqlString(selectSql), QString("SELECT `expr_column` = 1 and `age` = 'xiuxiu' AS `name_alias` FROM `userinfo`"));
      }
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

void TestSql::testInsertSql()
{
   try{
      Sql sql(m_engine, "userinfo");
      {
         QSharedPointer<Insert> insertSql = sql.getInsertSql();
         insertSql->values({
                              {"name", "softboy"},
                              {"age", 21}
                           });
         //qDebug() << sql.buildSqlString(insertSql);
         QCOMPARE(sql.buildSqlString(insertSql), QString("INSERT INTO `userinfo` (`age`, `name`) VALUES (21, 'softboy')"));
      }
      {
         QSharedPointer<Insert> insertSql = sql.getInsertSql();
         QSharedPointer<Select> selectSql(new Select("meta"));
         insertSql->values(selectSql);
         //qDebug() << sql.buildSqlString(insertSql);
         QCOMPARE(sql.buildSqlString(insertSql), QString("INSERT INTO `userinfo`  SELECT `meta`.* FROM `meta`"));
      }
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestSql)